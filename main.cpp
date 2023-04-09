#include "classes/Network.h"

/**
 * @brief Network
 *
 * @details The network object
 */
auto network = make<Network>();

/**
 * @brief Stations
 *
 * @details Map of stations
 */
std::unordered_map<std::string, ptr<Station>> stations;

/**
 * @brief Municipality Capacities
 *
 * @details Map of municipality capacities
 */
std::unordered_map<std::string, int> municipality_capacities;

// Start Screen
void starting_screen();

// Specify the graph
void specify_graph();

// Menu Button 1
void train_analysis();

void station_capacity(); // Menu Button 1.1
void high_traffic_routes(); // Menu Button 1.2
void budget_allocation(); // Menu Button 1.3
void station_arrival_capacity(); // Menu Button 1.4

// Menu Button 2
void service_allocation();

void optimal_route(); // Menu Button 2.1

// Menu Button 3
void failure_forecasting();

void reduced_connectivity(); // Menu Button 3.1
void segment_failure_report(); // Menu Button 3.2

// Support Functions
void clear_screen();
void wait();

/**
 * @brief Reads the Stations
 *
 * @details Reads the stations from the stations.csv file and adds them to the network
 *
 * @warning The file must be in the data folder
 */
void readStations() {
    std::ifstream file("../data/stations.csv");
    std::string line;
    std::getline(file, line);
    int id = 0;
    stations.clear();
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, municipality, township, district;
        std::getline(ss, name, ',');
        std::getline(ss, district, ',');
        std::getline(ss, municipality, ',');
        std::getline(ss, township, ',');
        std::getline(ss, line);     // ignore this
        auto station = make<Station>(id++, name, municipality, township, district);
        network->addStation(station);
        stations[name] = station;
        municipality_capacities[municipality] = 0;
    }
    file.close();
}


/**
 * @brief Reads the Links
 *
 * @details Reads the links from the network.csv file and adds them to the network
 *
 * @warning The file must be in the data folder
 */
void readLinks() {
    std::ifstream file("../data/network.csv");
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string st1, st2, service;
        int capacity;
        std::getline(ss, st1, ',');
        std::getline(ss, st2, ',');
        ss >> capacity; ss.ignore();
        ss >> service;
        if (service[service.size()-1] == '\r') service = service.substr(0, service.size() - 1);
        int srvc = service == "STANDARD" ? STANDARD : PENDULAR;
        network->addLink(stations.at(st1), stations.at(st2), capacity, srvc);
        std::string municipality1 = stations.at(st1)->getMunicipality();
        std::string municipality2 = stations.at(st2)->getMunicipality();
        if (municipality1 != municipality2) {
            municipality_capacities[municipality1] += capacity;
            municipality_capacities[municipality2] += capacity;
        }
        else {
            municipality_capacities[municipality1] += capacity;
        }
    }
    file.close();
}

/**
 * @brief Main function
 *
 * @details Reads the stations and links from the files and runs the menu
 *
 * @return 0
 */
