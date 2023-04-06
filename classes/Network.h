#ifndef RAILWAYS_NETWORK_H
#define RAILWAYS_NETWORK_H



#include "StationLink.h"

class Network {
private:

    /**
     * @brief Vector of stations
     */
    vec<ptr<Station>> stations;

    /**
     * @brief Vector of links
     */
    vec<ptr<Link>> links;

public:

    /**
     * @brief Network Constructor
     */
    Network();

    /**
     * @brief Add Station to Network
     *
     * @param station Station to be added
     *
     * @return true if station was added successfully
     * @return false if station already exists
     *
     * @details Adds a station to the network
     *
     * @warning If station already exists, it will not be added
     */
    bool addStation(const ptr<Station>& station);

    /**
     * @brief Add Link to Network
     *
     * @param st1 Source station
     * @param st2 Destination station
     * @param capacity Link capacity
     * @param service Link service
     *
     * @return true if link was added successfully
     * @return false if link already exists
     *
     * @details Adds a link to the network
     *
     * @warning If link already exists, it will not be added
     */
    void addLink(const ptr<Station>& st1, const ptr<Station>& st2, int capacity, int service);

    /**
     * @brief Check if link exists
     *
     * @param st1 Source station
     * @param st2 Destination station
     *
     * @return true if link exists
     * @return false if link does not exist
     *
     * @details Checks if a link exists between two stations
     */
    static bool linkExists(const ptr<Station>& st1, const ptr<Station>& st2);

    /**
     * @brief Get Station
     *
     * @param id Station ID
     *
     * @return Station with ID
     *
     * @details Returns a station with a given ID
     */
    ptr<Station> getStation(int id);

    /**
     * @brief Get Station
     *
     * @param name Station name
     *
     * @return Station with name
     *
     * @details Returns a station with a given name
     */
    ptr<Station> getStation(const std::string& name);

    /**
     * @brief Get Stations
     *
     * @return Vector of stations
     *
     * @details Returns a vector of all stations in the network
     */
    vec<ptr<Station>> getStations();

    /**
     * @brief Get Links
     *
     * @return Vector of links
     *
     * @details Returns a vector of all links in the network
     */
    vec<ptr<Link>> getLinks();

    /**
     * @brief Get Max Flow
     *
     * @param src Source station
     * @param dest Destination station
     *
     * @return Max flow between src and dest
     *
     * @details Returns the maximum flow between two stations
     */
    unsigned int maxFlow(const ptr<Station> &src, const ptr<Station> &dest);

    /**
     * @brief Get Cost
     *
     * @param src Source station
     * @param dest Destination station
     *
     * @return Cost of flow between src and dest
     *
     * @details Returns the cost between two stations
     */
    unsigned int maxCost(const ptr<Station> &src, const ptr<Station> &dest);

    /**
     * @brief Get Max Flow Reduced
     *
     * @param src Source station
     * @param dest Destination station
     * @param _stations Vector of stations
     * @param _links Vector of links
     *
     * @return Max flow between src and dest
     *
     * @details Returns the maximum flow between two stations
     *
     * @warning This function is used for the reduced network
     */
    unsigned int maxFlowReduced(const ptr<Station> &src, const ptr<Station> &dest, const vec<ptr<Station>> &_stations, const vec<ptr<Link>> &_links);

    /**
     * @brief Get Augmenting Path
     *
     * @param src Source station
     * @param dest Destination station
     *
     * @return true if path exists
     * @return false if path does not exist
     *
     * @details Returns true if an augmenting path exists between two stations
     *
     * @warning This function is used for the Ford-Fulkerson algorithm
     */
    bool getAugmentingPath(const ptr<Station> &src, const ptr<Station> &dest);

    /**
     * @brief Get Augmenting Path with Costs
     *
     * @param src Source station
     * @param dest Destination station
     *
     * @return true if path exists
     * @return false if path does not exist
     *
     * @details Returns true if an augmenting path exists between two stations
     *
     * @warning This function gives priority to paths with lower costs in this case Standard Priority
     */
    bool getAugmentingPathWithCosts(const ptr<Station> &src, const ptr<Station> &dest);

    /**
     * @brief Get Bottleneck
     * @param src Source station
     * @param dest Destination station
     *
     * @return Bottleneck of path between src and dest
     *
     * @details Returns the bottleneck of the path between two stations
     */
    static int getBottleneck(const ptr<Station> &src, const ptr<Station> &dest);

    /**
     * @brief Update Path
     *
     * @param source Source station
     * @param dest Destination station
     * @param flow Flow to be added
     * @param cost Cost of flow
     *
     * @details Updates the path between two stations
     */
    static void updatePath(const ptr<Station> &source, const ptr<Station> &dest, int flow, unsigned int *cost = nullptr);

    /**
     * @brief Get Max Flow Network
     *
     * @param pairs Vector of pairs of stations
     *
     * @return Max flow network
     *
     * @details Returns the max flow network
     */
    unsigned int getMaxFlowNetwork(vec<std::pair<ptr<Station>, ptr<Station>>>& pairs);

    /**
     * @brief Get Max Trains
     *
     * @param sink Sink station
     *
     * @return Max trains
     *
     * @details Returns the max trains that can be sent to a sink station from all sources in the network
     */
    unsigned int maxTrains(const ptr<Station>& sink);

    /**
     * @brief Create Super Source
     *
     * @param sources Vector of sources stations
     *
     * @return Super source
     *
     * @details Creates a super source from a vector of source stations and adds it to the network
     */
    ptr<Station> createSuperSource(const vec<ptr<Station>>& sources);

    /**
     * @brief Remove Super Source
     *
     * @param superSource Super source
     *
     * @details Removes a super source from the network
     */
    void removeSuperSource(ptr<Station>& superSource);
};


#endif //RAILWAYS_NETWORK_H
