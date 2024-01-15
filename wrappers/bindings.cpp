#pragma once
#include <pybind11/pybind11.h>
#include "controllerwrapper.h"

namespace py = pybind11;





PYBIND11_MODULE(gui_bind, m) {
     // Optional docstring
     
     m.doc() = "simulator library";
     init_cribbage(m);
}

