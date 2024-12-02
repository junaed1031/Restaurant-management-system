
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME 50
#define MAX_ITEMS 100
#define MAX_USERNAME 30
#define STUDENT_DISCOUNT_RATE 10

#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define CYAN "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define BOLD "\033[1m"

// Structure for menu items
typedef struct {
    char name[MAX_NAME];
    float price;
    int stock;
} Item;

typedef struct {
    char userName[MAX_NAME];
    char password[MAX_NAME];
} Account;

void adminMenu();
void customerMenu();
void addItems();
void viewItems();
void updateItems();
void viewOrders();
void viewReviews();
void viewReservations();
void customerViewItems();
void makeOrder(char *username);
int processPayment(float totalBill, char *username, int isStudentOrder);
void takeBill(char *username);
void giveReview(char *username);
void bookReservation(char *username);
void takeBillWithDiscount(char *username);
int isStudent();
void toLowerCase(char *str);

// Convert string to lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}


// Utility functions
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void registrationAccount() {
    FILE *file = fopen("account.txt", "a");
    if (!file) {
        printf(RED "Error opening file!\n" RESET);
        return;
    }

    Account user;

    printf("Enter name: ");
    fgets(user.userName, MAX_NAME, stdin);
    user.userName[strcspn(user.userName, "\n")] = 0;  // Remove newline character

    printf("Enter Password: ");
    fgets(user.password, MAX_NAME, stdin);
    user.password[strcspn(user.password, "\n")] = 0;  // Remove newline character

    fprintf(file, "%s %s\n", user.userName, user.password);

    fclose(file);

    printf("Account created successfully!\n");

    return;
}

void login() {
    FILE *file = fopen("account.txt", "r");
    if (!file) {
        printf(RED "Error opening file!\n" RESET);
        return;
    }

    char name[MAX_NAME];
    printf("Enter name: ");
    fgets(name, MAX_NAME, stdin);
    name[strcspn(name, "\n")] = 0;  // Remove newline character

    char password[MAX_NAME];
    printf("Enter Password: ");
    fgets(password, MAX_NAME, stdin);
    password[strcspn(password, "\n")] = 0;  // Remove newline character

    rewind(file);
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        Account account;
        sscanf(line, "%s %s", account.userName, account.password);

        if ((strcmp(account.userName, name) == 0) && (strcmp(account.password, password) == 0)) {
            printf("Successful\n");
            fclose(file);

            adminMenu();
            return;
        }
    }

    printf("Invalid username or password.\n");
    fclose(file);
}

void mainLogin() {
    int choice;
    printf("1. Register\n2. Login\nEnter your choice: ");
    scanf("%d", &choice);
    getchar();  // To consume the newline character left by scanf

    if (choice == 1) {
        registrationAccount();
    } else if (choice == 2) {
        login();
    } else {
        printf("Invalid choice\n");
    }

    return 0;
}



// Admin menu
void adminMenu() {
    int choice;
    while (1) {
        printf(BLUE"\n===== Admin Menu =====\n"RESET);
        printf(CYAN"1. Add Items\n"RESET);
        printf(CYAN"2. View Items\n"RESET);
        printf(CYAN"3. Update Items\n"RESET);
        printf(CYAN"4. View Orders\n"RESET);
        printf(CYAN"5. View Reviews\n"RESET);
        printf(CYAN"6. View Reservations\n"RESET);
        printf(CYAN"7. Search Menu Item\n"RESET);
        printf(CYAN"8. Back\n"RESET);
        printf(MAGENTA"Enter your choice: "RESET);
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
        case 1:
            addItems();
            break;
        case 2:
            viewItems();
            break;
        case 3:
            updateItems();
            break;
        case 4:
            viewOrders();
            break;
        case 5:
            viewReviews();
            break;
        case 6:
            viewReservations();
            break;
        case 7:
            searchItems();
        case 8:
            return;
        default:
            printf(RED"Invalid choice. Try again.\n"RESET);
        }
    }
}

