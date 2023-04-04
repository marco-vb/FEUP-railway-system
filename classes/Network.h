#ifndef RAILWAYS_NETWORK_H
#define RAILWAYS_NETWORK_H

#include <bits/stdc++.h>
#include "StationLink.h"

class Network {
private:
    std::vector<ptr<Station>> stations;
    std::vector<ptr<Link>> links;

public:
    Network();
    bool addStation(const ptr<Station>& station);
    void addLink(const ptr<Station>& st1, const ptr<Station>& st2, int capacity, int service);
    static bool linkExists(const ptr<Station>& st1, const ptr<Station>& st2);
    ptr<Station> getStation(int id);
    ptr<Station> getStation(const std::string& name);
    std::vector<ptr<Station>> getStations();
    std::vector<ptr<Link>> getLinks();
    unsigned int maxFlow(const ptr<Station> &src, const ptr<Station> &dest);
    unsigned int maxCost(const ptr<Station> &src, const ptr<Station> &dest);
    bool getAugmentingPath(const ptr<Station> &src, const ptr<Station> &dest, std::vector<ptr<Station>> &path, bool standard = false);
    static int getBottleneck(const std::vector<ptr<Station>>& path);
    static void updatePath(const std::vector<ptr<Station>>& path, int flow);
    unsigned int getMaxFlowNetwork(std::vector<std::pair<ptr<Station>, ptr<Station>>>& pairs);
    unsigned int maxTrains(const ptr<Station>& sink);
    ptr<Station> createSuperSource(const std::vector<ptr<Station>>& sources);
    void removeSuperSource(ptr<Station>& superSource);
};


#endif //RAILWAYS_NETWORK_H
