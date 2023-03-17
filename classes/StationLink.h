#ifndef RAILWAYS_STATIONLINK_H
#define RAILWAYS_STATIONLINK_H

#include <bits/stdc++.h>

#define ptr std::shared_ptr
#define make std::make_shared

class Station;

class Link {
protected:
    ptr<Station> src;
    ptr<Station> dest;
    int capacity;
    int service;    // 1 Standard, 2 Pendular

public:
    Link(ptr<Station> src, ptr<Station> dest, int capacity, int service);
    ptr<Station> getSrc();
    ptr<Station> getDest();
    int getCapacity() const;
    int getService() const;
    void setCapacity(int capacity);
    void setService(int service);
};

class Station {
protected:
    int id;
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
};


#endif //RAILWAYS_STATIONLINK_H