// Customer menu
void customerMenu() {
    int choice;
    char username[MAX_USERNAME];

    printf(MAGENTA"Enter your username: "RESET);
    fgets(username, MAX_USERNAME, stdin);
    username[strcspn(username, "\n")] = 0; // Remove newline

    while (1) {
        printf(BLUE"\n===== Customer Menu =====\n"RESET);
        printf(YELLOW"1. View Items\n"RESET);
        printf(YELLOW"2. Search Menu Item\n"RESET);
        printf(YELLOW"3. Make Order\n"RESET);
        printf(YELLOW"4. Give Review\n"RESET);
        printf(YELLOW"5. Book Reservation\n"RESET);
        printf(YELLOW"6. Back\n"RESET);
        printf(MAGENTA"Enter your choice: "RESET);
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
        case 1:
            customerViewItems();
            break;
        case 2:
            searchItems();
            break;
        case 3:
            makeOrder(username);
            break;
        case 4:
            giveReview(username);
            break;
        case 5:
            bookReservation(username);
            break;
        case 6:
            return;
        default:
            printf(RED"Invalid choice. Try again.\n"RESET);
        }
    }
}

// Admin functionalities
void addItems() {
    FILE *file = fopen("menu.txt", "a");
    if (!file) {
        printf(RED"Error opening file!\n"RESET);
        return;
    }

    int numItems;
    printf(MAGENTA"Enter the number of items to add: "RESET);
    scanf("%d", &numItems);
    clearBuffer();

    for (int i = 0; i < numItems; i++) {
        Item item;
        printf(MAGENTA"Enter item name: "RESET);
        fgets(item.name, MAX_NAME, stdin);
        item.name[strcspn(item.name, "\n")] = 0;
        printf(MAGENTA"Enter item price: "RESET);
        scanf("%f", &item.price);
        printf(MAGENTA"Enter item stock: "RESET);
        scanf("%d", &item.stock);
        clearBuffer();

        fprintf(file, "%s,%.2f,%d\n", item.name, item.price, item.stock);
    }
    printf(GREEN"Item added successfully!\n"RESET);
    fclose(file);
}

void viewItems() {
    FILE *file = fopen("menu.txt", "r");
    if (!file) {
        printf(RED"No items found.\n"RESET);
        return;
    }

    printf(BLUE"\n===== Items =====\n"RESET);
    char line[100];
    int count = 1;
    while (fgets(line, sizeof(line), file)) {
        Item item;
        sscanf(line, "%[^,],%f,%d", item.name, &item.price, &item.stock);
        printf("%d. "RED"%s"RESET" "GREEN"- Price: %.2f"RESET","YELLOW" Stock: %d\n"RESET, count++, item.name, item.price, item.stock);
    }
    fclose(file);
}

void updateItems() {
    FILE *file = fopen("menu.txt", "r+");
    if (!file) {
        printf(RED"No items available to update.\n"RESET);
        return;
    }

    Item items[MAX_ITEMS];
    int count = 0;

    while (fscanf(file, "%[^,],%f,%d\n", items[count].name, &items[count].price, &items[count].stock) != EOF) {
        count++;
    }

    viewItems();
    int choice;
    printf(MAGENTA"Enter the item number to update: "RESET);
    scanf("%d", &choice);
    clearBuffer();

    if (choice < 1 || choice > count) {
        printf(RED"Invalid item number.\n"RESET);
        fclose(file);
        return;
    }

    choice--;
    printf(MAGENTA"Enter new price: "RESET);
    scanf("%f", &items[choice].price);
    printf(MAGENTA"Enter new stock: "RESET);
    scanf("%d", &items[choice].stock);
    clearBuffer();

    freopen("menu.txt", "w", file);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%.2f,%d\n", items[i].name, items[i].price, items[i].stock);
    }
    fclose(file);
    printf(GREEN"Item updated successfully!\n"RESET);
}

