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
};


#endif //RAILWAYS_NETWORK_H
