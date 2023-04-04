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
        st2->addLink(link);
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

bool Network::linkExists(const ptr<Station> &st1, const ptr<Station> &st2) {
    for (auto &l : st1->getLinks()) if (l->getDest() == st2 || l->getSrc() == st2) return true;

    return false;
}

unsigned int Network::maxFlow(const ptr<Station> &src, const ptr<Station> &dest) {
    int max_flow = 0;

    for (auto &l : links) l->setFlowSrc(0), l->setFlowDest(0);

    std::vector<ptr<Station>> path;

    while (getAugmentingPath(src, dest, path)) {
        int flow = getBottleneck(path);
        max_flow += flow;
        updatePath(path, flow);
        path.clear();
    }

    return max_flow;
}

bool Network::getAugmentingPath(const ptr<Station> &src, const ptr<Station> &dest, std::vector<ptr<Station>> &path) {
    for (auto &s : stations) s->setVisited(false), s->setParent(nullptr);

    std::queue<ptr<Station>> q;
    q.push(src);
    src->setVisited(true);

    while (!q.empty()) {
        auto s = q.front(); q.pop();

        if (s == dest) break;
        if (!s->getEnabled()) continue;

        for (auto &l : s->getLinks()) {
            if (!l->getEnabled()) continue;
            if (s == l->getSrc()) {
                auto w = l->getDest();
                if (!w->isVisited() && l->getFlowSrc() + l->getFlowDest() < l->getCapacity()) {
                    w->setVisited(true);
                    w->setParent(s);
                    q.push(w);
                }
            }
            else if (s == l->getDest()) {
                auto w = l->getSrc();
                if (!w->isVisited() && l->getFlowSrc() + l->getFlowDest() < l->getCapacity()) {
                    w->setVisited(true);
                    w->setParent(s);
                    q.push(w);
                }
            }
        }
    }

    if (!dest->isVisited()) return false;

    auto s = dest;
    while (s != nullptr) {
        path.push_back(s);
        s = s->getParent();
    }
    std::reverse(path.begin(), path.end());

    return true;
}

int Network::getBottleneck(const std::vector<std::shared_ptr<Station>> &path) {
    int bottleneck = INT_MAX;

    for (int i = 0; i < path.size() - 1; i++) {
        const auto& s = path[i];
        const auto& t = path[i + 1];

        for (auto &l : s->getLinks()) {
            if ((l->getSrc() == s && l->getDest() == t) || (l->getSrc() == t && l->getDest() == s))
                bottleneck = std::min(bottleneck, l->getCapacity() - l->getFlowSrc() - l->getFlowDest());
        }
    }

    return bottleneck;
}

void Network::updatePath(const std::vector<std::shared_ptr<Station>> &path, int flow) {
    for (int i = 0; i < path.size() - 1; i++) {
        const auto& s = path[i];
        const auto& t = path[i + 1];

        for (auto &l : s->getLinks()) {
            if (l->getSrc() == s && l->getDest() == t) {
                l->setFlowSrc(l->getFlowSrc() + flow);
            }
            else if (l->getSrc() == t && l->getDest() == s) {
                l->setFlowDest(l->getFlowDest() + flow);
            }
        }
    }
}

unsigned int Network::getMaxFlowNetwork(std::vector<std::pair<ptr<Station>, ptr<Station>>>& pairs) {
    unsigned int max_flow = 0;
    std::sort(links.begin(), links.end(), [](ptr<Link>& l1, ptr<Link>& l2) { return l1->getCapacity() > l2->getCapacity(); });

    ptr<Station> src = links.front()->getSrc();

    for (auto &s : stations) {
        if (s == src) continue;
        unsigned int flow = maxFlow(src, s);
        if (flow > max_flow) {
            max_flow = flow;
            pairs.clear();
            pairs.emplace_back(src, s);
        }
        else if (flow == max_flow) pairs.emplace_back(src, s);
    }

    for (int i = 0; i < (int) stations.size() - 1; i++) {
        ptr<Station> s1 = stations[i];
        if (s1 == src || s1->maxPossibleFlow() < max_flow) continue;

        for (int j = i + 1; j < (int) stations.size(); j++) {
            ptr<Station> s2 = stations[j];
            if (s2 == src || s2->maxPossibleFlow() < max_flow) continue;

            unsigned int flow = maxFlow(stations[i], stations[j]);

            if (flow > max_flow) {
                max_flow = flow;
                pairs.clear();
                pairs.emplace_back(stations[i], stations[j]);
            }
            else if (flow == max_flow) pairs.emplace_back(stations[i], stations[j]);
        }
    }

    return max_flow;
}

unsigned int Network::maxTrains(const std::shared_ptr<Station> &sink) {
    if (sink->getLinks().size() == 1) return sink->getLinks().front()->getCapacity();

    std::vector<ptr<Station>> sources;
    for (auto &s : stations) if (s->getLinks().size() == 1) sources.push_back(s);

    ptr<Station> super_source = createSuperSource(sources);
    unsigned int max_trains = maxFlow(super_source, sink);
    removeSuperSource(super_source);

    return max_trains;
}

ptr<Station> Network::createSuperSource(const std::vector<std::shared_ptr<Station>> &sources) {
    ptr<Station> super_source = make<Station>(-1, "N/A", "N/A", "N/A", "N/A");
    stations.push_back(super_source);

    for (auto &s : sources) addLink(super_source, s, 10000000, STANDARD);

    return super_source;
}

void Network::removeSuperSource(std::shared_ptr<Station> &superSource) {
    for (auto it = links.begin(); it != links.end();)
        if ((*it)->getSrc() == superSource || (*it)->getDest() == superSource) it = links.erase(it);
        else it++;

    for (auto it = stations.begin(); it != stations.end();)
        if (*it == superSource) it = stations.erase(it);
        else it++;
}

unsigned int Network::maxCost(const ptr<Station> &src, const ptr<Station> &dest) {
    int max_cost = 0;

    for (auto &l : links) {
        l->setFlowSrc(0), l->setFlowDest(0);
        if (l->getService() == PENDULAR) l->setEnabled(false);
    }

    std::vector<ptr<Station>> path;

    while (getAugmentingPath(src, dest, path)) {  // path with ONLY standard service links
        int flow = getBottleneck(path);
        max_cost += flow * STANDARD_COST * (path.size() - 1);
        updatePath(path, flow);
        path.clear();
    }

    for (auto &l : links) l->setEnabled(true);

    while (getAugmentingPath(src, dest, path)) { // path with all service links
        int flow = getBottleneck(path);
        for (int i = 0; i < path.size() - 1; i++) {
            const auto& s = path[i];
            const auto& t = path[i + 1];

            for (auto &l : s->getLinks()) {
                if ((l->getSrc() == s && l->getDest() == t) || (l->getSrc() == t && l->getDest() == s)) {
                    if (l->getService() == STANDARD) max_cost += flow * STANDARD_COST;
                    else if (l->getService() == PENDULAR) max_cost += flow * PENDULAR_COST;
                    break;
                }
            }
        }
        updatePath(path, flow);
        path.clear();
    }

    return max_cost;
}

unsigned int Network::maxFlowReduced(const std::shared_ptr<Station> &src, const std::shared_ptr<Station> &dest) {
    sort(stations.begin(), stations.end(), [](const ptr<Station>& s1, const ptr<Station>& s2) { return s1->getLinks().size() > s2->getLinks().size(); });
    for (auto &s : stations) {
        std::cout << s->getName() << " " << s->getLinks().size() << std::endl;
    }
}

