//
// Created by marco-vb on 15-03-2023.
//

#include "Station.h"

Station::Station(std::string name, std::string district, std::string municipality, std::string township, std::shared_ptr<Line> line) {
    this->name = std::move(name);
    this->district = std::move(district);
    this->municipality = std::move(municipality);
    this->township = std::move(township);
    this->line = std::move(line);
}

std::string Station::getName() {
    return this->name;
}

std::string Station::getDistrict() {
    return this->district;
}

std::string Station::getMunicipality() {
    return this->municipality;
}

std::string Station::getTownship() {
    return this->township;
}

std::shared_ptr<Line> Station::getLine() {
    return this->line;
}
