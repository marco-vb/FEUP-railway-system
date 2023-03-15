//
// Created by marco-vb on 15-03-2023.
//

#include "Line.h"

Line::Line() {
    this->name = "No name";
    this->stations = std::vector<std::string>();
}

Line::Line(std::string name, std::vector<std::string> stations) {
    this->name = std::move(name);
    this->stations = std::move(stations);
}

std::string Line::getName() {
    return this->name;
}

std::vector<std::string> Line::getStations() {
    return this->stations;
}
