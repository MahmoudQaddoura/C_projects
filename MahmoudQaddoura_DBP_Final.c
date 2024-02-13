#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define ENTITY_COUNT 7

//entity structs
typedef struct {
    int branchID;
    char name[50];
    char phone[20];
    char address[100];
    int numberOfRooms;
} Branch;

typedef struct {
    int roomID;
    int number;
    int floor;
    int cleaningCrewID;  
    int branchID;       
} Room;

typedef struct {
    int customerID;
    char SSN[11];
    char name[50];
    char dateOfBirth[10];
    char email[50];
    int bookingID;  
    char phone[20];
} Customer;

typedef struct {
    int cleaningCrewID;
    char employees[50];  
    char status[20];
    int roomID;    
} CleaningCrew;

typedef struct {
    int bookingID;
    char date[10];
    char time[10];
    char customerSSN[11]; 
    int registrarID;     
    int roomNumber;     
} Booking;

typedef struct {
    int employeeID;
    char name[50];
    char password[20];
    char email[50];
    double salary;
    int branchID;
    char position[20];
} Employee;

typedef struct {
    int phoneID;
    char number[20];
    char customerSSN[11];
} Phone;

// Linked list node structures
typedef struct BranchNode {
   Branch data;
   struct BranchNode *next;
} BranchNode;

typedef struct RoomNode {
   Room data;
   struct RoomNode *next;
} RoomNode;

typedef struct CustomerNode {
   Customer data;
   struct CustomerNode *next;
} CustomerNode;

typedef struct CleaningCrewNode {
   CleaningCrew data;
   struct CleaningCrewNode *next;
} CleaningCrewNode;

typedef struct BookingNode {
   Booking data;
   struct BookingNode *next;
} BookingNode;

typedef struct EmployeeNode {
   Employee data;
   struct EmployeeNode *next;
} EmployeeNode;

typedef struct PhoneNode {
   Phone data;
   struct PhoneNode *next;
} PhoneNode;

BranchNode *branchHead = NULL;
RoomNode *roomHead = NULL;
CustomerNode *customerHead = NULL;
CleaningCrewNode *cleaningCrewHead = NULL;
BookingNode *bookingHead = NULL;
EmployeeNode *employeeHead = NULL;
PhoneNode *phoneHead = NULL;

//generic CRUD operations and thread function prototypes
void createRecord(void **head, int entityType);
void *readRecord(void* head, int entityType);
void updateRecord(void *head, int entityType);
void deleteRecord(void **head, int entityType);
void* threadFunction(void* args);

//enums that point to entity type to be used in generic CRUD functions
typedef enum {
    ENTITY_BRANCH,
    ENTITY_ROOM,
    ENTITY_CUSTOMER,
    ENTITY_CLEANING_CREW,
    ENTITY_BOOKING,
    ENTITY_EMPLOYEE,
    ENTITY_PHONE
} EntityType;

//globaly declared array of mutexes
pthread_mutex_t entityMutexes[ENTITY_COUNT];

//mutex for db log file
pthread_mutex_t fileMutex = PTHREAD_MUTEX_INITIALIZER;

// Path to the log file
const char* logFilePath = "operation_log.txt";

