#include "Network.h"

Network::Network() {
    this->stations = vec<ptr<Station>>();
    this->links = vec<ptr<Link>>();
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
        auto rev = make<Link>(st2, st1, capacity, service);
        link->setReverse(rev); rev->setReverse(link);
        links.push_back(link); links.push_back(rev);
        st1->addLink(link); st2->addLink(rev);
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

vec<ptr<Station>> Network::getStations() {
    return stations;
}

vec<ptr<Link>> Network::getLinks() {
    return links;
}

bool Network::linkExists(const ptr<Station> &st1, const ptr<Station> &st2) {
    for (auto &l : st1->getLinks()) if (l->getDest() == st2) return true;
    return false;
}

unsigned int Network::maxFlow(const ptr<Station> &src, const ptr<Station> &dest) {
    int max_flow = 0;

    for (auto &l : links) l->setFlow(0);

    vec<ptr<Link>> path;

    while (getAugmentingPath(src, dest)) {
        int flow = getBottleneck(src, dest);
        max_flow += flow;
        updatePath(src, dest, flow, nullptr);
    }

    return max_flow;
}

bool Network::getAugmentingPath(const ptr<Station> &src, const ptr<Station> &dest) {
    for (auto &s : stations) s->setVisited(false), s->setPath(nullptr);

    std::queue<ptr<Station>> q;
    q.push(src);
    src->setVisited(true);

    while (!q.empty() && !dest->isVisited()) {
        auto s = q.front(); q.pop();

        if (!s->isEnabled()) continue;

        for (auto &l : s->getLinks()) {
            if (!l->isEnabled()) continue;
            auto w = l->getDest();
            if (!w->isVisited() && l->getFlow() < l->getCapacity()) {
                w->setVisited(true);
                w->setPath(l);
                q.push(w);
            }
        }

        for (auto &_l : s->getLinks()) {
            auto l = _l->getReverse();
            if (!l->isEnabled()) continue;
            auto w = l->getDest();
            if (!w->isVisited() && l->getFlow() > 0) {
                w->setVisited(true);
                w->setPath(l);
                q.push(w);
            }
        }
    }
    return dest->isVisited();
}

int Network::getBottleneck(const ptr<Station> &src, const ptr<Station> &dest) {
    int bottleneck = INT_MAX;
    auto s = dest;

    while (s != src) {
        auto l = s->getPath();
        if (s == l->getDest()) {
            bottleneck = std::min(bottleneck, l->getCapacity() - l->getFlow());
            s = l->getSrc();
        }
        else {
            bottleneck = std::min(bottleneck, l->getFlow());
            s = l->getDest();
        }
    }

    return bottleneck;
}

void Network::updatePath(const ptr<Station> &source, const ptr<Station> &dest, int flow, unsigned int *cost) {
    auto s = dest;

    while (s != source) {
        auto l = s->getPath();
        if (s == l->getDest()) {
            l->setFlow(l->getFlow() + flow);
            s = l->getSrc();
        }
        else {
            l->setFlow(l->getFlow() - flow);
            s = l->getDest();
        }
        if (cost) {
            if (l->getService() == STANDARD) *cost += STANDARD_COST * flow;
            else *cost += PENDULAR_COST * flow;
        }
    }
}

//check this
unsigned int Network::getMaxFlowNetwork(vec<std::pair<ptr<Station>, ptr<Station>>>& pairs) {
    unsigned int max_flow = 0;
    std::sort(stations.begin(), stations.end(), [](ptr<Station>& s1, ptr<Station>& s2) { return s1->maxPossibleFlow() > s2->maxPossibleFlow(); });

    for (int i = 0; i < (int) stations.size() - 1; i++) {
        ptr<Station> s1 = stations[i];
        if (s1->maxPossibleFlow() < max_flow) continue;

        for (int j = i + 1; j < (int) stations.size(); j++) {
            ptr<Station> s2 = stations[j];
            if (s2->maxPossibleFlow() < max_flow) continue;

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

unsigned int Network::maxTrains(const ptr<Station> &sink) {
    vec<ptr<Station>> sources;
    for (auto &s : stations) {
        if (s->getId() == sink->getId()) continue;
        if (s->getLinks().size() == 1) sources.push_back(s);
    }

    ptr<Station> super_source = make<Station>(-1, "N/A", "N/A", "N/A", "N/A");
    stations.push_back(super_source);

    for (auto &s : sources) addLink(super_source, s, 10000000, STANDARD);

    unsigned int max_trains = maxFlow(super_source, sink);
    removeSuperSource(super_source);

    return max_trains;
}

void Network::removeSuperSource(ptr<Station> &superSource) {
    for (auto &l : superSource->getLinks()) {
        auto s = l->getDest();
        s->removeLink(l->getReverse());
        s->removeLink(l);
        links.erase(std::find(links.begin(), links.end(), l));
    }
    stations.erase(std::find(stations.begin(), stations.end(), superSource));
}

unsigned int Network::maxCost(const ptr<Station> &src, const ptr<Station> &dest) {
    unsigned int max_cost = 0;

    for (auto &l : links) l->setFlow(0);

    while (getAugmentingPathWithCosts(src, dest)) { // path with all service links
        int flow = getBottleneck(src, dest);
        updatePath(src, dest, flow, &max_cost);
    }

    return max_cost;
}

unsigned int Network::maxFlowReduced(const ptr<Station> &src, const ptr<Station> &dest, const vec<ptr<Station>> &_stations, const vec<ptr<Link>> &_links) {
    for (auto &s : _stations) s->setEnabled(false);
    for (auto &l : _links) l->setEnabled(false);

    unsigned int max_flow = maxFlow(src, dest);

    for (auto &s : _stations) s->setEnabled(true);
    for (auto &l : _links) l->setEnabled(true);

    return max_flow;
}

bool Network::getAugmentingPathWithCosts(const ptr<Station> &src, const ptr<Station> &dest) {
    for (auto &s : stations) s->setVisited(false), s->setPath(nullptr);

    std::priority_queue<ptr<Station>, vec<ptr<Station>>, std::function<bool(ptr<Station>, ptr<Station>)>> pq(
            [](const ptr<Station> &s1, const ptr<Station> &s2) { return s1->getCost() < s2->getCost(); });

    pq.push(src);
    src->setVisited(true);

    while (!pq.empty() && !dest->isVisited()) {
        ptr<Station> s = pq.top(); pq.pop();

        for (auto &l : s->getLinks()) {
            auto w = l->getDest();
            if (!w->isVisited() && l->getFlow() < l->getCapacity()) {
                w->setVisited(true);
                w->setPath(l);
                w->setCost(l->getCost());
                pq.push(w);
            }
        }

        for (auto &_l : s->getLinks()) {
            auto l = _l->getReverse();
            auto w = l->getDest();
            if (!w->isVisited() && l->getFlow() > 0) {
                w->setVisited(true);
                w->setPath(l);
                w->setCost(l->getCost());
                pq.push(w);
            }
        }
    }
    return dest->isVisited();
}

void Network::topAffected(const std::shared_ptr<Link> &l_remove, std::vector<std::pair<int, int>> &ans) {
    vec<unsigned int> max_flows(stations.size());
    std::cout << "first loop" << std::endl;
    for (auto &s : stations) {
        auto max_flow = maxTrains(s);
        max_flows[s->getId()] = max_flow;
        if (s->getName() == "Porto Campanhã") std::cout << "Id: " << s->getId() << " " << s->getName() << " " << max_flow << std::endl;
    }

    vec<std::pair<int, int>> diffs(stations.size());

    l_remove->setEnabled(false); l_remove->getReverse()->setEnabled(false);
    for (auto &s : stations) {
        auto max_flow = maxTrains(s);
        diffs[s->getId()] = {max_flows[s->getId()] - max_flow, s->getId()};
        if (max_flows[s->getId()] - max_flow > 0) {
            std::cout << s->getName() << " was affected." << std::endl;
        }
    }
    l_remove->setEnabled(true); l_remove->getReverse()->setEnabled(true);

    std::sort(diffs.begin(), diffs.end(), std::greater<>());

    for (int i = 0; i < ans.size(); i++) {
        ans[i] = diffs[i];
    }
}

