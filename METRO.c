#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure to represent a ticket
typedef struct Ticket {
    int start_station;
    int destination_station;
    int num_tickets_sold;
    struct Ticket* next;
} Ticket;

// Structure to represent the linked list of tickets
typedef struct {
    Ticket* head;
} TicketList;

// Function to create a new ticket
Ticket* createTicket(int start, int destination) {
    Ticket* newTicket = (Ticket*)malloc(sizeof(Ticket));
    newTicket->start_station = start;
    newTicket->destination_station = destination;
    newTicket->num_tickets_sold = 1;
    newTicket->next = NULL;
    return newTicket;
}

// Function to check if a ticket is already present and update the count
int present(TicketList* ticketList, int start, int destination) {
    Ticket* temp = ticketList->head;
    while (temp != NULL) {
        if (temp->start_station == start && temp->destination_station == destination) {
            temp->num_tickets_sold++;
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

// Function to add a ticket to the linked list
void addTicket(TicketList* ticketList, int start, int destination) {
    if (!present(ticketList, start, destination)) {
        Ticket* newTicket = createTicket(start, destination);
        newTicket->next = ticketList->head;
        ticketList->head = newTicket; // Insert at the front
    }
}

// Function to print the tickets in the linked list
void printTickets(TicketList* ticketList) {
    Ticket* current = ticketList->head;
    printf("\nTicket Sales Report:\n");

    while (current != NULL) {
        printf("Start Station: %d, Destination Station: %d, Tickets Sold: %d\n",
               current->start_station, current->destination_station, current->num_tickets_sold);
        current = current->next;
    }
}

// Node to represent a station in the metro network
typedef struct Node {
    int station_id;
    char station_name[50]; // Assuming a maximum length of 50 characters for station names
    int fare;
    char lane[50];
    struct Node* next;
} Node;

// Structure to represent the metro network using adjacency list
typedef struct {
    Node** adjacency_list;
    int num_stations;
} MetroNetwork;

// Function to create a new node
Node* createNode(int station_id, const char* station_name) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->station_id = station_id;
    newNode->fare = 20;

    // Ensure null-terminated string for station_name
    size_t name_length = strlen(station_name);
    strncpy(newNode->station_name, station_name, sizeof(newNode->station_name) - 1);
    newNode->station_name[sizeof(newNode->station_name) - 1] = '\0';

    // Set lane based on station_id
    if (station_id == 1 || station_id == 2 || station_id == 4 || station_id == 5) {
        strncpy(newNode->lane, "Green", sizeof(newNode->lane) - 1);
        newNode->lane[sizeof(newNode->lane) - 1] = '\0';
    } else if (station_id == 6 || station_id == 7 || station_id == 8 || station_id == 9) {
        strncpy(newNode->lane, "Purple", sizeof(newNode->lane) - 1);
        newNode->lane[sizeof(newNode->lane) - 1] = '\0';
    } else if (station_id == 3) {
        strncpy(newNode->lane, "Change lane", sizeof(newNode->lane) - 1);
        newNode->lane[sizeof(newNode->lane) - 1] = '\0';
    }

    newNode->next = NULL;
    return newNode;
}

// Function to create a metro network with a given number of stations
MetroNetwork* createMetroNetwork(int num_stations) //CREATES adjacency list and initialises it to null
{
    MetroNetwork* metro = (MetroNetwork*)malloc(sizeof(MetroNetwork));
    metro->num_stations = num_stations;
    metro->adjacency_list = (Node**)malloc(num_stations * sizeof(Node*));

    for (int i = 0; i < num_stations; ++i) {
        metro->adjacency_list[i] = NULL;
    }

    return metro;
}

// Function to add an edge between two stations
void addEdge(MetroNetwork* metro, int source, int destination, const char* source_name, const char* destination_name) 
{
    Node* newNode = createNode(destination, destination_name);
    newNode->next = metro->adjacency_list[source];
    metro->adjacency_list[source] = newNode;

    // Assuming an undirected metro network, add an edge in the reverse direction
    newNode = createNode(source, source_name);
    newNode->next = metro->adjacency_list[destination];
    metro->adjacency_list[destination] = newNode;
}

// Function to print the route between two stations
void printRoute(Node** nodes, int* path, int start, int destination) {
    printf("\nRoute from Station %d to Station %d: ", start, destination);

    int current = destination;
    int count = 0;
    while (current != start) {
        printf("Station %d(%s) <- ", nodes[current]->station_id, nodes[current]->lane);
        current = path[current];
        count += nodes[current]->fare;
    }
    printf("Station %d(%s)\n", start, nodes[current]->lane);
    printf("\nTotal amount spent for traveling from Station %d to Station %d is %d", start, destination, count);
}

// Function to perform depth-first search to find a route between two stations
void dfs(MetroNetwork* metro, int current, int destination, int* visited, int* path, int* found) {
    visited[current] = 1;

    if (current == destination) {
        *found = 1;
        return;
    }

    Node* temp = metro->adjacency_list[current];
    while (temp != NULL) {
        if (!visited[temp->station_id]) {
            path[temp->station_id] = current;
            dfs(metro, temp->station_id, destination, visited, path, found);
        }
        temp = temp->next;
    }
}

// Function to find a route between two stations
void findRoute(MetroNetwork* metro, Node** nodes, int start, int destination) {
    int* visited = (int*)malloc(metro->num_stations * sizeof(int));
    int* path = (int*)malloc(metro->num_stations * sizeof(int));
    int found = 0;

    for (int i = 0; i < metro->num_stations; ++i) {
        visited[i] = 0;
        path[i] = -1;
    }

    dfs(metro, start, destination, visited, path, &found);

    if (found) {
        printRoute(nodes, path, start, destination);
    } else {
        printf("No route found between %s and %s.\n", nodes[start]->station_name, nodes[destination]->station_name);
    }

    free(visited);
    free(path);
}

int main() {

    printf("      ~--Safety Measurements---~  \n");
    printf("-------------------------------------\n");
    printf("1.Stay calm and listen for instructions\n");
    printf("2.If between Stations,do not exit the train unless instructed to o so by Metro personnel\n");
    printf("3.Do not touch wires due to an eletrical hazard\n");
    printf("4.Trains may still be running , on either track, so lookboth ways and listen for Metro\n");
    printf("5.Take your personal items with you\n");
    printf("6.Injuries and Fatalities near Train Tracks are too common.Stay Alert and don't Tempt Fate\n\n");

    char Passenger[20],Gender[1];
    int start, destination;
    printf("Enter your name :");
    scanf("%s",Passenger);
    printf("Enter your Gender :");
    scanf("%s",Gender);
    printf("\n");
    // Example usage
    int num_stations = 10;
    MetroNetwork* metro = createMetroNetwork(num_stations);

    // Creating an array to hold station names
    Node** nodes = (Node**)malloc(num_stations * sizeof(Node*));

    // Adding nodes to the metro network and storing them in the array
    nodes[1] = createNode(1, "Station 1");
    nodes[2] = createNode(2, "Station 2");
    nodes[3] = createNode(3, "Station 3");
    nodes[4] = createNode(4, "Station 4");
    nodes[5] = createNode(5, "Station 5");
    nodes[6] = createNode(6, "Station 6");
    nodes[7] = createNode(7, "Station 7");
    nodes[8] = createNode(8, "Station 8");
    nodes[9] = createNode(9, "Station 9");

    // Adding edges between stations (representing metro routes)
    addEdge(metro, 1, 2, "Station 1", "Station 2");
    addEdge(metro, 2, 3, "Station 2", "Station 3");
    addEdge(metro, 3, 4, "Station 3", "Station 4");
    addEdge(metro, 4, 5, "Station 4", "Station 5");
    addEdge(metro, 7, 6, "Station 7", "Station 6");
    addEdge(metro, 6, 3, "Station 6", "Station 3");
    addEdge(metro, 3, 8, "Station 3", "Station 8");
    addEdge(metro, 8, 9, "Station 8", "Station 9");
    
    TicketList ticketList;
    ticketList.head = NULL;
    int book = 1;
    printf("\nDo you want to book ticket? If yes, enter 1; else, enter 0: ");
        scanf("%d", &book);

        if (book == 1) {
            int start, destination;
            printf("Enter the start station: ");
            scanf("%d", &start);
            printf("Enter the destination station: ");
            scanf("%d", &destination);

            addTicket(&ticketList, start, destination);
            findRoute(metro, nodes, start, destination);
        } else {
            printf("You chose not to book another ticket.\n");
        }


    while (book) {
        printf("\n\n\n");
        printf("\nDo you want to book another ticket? If yes, enter 1; else, enter 0: ");
        scanf("%d", &book);

        if (book == 1) {
            int start, destination;
            printf("Enter the start station: ");
            scanf("%d", &start);
            printf("Enter the destination station: ");
            scanf("%d", &destination);

            addTicket(&ticketList, start, destination);
            findRoute(metro, nodes, start, destination);
        } else {
            printf("You chose not to book another ticket.\n");
        }
    }
    printTickets(&ticketList);
    // Freeing allocated memory
    for (int i = 1; i <= num_stations; ++i) {
        free(nodes[i]);
    }
    free(nodes);
    free(metro->adjacency_list);
    free(metro);
    // Freeing ticket linked list
    Ticket* current = ticketList.head;
    while (current != NULL) {
        Ticket* temp = current;
        current = current->next;
        free(temp);
    }
    return 0;
}
