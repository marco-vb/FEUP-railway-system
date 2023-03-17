#include "Network.h"

Network::Network() {
    this->stations = std::vector<ptr<Station>>();
    this->links = std::vector<ptr<Link>>();
}

bool Network::addStation(const ptr<Station>& station) {
    for (auto &s : stations)
        if (s->getId() == station->getId()) return false;

    stations.push_back(station);
    return true;
}

void Network::addLink(const ptr<Station>& st1, const ptr<Station>& st2, int capacity, int service) {
    if (!linkExists(st1, st2)) {
        auto link = make<Link>(st1, st2, capacity, service);
        links.push_back(link);
        st1->addLink(link);
    }

    if (!linkExists(st2, st1)) {
        auto reverse = make<Link>(st2, st1, capacity, service);
        links.push_back(reverse);
        st2->addLink(reverse);
    }
}

ptr<Station> Network::getStation(int id) {
    for (auto &s : stations) {
        if (s->getId() == id) {
            return s;
        }
    }
    return nullptr;
}

ptr<Station> Network::getStation(const std::string& name) {
    for (auto &s : stations) {
        if (s->getName() == name) {
            return s;
        }
    }
    return nullptr;
}

std::vector<ptr<Station>> Network::getStations() {
    return stations;
}

std::vector<ptr<Link>> Network::getLinks() {
    return links;
}

bool Network::linkExists(const std::shared_ptr<Station> &st1, const std::shared_ptr<Station> &st2) {
    for (auto &l : st1->getLinks()) if (l->getDest() == st2) return true;

    return false;
}
