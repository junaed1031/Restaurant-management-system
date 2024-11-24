#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define MAX_ITEMS 100
#define MAX_USERNAME 30

typedef struct {
    char name[MAX_NAME];
    float price;
    int stock;
} Item;

void adminMenu();
void customerMenu();
void addItems();
void viewItems();
void updateItems();
void viewOrders();
void viewReviews();
void viewReservations();
void customerViewItems();
void makeOrder();
void takeBill(char *username);
void giveReview(char *username);
void bookReservation(char *username);

// Utility functions
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Admin menu
void adminMenu() {
    int choice;
    while (1) {
        printf("\n===== Admin Menu =====\n");
        printf("1. Add Items\n");
        printf("2. View Items\n");
        printf("3. Update Items\n");
        printf("4. View Orders\n");
        printf("5. View Reviews\n");
        printf("6. View Reservations\n");
        printf("7. Search Menu Item\n");
        printf("8. Back\n");
        printf("Enter your choice: ");
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
            printf("Invalid choice. Try again.\n");
        }
    }
}

// Customer menu
void customerMenu() {
    int choice;
    char username[MAX_USERNAME];

    printf("Enter your username: ");
    fgets(username, MAX_USERNAME, stdin);
    username[strcspn(username, "\n")] = 0; // Remove newline

    while (1) {
        printf("\n===== Customer Menu =====\n");
        printf("1. View Items\n");
        printf("2. Make Order\n");
        printf("3. Take Bill\n");
        printf("4. Give Review\n");
        printf("5. Book Reservation\n");
        printf("6. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
        case 1:
            customerViewItems();
            break;
        case 2:
            makeOrder(username);
            break;
        case 3:
            takeBill(username);
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
            printf("Invalid choice. Try again.\n");
        }
    }
}

// Admin functionalities
void addItems() {
    FILE *file = fopen("menu.txt", "a");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    int numItems;
    printf("Enter the number of items to add: ");
    scanf("%d", &numItems);
    clearBuffer();

    for (int i = 0; i < numItems; i++) {
        Item item;
        printf("Enter item name: ");
        fgets(item.name, MAX_NAME, stdin);
        item.name[strcspn(item.name, "\n")] = 0;
        printf("Enter item price: ");
        scanf("%f", &item.price);
        printf("Enter item stock: ");
        scanf("%d", &item.stock);
        clearBuffer();

        fprintf(file, "%s,%.2f,%d\n", item.name, item.price, item.stock);
        printf("Item added successfully!\n");
    }
    fclose(file);
}

void viewItems() {
    FILE *file = fopen("menu.txt", "r");
    if (!file) {
        printf("No items found.\n");
        return;
    }

    printf("\n===== Items =====\n");
    char line[100];
    int count = 1;
    while (fgets(line, sizeof(line), file)) {
        Item item;
        sscanf(line, "%[^,],%f,%d", item.name, &item.price, &item.stock);
        printf("%d. %s - Price: %.2f, Stock: %d\n", count++, item.name, item.price, item.stock);
    }
    fclose(file);
}

void updateItems() {
    FILE *file = fopen("menu.txt", "r+");
    if (!file) {
        printf("No items available to update.\n");
        return;
    }

    Item items[MAX_ITEMS];
    int count = 0;

    while (fscanf(file, "%[^,],%f,%d\n", items[count].name, &items[count].price, &items[count].stock) != EOF) {
        count++;
    }

    viewItems();
    int choice;
    printf("Enter the item number to update: ");
    scanf("%d", &choice);
    clearBuffer();

    if (choice < 1 || choice > count) {
        printf("Invalid item number.\n");
        fclose(file);
        return;
    }

    choice--;
    printf("Enter new price: ");
    scanf("%f", &items[choice].price);
    printf("Enter new stock: ");
    scanf("%d", &items[choice].stock);
    clearBuffer();

    freopen("menu.txt", "w", file);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%.2f,%d\n", items[i].name, items[i].price, items[i].stock);
    }
    fclose(file);
    printf("Item updated successfully!\n");
}

