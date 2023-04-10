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

bool similar(const std::string& s1, const std::string& s2);

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
        if (stations.find(name) != stations.end()) continue;
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

    //if(!SetConsoleOutputCP(65001))return 1;
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

    } while(option != 0);

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
    std::cout << R"(||           ."   """"   """""" """"| .--`                                   ||)" << std::endl;
    std::cout << "||          (:--:--:--:--:--:--:--:-| [___    .------------------------.     ||" << std::endl;
    std::cout << "||           |D&A  :  :  :  :  :  : [_5_] |'='|.----D------L------M---.|     ||" << std::endl;
    std::cout << "||          /|.___________________________|___|'--.___.--.___.--.___.-'|     ||" << std::endl;
    std::cout << R"(||         / ||_.--.______.--.______.--._ |---\'--\-.-/==\-.-/==\-.-/-'/--   ||)" << std::endl;
    std::cout << "||        /__;^=(==)======(==)======(==)=^~^^^ ^^^^(-)^^^^(-)^^^^(-)^^^      ||" << std::endl;
    std::cout << "||  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  ||" << std::endl;
    std::cout << "||                                                                           ||" << std::endl;
    std::cout << "||                      --- Press Enter to Continue ---                      ||" << std::endl;
    std::cout << "\\\\                                                                           //" << std::endl;
    std::cout << "  ===========================================================================  " << std::endl;
    if (similar("Porto Campanha", "Porto Campanhã")) std::cout << "true" << std::endl;
    else std::cout << "false" << std::endl;
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
    std::cout << std::endl;
    //confirm if station2 exist
    if(stations.find(station2) == stations.end()){
        clear_screen();
        std::cout << "  > The destination station does not exist!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        clear_screen();
        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing destination station: ";
            std::getline(std::cin >> std::ws, station2);
            std::cout << std::endl;
        } while (stations.find(station2) == stations.end());
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
    int k;
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

    do {
        std::cout << "  > Please enter the number of municipalities to be displayed: ";
        std::cin >> k;
        std::cout << std::endl;
    } while (k < 1 || k > pq.size());

    std::cout << "                > Top "<< k <<" municipalities with the most capacity <" << std::endl;
    std::cout << std::endl;

    int n;
    for (n = 1 ; n <= k ; n++) {
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
    std::cout << std::endl;
    //confirm if station2 exist
    if(stations.find(station2) == stations.end()){
        clear_screen();
        std::cout << "  > The destination station does not exist!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        clear_screen();
        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing destination station: ";
            std::getline(std::cin >> std::ws, station2);
            std::cout << std::endl;
        } while (stations.find(station2) == stations.end());
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
    std::string station1, station2;

    std::cout << "  > Enter the name of the source station: ";
    std::getline(std::cin >> std::ws, station1);
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
    std::cout << std::endl;
    //confirm if station2 exist
    if(stations.find(station2) == stations.end()){
        clear_screen();
        std::cout << "  > The destination station does not exist!" << std::endl;
        std::cout << "  > Press Enter to Continue..." << std::endl;
        wait();

        clear_screen();
        do {
            clear_screen();
            std::cout << "  > Please enter the name of a existing destination station: ";
            std::getline(std::cin >> std::ws, station2);
            std::cout << std::endl;
        } while (stations.find(station2) == stations.end());
    }

    auto st1 = stations.at(station1);
    auto st2 = stations.at(station2);

    vec<ptr<Station>> st_remove;
    vec<ptr<Link>> ln_remove;

    std::cout << "Max flow in reduced network: " << network->maxFlowReduced(st1, st2, st_remove, ln_remove) << std::endl;
    wait();
}

// Button 2 in the Failure Forecasting Menu
void segment_failure_report() {
    vec<unsigned int> max_flows(stations.size());
    std::cout << "first loop" << std::endl;
    for (auto &station : stations) {
        auto s = station.second;
        auto max_flow = network->maxTrains(s);
        max_flows[s->getId()] = max_flow;
        if (s->getName() == "Porto Campanhã") std::cout << "Id: " << s->getId() << " " << s->getName() << " " << max_flow << std::endl;
    }
    auto l = stations.at("Porto Campanhã")->getLinks().front();
    l->setEnabled(false);
    l->getReverse()->setEnabled(false);
    std::cout << "second loop" << std::endl;
    for (auto &station : stations) {
        auto s = station.second;
        auto max_flow = network->maxTrains(s);
        if (max_flows[s->getId()] != max_flow) {
            std::cout << s->getName() << " " << max_flows[s->getId()]- max_flow << std::endl;
        }
    }
    wait();
    starting_screen();
}

void clear_screen(){
    for (int i = 0; i < 50; i++) {
        std::cout << std::endl;
    }
}

void wait(){
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::cin.get();
}

bool similar(const std::string &s1, const std::string &s2) {
    double threshold = 0.85;
    vec<int> v1(26, 0), v2(26, 0);

    for (auto c : s1) {
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
        if (c >= 'a' && c <= 'z') v1[c - 'a']++;
    }

    for (auto c : s2) {
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
        if (c >= 'a' && c <= 'z') v2[c - 'a']++;
    }

    int count = 0;
    for (int i = 0; i < 26; i++) if (v1[i] == v2[i] && v1[i] != 0) count += v1[i];

    int min = s1.size() < s2.size() ? s1.size() : s2.size();
    double similarity = (double) count / min;
    std::cout << similarity << std::endl;

    return similarity >= threshold;
}