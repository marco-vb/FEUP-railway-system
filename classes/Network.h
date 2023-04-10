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
     *
     * @details Creates a new network
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
     * @details Adds a station to the network.
     * This function as Complexity O(n) where n is the number of stations
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
     * @details Adds a link to the network. This function has Complexity O(1)
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
     * @details Checks if a link exists between two stations. This function has Complexity O(n)
     */
    static bool linkExists(const ptr<Station>& st1, const ptr<Station>& st2);

    /**
     * @brief Get Station
     *
     * @param id Station ID
     *
     * @return Station with ID
     *
     * @details Returns a station with a given ID. This function has Complexity O(n)
     */
    ptr<Station> getStation(int id);

    /**
     * @brief Get Station
     *
     * @param name Station name
     *
     * @return Station with name
     *
     * @details Returns a station with a given name. This function has Complexity O(n)
     */
    ptr<Station> getStation(const std::string& name);

    /**
     * @brief Get Stations
     *
     * @return Vector of stations
     *
     * @details Returns a vector of all stations in the network.
     */
    vec<ptr<Station>> getStations();

    /**
     * @brief Get Links
     *
     * @return Vector of links
     *
     * @details Returns a vector of all links in the network.
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
     * @details Returns the maximum flow between two stations.
     * This function has Complexity O(VE^2) where V is the number of vertices and E is the number of edges.
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
     * @details Returns the cost between two stations.
     * This function has Complexity O(V(E^2)log(V)) where V is the number of vertices and E is the number of edges.
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
     * @details Returns the maximum flow between two stations.
     * This function has Complexity O(VE^2) where V is the number of vertices and E is the number of edges.
     *
     * @warning This function is used for the reduced network.
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
     * @details Returns true if an augmenting path exists between two stations.
     * This function has Complexity O(V + E) where V is the number of vertices and E is the number of edges
     *
     * @warning This function is used for the Ford-Fulkerson algorithm.
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
     * @details Returns true if an augmenting path exists between two stations.
     * This function has Complexity O((V + E)log(V)) where V is the number of vertices and E is the number of edges
     *
     * @warning This function also gets an augmenting path in the network, however it first greedily chooses the standard service edges, and only then considers the alfa edges.
     */
    bool getAugmentingPathWithCosts(const ptr<Station> &src, const ptr<Station> &dest);

    /**
     * @brief Get Bottleneck
     * @param src Source station
     * @param dest Destination station
     *
     * @return Bottleneck of path between src and dest
     *
     * @details The path info is stored in each station with the parameter 'path': each stations saves the edge that 'discovered' it.
     * The time complexity is O(n), where n is the number of stations in the path.
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
     * @details Updates the path between two stations.
     * This function has Complexity O(n) where n is the number of stations in the path.
     */
    static void updatePath(const ptr<Station> &source, const ptr<Station> &dest, int flow, unsigned int *cost = nullptr);

    /**
     * @brief Get Max Flow Network
     *
     * @param pairs Vector of pairs of stations
     *
     * @return Max flow network
     *
     * @details Returns the max flow network.
     * This function has Complexity O(V^3 * E^2), as we need to run Edmonds-Karp algorithm for each pair of stations.
     * The time complexity of the Edmonds-Karp algorithm is O(V * E^2), where V is the number of vertices and E is the number of edges.
     * However, we start with the "most promising" stations (the ones that have more incoming capacity) in order to greatly prune our search space.
     * This technique is in fact efficient because we noticed a great improvement over the simple brute force (the running time improved from ~20s to ~0.12s)
     *
     * @warning This is FAST!!!
     */
    unsigned int getMaxFlowNetwork(vec<std::pair<ptr<Station>, ptr<Station>>>& pairs);

    /**
     * @brief Get Max Trains
     *
     * @param sink Sink station
     *
     * @return Max trains
     *
     * @details Returns the max trains that can be sent to a sink station from all sources in the network.
     * This function has Complexity O(VE^2) where V is the number of vertices and E is the number of edges.
     */
    unsigned int maxTrains(const ptr<Station>& sink);

    /**
     * @brief Create Super Source
     *
     * @param sources Vector of sources stations
     *
     * @return A pseudo Station which is a Super Source
     *
     * @details Creates a super source from a vector of source stations and adds it to the network.
     * This function as Complexity O(n) where n is the number of stations passed as parameter
     */
    void createSuperSource(ptr<Station> &ss, const vec<ptr<Station>> &sources);

    /**
     * @brief Remove Super Source
     *
     * @param superSource Super source
     *
     * @details Removes a super source from the network. This function as Complexity O(n) where n is size of the adjacency list of the super source
     */
    void removeSuperSource(ptr<Station>& superSource);

    /**
     * @brief Get k-top affected stations by the removal of a link
     *
     * @param l_remove Link to be removed
     * @param ans Vector of pairs of stations and the respective flow that would be lost
     *
     * @details This function returns the k-top affected stations by the removal of a link.
     * This function has Complexity O(V^2 * E^2) where V is the number of vertices and E is the number of edges.
     */
    void topAffected(const ptr<Link>& l_remove, vec<std::pair<int, int>> &ans);
};


#endif //RAILWAYS_NETWORK_H
