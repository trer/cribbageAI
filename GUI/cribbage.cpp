#include ".././simulator/cribbage.h"
#include <pybind11/stl.h>
#include <pybind11/pybind11.h>


namespace py = pybind11;

void init_cribbage(py::module &m) {
    
    py::class_<cribbage>(m, "cribbage")
    .def(py::init<std::string>(), py::arg("seed"))
    .def("round",
         py::overload_cast<>( &cribbage::round, py::const_))

}