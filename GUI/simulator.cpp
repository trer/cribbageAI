
#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int a, int b) {
    return a+b;
}

PYBIND11_MODULE(gui_bind, m) {
    // Optional docstring
    m.doc() = "simulator library";
    
    m.def("add", &add, "stupid docsting");
}
