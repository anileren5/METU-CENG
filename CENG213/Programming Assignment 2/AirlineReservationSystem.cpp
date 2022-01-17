#include "AirlineReservationSystem.h"

void AirlineReservationSystem::addPassenger(const std::string &firstname, const std::string &lastname) {
    /* TODO */
    passengers.insert(Passenger(firstname,lastname));
}

Passenger *AirlineReservationSystem::searchPassenger(const std::string &firstname, const std::string &lastname) {
    /* TODO */
    return &(passengers.search(Passenger(firstname,lastname))->data);
}

void AirlineReservationSystem::addFlight(const std::string &flightCode, const std::string &departureTime, const std::string &arrivalTime, const std::string &departureCity, const std::string &arrivalCity, int economyCapacity, int businessCapacity) {
    /* TODO */
    flights.insert(Flight(flightCode,departureTime,arrivalTime,departureCity,arrivalCity,economyCapacity,businessCapacity));
    // Check the case where a fligt with already existing name is given. !!!!
}

std::vector<Flight *> AirlineReservationSystem::searchFlight(const std::string &departureCity, const std::string &arrivalCity) {
    /* TODO */
    std::vector<Flight*> searched_flights;
    searchFlight_helper(departureCity,arrivalCity,flights.getRoot(),searched_flights);
    return searched_flights;
}

void AirlineReservationSystem::issueTicket(const std::string &firstname, const std::string &lastname, const std::string &flightCode, TicketType ticketType) {
    /* TODO */
    bool PassengerExist = isPassengerExist(passengers.getRoot(),firstname,lastname);
    bool FlightExist = isFlightExist(flights.getRoot(),flightCode);
    
    if (PassengerExist == false || FlightExist == false) return ;
    // else if (ticketType == economy && findFlight(flights.getRoot(),flightCode)->data.getEconomyCapacity() <= 0) return ;
    // else if (ticketType == business && findFlight(flights.getRoot(),flightCode)->data.getBusinessCapacity() <= 0) return ;
    else{
        Ticket new_ticket = Ticket(searchPassenger(firstname,lastname),&(findFlight(flights.getRoot(),flightCode)->data),ticketType);
        findFlight(flights.getRoot(),flightCode)->data.addTicket(new_ticket);
    }
}

void AirlineReservationSystem::saveFreeTicketRequest(const std::string &firstname, const std::string &lastname, const std::string &flightCode, TicketType ticketType) {
    /* TODO */
    bool PassengerExist = isPassengerExist(passengers.getRoot(),firstname,lastname);
    bool FlightExist = isFlightExist(flights.getRoot(),flightCode);
    
    if (PassengerExist && FlightExist) freeTicketRequests.enqueue(Ticket(searchPassenger(firstname,lastname),&(findFlight(flights.getRoot(),flightCode)->data),ticketType));
}

void AirlineReservationSystem::executeTheFlight(const std::string &flightCode) {
    /* TODO */
    // int n_econ = findFlight(flights.getRoot(),flightCode)->data.getEconomyCapacity();
    // int n_bus = findFlight(flights.getRoot(),flightCode)->data.getBusinessCapacity();
    
    int i,n = freeTicketRequests.size();
    for (i=0;i<n;i++){
        Ticket front_ticket = freeTicketRequests.peek();
        if (front_ticket.getTicketType() == economy){
            if (*front_ticket.getFlight() == findFlight(flights.getRoot(),flightCode)->data){
                bool flag = findFlight(flights.getRoot(),flightCode)->data.addTicket(front_ticket);
                if (flag == true){
                    freeTicketRequests.dequeue();
                }
                else{
                    freeTicketRequests.enqueue(front_ticket);
                    freeTicketRequests.dequeue();
                }
            }
            else {
                freeTicketRequests.enqueue(front_ticket);
                freeTicketRequests.dequeue();
            }
        }
        else if (front_ticket.getTicketType() == business){
            if (*front_ticket.getFlight() == findFlight(flights.getRoot(),flightCode)->data){
                bool flag = findFlight(flights.getRoot(),flightCode)->data.addTicket(front_ticket);
                if (flag == true){
                    freeTicketRequests.dequeue();
                }
                else {
                    freeTicketRequests.enqueue(front_ticket);
                    freeTicketRequests.dequeue();
                }
            }
            else {
                freeTicketRequests.enqueue(front_ticket);
                freeTicketRequests.dequeue();
            }
        }
    }
    findFlight(flights.getRoot(),flightCode)->data.setCompleted(true);
}

void AirlineReservationSystem::print() const {
    std::cout << "# Printing the airline reservation system ..." << std::endl;

    std::cout << "# Passengers:" << std::endl;
    passengers.print(inorder);

    std::cout << "# Flights:" << std::endl;
    flights.print(inorder);

    std::cout << "# Free ticket requests:" << std::endl;
    freeTicketRequests.print();

    std::cout << "# Printing is done." << std::endl;
}

void AirlineReservationSystem::searchFlight_helper(const std::string &departureCity, const std::string &arrivalCity,BSTNode<Flight>* root,std::vector<Flight *>& searched_flights){
    if (root == NULL) return;
    else {
        if (root->data.getArrivalCity() == arrivalCity && root->data.getDepartureCity() == departureCity) searched_flights.push_back(&(root->data));
        searchFlight_helper(departureCity,arrivalCity,root->left,searched_flights);
        searchFlight_helper(departureCity,arrivalCity,root->right,searched_flights);
    }
}

bool AirlineReservationSystem::isPassengerExist(BSTNode<Passenger>* root,const std::string &firstname, const std::string &lastname){
    if (root == NULL) return false;
    else {
        if (root->data.getFirstname() == firstname && root->data.getLastname() == lastname) return true;
        else return (isPassengerExist(root->left,firstname,lastname) || isPassengerExist(root->right,firstname,lastname));
    }
}

bool AirlineReservationSystem::isFlightExist(BSTNode<Flight>* root,const std::string &flightCode){
    if (root == NULL) return false;
    else {
        if (root->data.getFlightCode() == flightCode) return true;
        else return (isFlightExist(root->left,flightCode) || isFlightExist(root->right,flightCode));
    }
}

BSTNode<Flight>* AirlineReservationSystem::findFlight(BSTNode<Flight>* root, const std::string &flightCode){
    if (root == NULL) return NULL;
    else {
        if (root->data.getFlightCode() == flightCode) return root;
        else {
            if (flightCode > root->data.getFlightCode()) return findFlight(root->right,flightCode);
            else if (flightCode < root->data.getFlightCode()) return findFlight(root->left,flightCode);
        }
    }
}





















