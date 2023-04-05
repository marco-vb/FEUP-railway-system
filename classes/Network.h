#ifndef RAILWAYS_NETWORK_H
#define RAILWAYS_NETWORK_H

#include <bits/stdc++.h>
#include "StationLink.h"

class Network {
private:
    vec<ptr<Station>> stations;
    vec<ptr<Link>> links;

public:
    Network();
    bool addStation(const ptr<Station>& station);
    void addLink(const ptr<Station>& st1, const ptr<Station>& st2, int capacity, int service);
    static bool linkExists(const ptr<Station>& st1, const ptr<Station>& st2);
    ptr<Station> getStation(int id);
    ptr<Station> getStation(const std::string& name);
    vec<ptr<Station>> getStations();
    vec<ptr<Link>> getLinks();
    unsigned int maxFlow(const ptr<Station> &src, const ptr<Station> &dest);
    unsigned int maxCost(const ptr<Station> &src, const ptr<Station> &dest);
    unsigned int maxFlowReduced(const ptr<Station> &src, const ptr<Station> &dest, const vec<ptr<Station>> &_stations, const vec<ptr<Link>> &_links);
    bool getAugmentingPath(const ptr<Station> &src, const ptr<Station> &dest);
    bool getAugmentingPathWithCosts(const ptr<Station> &src, const ptr<Station> &dest);
    static int getBottleneck(const ptr<Station> &src, const ptr<Station> &dest);
    static void updatePath(const ptr<Station> &source, const ptr<Station> &dest, int flow, unsigned int *cost = nullptr);
    unsigned int getMaxFlowNetwork(vec<std::pair<ptr<Station>, ptr<Station>>>& pairs);
    unsigned int maxTrains(const ptr<Station>& sink);
    ptr<Station> createSuperSource(const vec<ptr<Station>>& sources);
    void removeSuperSource(ptr<Station>& superSource);
};


#endif //RAILWAYS_NETWORK_H
