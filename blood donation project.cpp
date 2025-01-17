#include <iostream>
#include <string>
using namespace std;

// Structure for Donor Node in Doubly Linked List
struct Donor {
    int donorID;
    string name;
    string bloodGroup;
    string contact;
    string city;
    string lastDonation;
    Donor* next;
    Donor* prev; // Added prev pointer for doubly linked list
};

// Structure for Inventory
struct BloodInventory {
    string bloodGroup;
    int quantity;
};

// Global Variables
Donor* donorHead = NULL; // Head of donor doubly linked list
Donor* donorTail = NULL; // Tail of donor doubly linked list (to add donors efficiently)
const int MAX_REQUESTS = 100; // Maximum number of requests in the queue
int front = -1, rear = -1;    // Front and rear for the request queue
BloodInventory inventory[8] = {
    {"A+", 0}, {"A-", 0}, {"B+", 0}, {"B-", 0},
    {"AB+", 0}, {"AB-", 0}, {"O+", 0}, {"O-", 0}
};

// Array for Blood Request Queue
struct BloodRequest {
    string bloodGroup;
    string patientName;
} requestQueue[MAX_REQUESTS];

// Function to add a donor to the doubly linked list
void addDonor(int id, string name, string bloodGroup, string contact, string city, string lastDonation) {
    Donor* newDonor = new Donor{id, name, bloodGroup, contact, city, lastDonation, NULL, NULL};
    if (donorHead == NULL) {
        donorHead = donorTail = newDonor; // Initialize both head and tail
    } else {
        donorTail->next = newDonor; // Attach new donor to the end
        newDonor->prev = donorTail; // Set the prev pointer of the new donor to the old tail
        donorTail = newDonor;       // Update the tail pointer
    }
    cout << "Donor added successfully!\n";
}