void viewOrders() {
    FILE *file = fopen("orders.txt", "r");
    if (!file) {
        printf(RED"No orders found.\n"RESET);
        return;
    }

    printf(BLUE"\n===== Orders =====\n"RESET);
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

void viewReviews() {
    FILE *file = fopen("reviews.txt", "r");
    if (!file) {
        printf(RED"No reviews found.\n"RESET);
        return;
    }

    printf(BLUE"\n===== Reviews =====\n"RESET);
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

void viewReservations() {
    FILE *file = fopen("reservations.txt", "r");
    if (!file) {
        printf(RED"No reservations found.\n"RESET);
        return;
    }

    printf(BLUE"\n===== Reservations =====\n"RESET);
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

// Function to search for an item in the menu
// Function to search for an item in the menu
void searchItems() {
    FILE *file = fopen("menu.txt", "r");
    if (!file) {
        printf(RED "No items found.\n" RESET);
        return;
    }

    while (1) {
        char searchName[MAX_NAME];
        printf(MAGENTA "Enter the name of the item to search (or type 'back' to return): " RESET);
        fgets(searchName, MAX_NAME, stdin);

        // Remove newline character if present
        searchName[strcspn(searchName, "\n")] = 0;

        // Exit the search if the user types "back"
        if (strcasecmp(searchName, "back") == 0) {
            printf(YELLOW "Returning to the previous menu.\n" RESET);
            break; // Exit the loop and return to the calling function
        }

        printf(BLUE "\n===== Search Results =====\n" RESET);

        // Reset the file pointer to the beginning of the file
        rewind(file);

        char line[100];
        int found = 0;
        while (fgets(line, sizeof(line), file)) {
            Item item;
            sscanf(line, "%[^,],%f,%d", item.name, &item.price, &item.stock);

            // Convert strings to lowercase for case-insensitive comparison
            char itemNameLower[MAX_NAME], searchNameLower[MAX_NAME];
            strcpy(itemNameLower, item.name);
            strcpy(searchNameLower, searchName);
            for (int i = 0; itemNameLower[i]; i++) itemNameLower[i] = tolower(itemNameLower[i]);
            for (int i = 0; searchNameLower[i]; i++) searchNameLower[i] = tolower(searchNameLower[i]);

            // Case-insensitive substring search
            if (strstr(itemNameLower, searchNameLower) != NULL) {
                printf(GREEN "Item found: %s - Price: %.2f, Stock: %d\n" RESET, item.name, item.price, item.stock);
                found = 1;
            }
        }

        if (!found) {
            printf(RED "Item not found.\n" RESET);
        }
    }

    fclose(file); // Close the file when exiting the function
}


void customerViewItems() {
    viewItems();
}

// Place an order
void makeOrder(char *username) {
    FILE *menuFile = fopen("menu.txt", "r");
    FILE *tempFile = fopen("temp_menu.txt", "w");
    FILE *orderFile = fopen("orders.txt", "a");

    if (!menuFile || !tempFile || !orderFile) {
        printf(RED "Error accessing necessary files.\n" RESET);
        if (menuFile) fclose(menuFile);
        if (tempFile) fclose(tempFile);
        if (orderFile) fclose(orderFile);
        return;
    }

    char itemName[MAX_NAME];
    int quantity;
    float totalBill = 0;

    int isStudentOrder = isStudent();

    while (1) {
        printf(MAGENTA "Enter item name to order (or 'done' to finish): " RESET);
        fgets(itemName, MAX_NAME, stdin);
        itemName[strcspn(itemName, "\n")] = 0;
        toLowerCase(itemName);

        if (strcmp(itemName, "done") == 0) break;

        printf(MAGENTA "Enter quantity: " RESET);
        scanf("%d", &quantity);
        clearBuffer();

        if (quantity <= 0) {
            printf(RED "Invalid quantity. Please enter a positive number.\n" RESET);
            continue;
        }

        char line[256];
        int found = 0;
        rewind(menuFile);

        while (fgets(line, sizeof(line), menuFile)) {
            Item item;
            sscanf(line, "%[^,],%f,%d", item.name, &item.price, &item.stock);
            toLowerCase(item.name);

            if (strcmp(item.name, itemName) == 0) {
                found = 1;
                if (quantity > item.stock) {
                    printf(RED "Insufficient stock for %s.\n" RESET, item.name);
                } else {
                    float orderPrice = quantity * item.price;
                    totalBill += orderPrice;
                    item.stock -= quantity;
                    fprintf(orderFile, "Customer: %s, Item: %s, Quantity: %d, Total: %.2f\n",
                            username, item.name, quantity, orderPrice);
                    printf(GREEN "Order added: %s x%d. Subtotal: %.2f\n" RESET, item.name, quantity, orderPrice);
                }
            }

            // Write updated stock information to the temporary file
            fprintf(tempFile, "%s,%.2f,%d\n", item.name, item.price, item.stock);
        }

        if (!found) {
            printf(RED "Item not found in menu.\n" RESET);
        }
    }

    fclose(menuFile);
    fclose(tempFile);
    fclose(orderFile);

    int paymentSuccess = processPayment(totalBill, username, isStudentOrder);

    // Ensure the menu file is only updated if the payment is successful
    if (paymentSuccess) {
        remove("menu.txt");
        rename("temp_menu.txt", "menu.txt");
    } else {
        remove("temp_menu.txt");
    }
}


// Process payment
int processPayment(float totalBill, char *username, int isStudentOrder) {
    if (isStudentOrder) {
        float discount = totalBill * STUDENT_DISCOUNT_RATE / 100;
        totalBill -= discount;
        printf(GREEN"Student discount applied: -%.2f\n"RESET, discount);
    }

    printf(BLUE"Final Bill: %.2f\n"RESET, totalBill);
    printf(MAGENTA"Do you want to proceed with payment? (Y/N): "RESET);
    char confirm;
    scanf(" %c", &confirm);
    clearBuffer();

    if (confirm == 'Y' || confirm == 'y') {
        printf(GREEN"Payment successful for %.2f! Thank you, %s.\n"RESET, totalBill, username);
        return 1;
    } else {
        printf(RED"Payment canceled. Order not completed.\n"RESET);
        return 0;
    }
}



// Check if the user is a student
int isStudent() {
    char studentID[20];
    char choice;
    printf(MAGENTA"Are you a student? (Y/N): "RESET);
    scanf(" %c", &choice);
    if (choice == 'Y' || choice == 'y') {
        printf(MAGENTA"Enter your Student ID: "RESET);
        scanf("%s", studentID);
        printf(GREEN"Student ID verified successfully!\n"RESET);
        clearBuffer();
        return (choice == 'Y' || choice == 'y'); // Student confirmed
    }
}

void giveReview(char *username) {
    FILE *reviewFile = fopen("reviews.txt", "a");
    if (!reviewFile) {
        printf(RED"Error: Unable to access reviews file.\n"RESET);
        return;
    }

    char review[200];
    printf(MAGENTA"Enter your review: "RESET);
    fgets(review, sizeof(review), stdin);
    review[strcspn(review, "\n")] = 0; // Remove newline

    fprintf(reviewFile, "Customer: %s, Review: %s\n", username, review);
    printf(GREEN"Thank you for your review!\n"RESET);

    fclose(reviewFile);
}

void bookReservation(char *username) {
    FILE *reservationFile = fopen("reservations.txt", "a");
    if (!reservationFile) {
        printf(RED"Error: Unable to access reservations file.\n"RESET);
        return;
    }

    char date[20], time[10], table[10];
    printf("Enter reservation date (YYYY-MM-DD): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = 0; // Remove newline

    printf("Enter reservation time (HH:MM): ");
    fgets(time, sizeof(time), stdin);
    time[strcspn(time, "\n")] = 0; // Remove newline

    printf("Enter table number: ");
    fgets(table, sizeof(table), stdin);
    table[strcspn(table, "\n")] = 0; // Remove newline

    fprintf(reservationFile, "Customer: %s, Date: %s, Time: %s, Table: %s\n", username, date, time, table);
    printf("Reservation booked successfully!\n");

    fclose(reservationFile);
}

void displayLogo() {
    printf(GREEN"\t\t\t*************************************************\n");
    printf("\t\t\t*                                               *\n");
    printf("\t\t\t*        "BLUE"+----------------------------+         "GREEN"*\n");
    printf(GREEN"\t\t\t*        "BLUE"|  "RESET MAGENTA"WELCOME TO FOOD PARADISE"RESET BLUE"  |"RESET "         "GREEN"*\n");
    printf("\t\t\t*        "BLUE"+----------------------------+         "GREEN"*\n");
    printf("\t\t\t*         [====== RESTAURANT =======]           *\n");
    printf("\t\t\t*        "YELLOW"TASTE THE BEST, FEEL THE REST!        "GREEN" *\n");
    printf("\t\t\t*                                               *\n");
    printf("\t\t\t*************************************************\n\n");
}


void clearScreen() {
    #ifdef _WIN32
        system("cls");  // For Windows
    #else
        system("clear");  // For Unix-based systems (Linux/macOS)
    #endif // _WIN32
}


// Main function
int main() {
    int choice;
    displayLogo();

    while (1) {
        printf(BLUE"\n===== Restaurant Management System =====\n"RESET);
        printf(CYAN"1. Admin\n"RESET);
        printf(YELLOW"2. Customer\n"RESET);
        printf(RED"3. Exit\n"RESET);
        printf(MAGENTA"Enter your choice: "RESET);
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
        case 1:
            mainLogin();
            break;
        case 2:
            customerMenu();
            break;
        case 3:
            printf("Exiting... Thank you!\n");
            return 0;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
}


