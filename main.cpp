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
bool is_number(const std::string& s);
bool is_linked(const std::string& s1, const std::string& s2);

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
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        if(stations.find(name) != stations.end()) continue;
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

void readPartialStations(){
    std::ifstream file("../data/partial_stations.csv");
    std::string line;
    std::getline(file, line);
    int id = 0;
    stations.clear();
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, municipality, township, district;
        std::getline(ss, name, ',');
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        if(stations.find(name) != stations.end()) continue;
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
        std::transform(st1.begin(), st1.end(), st1.begin(), ::toupper);
        std::getline(ss, st2, ',');
        std::transform(st2.begin(), st2.end(), st2.begin(), ::toupper);
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

void readPartialLinks() {
    std::ifstream file("../data/partial_network.csv");
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string st1, st2, service;
        int capacity;
        std::getline(ss, st1, ',');
        std::transform(st1.begin(), st1.end(), st1.begin(), ::toupper);
        std::getline(ss, st2, ',');
        std::transform(st2.begin(), st2.end(), st2.begin(), ::toupper);
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
    system("Color 0C");
    std::string option;
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
        std::getline(std::cin >> std::ws, option);

        if(option == "1"){
            train_analysis();
        }
        else if(option == "2"){
            service_allocation();
        }
        else if(option == "3"){
            failure_forecasting();
        }
        else if(option == "0"){
            break;
        }
        else{
            clear_screen();
            std::cout << "  > Invalid Option!" << std::endl;
            std::cout << "  > Press Enter to Continue..." << std::endl;
            wait();
        }
    } while(option != "0");

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
    std::cout <<R"(||           ."   """"   """""" """"| .--`                                   ||)" << std::endl;
    std::cout << "||          (:--:--:--:--:--:--:--:-| [___    .------------------------.     ||" << std::endl;
    std::cout << "||           |D&A  :  :  :  :  :  : [_5_] |'='|.----D------L------M---.|     ||" << std::endl;
    std::cout << "||          /|.___________________________|___|'--.___.--.___.--.___.-'|     ||" << std::endl;
    std::cout <<R"(||         / ||_.--.______.--.______.--._ |---\'--\-.-/==\-.-/==\-.-/-'/--   ||)" << std::endl;
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
    std::string option;
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
        std::getline(std::cin >> std::ws, option);

        if(option == "1"){
            readStations();
            readLinks();
            break;
        }
        else if(option == "2"){
            readPartialStations();
            readPartialLinks();
            break;
        }
        else{
            clear_screen();
            std::cout << "  > Invalid Option!" << std::endl;
            std::cout << "  > Press Enter to Continue..." << std::endl;
            wait();
        }

    }while(option != "1" && option != "2");

}


