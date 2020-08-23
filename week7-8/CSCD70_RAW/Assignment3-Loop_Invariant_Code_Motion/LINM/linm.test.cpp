#include "linm.h"

#include <Python.h>
#include <tensorflow/core/grappler/grappler_item.h>     // GrapplerItem
#include <tensorflow/core/grappler/utils/frame.h>       // FrameView
#include <tensorflow/core/grappler/utils/graph_view.h>  // GraphView
#include <tensorflow/core/platform/test.h>              // gtest
#include <tensorflow/core/platform/types.h>             // string
#include <tensorflow/core/lib/core/status_test_util.h>  // TF_EXPECT_OK
                                                        // TF_ASSERT_OK


namespace {

class TensorFlowOpEnvironment final : public ::testing::Environment {
 public:
        ~TensorFlowOpEnvironment() override {}
        
        void SetUp() override
        {
                Py_Initialize();
                PyObject * sys_path = PySys_GetObject("path");
#if !defined(TENSORFLOW_PYWRAP_INTERNAL_PATH)
#error "Path to _pywrap_tensorflow_internal.so must be defined."
#endif
#define TO_CSTR_INTERNAL(s)  #s
#define TO_CSTR(s)  TO_CSTR_INTERNAL(s)
#define TENSORFLOW_PYWRAP_INTERNAL_PATH_CSTR  TO_CSTR(TENSORFLOW_PYWRAP_INTERNAL_PATH)
                PyList_Append(sys_path, PyUnicode_FromString(TENSORFLOW_PYWRAP_INTERNAL_PATH_CSTR));
                PyObject * py_name = PyUnicode_FromString("_pywrap_tensorflow_internal");
                PyObject * py_module = PyImport_Import(py_name);
        }
        void TearDown() override 
        {
                Py_Finalize();
        }
};

::testing::Environment * const tensorflow_op_environment =
        ::testing::AddGlobalTestEnvironment(new TensorFlowOpEnvironment);

NodeDef * AddNode(GraphDef * graph, const string & name, const string & op,
                  const std::vector < string > & inputs,
                  const std::vector < std::pair < string, AttrValue > > & attributes)
{
        NodeDef * node = graph->add_node();
        
        node->set_name(name);
        node->set_op(op);
        for (const string & input : inputs)
        {
                node->add_input(input);
        }
        for (const std::pair < string, AttrValue > & attr : attributes)
        {
                (*node->mutable_attr())[attr.first] = attr.second;
        }
        return node;
}

void AddEnterNode(GraphDef * graph, const string & name, const string & frame_name,
                  const bool is_constant, const int parallel_iterations,
                  const std::vector < string > & inputs)
{
        std::vector < std::pair < string, AttrValue > > attributes (4);

        attributes[0].first = "T";
        attributes[0].second.set_type(DT_FLOAT);
        attributes[1].first = "frame_name";
        attributes[1].second.set_s(frame_name);
        attributes[2].first = "is_constant";
        attributes[2].second.set_b(is_constant);
        attributes[3].first = "parallel_iterations";
        attributes[3].second.set_i(parallel_iterations);
        
        AddNode(graph, name, "Enter", inputs, attributes);
}

void AddSimpleNode(GraphDef * graph, const string & name, const string & op,
                   const std::vector < string > & inputs)
{
        std::vector < std::pair < string, AttrValue > > attributes (1);

        attributes[0].first = "T";
        attributes[0].second.set_type(DT_FLOAT);
    
        AddNode(graph, name, op, inputs, attributes);
}

}  // namespace anonymous


TEST(LoopInvariantNodeMotionOptimizerTest, Basic)
{
        GraphDef graph;

        AddSimpleNode(&graph, "i_init",  "Const", {});
        AddSimpleNode(&graph, "i_incr",  "Const", {});
        AddSimpleNode(&graph, "i_range", "Const", {});

        AddEnterNode(&graph, "i_init_enter",  "while/while_context", false, 1, {"i_init"});
        AddEnterNode(&graph, "i_incr_enter",  "while/while_context", true,  1, {"i_incr"});
        AddEnterNode(&graph, "i_range_enter", "while/while_context", true,  1, {"i_range"});

        AddSimpleNode(&graph, "InvariantAdd", "Add",
                      {"i_incr_enter", "i_incr_enter"});
        AddSimpleNode(&graph, "Merge", "Merge",
                      {"i_init_enter", "NextIteration"});
        AddSimpleNode(&graph, "Less", "Less", {"Merge", "i_range_enter"});
        AddSimpleNode(&graph, "LoopCond", "LoopCond", {"Less"});
        AddSimpleNode(&graph, "Switch", "Switch", {"Merge", "LoopCond"});
        AddSimpleNode(&graph, "VariantAdd", "Add",
                      {"i_incr_enter", "Switch:1"});
        AddSimpleNode(&graph, "NextIteration", "NextIteration", {"VariantAdd"});
        AddSimpleNode(&graph, "Exit", "Exit", {"Switch:0"});

        GrapplerItem item;
        item.graph = graph;

        LoopInvariantNodeMotionOptimizer optimizer;
        GraphDef output;
        TF_EXPECT_OK(optimizer.Optimize(nullptr, item, &output));

        {  // Original Graph
                Status status;
                utils::GraphView view(&graph, &status);
                TF_ASSERT_OK(status);
                FrameView frames;
                TF_EXPECT_OK(frames.InferFromGraphView(view));

                EXPECT_EQ(frames.num_frames(), 1);
                const utils::NodeView * invariant_add_node = view.GetNode("InvariantAdd");
                ASSERT_NE(invariant_add_node, nullptr);
                const NodeDef * invariant_add_node_def = invariant_add_node->node();
                ASSERT_EQ(frames.Frames(*invariant_add_node_def).size(), 1);
                EXPECT_EQ(frames.Frames(*invariant_add_node_def).back(), 0);
                const utils::NodeView * variant_add_node = view.GetNode("VariantAdd");
                ASSERT_NE(variant_add_node, nullptr);
                const NodeDef * variant_add_node_def = variant_add_node->node();
                ASSERT_EQ(frames.Frames(*variant_add_node_def).size(), 1);
                EXPECT_EQ(frames.Frames(*variant_add_node_def).back(), 0);
        }

        {  // Optimized Graph
                // @TODO Add the test case, similar to the one above.
        }
}
