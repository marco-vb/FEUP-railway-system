#include <bits/stdc++.h>
#include "classes/Network.h"

void readStations(const ptr<Network>& network, std::unordered_map<std::string, ptr<Station>> &stations) {
    std::ifstream file("../data/stations.csv");
    std::string line;
    std::getline(file, line);
    int id = 0;
    stations.clear();
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, municipality, township, district;
        std::getline(ss, name, ',');
        std::getline(ss, district, ',');
        std::getline(ss, municipality, ',');
        std::getline(ss, township, ',');
        std::getline(ss, line);     // ignore this
        auto station = make<Station>(id++, name, municipality, township, district);
        network->addStation(station);
        stations[name] = station;
    }
    file.close();
}

void readLinks(const ptr<Network>& network, const std::unordered_map<std::string, ptr<Station>> &stations) {
    std::ifstream file("../data/network.csv");
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string st1, st2, service;
        int capacity;
        std::getline(ss, st1, ',');
        std::getline(ss, st2, ',');
        ss >> capacity;
        std::getline(ss, service);
        int srvc = service == "STANDARD" ? 1 : 2;
        network->addLink(stations.at(st1), stations.at(st2), capacity, srvc);
        std::cout << st1 << "->" << st2 << std::endl;
    }
    file.close();
}

int main() {
    auto network = make<Network>();
    std::unordered_map<std::string, ptr<Station>> stations;
    readStations(network, stations);
    readLinks(network, stations);
    return 0;
}
