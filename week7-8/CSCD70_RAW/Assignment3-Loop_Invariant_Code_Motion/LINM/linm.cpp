#include "linm.h"

#include <deque>
#include <stack>
#include <typeinfo>

#include <tensorflow/core/grappler/op_types.h>
#include <tensorflow/core/grappler/optimizers/constant_folding.h>
#include <tensorflow/core/grappler/utils/frame.h>
#include <tensorflow/core/lib/strings/strcat.h>

using namespace tensorflow;
using tensorflow::strings::StrCat;

#define LINM_OPTIMIZER_STR  typeid(LoopInvariantNodeMotionOptimizer).name()


Status
LoopInvariantNodeMotionOptimizer::HandleInvariantEnter(NodeDef * node)
{
        std::vector < string > enter_control_inputs;
        string enter_input;
  
        for (const string & input : node->input())
        {
                if (IsControlInput(input))
                {
                        enter_control_inputs.push_back(input);
                }
                else
                {
                        enter_input = input;
                }
        }
        for (NodeDef * consumer : _node_map->GetOutputs(node->name()))
        {
                if (_invariant_nodes.count(consumer))
                {
                        for (int i = 0; i < consumer->input_size(); ++i)
                        {
                                if (NodeName(consumer->input(i)) == node->name())
                                {
                                        consumer->set_input(i, enter_input);
                                        _node_map->AddOutput(NodeName(enter_input), consumer->name());
                                        _node_map->RemoveOutput(node->name(), consumer->name());
                                }
                        }
                        for (const string & control_input : enter_control_inputs)
                        {
                                consumer->add_input(control_input);
                                _node_map->AddOutput(NodeName(control_input), consumer->name());
                        }
                }
        }
        return Status::OK();
}


Status
LoopInvariantNodeMotionOptimizer::HandleConst(NodeDef * node, const int frame_id)
{
        const string const_node_name = AddPrefixToNodeName(
                node->name(), LINM_OPTIMIZER_STR);
        NodeDef * const_node = _node_map->GetNode(const_node_name);
        if (const_node == nullptr)
        {
                const_node = _optimized_graph->add_node();
                const_node->set_name(const_node_name);
                const_node->set_op("Const");
                const_node->set_device(node->device());
                *const_node->mutable_attr() = node->attr();
                _node_map->AddNode(const_node->name(), const_node);
        }
        for (NodeDef * consumer : _node_map->GetOutputs(node->name()))
        {
                if (_invariant_nodes.count(consumer))
                {
                        for (int i = 0; i < consumer->input_size(); ++i)
                        {
                                if (NodeName(consumer->input(i)) == node->name())
                                {
                                        if (IsControlInput(consumer->input(i)))
                                        {
                                                *consumer->mutable_input(i) = AsControlDependency(*const_node);
                                        } 
                                        else 
                                        {
                                                *consumer->mutable_input(i) = const_node->name();
                                        }
                                        _node_map->AddOutput(const_node->name(), consumer->name());
                                        _node_map->RemoveOutput(node->name(), consumer->name());
                                }
                        }  // for (i ∈ [0, consumer->input_size())])
                }  // if (consumer ∈ invariant_nodes)
        }  // for (consumer ∈ node->outputs)
        if (_frame_parent[frame_id] != -1)
        {
                int parent_id = _frame_parent[frame_id];

                auto loop_cond_it = _loop_cond.find(parent_id);
                if (loop_cond_it == _loop_cond.end())
                {
                        return errors::InvalidArgument("Frame ", frame_id, " "
                                "doesn't have a LoopCond node");
                }
                const string & loop_cond_name = loop_cond_it->second->name();
                NodeDef * switch_node = nullptr;
                for (auto * node : _node_map->GetOutputs(loop_cond_name))
                {
                        if (node->op() == "Switch")
                        {
                                switch_node = node;
                                break;
                        }
                }
                if (!switch_node) 
                {
                        return errors::InvalidArgument(
                                "LoopCond node of Frame ", frame_id, " "
                                "doesn't connect to any Switch node");
                }
                string switch_output = StrCat(switch_node->name(), ":1");
                const string ctrl_dep = ConstantFolding::AddControlDependency(
                        switch_output, _optimized_graph, _node_map.get());
                const_node->add_input(ctrl_dep);
                _node_map->AddOutput(NodeName(ctrl_dep), const_node->name());
        }  // if (frame_parent[frame_id] != -1)
        return Status::OK();
}


