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

void Station::addLink(const ptr<Link>& link) {
    this->links.push_back(link);
}

std::list<ptr<Link>> Station::getLinks() {
    return this->links;
}

void Station::setVisited(bool _visited) {
    this->visited = _visited;
}

bool Station::isVisited() const {
    return this->visited;
}

unsigned int Station::maxPossibleFlow() {
    unsigned int maxFlow = 0;
    for (const auto& link : this->links) maxFlow += link->getCapacity();
    return maxFlow;
}

bool Station::isEnabled() const {
    return this->enabled;
}

void Station::setEnabled(bool _enabled) {
    this->enabled = _enabled;
}

ptr<Link> Station::getPath() {
    return this->path;
}

void Station::setPath(const ptr<Link>& _path) {
    this->path = _path;
}

int Station::getCost() const {
    return this->cost;
}

void Station::setCost(int _cost) {
    this->cost = _cost;
}

void Station::removeLink(const std::shared_ptr<Link> &link) {
    this->links.remove(link);
}

bool Link::isEnabled() const {
    return this->enabled;
}

void Link::setEnabled(bool _enabled) {
    this->enabled = _enabled;
}

void Link::setReverse(const ptr<Link>& _reverse) {
    this->reverse = _reverse;
}

ptr<Link> Link::getReverse() {
    return this->reverse;
}

int Link::getFlow() const {
    return this->flow;
}

void Link::setFlow(int _flow) {
    this->flow = _flow;
}

int Link::getCost() const {
    return this->service == STANDARD ? STANDARD_COST : PENDULAR_COST;
}
