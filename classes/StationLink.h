#ifndef RAILWAYS_STATIONLINK_H
#define RAILWAYS_STATIONLINK_H

#include <bits/stdc++.h>

#define ptr std::shared_ptr
#define make std::make_shared
#define STANDARD 1
#define PENDULAR 2

#define STANDARD_COST 2
#define PENDULAR_COST 4

class Station;

class Link {
protected:
    ptr<Station> src;
    ptr<Station> dest;
    int capacity;
    int service;    // 1 Standard, 2 Pendular
    int flowSrc;    // flow from src to dest
    int flowDest;   // flow from dest to src
    bool enabled = true;

public:
    Link(ptr<Station> src, ptr<Station> dest, int capacity, int service);
    ptr<Station> getSrc();
    ptr<Station> getDest();
    int getCapacity() const;
    int getService() const;
    int getFlowSrc() const;
    int getFlowDest() const;
    bool getEnabled() const;
    void setCapacity(int capacity);
    void setService(int service);
    void setFlowSrc(int flow);  // flow from src to dest
    void setFlowDest(int flow); // flow from dest to src
    void setEnabled(bool enabled);
};

class Station {
protected:
    int id;
    bool visited;
    bool enabled = true;
    ptr<Station> parent;
    std::string name, district, municipality, township;
    std::vector<ptr<Link>> links;

public:
    Station(int id, std::string name, std::string municipality, std::string township, std::string district);
    int getId() const;
    bool addLink(const ptr<Link>& link);
    std::string getName();
    std::string getDistrict();
    std::string getMunicipality();
    std::string getTownship();
    std::vector<ptr<Link>> getLinks();
    bool getEnabled() const;
    void setVisited(bool visited);
    bool isVisited() const;
    void setParent(const ptr<Station>& parent);
    void setEnabled(bool enabled);
    ptr<Station> getParent();
    unsigned int maxPossibleFlow();
};


#endif //RAILWAYS_STATIONLINK_H
