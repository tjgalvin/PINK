/**
 * @file   Pink/main_gpu.cpp
 * @brief  Main routine of PINK.
 * @date   Oct 15, 2018
 * @author Bernd Doser, HITS gGmbH
 */

#include <cmath>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>

#ifndef NDEBUG
    #include <fenv.h>
#endif

#include "main_gpu.h"
#include "main_gpu_generic.h"
#include "UtilitiesLib/InputData.h"
#include "UtilitiesLib/pink_exception.h"

namespace pink {

void main_gpu(InputData const & input_data)
{
	if (input_data.layout == Layout::CARTESIAN)
		if (input_data.dimensionality == 1)
			main_gpu_generic<CartesianLayout<1>, CartesianLayout<2>, float>(input_data);
		else if (input_data.dimensionality == 2)
			main_gpu_generic<CartesianLayout<2>, CartesianLayout<2>, float>(input_data);
		else if (input_data.dimensionality == 3)
			main_gpu_generic<CartesianLayout<3>, CartesianLayout<2>, float>(input_data);
		else
			pink::exception("Unsupported dimensionality of " + input_data.dimensionality);
	else if (input_data.layout == Layout::HEXAGONAL)
		main_gpu_generic<HexagonalLayout, CartesianLayout<2>, float>(input_data);
	else
		pink::exception("Unknown layout");
}

} // namespace pink
