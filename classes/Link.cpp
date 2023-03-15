//
// Created by marco-vb on 15-03-2023.
//

#include "Link.h"

#include <utility>

Link::Link(std::shared_ptr<Station> src, std::shared_ptr<Station> dest, int capacity, int service) {
    this->src = std::move(src);
    this->dest = std::move(dest);
    this->capacity = capacity;
    this->service = service;
}

std::shared_ptr<Station> Link::getSrc() {
    return src;
}

std::shared_ptr<Station> Link::getDest() {
    return dest;
}

int Link::getCapacity() {
    return capacity;
}

int Link::getService() {
    return service;
}

void Link::setCapacity(int _capacity) {
    Link::capacity = _capacity;
}

void Link::setService(int _service) {
    Link::service = _service;
}