// Button 1 in the main menu
void train_analysis(){

    std::string option;

    /**
     * @brief Train Analysis Menu
     *
     * @details This menu is used to analyze the train network:
     *
     * [1] Station Capacity - This button calculates the maximum number of trains that can simultaneously arrive at a given station.
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
        std::getline(std::cin >> std::ws, option);

        if (option == "0") {
            break;
        }
        else if (option == "1") {
            station_capacity();
        }
        else if (option == "2") {
            high_traffic_routes();
        }
        else if (option == "3") {
            budget_allocation();
        }
        else if (option == "4") {
            station_arrival_capacity();
        }
        else {
            clear_screen();
            std::cout << "  > Invalid Option!" << std::endl;
            std::cout << "  > Press Enter to Continue..." << std::endl;
            wait();
        }

    }while(option != "0");
}

// Button 1 in the Train Analysis Menu
void station_capacity() {

    std::string station1,  station2;


    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                          --- Station Capacity ---                         ||" << std::endl;
    std::cout << "||                        (Max Flow Between Stations)                        ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Enter the name of the source station: ";
    std::getline(std::cin >> std::ws, station1);
    std::transform(station1.begin(), station1.end(), station1.begin(), ::toupper);
    std::cout << std::endl;
    //confirm if station1 exist
    if(stations.find(station1) == stations.end()){
        clear_screen();
        std::cout << "  > The source station does not exist!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        clear_screen();
        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing source station: ";
            std::getline(std::cin >> std::ws, station1);
            std::cout << std::endl;
        } while (stations.find(station1) == stations.end());
    }


    std::cout << "  > Enter the name of the destination station: ";
    std::getline(std::cin >> std::ws, station2);
    std::transform(station2.begin(), station2.end(), station2.begin(), ::toupper);
    std::cout << std::endl;
    //confirm if station2 exist
    if(stations.find(station2) == stations.end() || station2 == station1){
        clear_screen();
        std::cout << "  > The destination station does not exist or is alredy used as source!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        clear_screen();
        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing destination station, or not used yet: ";
            std::getline(std::cin >> std::ws, station2);
            std::cout << std::endl;
        } while (stations.find(station2) == stations.end() || station2 == station1);
    }

    auto st1 = stations.at(station1);
    auto st2 = stations.at(station2);

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                          --- Station Capacity ---                         ||" << std::endl;
    std::cout << "||                        (Max Flow Between Stations)                        ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;
    std::cout << "  > Max flow between " << st1->getName() << " and " << st2->getName() << ": " << network->maxFlow(st1, st2) << std::endl;
    std::cout << std::endl;
    std::cout << "  > Press Enter to Continue..." << std::endl;
    wait();

}

// Button 2 in the Train Analysis Menu
void high_traffic_routes() {

    vec<std::pair<ptr<Station>, ptr<Station>>> pairs;


    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                        --- High Traffic Routes ---                        ||" << std::endl;
    std::cout << "||                    (Max Network Flow Between Stations)                    ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "                           > Max network flow: " << network->getMaxFlowNetwork(pairs) << " <" << std::endl;
    std::cout << std::endl;

    for (const auto& pair : pairs) {
        std::cout << "  > " << pair.first->getName() << " -> " << pair.second->getName() << std::endl;
    }

    std::cout << std::endl;
    std::cout << "  > Press Enter to Continue..." << std::endl;
    wait();

}

// Button 3 in the Train Analysis Menu
void budget_allocation() {

    std::priority_queue<std::pair<int, std::string>> pq;
    for (const auto &pair: municipality_capacities) pq.emplace(pair.second, pair.first);

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                         --- Budget Allocation ---                         ||" << std::endl;
    std::cout << "||                  (Municipalities with the most capacity)                  ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    int random = rand() % (10 - 3 + 1) + 3;

    std::cout << "                > Top "<< random <<" municipalities with the most capacity <" << std::endl;
    std::cout << std::endl;

    int n;
    for (n = 1 ; n <= random && n < municipality_capacities.size(); n++) {
        auto pair = pq.top();
        pq.pop();
        std::cout << "  > " << n << " - " << pair.second << " -> " << pair.first << std::endl;
    }

    std::cout << std::endl;
    std::cout << "  > Press Enter to Continue..." << std::endl;
    wait();
}

// Button 4 in the Train Analysis Menu
void station_arrival_capacity() {

    std::string station_name;

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                      --- Station Arrival Capacity ---                     ||" << std::endl;
    std::cout << "||                                (Max Trains)                               ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Enter the name of the station: ";
    std::getline(std::cin >> std::ws, station_name);
    std::transform(station_name.begin(), station_name.end(), station_name.begin(), ::toupper);
    std::cout << std::endl;

    //confirm if station exist
    if(stations.find(station_name) == stations.end()){
        clear_screen();
        std::cout << "  > The station does not exist!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        clear_screen();
        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing station: ";
            std::getline(std::cin >> std::ws, station_name);
            std::cout << std::endl;
        } while (stations.find(station_name) == stations.end());
    }

    auto st = stations.at(station_name);

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                      --- Station Arrival Capacity ---                     ||" << std::endl;
    std::cout << "||                                (Max Trains)                               ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Max trains that can arrive at " << st->getName() << ": " << network->maxTrains(st) << std::endl;
    std::cout << std::endl;

    std::cout << "  > Press Enter to Continue..." << std::endl;
    wait();

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


    std::string option;

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
        std::getline(std::cin >> std::ws, option);

        if(option == "1") optimal_route();
        else if(option == "0") break;
        else{
            clear_screen();
            std::cout << "  > Invalid Option!" << std::endl;
            std::cout << "  > Press Enter to Continue..." << std::endl;
            wait();
        }

    }while(option != "0");

}

// Button 1 in the Service Allocation Menu
void optimal_route() {

    std::string station1, station2;

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                           --- Optimal Route ---                           ||" << std::endl;
    std::cout << "||              (Max Trains with Min Cost Between two Stations)              ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Enter the name of the source station: ";
    std::getline(std::cin >> std::ws, station1);
    std::transform(station1.begin(), station1.end(), station1.begin(), ::toupper);
    std::cout << std::endl;
    //confirm if station1 exist
    if(stations.find(station1) == stations.end()){
        clear_screen();
        std::cout << "  > The source station does not exist!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        clear_screen();
        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing source station: ";
            std::getline(std::cin >> std::ws, station1);
            std::cout << std::endl;
        } while (stations.find(station1) == stations.end());
    }


    std::cout << "  > Enter the name of the destination station: ";
    std::getline(std::cin >> std::ws, station2);
    std::transform(station2.begin(), station2.end(), station2.begin(), ::toupper);
    std::cout << std::endl;
    //confirm if station2 exist
    if(stations.find(station2) == stations.end() || station2 == station1){
        clear_screen();
        std::cout << "  > The destination station does not exist or is alredy used as source!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        clear_screen();
        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing destination station, or not used yet: ";
            std::getline(std::cin >> std::ws, station2);
            std::cout << std::endl;
        } while (stations.find(station2) == stations.end() || station2 == station1);
    }

    auto st1 = stations.at(station1);
    auto st2 = stations.at(station2);

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                           --- Optimal Route ---                           ||" << std::endl;
    std::cout << "||              (Max Trains with Min Cost Between two Stations)              ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Source Station: " << station1 << std::endl;
    std::cout << "  > Destination Station: " << station2 << std::endl;
    std::cout << std::endl;

    std::cout << "  > Max Trains with min cost for the company: " << network->maxCost(st1, st2) << std::endl;
    std::cout << std::endl;

    std::cout << "  > Press Enter to Continue..." << std::endl;
    wait();
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

    std::string option;

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
        std::getline(std::cin >> std::ws, option);

        if(option == "1") reduced_connectivity();
        else if(option == "2") segment_failure_report();
        else if(option == "0") break;
        else{
            clear_screen();
            std::cout << "  > Invalid Option!" << std::endl;
            std::cout << "  > Press Enter to Continue..." << std::endl;
            wait();
        }


    }while(option != "0");
}


// Button 1 in the Failure Forecasting Menu
void reduced_connectivity() {

    std::string station_remove;
    std::string station1, station2;
    vec<ptr<Station>> remove_stations;
    vec<ptr<Link>> removed_links;

    std::string option;

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                        --- Reduced Connectivity ---                       ||" << std::endl;
    std::cout << "||                         (Source and Sink Stations)                        ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Enter the name of the source station: ";
    std::getline(std::cin >> std::ws, station1);
    std::transform(station1.begin(), station1.end(), station1.begin(), ::toupper);
    std::cout << std::endl;
    //confirm if station1 exist
    if(stations.find(station1) == stations.end()){
        clear_screen();
        std::cout << "  > The source station does not exist!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        clear_screen();
        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing source station: ";
            std::getline(std::cin >> std::ws, station1);
            std::cout << std::endl;
        } while (stations.find(station1) == stations.end());
    }


    std::cout << "  > Enter the name of the destination station: ";
    std::getline(std::cin >> std::ws, station2);
    std::transform(station2.begin(), station2.end(), station2.begin(), ::toupper);
    std::cout << std::endl;
    //confirm if station2 exist
    if(stations.find(station2) == stations.end() || station2 == station1){
        clear_screen();
        std::cout << "  > The destination station does not exist or is alredy used as source!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        clear_screen();
        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing destination station, or not used yet: ";
            std::getline(std::cin >> std::ws, station2);
            std::cout << std::endl;
        } while (stations.find(station2) == stations.end() || station2 == station1);
    }

    auto st1 = stations.at(station1);
    auto st2 = stations.at(station2);

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                        --- Reduced Connectivity ---                       ||" << std::endl;
    std::cout << "||                             (Remove Stations)                             ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Enter the name of the station to remove: ";
    std::getline(std::cin >> std::ws, station_remove);
    std::transform(station_remove.begin(), station_remove.end(), station_remove.begin(), ::toupper);
    std::cout << std::endl;
    //confirm if station_remove exist
    if(stations.find(station_remove) == stations.end() || station_remove == station1 || station_remove == station2){
        clear_screen();
        std::cout << "  > The station does not exist or is alredy used as source or destination!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        clear_screen();
        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing station, diferent of source and destination: ";
            std::getline(std::cin >> std::ws, station_remove);
            std::cout << std::endl;
        } while (stations.find(station_remove) == stations.end() || station_remove == station1 || station_remove == station2);
    }

    auto st_remove = stations.at(station_remove);
    remove_stations.push_back(st_remove);

    do{
        clear_screen();
        std::cout << "  ===========================================================================  " << std::endl;
        std::cout << "//                                                                           \\\\" << std::endl;
        std::cout << "||                        --- Reduced Connectivity ---                       ||" << std::endl;
        std::cout << "||                             (Remove Stations)                             ||" << std::endl;
        std::cout << "\\\\                                                                           //" << std::endl;
        std::cout << "  ===========================================================================  " << std::endl;
        std::cout << std::endl;

        std::cout << "  > Source Station: " << st1->getName() << std::endl;
        std::cout << "  > Destination Station: " << st2->getName() << std::endl;

        std::cout << "  > Stations to remove: ";
        for (auto &station: remove_stations) {
            std::cout << station->getName() << "; ";
        }
        std::cout << std::endl;
        std::cout << std::endl;

        std::cout << "  > Do you want to remove another station? (y/n): ";
        std::getline(std::cin >> std::ws, option);
        std::transform(option.begin(), option.end(), option.begin(), ::toupper);
        std::cout << std::endl;

        if(option == "y" || option == "Y"){
            clear_screen();
            std::cout << "  > Enter the name of the station to remove: ";
            std::getline(std::cin >> std::ws, station_remove);
            std::transform(station_remove.begin(), station_remove.end(), station_remove.begin(), ::toupper);
            std::cout << std::endl;
            //confirm if station_remove exist
            if(stations.find(station_remove) == stations.end() || station_remove == station1 || station_remove == station2 || std::find(remove_stations.begin(), remove_stations.end(), stations.at(station_remove)) != remove_stations.end()){
                clear_screen();
                std::cout << "  > The station does not exist or is alredy used as source or destination or one of the alredy stations to remove!" << std::endl;
                std::cout << "  > Press Enter to Continue..." << std::endl;
                wait();

                clear_screen();
                do {
                    clear_screen();
                    std::cout << "  > Please enter the name of a existing station, diferent of source and destination: ";
                    std::getline(std::cin >> std::ws, station_remove);
                    std::transform(station_remove.begin(), station_remove.end(), station_remove.begin(), ::toupper);
                    std::cout << std::endl;
                } while (stations.find(station_remove) == stations.end() || station_remove == station1 || station_remove == station2);
            }

            st_remove = stations.at(station_remove);
            remove_stations.push_back(st_remove);
        }
        else if(option != "n" && option != "N"){
            clear_screen();
            std::cout << "  > Invalid option!" << std::endl;
            std::cout << "  > Press Enter to Continue..." << std::endl;
            wait();
        }


    }while(option != "n" && option != "N");

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                        --- Reduced Connectivity ---                       ||" << std::endl;
    std::cout << "||                           (Flow Between Stations)                         ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Source Station: " << st1->getName() << std::endl;
    std::cout << "  > Destination Station: " << st2->getName() << std::endl;

    std::cout << "  > Stations to remove: ";
    for (auto &station: remove_stations) {
        std::cout << station->getName() << "; ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "  > Max flow between " << st1->getName() << " and " << st2->getName() << " in reduced network: ";
    std::cout << network->maxFlowReduced(st1, st2, remove_stations, removed_links) << std::endl;
    std::cout << std::endl;

    std::cout << "  > Press Enter to Continue..." << std::endl;
    wait();

}

// Button 2 in the Failure Forecasting Menu
void segment_failure_report() {

    std::string option;
    std::string station1, station2;
    int k; // std::cin >> k;

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                        --- Failure Forecasting ---                        ||" << std::endl;
    std::cout << "||                          (Segment Failure Report)                         ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Number of stations to be reported: ";
    std::getline(std::cin >> std::ws, option);
    std::cout << std::endl;

    do {
        if (!is_number(option)) {
            clear_screen();
            std::cout << "  > Invalid option!" << std::endl;
            std::cout << "  > Press Enter to Continue..." << std::endl;
            wait();

            clear_screen();
            std::cout << "  > Please enter the number of stations to be reported: ";
            std::getline(std::cin >> std::ws, option);
            std::cout << std::endl;
        }
    } while (!is_number(option));

    k = std::stoi(option);

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                        --- Failure Forecasting ---                        ||" << std::endl;
    std::cout << "||                          (Segment Failure Report)                         ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Enter station name to remove a link: ";
    std::getline(std::cin >> std::ws, station1);
    std::transform(station1.begin(), station1.end(), station1.begin(), ::toupper);
    std::cout << std::endl;

    //confirm if station exist
    if (stations.find(station1) == stations.end()) {
        clear_screen();
        std::cout << "  > The station does not exist!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing station: ";
            std::getline(std::cin >> std::ws, station1);
            std::transform(station1.begin(), station1.end(), station1.begin(), ::toupper);
            std::cout << std::endl;
        } while (stations.find(station1) == stations.end());
    }

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                        --- Failure Forecasting ---                        ||" << std::endl;
    std::cout << "||                          (Segment Failure Report)                         ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Enter station name to remove a link with: " << station1 << std::endl;

    std::cout << "  > Links from " << station1 << ":" << std::endl;
    std::cout << std::endl;

    for (const auto &x: stations.at(station1)->getLinks()){
        std::cout << "  > " << x->getDest()->getName() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "  > Enter station name: ";
    std::getline(std::cin >> std::ws, station2);
    std::transform(station2.begin(), station2.end(), station2.begin(), ::toupper);
    std::cout << std::endl;

    //confirm if station exist
    if (stations.find(station2) == stations.end() || station1 == station2 || !is_linked(station1,station2)) {
        clear_screen();
        std::cout << "  > The station does not exist or is not linked to source station!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        do {
            clear_screen();
            std::cout << "  > Please enter the name of a valid station: ";
            std::getline(std::cin >> std::ws, station2);
            std::transform(station2.begin(), station2.end(), station2.begin(), ::toupper);
            std::cout << std::endl;
        } while (stations.find(station2) == stations.end() || station1 == station2 || !is_linked(station1,station2));
    }

    auto st1 = stations.at(station1);
    auto st2 = stations.at(station2);
    ptr<Link> link;

    for (const auto &x: st1->getLinks()){
        if (x->getDest() == st2){
            link = x;
            break;
        }
    }

    clear_screen();
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << "//                                                                           \\\\" << std::endl;
    std::cout << "||                        --- Failure Forecasting ---                        ||" << std::endl;
    std::cout << "||                          (Segment Failure Report)                         ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    std::cout << std::endl;

    std::cout << "  > Removed link between " << station1 << " and " << station2 << std::endl;
    std::cout << std::endl;
    std::cout << "  > Top " << k << " affected stations: " << std::endl;
    std::cout << std::endl;

    int n = 1;
    vec<std::pair<int, int>> topk(k); // Top k affected stations {Diff in flow, Id};
    network->topAffected(link, topk);
    for (auto &p : topk) {
        if(p.first != 0) {
            std::cout << "  > " << n << " Station " << network->getStation(p.second)->getName() << " with " << p.first << " difference in flow" << std::endl;
            n++;
        }
        else{
            if(n == 1){
                std::cout << "  > No affected stations" << std::endl;
                break;
            }
            std::cout << std::endl;
            std::cout << "  > No more affected stations" << std::endl;
            break;
        }
    }
    std::cout << std::endl;

    std::cout << "  > Press Enter to Continue..." << std::endl;
    wait();
}


void clear_screen(){
    for (int i = 0; i < 50; i++) {
        std::cout << std::endl;
    }
}

void wait(){
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::cin.get();
}

bool is_number(const std::string& s){
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

bool is_linked(const std::string& s1, const std::string& s2){
    for(const auto &x: stations.at(s1)->getLinks()){
        if(x->getDest()->getName() == s2){
            return true;
        }
    }
    return false;
}