void viewOrders() {
    FILE *file = fopen("orders.txt", "r");
    if (!file) {
        printf("No orders found.\n");
        return;
    }

    printf("\n===== Orders =====\n");
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

void viewReviews() {
    FILE *file = fopen("reviews.txt", "r");
    if (!file) {
        printf("No reviews found.\n");
        return;
    }

    printf("\n===== Reviews =====\n");
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

void viewReservations() {
    FILE *file = fopen("reservations.txt", "r");
    if (!file) {
        printf("No reservations found.\n");
        return;
    }

    printf("\n===== Reservations =====\n");
    char line[200];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

// Function to search for an item in the menu
void searchItems() {
    FILE *file = fopen("menu.txt", "r");
    if (!file) {
        printf("No items found.\n");
        return;
    }

    char searchName[MAX_NAME];
    printf("Enter the name of the item to search: ");
    fgets(searchName, MAX_NAME, stdin);
    searchName[strcspn(searchName, "\n")] = 0; // Remove newline

    printf("\n===== Search Results =====\n");
    char line[100];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        Item item;
        sscanf(line, "%[^,],%f,%d", item.name, &item.price, &item.stock);

        // Case insensitive comparison
        if (strstr(item.name, searchName) != NULL) {
            printf("Item found: %s - Price: %.2f, Stock: %d\n", item.name, item.price, item.stock);
            found = 1;
        }
    }

    if (!found) {
        printf("Item not found.\n");
    }

    fclose(file);
}



void customerViewItems() {
    viewItems();
}

void makeOrder(char *username) {
    FILE *menuFile = fopen("menu.txt", "r");
    FILE *tempFile = fopen("temp_menu.txt", "w");
    FILE *orderFile = fopen("orders.txt", "a");

    if (!menuFile || !tempFile || !orderFile) {
        printf("Error: Unable to access required files.\n");
        return;
    }

    printf("\n===== Place Your Order =====\n");
    printf("Available items:\n");
    viewItems();

    char itemName[MAX_NAME];
    int quantity;
    float totalPrice = 0;

    printf("Enter the item name to order: ");
    fgets(itemName, MAX_NAME, stdin);
    itemName[strcspn(itemName, "\n")] = 0; // Remove newline

    printf("Enter quantity: ");
    scanf("%d", &quantity);
    clearBuffer();

    char line[100];
    int found = 0;

    while (fgets(line, sizeof(line), menuFile)) {
        Item item;
        sscanf(line, "%[^,],%f,%d", item.name, &item.price, &item.stock);

        if (strcmp(item.name, itemName) == 0) {
            found = 1;
            if (quantity > item.stock) {
                printf("Not enough stock available for %s.\n", item.name);
                fclose(menuFile);
                fclose(tempFile);
                fclose(orderFile);
                remove("temp_menu.txt"); // Remove temp file
                return;
            }

            // Update stock and calculate total price
            item.stock -= quantity;
            totalPrice = quantity * item.price;

            // Write updated item to temp file
            fprintf(tempFile, "%s,%.2f,%d\n", item.name, item.price, item.stock);

            // Record order in orders file
            fprintf(orderFile, "Customer: %s, Item: %s, Quantity: %d, Total: %.2f\n", username, item.name, quantity, totalPrice);
            printf("Order placed successfully! Total Price: %.2f\n", totalPrice);
        } else {
            // Write unchanged item to temp file
            fprintf(tempFile, "%s", line);
        }
    }

    if (!found) {
        printf("Item not found in the menu.\n");
    }

    fclose(menuFile);
    fclose(tempFile);
    fclose(orderFile);

    // Replace original menu file with updated temp file
    remove("menu.txt");
    rename("temp_menu.txt", "menu.txt");
}


void takeBill(char *username) {
    FILE *orderFile = fopen("orders.txt", "r");
    if (!orderFile) {
        printf("Error: Unable to access orders file.\n");
        return;
    }

    printf("\n===== Bill for %s =====\n", username);
    char line[200];
    int found = 0;
    while (fgets(line, sizeof(line), orderFile)) {
        if (strstr(line, username)) {
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf("No orders found for %s.\n", username);
    }
    fclose(orderFile);
}

void giveReview(char *username) {
    FILE *reviewFile = fopen("reviews.txt", "a");
    if (!reviewFile) {
        printf("Error: Unable to access reviews file.\n");
        return;
    }

    char review[200];
    printf("Enter your review: ");
    fgets(review, sizeof(review), stdin);
    review[strcspn(review, "\n")] = 0; // Remove newline

    fprintf(reviewFile, "Customer: %s, Review: %s\n", username, review);
    printf("Thank you for your review!\n");

    fclose(reviewFile);
}

void bookReservation(char *username) {
    FILE *reservationFile = fopen("reservations.txt", "a");
    if (!reservationFile) {
        printf("Error: Unable to access reservations file.\n");
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


// Main function
int main() {
    int choice;
    //system("color 0A");

    printf("\t\t\t\t\t ______________________________________\n");
    printf("\t\t\t\t\t|                                      |\n");
    printf("\t\t\t\t\t|    ***Welcome To Our Website***      |\n");
    printf("\t\t\t\t\t|______________________________________|\n");

    while (1) {
        printf("\n===== Restaurant Management System =====\n");
        printf("1. Admin\n");
        printf("2. Customer\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearBuffer();

        switch (choice) {
        case 1:
            adminMenu();
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