int main() {
/*

    system("Color 0C");

    int option;

    starting_screen();

    specify_graph();

    do{

        clear_screen();
        std::cout << "  ===========================================================================  " << std::endl;
        std::cout << "//                                                                           \\\\" << std::endl;
        std::cout << "||       #####    ####   ##  ##   ##        ##   ####   ##  ##   #####       ||" << std::endl;
        std::cout << "||       ##  ##  ##  ##  ##  ##   ##   ##   ##  ##  ##  ##  ##  ##           ||" << std::endl;
        std::cout << "||       #####   ######  ##  ##   ##   ##   ##  ######   ####    ####        ||" << std::endl;
        std::cout << "||       ##  ##  ##  ##  ##  ##    ## #### ##   ##  ##    ##        ##       ||" << std::endl;
        std::cout << "||       ##  ##  ##  ##  ##  ####   ###  ###    ##  ##    ##    #####        ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||      ####     ####   ####   ##    ##   ####    #####   ######  #####      ||" << std::endl;
        std::cout << "||      ## ##   ## ##  ##  ##  ####  ##  ##  ##  ##       ##      ##  ##     ||" << std::endl;
        std::cout << "||      ##  ## ##  ##  ######  ## ## ##  ######  ##  ###  ####    #####      ||" << std::endl;
        std::cout << "||      ##   ###   ##  ##  ##  ##  ####  ##  ##  ##   ##  ##      ##  ##     ||" << std::endl;
        std::cout << "||      ##         ##  ##  ##  ##    ##  ##  ##   #####   ######  ##  ##     ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||    [1] Train Analysis                                                     ||" << std::endl;
        std::cout << "||    [2] Service Allocation                                                 ||" << std::endl;
        std::cout << "||    [3] Failure Forecasting                                                ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                [0] Exit   ||" << std::endl;
        std::cout << "\\\\                                                                           //" << std::endl;
        std::cout << "  ===========================================================================  " << std::endl;

        std::cout << "  > ";
        std::cin >> option;

        switch(option){
            case 1:
                train_analysis();
                break;
            case 2:
                service_allocation();
                break;
            case 3:
                failure_forecasting();
                break;
            case 0:
                break;
            default:
                clear_screen();
                std::cout << " > Invalid Option!" << std::endl;
                std::cout << " > Press Enter to Continue...";
                wait();
                break;
        }

    }while(option != 0);
*/

    readStations();
    readLinks();

    auto st1 = stations.at("Vila Nova de Gaia-Devesas");
    auto st2 = stations.at("Lisboa Oriente");

    // 2.1 exemplo
    //std::cout << "Max flow between " << st1->getName() << " and " << st2->getName() << ": " << network->maxFlow(st1, st2) << std::endl;

    // 2.2 exemplo
/*    vec<std::pair<ptr<Station>, ptr<Station>>> pairs;
    std::cout << "Max network flow: " << network->getMaxFlowNetwork(pairs) << std::endl;
    for (const auto& pair : pairs) {
        std::cout << pair.first->getName() << " -> " << pair.second->getName() << std::endl;
    }*/

    // 2.3 exemplo
    /*std::priority_queue<std::pair<int, std::string>> pq;
    for (const auto& pair : municipality_capacities) pq.push({pair.second, pair.first});

    std::cout << "Municipalities with the most capacity:" << std::endl;
    int n = 5; //std::cin >> n;
    while (n--) {
        auto pair = pq.top(); pq.pop();
        std::cout << pair.second << " (" << pair.first << ")" << std::endl;
    }*/

    // 2.4 exemplo
    std::cout << "Max trains that can arrive at " << st1->getName() << ": " << network->maxTrains(st1) << std::endl;

    // 3.1 exemplo
    /*std::cout << "Max cost between " << st1->getName() << " and " << st2->getName() << ": \n" << network->maxCost(st1, st2) << std::endl;*/

    // 4.1 exemplo
    /* vec<ptr<Station>> remove_stations = {stations.at("Pombal")};
    vec<ptr<Link>> removed_links;

    std::cout << "Max flow between " << st1->getName() << " and " << st2->getName() << " in reduced network: ";
    std::cout << network->maxFlowReduced(st1, st2, remove_stations, removed_links) << std::endl;*/

    // 4.2 exemplo
    int k = 5; //std::cin >> k;
    vec<std::pair<unsigned int, int>> top_stations(5);
    network->topAffectedStations(k, st1, top_stations);
    for (const auto& p : top_stations) {
        std::cout << network->getStations()[p.second]->getName() << " (" << p.first << ")" << std::endl;
    }

    return 0;
}


