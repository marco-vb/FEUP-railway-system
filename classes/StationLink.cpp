#include "StationLink.h"

Link::Link(ptr<Station> src, ptr<Station> dest, int capacity, int service) {
    this->src = std::move(src);
    this->dest = std::move(dest);
    this->capacity = capacity;
    this->service = service;
}

ptr<Station> Link::getSrc() {
    return src;
}

ptr<Station> Link::getDest() {
    return dest;
}

int Link::getCapacity() const {
    return capacity;
}

int Link::getService() const {
    return service;
}

void Link::setCapacity(int _capacity) {
    Link::capacity = _capacity;
}

void Link::setService(int _service) {
    Link::service = _service;
}

Station::Station(int id, std::string name, std::string municipality, std::string township, std::string district) {
    this->id = id;
    this->name = std::move(name);
    this->district = std::move(district);
    this->municipality = std::move(municipality);
    this->township = std::move(township);
}

std::string Station::getName() {
    return this->name;
}

std::string Station::getDistrict() {
    return this->district;
}

std::string Station::getMunicipality() {
    return this->municipality;
}

std::string Station::getTownship() {
    return this->township;
}

int Station::getId() const {
    return this->id;
}

bool Station::addLink(const ptr<Link>& link) {
    this->links.push_back(link);
}

std::vector<ptr<Link>> Station::getLinks() {
    return this->links;
}

void Station::setVisited(bool _visited) {
    this->visited = _visited;
}

bool Station::isVisited() const {
    return this->visited;
}

void Station::setParent(const ptr<Station>& _parent) {
    this->parent = _parent;
}

ptr<Station> Station::getParent() {
    return this->parent;
}

void Link::setFlowSrc(int flow) {
    this->flowSrc = flow;
}

void Link::setFlowDest(int flow) {
    this->flowDest = flow;
}

int Link::getFlowSrc() const {
    return this->flowSrc;
}

int Link::getFlowDest() const {
    return this->flowDest;
}

