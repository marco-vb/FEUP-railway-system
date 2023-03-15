//
// Created by marco-vb on 15-03-2023.
//

#ifndef RAILWAYS_LINK_H
#define RAILWAYS_LINK_H

#include <bits/stdc++.h>
#include "Station.h"

class Link {
private:
    std::shared_ptr<Station> src;
    std::shared_ptr<Station> dest;
    int capacity;
    int service;    // 1 Standard, 2 Pendular

public:
    Link(std::shared_ptr<Station> src, std::shared_ptr<Station> dest, int capacity, int service);
    std::shared_ptr<Station> getSrc();
    std::shared_ptr<Station> getDest();
    int getCapacity();
    int getService();
    void setCapacity(int capacity);
    void setService(int service);
};


#endif //RAILWAYS_LINK_H