void starting_screen(){

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||    ##        ##  ######  ##      #####    #####  ####     ####  ######    ||" << std::endl;
    std::cout << "||    ##   ##   ##  ##      ##     ##   ##  ##  ##  ## ##   ## ##  ##        ||" << std::endl;
    std::cout << "||    ##   ##   ##  ####    ##     ##       ##  ##  ##  ## ##  ##  ####      ||" << std::endl;
    std::cout << "||     ## #### ##   ##      ##     ##   ##  ##  ##  ##   ###   ##  ##        ||" << std::endl;
    std::cout << "||      ###  ###    ######  #####   #####   #####   ##         ##  ######    ||" << std::endl;
    std::cout << "||                                                                           ||" << std::endl;
    std::cout << "||                                            (###)     (#####)              ||" << std::endl;
    std::cout << "||                                     (##)     (#######)        (#######)   ||" << std::endl;
    std::cout << "||                               (#######)   (##R##)       (###########)     ||" << std::endl;
    std::cout << "||                          (###)     (#######)   (######)             (#7#) ||" << std::endl;
    std::cout << "||                     (##F###)    (######)                (#)               ||" << std::endl;
    std::cout << "||                 (###)  (####)           (##)                              ||" << std::endl;
    std::cout << "||              (##)              (###)                                      ||" << std::endl;
    std::cout << "||             .-.                                                           ||" << std::endl;
    std::cout << "||             ] [    .-.      _    .-----.                                  ||" << std::endl;
    std::cout << "||           .\"   \"\"\"\"   \"\"\"\"\"\" \"\"\"\"| .--`                                   ||" << std::endl;
    std::cout << "||          (:--:--:--:--:--:--:--:-| [___    .------------------------.     ||" << std::endl;
    std::cout << "||           |D&A  :  :  :  :  :  : [_5_] |'='|.----D------L------M---.|     ||" << std::endl;
    std::cout << "||          /|.___________________________|___|'--.___.--.___.--.___.-'|     ||" << std::endl;
    std::cout << "||         / ||_.--.______.--.______.--._ |---\\'--\\-.-/==\\-.-/==\\-.-/-'/--   ||" << std::endl;
    std::cout << "||        /__;^=(==)======(==)======(==)=^~^^^ ^^^^(-)^^^^(-)^^^^(-)^^^      ||" << std::endl;
    std::cout << "||  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ||" << std::endl;
    std::cout << "||                                                                           ||" << std::endl;
    std::cout << "||                      --- Press Enter to Continue ---                      ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    wait();

}


// Specify the graph
void specify_graph(){

    /**
     * @brief Specify Graph Menu
     *
     * @details This menu is used to specify the graph:
     *
     * [1] Complete Railway System - This button creates a complete graph.
     * [2] Partial Railway System - This button creates a subgraph.
     */


    int option;


    do{
        clear_screen();
        std::cout << "  ===========================================================================  " << std::endl;
        std::cout << "//                                                                           \\\\" << std::endl;
        std::cout << "||             #####  #####   ######   #####   ##  ######  ##  ##            ||" << std::endl;
        std::cout << "||            ##      ##  ##  ##      ##   ##  ##  ##      ##  ##            ||" << std::endl;
        std::cout << "||             ####   #####   ####    ##       ##  ####     ####             ||" << std::endl;
        std::cout << "||                ##  ##      ##      ##   ##  ##  ##        ##              ||" << std::endl;
        std::cout << "||            #####   ##      ######   #####   ##  ##        ##              ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||   ######  ##  ##  ######        #####    #####    ####   #####   ##  ##   ||" << std::endl;
        std::cout << "||     ##    ##  ##  ##           ###       ##  ##  ##  ##  ##  ##  ##  ##   ||" << std::endl;
        std::cout << "||     ##    ######  ####         ##  ####  #####   ######  #####   ######   ||" << std::endl;
        std::cout << "||     ##    ##  ##  ##           ##   ##   ##  ##  ##  ##  ##      ##  ##   ||" << std::endl;
        std::cout << "||     ##    ##  ##  ######        #####    ##  ##  ##  ##  ##      ##  ##   ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||    [1] Complete Railway System                                            ||" << std::endl;
        std::cout << "||    [2] Partial Railway System                                             ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "\\\\                                                                           //" << std::endl;
        std::cout << "  ===========================================================================  " << std::endl;

        std::cout << "  > ";
        std::cin >> option;

        switch (option) {
            case 1:
                readStations();
                readLinks();
                break;
            case 2:
                break;
            default:
                clear_screen();
                std::cout << "  > Invalid Option!" << std::endl;
                std::cout << "  > Press Enter to Continue..." << std::endl;
                wait();
                break;
        }

    }while(option != 1 && option != 2);



}