Status
LoopInvariantNodeMotionOptimizer::HandleInvariantNode(NodeDef * node, const int frame_id)
{
        for (int i = 0; i < node->input_size(); ++i)
        {
                if (IsControlInput(node->input(i)))
                {
                        node->mutable_input()->SwapElements(i, node->input_size() - 1);
                        node->mutable_input()->RemoveLast();
                }
        }

        DataTypeVector input_types, output_types;
        OpRegistryInterface * op_registry = OpRegistry::Global();
        const OpRegistrationData * op_reg_data = nullptr;
        TF_RETURN_IF_ERROR(op_registry->LookUp(node->op(), &op_reg_data));
        TF_RETURN_IF_ERROR(InOutTypesForNode(*node, op_reg_data->op_def,
                                             &input_types,
                                             &output_types));
        const auto & attr = _invariant_enters[frame_id][0]->attr();
        auto invariant_enter_frame_name = attr.at("frame_name").s();
        auto invariant_enter_parallel_iterations =
                attr.at("parallel_iterations").i();

        for (NodeDef * consumer : _node_map->GetOutputs(node->name()))
        {
                if (!_invariant_nodes.count(consumer))
                {
                        for (int i = 0; i < consumer->input_size(); ++i)
                        {
                                int port;
                                string node_name = ParseNodeName(consumer->input(i), &port);
                                if (node_name != node->name())
                                {
                                        continue;
                                }
                                if (port < 0) 
                                {
                                        return errors::InvalidArgument(
                                                "Invariant node should not have control outputs "
                                                "to variant node");
                                }
                                DataType output_type = output_types[port];
                                        NodeDef * new_enter = _optimized_graph->add_node();
                                new_enter->set_op("Enter");
                                new_enter->set_device(node->device());
                                new_enter->set_name(AddPrefixToNodeName(
                                        StrCat(invariant_enter_frame_name, 
                                               "_enter_", _new_enter_id++), 
                                        LINM_OPTIMIZER_STR));

                                AttrValue data_type, frame_name, is_constant,
                                          parallel_iterations;
                                data_type.set_type(output_type);
                                frame_name.set_s(invariant_enter_frame_name);
                                is_constant.set_b(true);
                                parallel_iterations.set_i(invariant_enter_parallel_iterations);
                                new_enter->mutable_attr()->insert(
                                        {"T", data_type});
                                new_enter->mutable_attr()->insert(
                                        {"frame_name", frame_name});
                                new_enter->mutable_attr()->insert(
                                        {"is_constant", is_constant});
                                new_enter->mutable_attr()->insert(
                                        {"parallel_iterations", 
                                          parallel_iterations});
                                new_enter->add_input(consumer->input(i));
                                *consumer->mutable_input(i) = new_enter->name();
                                _node_map->AddNode(new_enter->name(), new_enter);
                                _node_map->AddOutput(node->name(), new_enter->name());
                                _node_map->AddOutput(new_enter->name(), consumer->name());
                        }  // for (i ∈ consumer->input_size())
                }  // if (consumer ∉ invariant_nodes)
        }  // for (consumer ∈ consumers)
        return Status::OK();
}


Status
LoopInvariantNodeMotionOptimizer::MoveInvariantNodes(const int frame_id)
{
        for (auto iter = _invariant_nodes.begin();
             iter != _invariant_nodes.end(); ++iter)
        {
                NodeDef * invariant_node = *iter;

                if (IsEnter(*invariant_node))
                {
                        TF_RETURN_IF_ERROR(HandleInvariantEnter(invariant_node));
                } 
                else if (IsConstant(*invariant_node))
                {
                        TF_RETURN_IF_ERROR(HandleConst(invariant_node, frame_id));
                } 
                else
                {
                        TF_RETURN_IF_ERROR(HandleInvariantNode(invariant_node, frame_id));
                }
        }
        return Status::OK();
}


Status
LoopInvariantNodeMotionOptimizer::Optimize(
        Cluster * cluster, const GrapplerItem & item,
        GraphDef * optimized_graph)
{
        *optimized_graph = item.graph;
        _optimized_graph = optimized_graph;
        _node_map.reset(new NodeMap(optimized_graph));

        FrameView frame_view;
        TF_RETURN_IF_ERROR(frame_view.InferFromGraph(*optimized_graph));

        _frame_parent  .resize(frame_view.num_frames(), -1);
        _frame_children.resize(frame_view.num_frames(),
                               std::set < int > ());

        for (const NodeDef & node : optimized_graph->node())
        {
                const std::vector < int > & frame_ids = frame_view.Frames(node);

                if (frame_ids.size() >= 3)
                {
                        for (unsigned int i = 1; i < frame_ids.size() - 1; ++i)
                        {
                                _frame_parent  [frame_ids[i]] = frame_ids[i - 1];
                                _frame_children[frame_ids[i]].insert(frame_ids[i + 1]);
                        }
                }
                if (frame_ids.size() >= 2)
                {
                        _frame_parent  [frame_ids.back()] = frame_ids[frame_ids.size() - 2];
                        _frame_children[frame_ids[0]].insert(frame_ids[1]);
                }
                if (!frame_ids.empty())
                {
                        if (node.op() == "LoopCond")
                        {
                                if (_loop_cond.count(frame_ids.back()))
                                {
                                        return errors::InvalidArgument(
                                                "Loop ", frame_ids.back(), " "
                                                "has more than one LoopCond node: ",
                                                node.name(), " and ", _loop_cond[frame_ids.back()]->name());
                                }
                                _loop_cond[frame_ids.back()] = &node;
                        }
                        if (IsEnter(node) && node.attr().at("is_constant").b())
                        {
                                _invariant_enters[frame_ids.back()].push_back(
                                        const_cast < NodeDef * > (&node));
                        }
                }  // if (!frame_ids.empty())
        }  // for (node ∈ optimized_graph->node())

        // @TODO Implement LINM using the worklist algorithm.

        return Status::OK();
}
