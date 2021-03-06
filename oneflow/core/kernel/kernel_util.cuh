/*
Copyright 2020 The OneFlow Authors. All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#ifndef ONEFLOW_CORE_KERNEL_KERNEL_UTIL_CUH_
#define ONEFLOW_CORE_KERNEL_KERNEL_UTIL_CUH_

namespace oneflow {

template<typename T>
__device__ T gpu_atomic_add(T* address, const T val);

template<typename T>
__device__ T gpu_atomic_max(T* address, const T val);

template<typename T>
__host__ __device__ T MaxWithLogThreshold(T x) {
  const T threshold = 1e-20;
  return x > threshold ? x : threshold;
}

template<typename T>
__host__ __device__ T SafeLog(T x) {
  return logf(MaxWithLogThreshold(x));
}

}  // namespace oneflow

#endif  // ONEFLOW_CORE_KERNEL_KERNEL_UTIL_CUH_