// Button 1 in the main menu
void train_analysis(){

    int option;

    /**
     * @brief Train Analysis Menu
     *
     * @details This menu is used to analyze the train network:
     *
     * [1] Station Capacity - This button calculates the maximum number of trains that can simultaneously travel between two specific stations.
     * [2] High Traffic Routes - This button determines, from all pairs of stations, which ones require the most amount of trains when taking full advantage of the existing network capacity.
     * [3] Budget Allocation - This button indicates where management should assign larger budgets for the purchasing and maintenance of trains. The implementation should be able to report the top-k municipalities and districts, regarding their transportation needs.
     * [4] Station Arrival Capacity - This button reports the maximum number of trains that can simultaneously arrive at a given station, taking into consideration the entire railway grid.
     *
     * [0] Go Back - This button returns to the main menu.
     *
     */

    do{
        clear_screen();
        std::cout << "  ===========================================================================  " << std::endl;
        std::cout << "//                                                                           \\\\" << std::endl;
        std::cout << "||                   ######  #####    ####   ##  ##    ##                    ||" << std::endl;
        std::cout << "||                     ##    ##  ##  ##  ##  ##  ####  ##                    ||" << std::endl;
        std::cout << "||                     ##    #####   ######  ##  ## ## ##                    ||" << std::endl;
        std::cout << "||                     ##    ##  ##  ##  ##  ##  ##  ####                    ||" << std::endl;
        std::cout << "||                     ##    ##  ##  ##  ##  ##  ##    ##                    ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||          ####   ##    ##   ####   ##   ##  ##   #####  ##   #####         ||" << std::endl;
        std::cout << "||         ##  ##  ####  ##  ##  ##  ##   ##  ##  ##      ##  ##             ||" << std::endl;
        std::cout << "||         ######  ## ## ##  ######  ##    ####    ####   ##   ####          ||" << std::endl;
        std::cout << "||         ##  ##  ##  ####  ##  ##  ##     ##        ##  ##      ##         ||" << std::endl;
        std::cout << "||         ##  ##  ##    ##  ##  ##  #####  ##    #####   ##  #####          ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||    [1] Station Capacity                                                   ||" << std::endl;
        std::cout << "||    [2] High Traffic Routes                                                ||" << std::endl;
        std::cout << "||    [3] Budget Allocation                                                  ||" << std::endl;
        std::cout << "||    [4] Station Arrival Capacity                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                [0] Back   ||" << std::endl;
        std::cout << "\\\\                                                                           //" << std::endl;
        std::cout << "  ===========================================================================  " << std::endl;

        std::cout << "  > ";
        std::cin >> option;

        switch(option){
            case 1:
                station_capacity();
                break;
            case 2:
                high_traffic_routes();
                break;
            case 3:
                budget_allocation();
                break;
            case 4:
                station_arrival_capacity();
                break;
            case 0:
                break;
            default:
                clear_screen();
                std::cout << "  > Invalid Option!" << std::endl;
                std::cout << "  > Press Enter to Continue..." << std::endl;
                wait();
                break;
        }


    }while(option != 0);
}

// Button 1 in the Train Analysis Menu
void station_capacity() {
}

// Button 2 in the Train Analysis Menu
void high_traffic_routes() {
}

// Button 3 in the Train Analysis Menu
void budget_allocation() {
}

// Button 4 in the Train Analysis Menu
void station_arrival_capacity() {
}


// Button 2 in the main menu
void service_allocation(){

    /**
     * @brief Service Allocation Menu
     *
     * @details This menu is used to allocate the services of the train network:
     *
     * [1] Optimal Route - This button calculates the maximum amount of trains that can simultaneously travel between two specific stations with minimum cost for the company.
     *
     * [0] Go Back - This button returns to the main menu.
     */

    int option;

    do{
        clear_screen();
        std::cout << "  ===========================================================================  " << std::endl;
        std::cout << "//                                                                           \\\\" << std::endl;
        std::cout << "||             #####  ######  #####   ##   ##  ##   #####   ######           ||" << std::endl;
        std::cout << "||            ##      ##      ##  ##  ##   ##  ##  ##   ##  ##               ||" << std::endl;
        std::cout << "||             ####   ####    #####   ##   ##  ##  ##       ####             ||" << std::endl;
        std::cout << "||                ##  ##      ##  ##   ## ##   ##  ##   ##  ##               ||" << std::endl;
        std::cout << "||            #####   ######  ##  ##    ###    ##   #####   ######           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||      ####  ##    ##     #####  #####   ####  ###### ##  ##### ##    ##    ||" << std::endl;
        std::cout << "||     ##  ## ##    ##    ##  ## ##   ## ##  ##   ##   ## ##  ## ####  ##    ||" << std::endl;
        std::cout << "||     ###### ##    ##    ##  ## ##      ######   ##   ## ##  ## ## ## ##    ||" << std::endl;
        std::cout << "||     ##  ## ##    ##    ##  ## ##   ## ##  ##   ##   ## ##  ## ##  ####    ||" << std::endl;
        std::cout << "||     ##  ## ##### ##### #####   #####  ##  ##   ##   ## #####  ##    ##    ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||    [1] Optimal Route                                                      ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                [0] Back   ||" << std::endl;
        std::cout << "\\\\                                                                           //" << std::endl;
        std::cout << "  ===========================================================================  " << std::endl;

        std::cout << "  > ";
        std::cin >> option;

        switch(option){
            case 1:
                optimal_route();
                break;
            case 0:
                break;
            default:
                clear_screen();
                std::cout << "  > Invalid Option!" << std::endl;
                std::cout << "  > Press Enter to Continue..." << std::endl;
                wait();
                break;
        }


    }while(option != 0);

}

