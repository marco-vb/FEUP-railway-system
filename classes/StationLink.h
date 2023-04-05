#ifndef RAILWAYS_STATIONLINK_H
#define RAILWAYS_STATIONLINK_H

#include <bits/stdc++.h>

#define ptr std::shared_ptr
#define make std::make_shared
#define vec std::vector

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
    int flow = 0;       // flow from src to dest
    bool enabled = true;
    ptr<Link> reverse = nullptr; // reverse link

public:
    Link(ptr<Station> src, ptr<Station> dest, int capacity, int service);
    ptr<Station> getSrc();
    ptr<Station> getDest();
    int getCapacity() const;
    int getService() const;
    int getFlow() const;
    int getCost() const;
    bool isEnabled() const;
    ptr<Link> getReverse();
    void setCapacity(int capacity);
    void setService(int service);
    void setFlow(int flow);
    void setEnabled(bool enabled);
    void setReverse(const ptr<Link>& reverse);
};

class Station {
protected:
    int id;
    bool visited = false;
    bool enabled = true;
    ptr<Link> path = nullptr;
    int cost = 0;
    std::string name, district, municipality, township;
    std::list<ptr<Link>> links;

public:
    Station(int id, std::string name, std::string municipality, std::string township, std::string district);
    int getId() const;
    void addLink(const ptr<Link>& link);
    std::string getName();
    std::string getDistrict();
    std::string getMunicipality();
    std::string getTownship();
    std::list<ptr<Link>> getLinks();
    int getCost() const;
    void setCost(int cost);
    bool isEnabled() const;
    void setVisited(bool visited);
    bool isVisited() const;
    void setEnabled(bool enabled);
    ptr<Link> getPath();
    void setPath(const ptr<Link>& path);
    unsigned int maxPossibleFlow();
};


#endif //RAILWAYS_STATIONLINK_H
