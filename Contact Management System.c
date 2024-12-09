#include <stdio.h>  // For standard input/output functions
#include <stdlib.h>  // For general utilities such as memory allocation 
#include <string.h>  // For string manipulation functions
#include <ctype.h>   // For handling character-related functions like checking or converting case
#include "contactManagement.h"  // For Contact structure and related functions

// Define maximum sizes for different fields and the total number of contacts
#define MAX_CONTACTS 100
#define MAX_NAME_LEN 30
#define MAX_ADDRESS_LEN 50
#define MAX_EMAIL_LEN 30

// Function declarations
void addContact(struct Contact contacts[], int *count);
void listContacts(struct Contact contacts[], int count);
void searchContact(struct Contact contacts[], int count);
void editContact(struct Contact contacts[], int count);
void deleteContact(struct Contact contacts[], int *count);
void saveContactsToFile(struct Contact contacts[], int count);
void loadContactsFromFile(struct Contact contacts[], int *count);
void displayMenu();
int compareContacts(const void *a, const void *b);

// substring search that is case insensitive 
char *caseInsensitiveStrStr(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;

    size_t hay_len = strlen(haystack);
    size_t nee_len = strlen(needle);

    // Allocate memory for lowercase versions of haystack and needle
    char *haystack_lower = malloc(hay_len + 1);
    char *needle_lower = malloc(nee_len + 1);

    if (!haystack_lower || !needle_lower) {
        if (haystack_lower) free(haystack_lower);
        if (needle_lower) free(needle_lower);
        return NULL;
    }

    // convert haystack to lowercase
    for (size_t i = 0; i <= hay_len; i++)
        haystack_lower[i] = (char)tolower((unsigned char)haystack[i]);

    // Convert needle to lowercase
    for (size_t i = 0; i <= nee_len; i++)
        needle_lower[i] = (char)tolower((unsigned char)needle[i]);
    
    // use strstr To find the substring in lowercase
    char *pos = strstr(haystack_lower, needle_lower);
    char *result = NULL;
    if (pos) {
        size_t offset = pos - haystack_lower;
        result = (char *)haystack + offset;
    }

    // Free the allocated memory
    free(haystack_lower);
    free(needle_lower);

    return result;
}

int main() {
    struct Contact contacts[MAX_CONTACTS];  // Array to store up to MAX_CONTACTS contacts
    int count = 0;  // Initialize the contact count to 0
    loadContactsFromFile(contacts, &count); // Load existing contacts from a file

    int choice;  // Variable to store user's menu choice
    while (1) {
        displayMenu();  // Display the menu options
        printf("Enter your choice: ");
        // Validate input
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');  // Clear the input buffer to avoid infinite loop
            continue;
        }

        // Handle menu choices with a switch statement
        switch (choice) {
            case 1: addContact(contacts, &count); break;  // Add a new contact
            case 2: listContacts(contacts, count); break;  // List all contacts
            case 3: searchContact(contacts, count); break;  // Search for a contact
            case 4: editContact(contacts, count); break;   // Edit a contact's details
            case 5: deleteContact(contacts, &count); break;   // Delete a contact
            case 0:
                printf("Exiting the program. Goodbye!\n");
                saveContactsToFile(contacts, count);  // Save contacts to a file before exiting
                return 0;  // Exit the program
            default:
                printf("Invalid choice! Please enter a valid option.\n");   // Invalid menu choice
        }
    }
}

// Display the main menu with options for the user
void displayMenu() {
    printf("\n\t\t***** Contact Management System *****\n");
    printf("\t\t=====================================\n");
    printf("\t\t[1] Add a New Contact\n");
    printf("\t\t[2] List All Contacts\n");
    printf("\t\t[3] Search for a Contact\n");
    printf("\t\t[4] Edit a Contact\n");
    printf("\t\t[5] Delete a Contact\n");
    printf("\t\t[0] Exit\n");
    printf("\t\t=====================================\n");
}

// Add a new contact to the contact list
void addContact(struct Contact contacts[], int *count) {
    if (*count >= MAX_CONTACTS) {
        printf("Contact list is full. Cannot add more contacts.\n");
        return;
    }
    while (getchar() != '\n');

    // Get contact details from the user
    struct Contact *contact = &contacts[*count];
    int checkDuplicate = 0; // initialize variable to check if a duplicate name was added

    do {
        printf("Enter contact name: ");
        fgets(contact->name, sizeof(contact->name), stdin);
        contact->name[strcspn(contact->name, "\n")] = 0; // Remove newline from the name
        checkDuplicate = 0;

        if (strlen(contact->name) == 0) {
            printf("Name cannot be empty. Please enter a valid name.\n");
            continue;
        }

        checkDuplicate = 0;
        for (int i = 0; i < *count; i++) {
            if (strcasecmp(contact->name, contacts[i].name) == 0) {
                printf("A contact with this name already exists. Please Enter a New Name:\n");
                checkDuplicate = 1;
                break;
            }
        }
    } while (strlen(contact->name) == 0 || checkDuplicate == 1);

    int validNumber = 0; //set up a validity checker
    while (!validNumber) {
        printf("Enter phone number: ");
        validNumber = 1;
        if (scanf("%ld", &contact->phone) != 1) {
            printf("Invalid input, please enter a valid phone number.\n");
            while (getchar() != '\n');
            validNumber = 0; //again will continue loop
        } else if (contact->phone < 0) {
            printf("Phone number can only have positive numbers!\n");
            validNumber = 0; //again will continue loop
        }

        if (validNumber == 1) {
            for (int i = 0; i < *count; i++) {
                if (contacts[i].phone == contact->phone) {
                    printf("This phone number already exists please try again:\n");
                    validNumber = 0;
                    break; //break out of the loop if duplicate was found
                }
            }
        }
    }
    while (getchar() != '\n');

    printf("Enter address: ");
    fgets(contact->address, sizeof(contact->address), stdin);
    contact->address[strcspn(contact->address, "\n")] = 0;

    int validEmail = 0; // Check validity of email
    while (!validEmail) {
        printf("Enter email: ");
        fgets(contact->email, sizeof(contact->email), stdin); // Takes input
        contact->email[strcspn(contact->email, "\n")] = 0;   // Remove newline character

        if (strlen(contact->email) == 0) {
            printf("Email cannot be empty!\n");
            continue;
        }

        // Simple check to ensure both '@' and '.' are present in the email
        if (strchr(contact->email, '@') != NULL && strchr(contact->email, '.') != NULL) {
            validEmail = 1;
        } else {
            printf("Invalid email! Email must contain both '@' and '.'.\n");
        }
    }

    (*count)++;  // Increment the contact count
    printf("Contact added successfully!\n");
}

