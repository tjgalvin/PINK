/**
 * @file   CudaLib/copy_and_transform_kernel.h
 * @date   Feb 28, 2019
 * @author Bernd Doser, HITS gGmbH
 */

#pragma once

#include <cassert>
#include <cuda_runtime.h>

namespace pink {

/// CUDA device kernel to copy a submatrix and transform elements
template <typename T>
__global__
void copy_and_transform_kernel(T *dst, float const *src, uint32_t dst_dim,
    uint32_t src_dim, uint32_t offset, uint32_t factor)
{
    assert(offset == static_cast<uint32_t>((src_dim - dst_dim) * 0.5));

    uint32_t i = blockIdx.x * blockDim.x + threadIdx.x;
    uint32_t j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i >= dst_dim or j >= dst_dim) return;

    dst[blockIdx.z * dst_dim * dst_dim + i * dst_dim + j] =
    src[blockIdx.z * src_dim * src_dim + (i + offset) * src_dim + (j + offset)] * factor;
}

} // namespace pink
