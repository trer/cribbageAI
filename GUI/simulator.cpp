
#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_cribbage(py::module &);

namespace mcl {

PYBIND11_MODULE(simulator, m) {
    // Optional docstring
    m.doc() = "simulator library";
    
    init_cribbage(m);
}
}