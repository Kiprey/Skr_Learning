#pragma once

#include <memory>
#include <map>
#include <set>
#include <vector>

#include <tensorflow/core/framework/node_def.pb.h>                // NodeDef
#include <tensorflow/core/grappler/optimizers/graph_optimizer.h>  // GraphOptimizer
#include <tensorflow/core/grappler/utils.h>                       // NodeMap

using namespace tensorflow;
using namespace grappler;

class LoopInvariantNodeMotionOptimizer final : public GraphOptimizer
{
private:
        /// pointer reference to optimized graph
        GraphDef * _optimized_graph;
 
        /// utility data structure that looks up node and its outputs by name
        std::unique_ptr < NodeMap >        _node_map;
        /// ∀frame, its parent and set of children
        std::vector < int >                _frame_parent;
        std::vector < std::set < int > >   _frame_children;
        /// ∀frame, its loop condition
        std::map < int, const NodeDef * >  _loop_cond;
        /// ∀frame, its invariant enters
        std::map < int, std::vector < NodeDef * > >
                                           _invariant_enters;
        /// ∀frame, its invariant nodes and each node's number of outputs
        std::set < NodeDef * >             _invariant_nodes;
        int _new_enter_id;  ///< counter for instantiating new enter

        /// @brief Handle specific invariant node types. Those methods are
        ///        invoked by @c MoveInvariantNodes .
        /// 
        /// @todo  The implementations of those methods are already provided to
        ///        you. However, you will have to document in your report how
        ///        those methods function from a high-level perspective.
        Status HandleInvariantEnter(NodeDef * node);
        Status HandleConst(NodeDef * node, const int frame_id);
        Status HandleInvariantNode(NodeDef * node, const int frame_id);
        /// @brief Move invariant nodes outside of the execution frame.
        Status MoveInvariantNodes(const int frame_id);
public:
        LoopInvariantNodeMotionOptimizer() = default;
        virtual ~LoopInvariantNodeMotionOptimizer() override = default;

        virtual string name() const override
        {
                return "loop_invariant_node_motion";
        }

        bool UsesFunctionLibrary() const override { return false; }

        /// @brief Optimize the computation graph.
        ///
        /// @param cluster  Collection of Hardware Resources
        /// @param item     Optimization Target 
        /// @param optimized_graph  Optimized Computation Graph Definition
        ///
        /// @note  In this assignment, we are only concerned about the flow of
        ///        the computation graph itself. In other words, we will only
        ///        work on @p item.graph in our implementation.
        ///
        /// @todo  Finish the implementation of this method.
        Status Optimize(Cluster * cluster, const GrapplerItem & item,
                        GraphDef * optimized_graph) override;

        /// @brief Provide feedback on how well the "optimized_graph" (produced
        ///        as @p optimized_graph from a call to @c Optimize ) performed.
        ///        We will NOT be implementing this method in this assignment.
        void Feedback(Cluster * cluster, const GrapplerItem & item,
                      const GraphDef & optimized_graph,
                      double result) override
        {
                // We will NOT be implementing this method.
        }
};
