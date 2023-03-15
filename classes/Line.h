//
// Created by marco-vb on 15-03-2023.
//

#ifndef RAILWAYS_LINE_H
#define RAILWAYS_LINE_H

#include <bits/stdc++.h>

class Line {
private:
    std::string name;
    std::vector<std::string> stations;

public:
    Line();
    Line(std::string name, std::vector<std::string> stations);
    std::string getName();
    std::vector<std::string> getStations();
};


#endif //RAILWAYS_LINE_H