// List all contacts in the address book
void listContacts(struct Contact contacts[], int count) {
    if (count == 0) {
        printf("No contacts to display.\n");
        return;
    }

    qsort(contacts, count, sizeof(struct Contact), compareContacts);

    // Display the contact details in a formatted manner
    printf("\n\t\t*** List of Contacts ***\n");
    printf("==================================================\n");
    printf("Name\t\t\tPhone\t\tAddress\t\t\tEmail\n");
    printf("==================================================\n");

    for (int i = 0; i < count; i++) {
        printf("%-30s %-15ld %-30s %s\n", contacts[i].name, contacts[i].phone, contacts[i].address, contacts[i].email);
    }
}

// Comparison function for sorting contacts alphabetically
int compareContacts(const void *a, const void *b) {
    const struct Contact *contactA = (const struct Contact *)a;
    const struct Contact *contactB = (const struct Contact *)b;
    return strcasecmp(contactA->name, contactB->name);
}

// Search for a contact by name, or partial substring search
void searchContact(struct Contact contacts[], int count) {
    if (count == 0) {
        printf("No contacts to search.\n");
        return;
    }

    // *** Added this line to ensure alphabetical order when searching ***
    qsort(contacts, count, sizeof(struct Contact), compareContacts);

    // reads the name and does a substring search
    getchar();
    char input[MAX_NAME_LEN];
    printf("Please enter the contact's name: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    if (strlen(input) == 0) {
        printf("No input provided. Returning.\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < count; i++) {
        // partial substring search
        if (caseInsensitiveStrStr(contacts[i].name, input) != NULL) {
            if (!found) {
                // Print the contact details if found
                printf("\nContacts matching '%s':\n", input);
            }
            found = 1;
            printf("Name: %s\nPhone: %ld\nAddress: %s\nEmail: %s\n\n",
                   contacts[i].name, contacts[i].phone, contacts[i].address, contacts[i].email);
        }
    }

    if (!found) {
        printf("No contact found containing '%s'.\n", input);
    }
}

// Edit an existing contact
void editContact(struct Contact contacts[], int count) {
    char name[MAX_NAME_LEN];
    printf("Enter the name of the contact to edit: ");
    getchar();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    // Search for the contact to edit
    for (int i = 0; i < count; i++) {
        if (strcasecmp(contacts[i].name, name) == 0) {
            printf("Editing contact '%s'\n", contacts[i].name);
            // Get new contact details from the user
            printf("New Name: ");
            fgets(contacts[i].name, sizeof(contacts[i].name), stdin);
            contacts[i].name[strcspn(contacts[i].name, "\n")] = 0;

            printf("New Phone: ");
            scanf("%ld", &contacts[i].phone);
            getchar();

            printf("New Address: ");
            fgets(contacts[i].address, sizeof(contacts[i].address), stdin);
            contacts[i].address[strcspn(contacts[i].address, "\n")] = 0;

            printf("New Email: ");
            fgets(contacts[i].email, sizeof(contacts[i].email), stdin);
            contacts[i].email[strcspn(contacts[i].email, "\n")] = 0;
            printf("Contact updated successfully!\n");
            return;
        }
    }
    printf("No contact found with the name '%s'.\n", name);
}

// Delete a contact from the list
void deleteContact(struct Contact contacts[], int *count) {
    char name[MAX_NAME_LEN];
    printf("Enter the name of the contact to delete: ");
    getchar();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    // Find and delete the contact
    for (int i = 0; i < *count; i++) {
        if (strcasecmp(contacts[i].name, name) == 0) {
            // Shift the remaining contacts to remove the deleted one
            for (int j = i; j < *count - 1; j++) {
                contacts[j] = contacts[j + 1];
            }
            (*count)--;
            printf("Contact deleted successfully.\n");
            return;
        }
    }
    printf("No contact found with the name '%s'.\n", name);
}

// Save all contacts to a file
void saveContactsToFile(struct Contact contacts[], int count) {
    FILE *fp = fopen("contacts.txt", "w");
    if (!fp) {
        printf("Failed to open file for saving contacts.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s\n%ld\n%s\n%s\n", contacts[i].name, contacts[i].phone, contacts[i].address, contacts[i].email);
    }
    fclose(fp);
    printf("Contacts saved to file.\n");
}

// Load contacts from a file into the array
void loadContactsFromFile(struct Contact contacts[], int *count) {
    FILE *fp = fopen("contacts.txt", "r");
    if (!fp) {
        printf("No previous contacts file found. Starting fresh.\n");
        return;
    }

    while (fscanf(fp, "%[^\n]\n%ld\n%[^\n]\n%[^\n]\n", contacts[*count].name, &contacts[*count].phone, contacts[*count].address, contacts[*count].email) != EOF) {
        (*count)++;
    }
    fclose(fp);
}




