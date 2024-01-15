#pragma once
#include "controller.h"
#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_cribbage(py::module &m);