// Button 1 in the Service Allocation Menu
void optimal_route() {
}

// Button 3 in the main menu
void failure_forecasting(){

    /**
     * @brief Failure Forecasting Menu
     *
     * @details This menu is used to forecast the failure of the train network:
     *
     * [1] Reduced Connectivity - This button calculates the maximum number of trains that can simultaneously travel between two specific stations in a network of reduced connectivity, which is understood as being a subgraph (generated by your system) of the original railway network.
     * [2] Segment Failure Report - This button provides a report on the stations that are the most affected by each segment failure, i.e., the top-k most affected stations for each segment to be considered.
     *
     * [0] Go Back - This button returns to the main menu.
     */

    int option;

    do{
        clear_screen();
        std::cout << "  ===========================================================================  " << std::endl;
        std::cout << "//                                                                           \\\\" << std::endl;
        std::cout << "||              ######  ####   ##  ##     ##  ##  #####   ######             ||" << std::endl;
        std::cout << "||              ##     ##  ##  ##  ##     ##  ##  ##  ##  ##                 ||" << std::endl;
        std::cout << "||              ####   ######  ##  ##     ##  ##  #####   ####               ||" << std::endl;
        std::cout << "||              ##     ##  ##  ##  ##     ##  ##  ##  ##  ##                 ||" << std::endl;
        std::cout << "||              ##     ##  ##  ##  #####   ####   ##  ##  ######             ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||  #####  ##### ####  #####  ####   ####   #### ###### ## ##    ##  ####    ||" << std::endl;
        std::cout << "||  ##    ##  ## ## ## ##    ##  ## ##  ## ##      ##   ## ####  ## ##       ||" << std::endl;
        std::cout << "||  ####  ##  ## ####  ###   ##     ######  ###    ##   ## ## ## ## ##  ###  ||" << std::endl;
        std::cout << "||  ##    ##  ## ## ## ##    ##  ## ##  ##    ##   ##   ## ##  #### ##   #   ||" << std::endl;
        std::cout << "||  ##    #####  ## ## #####  ####  ##  ## ####    ##   ## ##    ##  ####    ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||    [1] Reduced Connectivity                                               ||" << std::endl;
        std::cout << "||    [2] Segment Failure Report                                             ||" << std::endl;
        std::cout << "||                                                                           ||" << std::endl;
        std::cout << "||                                                                [0] Back   ||" << std::endl;
        std::cout << "\\\\                                                                           //" << std::endl;
        std::cout << "  ===========================================================================  " << std::endl;

        std::cout << "  > ";
        std::cin >> option;

        switch(option){
            case 1:
                reduced_connectivity();
                break;
            case 2:
                segment_failure_report();
                break;
            case 0:
                break;
            default:
                clear_screen();
                std::cout << "  > Invalid Option!" << std::endl;
                std::cout << "  > Press Enter to Continue..." << std::endl;
                wait();
                break;
        }


    }while(option != 0);
}


// Button 1 in the Failure Forecasting Menu
void reduced_connectivity() {
}

// Button 2 in the Failure Forecasting Menu
void segment_failure_report() {
}


void clear_screen(){
    for (int i = 0; i < 50; i++) {
        std::cout << std::endl;
    }
}

void wait(){
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::cin.get();
}