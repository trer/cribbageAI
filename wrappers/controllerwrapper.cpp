#pragma once
#include "controllerwrapper.h"


namespace py = pybind11;


void init_cribbage(py::module &m) {
    py::class_<controller::game_controller>(m, "controller")
    .def(py::init<>())
    .def("set_player", 
        &controller::game_controller::set_player,
        py::arg("player_type"),
        py::arg("num"))
    .def("get_num_cards", 
        &controller::game_controller::get_num_cards,
        py::arg("player"))
    .def("get_cut_card", 
        &controller::game_controller::get_cut_card)
    .def("get_card",
        &controller::game_controller::get_card, 
        py::arg("index"), 
        py::arg("player"))
    .def("discard_set",
        &controller::game_controller::discard_set)
    .def("discard_action", 
        &controller::game_controller::discard_action,
        py::arg("player"),
        py::arg("index1"),
        py::arg("index2"))
    .def("play_action",
        &controller::game_controller::play_action,
        py::arg("player"),
        py::arg("index"))
    .def("handle_discards", 
        &controller::game_controller::handle_discards)
    .def("setup_game",
        &controller::game_controller::setup_game,
        py::arg("same_pone") = false)
    .def("setup_round", 
        &controller::game_controller::setup_round)
    .def("setup_play_phase", 
        &controller::game_controller::setup_play_phase)
    .def("get_current_player",
        &controller::game_controller::get_current_player)
    .def("has_legal_move",
        &controller::game_controller::has_legal_move,
        py::arg("player"))
    .def("has_called_go", 
        &controller::game_controller::has_called_go,
        py::arg("player"))
    .def("matching_setup",
        &controller::game_controller::matching_setup)
    .def("matching_score_pone",
        &controller::game_controller::matching_score_pone)
    .def("matching_score_dealer",
        &controller::game_controller::matching_score_dealer)
    .def("get_points",
        &controller::game_controller::get_points,
        py::arg("player"))
    .def("get_current_pone", 
        &controller::game_controller::get_current_pone)
    .def("poll_player",
        &controller::game_controller::poll_player,
        py::arg("player"));
}