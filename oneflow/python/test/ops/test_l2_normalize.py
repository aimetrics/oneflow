import os
import numpy as np
import tensorflow as tf
import oneflow as flow
from collections import OrderedDict 
from test_util import type_name_to_flow_type

from test_util import GenArgList
from test_util import GetSavePath
from test_util import Save

gpus = tf.config.experimental.list_physical_devices("GPU")
for gpu in gpus:
    tf.config.experimental.set_memory_growth(gpu, True)

def compare_with_tensorflow(device_type, x_shape, data_type, axis, epsilon):
    assert device_type in ["gpu", "cpu"]
    flow.clear_default_session()
    func_config = flow.FunctionConfig()
    func_config.default_data_type(flow.float)
    func_config.train.primary_lr(1e-4)
    func_config.train.model_update_conf(dict(naive_conf={}))


    @flow.function(func_config)
    def L2NormalizeJob():
        with flow.device_prior_placement(device_type, "0:0"):
            x = flow.get_variable(
                "x",
                shape=x_shape,
                dtype=type_name_to_flow_type[data_type],
                initializer=flow.random_uniform_initializer(minval=-10, maxval=10),
                trainable=True,
            )
            loss = flow.math.l2_normalize(x, axis=axis, epsilon=epsilon)
            flow.losses.add_loss(loss)

            flow.watch(x, Save("x"))
            flow.watch_diff(x, Save("x_diff"))
            flow.watch(loss, Save("loss"))
            flow.watch_diff(loss, Save("loss_diff"))

            return loss

    # OneFlow
    check_point = flow.train.CheckPoint()
    check_point.init()
    of_out = L2NormalizeJob().get()
    # TensorFlow
    with tf.GradientTape(persistent=True) as tape:
        x = tf.Variable(np.load(os.path.join(GetSavePath(), "x.npy")))
        tf_out = tf.math.l2_normalize(x, axis=axis, epsilon=epsilon)
    loss_diff = np.load(os.path.join(GetSavePath(), "loss_diff.npy"))
    tf_x_diff = tape.gradient(tf_out, x, loss_diff)
    assert np.allclose(of_out.ndarray(), tf_out.numpy(), rtol=1e-5, atol=1e-5)
    assert np.allclose(
        np.load(os.path.join(GetSavePath(), "x_diff.npy")), tf_x_diff.numpy(), rtol=1e-5, atol=1e-5
    )

def test_l2_normalize(test_case):
    arg_dict = OrderedDict()
    arg_dict["device_type"] = ["gpu", "cpu"]
    arg_dict["x_shape"] = [(10, 10, 20, 30)]
    arg_dict["data_type"] = ["float32"]
    arg_dict["axis"] = [-1, 0, 1, 2, 3]
    arg_dict["epsilon"] = [1e-10, 1e-5]
    for arg in GenArgList(arg_dict):
        compare_with_tensorflow(*arg)