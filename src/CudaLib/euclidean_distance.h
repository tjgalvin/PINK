/**
 * @file   CudaTest/euclidean_distance.h
 * @date   Apr 17, 2018
 * @author Bernd Doser <bernd.doser@h-its.org>
 */

#include <cstddef>

namespace pink {

typedef unsigned int uint;

void euclidean_distance_dp4a(int *d_in1, int *d_in2, int *d_in3, int *d_out, size_t size);

void euclidean_distance_dp4a(uint *d_in1, uint *d_in2, uint *d_in3, uint *d_out, size_t size);

} // namespace pink