//CRUD functions operations implementation with mutex Lock and Unlock to handle concurruncy
//create
void createRecord(void **head, int entityType) {
    pthread_mutex_lock(&entityMutexes[entityType - 1]);
     switch (entityType) {
        case ENTITY_BRANCH: {
            BranchNode *newNode = (BranchNode *)malloc(sizeof(BranchNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new BranchNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(BranchNode));

            printf("Enter new branch ID: ");
            scanf("%d", &newNode->data.branchID);
            printf("Enter new branch name: ");
            scanf("%49s", newNode->data.name);
            printf("Enter new branch phone: ");
            scanf("%19s", newNode->data.phone);
            printf("Enter new branch address: ");
            scanf(" %99[^\n]", newNode->data.address); // Note: space before % to consume any leftover newline
            printf("Enter number of rooms: ");
            scanf("%d", &newNode->data.numberOfRooms);

            newNode->next = *(BranchNode **)head;
            *(BranchNode **)head = newNode;
            break;
        }
        case ENTITY_ROOM: {
            RoomNode *newNode = (RoomNode *)malloc(sizeof(RoomNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new RoomNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(RoomNode));

            printf("Enter new room ID: ");
            scanf("%d", &newNode->data.roomID);
            printf("Enter room number: ");
            scanf("%d", &newNode->data.number);
            printf("Enter floor: ");
            scanf("%d", &newNode->data.floor);
            printf("Enter cleaning crew ID: ");
            scanf("%d", &newNode->data.cleaningCrewID);
            printf("Enter branch ID: ");
            scanf("%d", &newNode->data.branchID);

            newNode->next = *(RoomNode **)head;
            *(RoomNode **)head = newNode;
            break;
        }
        case ENTITY_CUSTOMER: {
            CustomerNode *newNode = (CustomerNode *)malloc(sizeof(CustomerNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new CustomerNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(CustomerNode));

            printf("Enter new customer ID: ");
            scanf("%d", &newNode->data.customerID);
            printf("Enter customer SSN: ");
            scanf("%19s", newNode->data.SSN);
            printf("Enter customer name: ");
            scanf(" %49[^\n]", newNode->data.name);
            printf("Enter date of birth (YYYY-MM-DD): ");
            scanf("%10s", newNode->data.dateOfBirth);
            printf("Enter email: ");
            scanf(" %49[^\n]", newNode->data.email);
            printf("Enter booking ID: ");
            scanf("%d", &newNode->data.bookingID);
            printf("Enter phone: ");
            scanf("%19s", newNode->data.phone);

            newNode->next = *(CustomerNode **)head;
            *(CustomerNode **)head = newNode;
            break;
        }
        case ENTITY_CLEANING_CREW: {
            CleaningCrewNode *newNode = (CleaningCrewNode *)malloc(sizeof(CleaningCrewNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new CleaningCrewNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(CleaningCrewNode));

            printf("Enter cleaning crew ID: ");
            scanf("%d", &newNode->data.cleaningCrewID);
            printf("Enter employees names: ");
            scanf(" %49[^\n]", newNode->data.employees);
            printf("Enter status: ");
            scanf(" %19[^\n]", newNode->data.status);
            printf("Enter room ID: ");
            scanf("%d", &newNode->data.roomID);

            newNode->next = *(CleaningCrewNode **)head;
            *(CleaningCrewNode **)head = newNode;
            break;
        }
        case ENTITY_BOOKING: {
            BookingNode *newNode = (BookingNode *)malloc(sizeof(BookingNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new BookingNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(BookingNode));

            printf("Enter booking ID: ");
            scanf("%d", &newNode->data.bookingID);
            printf("Enter date (YYYY-MM-DD): ");
            scanf("%10s", newNode->data.date);
            printf("Enter time (HH:MM): ");
            scanf("%10s", newNode->data.time);
            printf("Enter customer SSN: ");
            scanf("%11s", newNode->data.customerSSN);
            printf("Enter registrar ID: ");
            scanf("%d", &newNode->data.registrarID);
            printf("Enter room number: ");
            scanf("%d", &newNode->data.roomNumber);

            newNode->next = *(BookingNode **)head;
            *(BookingNode **)head = newNode;
            break;
        }
        case ENTITY_EMPLOYEE: {
            EmployeeNode *newNode = (EmployeeNode *)malloc(sizeof(EmployeeNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new EmployeeNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(EmployeeNode));

            printf("Enter employee ID: ");
            scanf("%d", &newNode->data.employeeID);
            printf("Enter name: ");
            scanf(" %49[^\n]", newNode->data.name);
            printf("Enter password: ");
            scanf("%19s", newNode->data.password);
            printf("Enter email: ");
            scanf(" %49[^\n]", newNode->data.email);
            printf("Enter salary: ");
            scanf("%lf", &newNode->data.salary);
            printf("Enter branch ID: ");
            scanf("%d", &newNode->data.branchID);
            printf("Enter position: ");
            scanf(" %19[^\n]", newNode->data.position);

            newNode->next = *(EmployeeNode **)head;
            *(EmployeeNode **)head = newNode;
            break;
        }
        case ENTITY_PHONE: {
            PhoneNode *newNode = (PhoneNode *)malloc(sizeof(PhoneNode));
            if (newNode == NULL) {
                perror("Failed to allocate memory for new PhoneNode");
                pthread_mutex_unlock(&entityMutexes[entityType - 1]);
                return;
            }
            memset(newNode, 0, sizeof(PhoneNode));

            printf("Enter phone ID: ");
            scanf("%d", &newNode->data.phoneID);
            printf("Enter number: ");
            scanf("%19s", newNode->data.number);
            printf("Enter customer SSN: ");
            scanf("%11s", newNode->data.customerSSN);

            newNode->next = *(PhoneNode **)head;
            *(PhoneNode **)head = newNode;
            break;
        }
    }

    pthread_mutex_unlock(&entityMutexes[entityType - 1]);
}
    }
        pthread_mutex_unlock(&entityMutexes[entityType - 1]);

}


//read
void *readRecord(void *head, int entityType) {
    pthread_mutex_lock(&entityMutexes[entityType - 1]);
    switch (entityType) {
        case ENTITY_BRANCH: {
            BranchNode *current = (BranchNode *)head;
            return current ? &current->data : NULL;
        }
        case ENTITY_ROOM: {
            RoomNode *current = (RoomNode *)head;
            return current ? &current->data : NULL;
        }
        case ENTITY_CUSTOMER: {
            CustomerNode *current = (CustomerNode *)head;
            return current ? &current->data : NULL;
        }
        case ENTITY_CLEANING_CREW: {
            CleaningCrewNode *current = (CleaningCrewNode *)head;
            return current ? &current->data : NULL;
        }
        case ENTITY_BOOKING: {
            BookingNode *current = (BookingNode *)head;
            return current ? &current->data : NULL;
        }
        case ENTITY_EMPLOYEE: {
            EmployeeNode *current = (EmployeeNode *)head;
            return current ? &current->data : NULL;
        }
        case ENTITY_PHONE: {
            PhoneNode *current = (PhoneNode *)head;
            return current ? &current->data : NULL;
        }
        default: 
            return NULL;
    }
        pthread_mutex_unlock(&entityMutexes[entityType - 1]);

}

//update
void updateRecord(void **head, int entityType, int searchID) {
    pthread_mutex_lock(&entityMutexes[entityType - 1]);

    // Assuming each entity type has a unique ID to search and update for
    switch (entityType) {
        case ENTITY_BRANCH: {
            BranchNode **current = (BranchNode **)head;
            while (*current) {
                if ((*current)->data.branchID == searchID) {
                    printf("Updating Branch ID %d\n", searchID);
                    printf("Enter new branch name: ");
                    scanf(" %49[^\n]", (*current)->data.name);
                    printf("Enter new branch phone: ");
                    scanf(" %19[^\n]", (*current)->data.phone);
                    printf("Enter new branch address: ");
                    getchar();
                    fgets((*current)->data.address, 100, stdin);
                    (*current)->data.address[strcspn((*current)->data.address, "\n")] = 0;
                    printf("Enter number of rooms: ");
                    scanf("%d", &(*current)->data.numberOfRooms);
            break;
                }
                current = &(*current)->next;
            }
            break;
        }
        case ENTITY_ROOM: {
            RoomNode **current = (RoomNode **)head;
            while (*current) {
                if ((*current)->data.roomID == searchID) {
                    printf("Updating Room ID %d\n", searchID);
                    printf("Updating room with ID %d\n", roomID);
                    printf("Enter room number: ");
                    scanf("%d", &current->data.number);
                    printf("Enter floor: ");
                    scanf("%d", &current->data.floor);
                    printf("Enter cleaning crew ID: ");
                    scanf("%d", &current->data.cleaningCrewID);
                    printf("Enter branch ID: ");
                    scanf("%d", &current->data.branchID);
                    break;
                }
                current = &(*current)->next;
            }
            break;
        }
        case ENTITY_CUSTOMER: {
            CustomerNode **current = (CustomerNode **)head;
            while (*current) {
                if ((*current)->data.customerID == searchID) {
                    printf("Updating Customer ID %d\n", searchID);
                    printf("Updating customer with ID %d\n", customerID);
                    printf("Enter customer SSN: ");
                    scanf("%10s", current->data.SSN);
                    printf("Enter customer name: ");
                    scanf("%49s", current->data.name);
                    printf("Enter date of birth (YYYY-MM-DD): ");
                    scanf("%9s", current->data.dateOfBirth);
                    printf("Enter email: ");
                    scanf("%49s", current->data.email);
                    printf("Enter booking ID: ");
                    scanf("%d", &current->data.bookingID);
                    printf("Enter phone: ");
                    scanf("%19s", current->data.phone);                    
                    break;
                }
                current = &(*current)->next;
            }
            break;
        }
        case ENTITY_CLEANING_CREW: {
            CleaningCrewNode **current = (CleaningCrewNode **)head;
            while (*current) {
                if ((*current)->data.cleaningCrewID == searchID) {
                    printf("Updating Cleaning Crew ID %d\n", searchID);
                    printf("Updating cleaning crew with ID %d\n", cleaningCrewID);
                    printf("Enter employees: ");
                    scanf("%49s", current->data.employees); // Consider using fgets for string with spaces
                    printf("Enter status: ");
                    scanf("%19s", current->data.status);
                    printf("Enter room ID: ");
                    scanf("%d", &current->data.roomID);                    
                    break;
                }
                current = &(*current)->next;
            }
            break;
        }
        case ENTITY_BOOKING: {
            BookingNode **current = (BookingNode **)head;
            while (*current) {
                if ((*current)->data.bookingID == searchID) {
                printf("Updating Booking ID %d\n", searchID);
                printf("Updating booking with ID %d\n", bookingID);
                printf("Enter date (YYYY-MM-DD): ");
                scanf("%9s", current->data.date);
                printf("Enter time (HH:MM): ");
                scanf("%9s", current->data.time);
                printf("Enter customer SSN: ");
                scanf("%10s", current->data.customerSSN);
                printf("Enter registrar ID: ");
                scanf("%d", &current->data.registrarID);
                printf("Enter room number: ");
                scanf("%d", &current->data.roomNumber);
                    break;
                }
                current = &(*current)->next;
            }
            break;
        }
        case ENTITY_EMPLOYEE: {
            EmployeeNode **current = (EmployeeNode **)head;
            while (*current) {
                if ((*current)->data.employeeID == searchID) {
                printf("Updating Employee ID %d\n", searchID);
                printf("Updating employee with ID %d\n", employeeID);
                printf("Enter name: ");
                scanf("%49s", current->data.name);
                printf("Enter password: ");
                scanf("%19s", current->data.password);
                printf("Enter email: ");
                scanf("%49s", current->data.email);
                printf("Enter salary: ");
                scanf("%lf", &current->data.salary);
                printf("Enter branch ID: ");
                scanf("%d", &current->data.branchID);
                printf("Enter position: ");
                scanf("%19s", current->data.position);                    
                break;
                }
                current = &(*current)->next;
            }
            break;
        }
        case ENTITY_PHONE: {
            PhoneNode **current = (PhoneNode **)head;
            while (*current) {
                if ((*current)->data.phoneID == searchID) {
                    printf("Updating Phone ID %d\n", searchID);
                    printf("Updating phone record with ID %d\n", phoneID);
                    printf("Enter number: ");
                    scanf("%19s", current->data.number);
                    printf("Enter customer SSN: ");
                    scanf("%10s", current->data.customerSSN);
                    break;
                }
                current = &(*current)->next;
            }
            break;
        }
        default:
            printf("Invalid entity type.\n");
            break;
    }

    pthread_mutex_unlock(&entityMutexes[entityType - 1]);
}

/*
void updateRecord(void *head, int entityType) {
    pthread_mutex_lock(&entityMutexes[entityType - 1]);
    switch (entityType) {
        case ENTITY_BRANCH: {
            int branchID;
            printf("Enter Branch ID of the branch to update: ");
            scanf("%d", &branchID);

            BranchNode *current = (BranchNode *)head;
            while (current != NULL && current->data.branchID != branchID) {
                current = current->next;
            }
            if (current != NULL) {
                printf("Updating branch with ID %d\n", branchID);
                printf("Enter new branch name: ");
                scanf("%49s", current->data.name); // Consider using fgets for safer input
                printf("Enter new branch phone: ");
                scanf("%19s", current->data.phone);
                printf("Enter new branch address: ");
                // Using getchar() to consume the newline left by scanf before fgets
                getchar();
                fgets(current->data.address, 100, stdin);
                current->data.address[strcspn(current->data.address, "\n")] = 0; // Remove newline character
                printf("Enter number of rooms: ");
                scanf("%d", &current->data.numberOfRooms);
            } else {
                printf("Branch with ID %d not found.\n", branchID);
            }
            break;
        }
        case ENTITY_ROOM: {
            int roomID;
            printf("Enter Room ID of the room to update: ");
            scanf("%d", &roomID);

            RoomNode *current = (RoomNode *)head;
            while (current != NULL && current->data.roomID != roomID) {
                current = current->next;
            }
            if (current != NULL) {
                printf("Updating room with ID %d\n", roomID);
                printf("Enter room number: ");
                scanf("%d", &current->data.number);
                printf("Enter floor: ");
                scanf("%d", &current->data.floor);
                printf("Enter cleaning crew ID: ");
                scanf("%d", &current->data.cleaningCrewID);
                printf("Enter branch ID: ");
                scanf("%d", &current->data.branchID);
            } else {
                printf("Room with ID %d not found.\n", roomID);
            }
            break;
        }
        case ENTITY_CUSTOMER: {
            int customerID;
            printf("Enter Customer ID of the customer to update: ");
            scanf("%d", &customerID);

            CustomerNode *current = (CustomerNode *)head;
            while (current != NULL && current->data.customerID != customerID) {
                current = current->next;
            }
            if (current != NULL) {
                printf("Updating customer with ID %d\n", customerID);
                printf("Enter customer SSN: ");
                scanf("%10s", current->data.SSN);
                printf("Enter customer name: ");
                scanf("%49s", current->data.name);
                printf("Enter date of birth (YYYY-MM-DD): ");
                scanf("%9s", current->data.dateOfBirth);
                printf("Enter email: ");
                scanf("%49s", current->data.email);
                printf("Enter booking ID: ");
                scanf("%d", &current->data.bookingID);
                printf("Enter phone: ");
                scanf("%19s", current->data.phone);
            } else {
                printf("Customer with ID %d not found.\n", customerID);
            }
            break;
        }
        case ENTITY_CLEANING_CREW: {
            int cleaningCrewID;
            printf("Enter Cleaning Crew ID of the cleaning crew to update: ");
            scanf("%d", &cleaningCrewID);

            CleaningCrewNode *current = (CleaningCrewNode *)head;
            while (current != NULL && current->data.cleaningCrewID != cleaningCrewID) {
                current = current->next;
            }
            if (current != NULL) {
                printf("Updating cleaning crew with ID %d\n", cleaningCrewID);
                printf("Enter employees: ");
                scanf("%49s", current->data.employees); // Consider using fgets for string with spaces
                printf("Enter status: ");
                scanf("%19s", current->data.status);
                printf("Enter room ID: ");
                scanf("%d", &current->data.roomID);
            } else {
                printf("Cleaning Crew with ID %d not found.\n", cleaningCrewID);
            }
            break;
        }
        case ENTITY_BOOKING: {
            int bookingID;
            printf("Enter Booking ID of the booking to update: ");
            scanf("%d", &bookingID);

            BookingNode *current = (BookingNode *)head;
            while (current != NULL && current->data.bookingID != bookingID) {
                current = current->next;
            }
            if (current != NULL) {
                printf("Updating booking with ID %d\n", bookingID);
                printf("Enter date (YYYY-MM-DD): ");
                scanf("%9s", current->data.date);
                printf("Enter time (HH:MM): ");
                scanf("%9s", current->data.time);
                printf("Enter customer SSN: ");
                scanf("%10s", current->data.customerSSN);
                printf("Enter registrar ID: ");
                scanf("%d", &current->data.registrarID);
                printf("Enter room number: ");
                scanf("%d", &current->data.roomNumber);
            } else {
                printf("Booking with ID %d not found.\n", bookingID);
            }
            break;
        }
        case ENTITY_EMPLOYEE: {
            int employeeID;
            printf("Enter Employee ID of the employee to update: ");
            scanf("%d", &employeeID);

            EmployeeNode *current = (EmployeeNode *)head;
            while (current != NULL && current->data.employeeID != employeeID) {
                current = current->next;
            }
            if (current != NULL) {
                printf("Updating employee with ID %d\n", employeeID);
                printf("Enter name: ");
                scanf("%49s", current->data.name);
                printf("Enter password: ");
                scanf("%19s", current->data.password);
                printf("Enter email: ");
                scanf("%49s", current->data.email);
                printf("Enter salary: ");
                scanf("%lf", &current->data.salary);
                printf("Enter branch ID: ");
                scanf("%d", &current->data.branchID);
                printf("Enter position: ");
                scanf("%19s", current->data.position);
            } else {
                printf("Employee with ID %d not found.\n", employeeID);
            }
            break;
        }
        case ENTITY_PHONE: {
            int phoneID;
            printf("Enter Phone ID of the phone record to update: ");
            scanf("%d", &phoneID);

            PhoneNode *current = (PhoneNode *)head;
            while (current != NULL && current->data.phoneID != phoneID) {
                current = current->next;
            }
            if (current != NULL) {
                printf("Updating phone record with ID %d\n", phoneID);
                printf("Enter number: ");
                scanf("%19s", current->data.number);
                printf("Enter customer SSN: ");
                scanf("%10s", current->data.customerSSN);
            } else {
                printf("Phone record with ID %d not found.\n", phoneID);
            }
            break;
        }
    }
    pthread_mutex_unlock(&entityMutexes[entityType - 1]);
}
*/

//delete
void deleteRecord(void **head, int entityType) {
    pthread_mutex_lock(&entityMutexes[entityType - 1]);
    switch (entityType) {
        case ENTITY_BRANCH: {
            BranchNode *temp = *(BranchNode **)head;
            if (temp) {
                *(BranchNode **)head = temp->next;
                free(temp);
            }
            break;
        }
        case ENTITY_ROOM: {
            RoomNode *temp = *(RoomNode **)head;
            if (temp) {
                *(RoomNode **)head = temp->next;
                free(temp);
            }
            break;
        }
        case ENTITY_CUSTOMER: {
            CustomerNode *temp = *(CustomerNode **)head;
            if (temp) {
                *(CustomerNode **)head = temp->next;
                free(temp);
            }
            break;
        }
        case ENTITY_CLEANING_CREW: {
            CleaningCrewNode *temp = *(CleaningCrewNode **)head;
            if (temp) {
                *(CleaningCrewNode **)head = temp->next;
                free(temp);
            }
            break;
        }
        case ENTITY_BOOKING: {
            BookingNode *temp = *(BookingNode **)head;
            if (temp) {
                *(BookingNode **)head = temp->next;
                free(temp);
            }
            break;
        }
        case ENTITY_EMPLOYEE: {
            EmployeeNode *temp = *(EmployeeNode **)head;
            if (temp) {
                *(EmployeeNode **)head = temp->next;
                free(temp);
            }
            break;
        }
        case ENTITY_PHONE: {
            PhoneNode *temp = *(PhoneNode **)head;
            if (temp) {
                *(PhoneNode **)head = temp->next;
                free(temp);
            }
            break;
        }
    }
    pthread_mutex_unlock(&entityMutexes[entityType - 1]);
}

//2 level menus for user input
void displayEntityMenu() {
    printf("Select an entity:\n");
    printf("1. Branch\n");
    printf("2. Room\n");
    printf("3. Customer\n");
    printf("4. Cleaning Crew\n");
    printf("5. Booking\n");
    printf("6. Employee\n");
    printf("7. Phone\n");
    printf("8. Back\n");
}

void displayCRUDMenu() {
    printf("Select an operation:\n");
    printf("1. Create\n");
    printf("2. Read\n");
    printf("3. Update\n");
    printf("4. Delete\n");
    printf("5. Back\n");
}

//thread function that handles concurrent access to the program using mutexes
void *threadFunction(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    int operationChoice;
    int entityChoice;

    while (1) {
        printf("\nChoose operation: 1. Create 2. Read 3. Update 4. Delete 5. Exit\n");
        scanf("%d", &operationChoice);

        if (operationChoice == 5) {
            break;
        }

        printf("\nChoose entity type: 1. Branch 2. Room 3. Customer 4. Cleaning Crew 5. Booking 6. Employee 7. Phone\n");
        scanf("%d", &entityChoice);

        switch (operationChoice) {
            case 1:
                createRecord(threadArgs->head, entityChoice, NULL);
                break;
            case 2:
                readRecord(threadArgs->head, entityChoice);
                break;
            case 3:
                updateRecord(threadArgs->head, entityChoice);
                break;
            case 4:
                deleteRecord(threadArgs->head, entityChoice);
                break;
            default:
                printf("Invalid operation choice\n");
                break;
        }
    }

    return NULL;
}

//clean up functions for each entity for exit
void freeBranchLinkedList(BranchNode *head) {
    BranchNode *current = head;
    while (current != NULL) {
        BranchNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freeRoomLinkedList(RoomNode *head) {
    RoomNode *current = head;
    while (current != NULL) {
        RoomNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freeCustomerLinkedList(CustomerNode *head) {
    CustomerNode *current = head;
    while (current != NULL) {
        CustomerNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freeCleaningCrewLinkedList(CleaningCrewNode *head) {
    CleaningCrewNode *current = head;
    while (current != NULL) {
        CleaningCrewNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freeBookingLinkedList(BookingNode *head) {
    BookingNode *current = head;
    while (current != NULL) {
        BookingNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freeEmployeeLinkedList(EmployeeNode *head) {
    EmployeeNode *current = head;
    while (current != NULL) {
        EmployeeNode *temp = current;
        current = current->next;
        free(temp);
    }
}

void freePhoneLinkedList(PhoneNode *head) {
    PhoneNode *current = head;
    while (current != NULL) {
        PhoneNode *temp = current;
        current = current->next;
        free(temp);
    }
}

//function to log operations to a file
void logOperation(const char* operation) {
    pthread_mutex_lock(&fileMutex);
    FILE *logFile = fopen(logFilePath, "a");
    if (logFile) {
        fprintf(logFile, "%s\n", operation);
        fclose(logFile);
    } else {
        perror("Failed to open log file");
    }
    pthread_mutex_unlock(&fileMutex);
}

typedef struct ThreadArgs {
    pthread_mutex_t *mutexes;
    int entityType;
    int operationType;
    void **heads; // Array of pointers to the head of each linked list
} ThreadArgs;


void* userInteractionThread(void* arg) {
    ThreadArgs *args = (ThreadArgs*) arg;

    extern BranchNode *branchHead;
    extern RoomNode *roomHead;
    extern CustomerNode *customerHead;
    extern CleaningCrewNode *cleaningCrewHead;
    extern BookingNode *bookingHead;
    extern EmployeeNode *employeeHead;
    extern PhoneNode *phoneHead;

    void *selectedHead = NULL;
    
    switch (args->entityType) {
        case ENTITY_BRANCH:
            selectedHead = &branchHead;
            break;
        case ENTITY_ROOM:
            selectedHead = &roomHead;
            break;
        case ENTITY_CUSTOMER:
            selectedHead = &customerHead;
            break;
        case ENTITY_CLEANING_CREW:
            selectedHead = &cleaningCrewHead;
            break;
        case ENTITY_BOOKING:
            selectedHead = &bookingHead;
            break;
        case ENTITY_EMPLOYEE:
            selectedHead = &employeeHead;
            break;
        case ENTITY_PHONE:
            selectedHead = &phoneHead;
            break;
    }

    if (selectedHead != NULL) {
        pthread_mutex_lock(&args->mutexes[args->entityType - 1]);

        printf("Thread handling entity type %d\n", args->entityType);

        switch (args->operationType) {
            case 1:
                createRecord(selectedHead, args->entityType);
                break;
            case 2:
                // Assume readRecord is updated accordingly
                break;
            case 3:
                // Assume updateRecord is updated accordingly
                break;
            case 4:
                // Assume deleteRecord is updated accordingly
                break;
            default:
                printf("Invalid operation. No action taken.\n");
                break;
        }

        pthread_mutex_unlock(&args->mutexes[args->entityType - 1]);
    } else {
        printf("Invalid entity type provided.\n");
    }

    pthread_exit(NULL);
}


int main() {
    pthread_mutex_t mutexes[ENTITY_COUNT];
    for (int i = 0; i < ENTITY_COUNT; i++) {
        pthread_mutex_init(&mutexes[i], NULL);
    }
    
    void *heads[ENTITY_COUNT] = {&branchHead, &roomHead, &customerHead, &cleaningCrewHead, &bookingHead, &employeeHead, &phoneHead};

    while (1) {
        int entityChoice = 0;
        int operationChoice = 0;

        displayEntityMenu();
        printf("Enter your choice (or 8 to exit): ");
        scanf("%d", &entityChoice);
        if (entityChoice == 8) break; // Exit program

        displayCRUDMenu();
        printf("Enter your choice (or 5 to go back): ");
        scanf("%d", &operationChoice);
        if (operationChoice == 5) continue; // Go back to the entity selection

        ThreadArgs threadArgs;
        threadArgs.mutexes = mutexes;
        threadArgs.entityType = entityChoice;
        threadArgs.operationType = operationChoice;
        threadArgs.heads = heads;

        pthread_t tid;
        pthread_create(&tid, NULL, userInteractionThread, (void*)&threadArgs);
        pthread_join(tid, NULL); // Wait for the thread to complete its operation
    }

    // Cleanup
    for (int i = 0; i < ENTITY_COUNT; i++) {
        pthread_mutex_destroy(&mutexes[i]);
    }
    
    freeBranchLinkedList(branchHead);
    freeRoomLinkedList(roomHead);
    freeCustomerLinkedList(customerHead);
    freeCleaningCrewLinkedList(cleaningCrewHead);
    freeBookingLinkedList(bookingHead);
    freeEmployeeLinkedList(employeeHead);
    freePhoneLinkedList(phoneHead);
    return 0;
}
