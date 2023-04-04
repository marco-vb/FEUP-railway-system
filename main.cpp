#include <bits/stdc++.h>
#include "classes/Network.h"

auto network = make<Network>();
std::unordered_map<std::string, ptr<Station>> stations;
std::unordered_map<std::string, int> municipality_capacities;

void readStations() {
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
        municipality_capacities[municipality] = 0;
    }
    file.close();
}

void readLinks() {
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
        int srvc = service == "STANDARD" ? STANDARD : PENDULAR;
        network->addLink(stations.at(st1), stations.at(st2), capacity, srvc);
        std::string municipality1 = stations.at(st1)->getMunicipality();
        std::string municipality2 = stations.at(st2)->getMunicipality();
        if (municipality1 != municipality2) {
            municipality_capacities[municipality1] += capacity;
            municipality_capacities[municipality2] += capacity;
        }
        else {
            municipality_capacities[municipality1] += capacity;
        }
    }
    file.close();
}

int main() {
    readStations();
    readLinks();

    auto st1 = stations.at("Porto Campanhã");
    auto st2 = stations.at("Vila Nova de Gaia-Devesas");

    // 2.1 exemplo
    std::cout << "Max flow between " << st1->getName() << " and " << st2->getName() << ": " << network->maxFlow(st1, st2) << std::endl;

    // 2.2 exemplo
    std::vector<std::pair<ptr<Station>, ptr<Station>>> pairs;
    std::cout << "Max network flow: " << network->getMaxFlowNetwork(pairs) << std::endl;
    for (const auto& pair : pairs) {
        std::cout << pair.first->getName() << " -> " << pair.second->getName() << std::endl;
    }

    // 2.3 exemplo
    std::priority_queue<std::pair<int, std::string>> pq;
    for (const auto& pair : municipality_capacities) pq.push({pair.second, pair.first});

    std::cout << "Municipalities with the most capacity:" << std::endl;
    int n = 5; //std::cin >> n;
    while (n--) {
        auto pair = pq.top(); pq.pop();
        std::cout << pair.second << " (" << pair.first << ")" << std::endl;
    }

    // 2.4 exemplo
    std::cout << "Max trains that can arrive at " << st1->getName() << ": " << network->maxTrains(st1) << std::endl;

    // 3.1 exemplo
    std::cout << "Max cost between " << st1->getName() << " and " << st2->getName() << ": " << network->maxCost(st1, st2) << std::endl;

    return 0;
}
