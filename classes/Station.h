//
// Created by marco-vb on 15-03-2023.
//

#ifndef RAILWAYS_STATION_H
#define RAILWAYS_STATION_H

#include <bits/stdc++.h>
#include "Line.h"

class Station {
private:
    std::string name, district, municipality, township;
    std::shared_ptr<Line> line;

public:
    Station(std::string name, std::string district, std::string municipality, std::string township, std::shared_ptr<Line> line);
    std::string getName();
    std::string getDistrict();
    std::string getMunicipality();
    std::string getTownship();
    std::shared_ptr<Line> getLine();
};


#endif //RAILWAYS_STATION_H
