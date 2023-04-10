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

/**
 * @brief Link class
 *
 * @details This class represents a link in the railway network between two stations
 */
class Link {
protected:

    /**
     * @brief Source station
     */
    ptr<Station> src;

    /**
     * @brief Destination station
     */
    ptr<Station> dest;

    /**
     * @brief Link capacity
     */
    int capacity;

    /**
     * @brief Link service
     *
     * @details 1 - Standard, 2 - Pendular
     */
    int service;

    /**
     * @brief Link flow
     *
     * @details Flow from source to destination
     */
    int flow = 0;

    /**
     * @brief Link Enabled status
     */
    bool enabled = true;

    /**
     * @brief Reverse link
     */
    ptr<Link> reverse = nullptr; // reverse link

public:

    /**
     * @brief Link constructor
     * @param src Source station
     * @param dest Destination station
     * @param capacity Link capacity
     * @param service Link service
     *
     * @return Link object
     *
     * @details This constructor creates a Link object with the given parameters.
     */
    Link(ptr<Station> src, ptr<Station> dest, int capacity, int service);

    /**
     * @brief Get Source Station
     *
     * @details This function returns the source station of the link.
     */
    ptr<Station> getSrc();

    /**
     * @brief Get Destination Station
     *
     * @details This function returns the destination station of the link.
     */
    ptr<Station> getDest();

    /**
     * @brief Get Link Capacity
     *
     * @details This function returns the capacity of the link.
     */
    int getCapacity() const;

    /**
     * @brief Get Link Service
     *
     * @details This function returns the service of the link.
     */
    int getService() const;

    /**
     * @brief Get Link Flow
     *
     * @details This function returns the flow of the link.
     */
    int getFlow() const;

    /**
     * @brief Get Link Cost
     *
     * @details This function returns the cost of the link.
     */
    int getCost() const;

    /**
     * @brief Get Link Enabled status
     *
     * @details This function returns the enabled status of the link.
     */
    bool isEnabled() const;

    /**
     * @brief Get Reverse Link
     *
     * @details This function returns the reverse link of the link.
     */
    ptr<Link> getReverse();

    /**
     * @brief Set Link Capacity
     *
     * @details This function sets the capacity of the link.
     */
    void setCapacity(int capacity);

    /**
     * @brief Set Link Service
     *
     * @details This function sets the service of the link.
     */
    void setService(int service);

    /**
     * @brief Set Link Flow
     *
     * @details This function sets the flow of the link.
     */
    void setFlow(int flow);

    /**
     * @brief Set Link Enabled status
     *
     * @details This function sets the enabled status of the link.
     */
    void setEnabled(bool enabled);

    /**
     * @brief Set Reverse Link
     *
     * @details This function sets the reverse link of the link.
     */
    void setReverse(const ptr<Link>& reverse);
};

/**
 * @brief Station class
 *
 * @details This class represents a station in the railway network
 */
class Station {
protected:

    /**
     * @brief Station id
     */
    int id;


    /**
     * @brief Station name, district, municipality and township
     */
    std::string name, district, municipality, township;

    /**
     * @brief Station Visited status
     */
    bool visited = false;

    /**
     * @brief Station Enabled status
     */
    bool enabled = true;

    /**
     * @brief Station Path
     */
    ptr<Link> path = nullptr;

    /**
     * @brief Station Cost
     */
    int cost = 0;

    /**
     * @brief List of Station Links
     */
    std::list<ptr<Link>> links;

public:
    /**
     * @brief Station constructor
     * @param id Station id
     * @param name Station name
     * @param municipality Station municipality
     * @param township Station township
     * @param district Station district
     *
     * @return Station object
     *
     * @details This constructor creates a Station object with the given parameters.
     */
    Station(int id, std::string name, std::string municipality, std::string township, std::string district);

    /**
     * @brief Get Id method
     *
     * @return Station id
     *
     * @details This method gets the id of the station
     */
    int getId() const;

    /**
     * @brief Add Link method
     *
     * @param link Link to be added
     *
     * @details This method adds a link between the station and the destination station
     */
    void addLink(const ptr<Link>& link);

    /**
     * @brief Get Name method
     *
     * @return Station name
     *
     * @details This method gets the name of the station
     */
    std::string getName();

    /**
     * @brief Get District method
     *
     * @return Station district
     *
     * @details This method gets the district of the station
     */
    std::string getDistrict();

    /**
     * @brief Get Municipality method
     *
     * @return Station municipality
     *
     * @details This method gets the municipality of the station
     */
    std::string getMunicipality();

    /**
     * @brief Get Township method
     *
     * @return Station township
     *
     * @details This method gets the township of the station
     */
    std::string getTownship();

    /**
     * @brief Get Links method
     *
     * @return List of links
     *
     * @details This method gets the list of links of the station
     */
    std::list<ptr<Link>> getLinks();

    /**
     * @brief Get Cost method
     *
     * @return Cost of the station
     *
     * @details This method gets the cost of the station
     */
    int getCost() const;

    /**
     * @brief Set Cost method
     *
     * @param cost Cost of the station
     *
     * @details This method sets the cost of the station
     */
    void setCost(int cost);

    /**
     * @brief Is Enabled method
     *
     * @return True if the station is enabled, false otherwise
     *
     * @details This method checks if the station is enabled
     */
    bool isEnabled() const;

    /**
     * @brief Set Enabled method
     *
     * @param enabled True if the station is enabled, false otherwise
     *
     * @details This method sets the station as enabled or disabled
     */
    void setEnabled(bool enabled);

    /**
     * @brief Is Visited method
     *
     * @return True if the station is visited, false otherwise
     *
     * @details This method checks if the station is visited
     */
    bool isVisited() const;

    /**
     * @brief Set Visited method
     *
     * @param visited True if the station is visited, false otherwise
     *
     * @details This method sets the station as visited or not visited
     */
    void setVisited(bool visited);

    /**
     * @brief Get Path method
     *
     * @return Path to the station
     *
     * @details This method gets the path to the station
     */
    ptr<Link> getPath();

    /**
     * @brief Set Path method
     *
     * @param path Path to the station
     *
     * @details This method sets the path to the station
     */
    void setPath(const ptr<Link>& path);

    /**
     * @brief Max Possible Flow Method
     *
     * @return Max possible flow
     *
     * @details This method gets the max possible flow of the station. This function as a Complexity of O(n)
     */
    unsigned int maxPossibleFlow();

    /**
     * @brief Remove Link method
     *
     * @param link Link to be removed
     *
     * @details This method removes the link
     */
    void removeLink(const ptr<Link>& link);
};


#endif //RAILWAYS_STATIONLINK_H
