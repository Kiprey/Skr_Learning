import tensorflow as tf
import os

tf.compat.v1.disable_eager_execution()  # Select the static graph as the
                                        # execution backend.
os.environ["TF_CPP_MIN_LOG_LEVEL"] = "2"  # Disable verbose logging.

i_init = tf.constant(0.0)
i_incr = tf.constant(1.0)
i_range = tf.constant(11.0)

i = tf.raw_ops.Enter(
        data=i_init,
        frame_name="while/while_context",
        is_constant=False,
        parallel_iterations=1)
i_incr = tf.raw_ops.Enter(
        data=i_incr,
        frame_name="while/while_context",
        is_constant=True,
        parallel_iterations=1)
i_range = tf.raw_ops.Enter(
        data=i_range,
        frame_name="while/while_context",
        is_constant=True,
        parallel_iterations=1)

InvariantAdd = tf.raw_ops.Add(x=i_incr, y=i_incr)

Merge = tf.raw_ops.Merge(inputs=[i, i])[0]  # We will be modifying the second
                                            # operand to the value that comes
                                            # from the next iteration.

Switch = tf.raw_ops.Switch(
        data=Merge,
        pred=tf.raw_ops.LoopCond(input=tf.raw_ops.Less(x=Merge, y=i_range)))

NextIteration = tf.raw_ops.NextIteration(
        data=tf.raw_ops.Add(
            x=InvariantAdd, 
            y=Switch[1]))
Merge.op._update_input(1, NextIteration)

Exit = tf.raw_ops.Exit(data=Switch[0])

with tf.compat.v1.Session() as sess:
    assert sess.run([Exit])[0] == 12, \
           "Exit value does not match what to expect."
