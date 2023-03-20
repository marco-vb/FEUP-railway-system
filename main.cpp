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
        int srvc = service == "STANDARD" ? STANDARD : PENDULAR;
        network->addLink(stations.at(st1), stations.at(st2), capacity, srvc);
    }
    file.close();
}

int main() {
    auto network = make<Network>();
    std::unordered_map<std::string, ptr<Station>> stations;
    readStations(network, stations);
    readLinks(network, stations);

    auto st1 = stations.at("Porto Camapnhã");
    auto st2 = stations.at("Vila Nova de Gaia-Devesas");
    std::cout << "Max flow between " << st1->getName() << " and " << st2->getName() << ": " << network->maxFlow(st1, st2) << std::endl;
    return 0;
}



/**
*   Considerações sobre as perguntas feitas:
 *   só usar o Edmonds-Karp
 *   só há 532 estações, por isso podemos recorrer a complete search para ver todos os pares
 *   pergunta estranha, mas suponho que temos de ver as linhas com mais capacidade e respetivas estacoes
 *   assumindo que 1 estaçao tem no maximo 2 'links' a apontar para ela, é a capacidade minima dos dois
 *   weird af this one
 *   ??????????
 *   ok bro
*/