// Function to search donors by blood group
void searchDonor(string bloodGroup) {
    if (donorHead == NULL) {
        cout << "No donors available.\n";
        return;
    }

    Donor* temp = donorHead;
    bool found = false;

    cout << "Donors with Blood Group " << bloodGroup << ":\n";
    while (temp != NULL) {
        if (temp->bloodGroup == bloodGroup) {
            cout << "ID: " << temp->donorID << ", Name: " << temp->name
                 << ", Contact: " << temp->contact
                 << ", City: " << temp->city
                 << ", Last Donation: " << temp->lastDonation << "\n";
            found = true;
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "No donors found with blood group " << bloodGroup << ".\n";
    }
}

// Function to delete a donor by ID
void deleteDonor(int donorID) {
    if (donorHead == NULL) {
        cout << "No donors available to delete.\n";
        return;
    }

    Donor* temp = donorHead;

    // If the donor to be deleted is the head node
    if (temp != NULL && temp->donorID == donorID) {
        donorHead = temp->next; // Move the head to the next node
        if (donorHead != NULL) donorHead->prev = NULL; // Set prev pointer of new head to NULL
        delete temp;            // Free memory
        cout << "Donor with ID " << donorID << " deleted successfully!\n";
        return;
    }

    // Search for the donor to be deleted
    while (temp != NULL && temp->donorID != donorID) {
        temp = temp->next;
    }

    // If donor with the given ID is not found
    if (temp == NULL) {
        cout << "Donor with ID " << donorID << " not found.\n";
        return;
    }

    // Unlink the donor from the doubly linked list
    if (temp->next != NULL) {
        temp->next->prev = temp->prev; // Set prev pointer of next donor
    }
    if (temp->prev != NULL) {
        temp->prev->next = temp->next; // Set next pointer of previous donor
    }
    delete temp; // Free memory
    cout << "Donor with ID " << donorID << " deleted successfully!\n";
}

// Function to view all donors
void viewDonors() {
    if (donorHead == NULL) {
        cout << "No donors available.\n";
        return;
    }
    Donor* temp = donorHead;
    cout << "Donor List:\n";
    while (temp != NULL) {
        cout << "ID: " << temp->donorID << ", Name: " << temp->name
             << ", Blood Group: " << temp->bloodGroup
             << ", Contact: " << temp->contact
             << ", City: " << temp->city
             << ", Last Donation: " << temp->lastDonation << "\n";
        temp = temp->next;
    }
}

// Function to add blood to inventory
void addBlood(string bloodGroup, int quantity) {
    for (int i = 0; i < 8; i++) {
        if (inventory[i].bloodGroup == bloodGroup) {
            inventory[i].quantity += quantity;
            cout << quantity << " units of " << bloodGroup << " added to inventory.\n";
            return;
        }
    }
    cout << "Invalid blood group.\n";
}

// Function to view the current blood inventory
void viewInventory() {
    cout << "Current Blood Inventory:\n";
    for (int i = 0; i < 8; i++) {
        cout << "Blood Group: " << inventory[i].bloodGroup
             << ", Quantity: " << inventory[i].quantity << " units\n";
    }
}

// Function to check if the queue is full
bool isQueueFull() {
    return (rear + 1) % MAX_REQUESTS == front;
}

// Function to check if the queue is empty
bool isQueueEmpty() {
    return front == -1;
}

// Function to add a blood request to the queue
void addRequest(string bloodGroup, string patientName) {
    if (isQueueFull()) {
        cout << "The blood request queue is full. Cannot add new requests.\n";
        return;
    }
    if (isQueueEmpty()) {
        front = 0; // Initialize front if the queue is empty
    }
    rear = (rear + 1) % MAX_REQUESTS;
    requestQueue[rear] = {bloodGroup, patientName};
    cout << "Blood request added successfully!\n";
}

// Function to process the next blood request
void processRequest() {
    if (isQueueEmpty()) {
        cout << "No pending requests.\n";
        return;
    }

    BloodRequest currentRequest = requestQueue[front];
    bool found = false;

    // Check inventory for the requested blood group
    for (int i = 0; i < 8; i++) {
        if (inventory[i].bloodGroup == currentRequest.bloodGroup) {
            if (inventory[i].quantity > 0) {
                inventory[i].quantity--;
                cout << "Processing Blood Request - Blood Group: " << currentRequest.bloodGroup
                     << ", Patient Name: " << currentRequest.patientName << "\n";
                cout << "Request fulfilled successfully.\n";
                found = true;
            } else {
                cout << "Insufficient stock for Blood Group: " << currentRequest.bloodGroup << ".\n";
            }
            break;
        }
    }

    if (!found) {
        cout << "Invalid blood group requested.\n";
    }

    if (front == rear) {
        front = rear = -1; // Queue becomes empty
    } else {
        front = (front + 1) % MAX_REQUESTS;
    }
}

// Main Function
int main() {
    int choice;
    while (true) {
        cout << "\nBlood Donation System Menu:\n";
        cout << "1. Add Donor\n2. Search Donor\n3. Delete Donor\n4. View Donors\n";
        cout << "5. Add Blood to Inventory\n6. View Inventory\n7. Add Blood Request\n";
        cout << "8. Process Blood Request\n9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            string name, bloodGroup, contact, city, lastDonation;
            cout << "Enter Donor ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Blood Group: ";
            cin >> bloodGroup;
            cout << "Enter Contact: ";
            cin >> contact;
            cout << "Enter City: ";
            cin >> city;
            cout << "Enter Last Donation Date: ";
            cin >> lastDonation;
            addDonor(id, name, bloodGroup, contact, city, lastDonation);
        } else if (choice == 2) {
            string bloodGroup;
            cout << "Enter Blood Group to Search: ";
            cin >> bloodGroup;
            searchDonor(bloodGroup);
        } else if (choice == 3) {
            int donorID;
            cout << "Enter Donor ID to Delete: ";
            cin >> donorID;
            deleteDonor(donorID);
        } else if (choice == 4) {
            viewDonors();
        } else if (choice == 5) {
            string bloodGroup;
            int quantity;
            cout << "Enter Blood Group: ";
            cin >> bloodGroup;
            cout << "Enter Quantity to Add: ";
            cin >> quantity;
            addBlood(bloodGroup, quantity);
        } else if (choice == 6) {
            viewInventory();
        } else if (choice == 7) {
            string bloodGroup, patientName;
            cout << "Enter Blood Group Needed: ";
            cin >> bloodGroup;
            cout << "Enter Patient Name: ";
            cin.ignore();
            getline(cin, patientName);
            addRequest(bloodGroup, patientName);
        } else if (choice == 8) {
            processRequest();
        } else if (choice == 9) {
            cout << "Exiting the system. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Please try again.\n";
        }
    }

    return 0;
}



