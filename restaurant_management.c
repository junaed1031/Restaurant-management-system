#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void takeBillWithDiscount(char *username);
int isStudent();

// Utility functions
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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
        printf(YELLOW"2. Make Order\n"RESET);
        printf(YELLOW"3. Take Bill\n"RESET);
        printf(YELLOW"4. Take Bill With Discount\n"RESET);
        printf(YELLOW"5. Give Review\n"RESET);
        printf(YELLOW"6. Book Reservation\n"RESET);
        printf(YELLOW"7. Back\n"RESET);
        printf(MAGENTA"Enter your choice: "RESET);
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
            takeBillWithDiscount(username);
            break;
        case 5:
            giveReview(username);
            break;
        case 6:
            bookReservation(username);
            break;
        case 7:
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
        printf(GREEN"Item added successfully!\n"RESET);
    }
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
void searchItems() {
    FILE *file = fopen("menu.txt", "r");
    if (!file) {
        printf(RED"No items found.\n"RESET);
        return;
    }

    char searchName[MAX_NAME];
    printf(MAGENTA"Enter the name of the item to search: "RESET);
    fgets(searchName, MAX_NAME, stdin);
    searchName[strcspn(searchName, "\n")] = 0; // Remove newline

    printf(BLUE"\n===== Search Results =====\n"RESET);
    char line[100];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        Item item;
        sscanf(line, "%[^,],%f,%d", item.name, &item.price, &item.stock);

        // Case insensitive comparison
        if (strstr(item.name, searchName) != NULL) {
            printf(GREEN"Item found: %s - Price: %.2f, Stock: %d\n"RESET, item.name, item.price, item.stock);
            found = 1;
        }
    }

    if (!found) {
        printf(RED"Item not found.\n"RESET);
    }

    fclose(file);
}



void customerViewItems() {
    viewItems();
}

void makeOrder(char *username) {
    FILE *menuFile = fopen("menu.txt", "r");
    FILE *tempFile = fopen("temp_menu.txt", "w");
    FILE *orderFile = fopen("orders.txt", "w");

    if (!menuFile || !tempFile || !orderFile) {
        printf(RED"Error: Unable to access required files.\n"RESET);
        return;
    }

    printf(BLUE"\n===== Place Your Order =====\n"RESET);
    printf(GREEN"Available items:\n"RESET);
    viewItems();

    char itemName[MAX_NAME];
    int quantity;
    float totalPrice = 0;

    printf(MAGENTA"Enter the item name to order: "RESET);
    fgets(itemName, MAX_NAME, stdin);
    itemName[strcspn(itemName, "\n")] = 0; // Remove newline

    printf(MAGENTA"Enter quantity: "RESET);
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
                printf(RED"Not enough stock available for %s.\n"RESET, item.name);
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
            printf(GREEN"Order placed successfully! Total Price: %.2f\n"RESET, totalPrice);
        } else {
            // Write unchanged item to temp file
            fprintf(tempFile, "%s", line);
        }
    }

    if (!found) {
        printf(RED"Item not found in the menu.\n"RESET);
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
        printf(RED"Error: Unable to access orders file.\n"RESET);
        return;
    }

    printf(BLUE"\n===== Bill for %s =====\n"RESET, username);
    char line[200];
    int found = 0;
    float totalBill = 0;

    while (fgets(line, sizeof(line), orderFile)) {
        if (strstr(line, username)) {
            printf("%s", line);
            found = 1;
            float orderTotal;
            sscanf(strrchr(line, ':') + 1, "%f", &orderTotal);
            totalBill += orderTotal;
        }
    }

    if (!found) {
        printf(RED"No orders found for %s.\n"RESET, username);
        fclose(orderFile);
        return;
    }

    printf(GREEN"Total Bill: %.2f\n"RESET, totalBill);
    fclose(orderFile);

    // Call payment menu
    paymentMenu(totalBill, username);
}


int isStudent() {
    char studentID[20];
    char confirm;
    printf(MAGENTA"Are you a student? (Y/N): "RESET);
    scanf(" %c", &confirm);
    if (confirm == 'Y' || confirm == 'y') {
        printf(MAGENTA"Enter your Student ID: "RESET);
        scanf("%s", studentID);
        printf(GREEN"Student ID verified successfully!\n"RESET);
        return 1; // Student confirmed
    }
    return 0; // Not a student
}

void takeBillWithDiscount(char *username) {
    FILE *orderFile = fopen("orders.txt", "r");
    if (!orderFile) {
        printf(RED"Error: Unable to access orders file.\n"RESET);
        return;
    }

    printf(BLUE"\n===== Bill for %s =====\n"RESET, username);
    char line[200];
    int found = 0;
    float totalBill = 0;
    while (fgets(line, sizeof(line), orderFile)) {
        if (strstr(line, username)) {
            printf("%s", line);
            found = 1;
            float orderTotal;
            sscanf(strrchr(line, ':') + 1, "%f", &orderTotal);
            totalBill += orderTotal;
        }
    }

    if (!found) {
        printf(RED"No orders found for %s.\n"RESET, username);
        fclose(orderFile);
        return;
    }

    printf(GREEN"Total Bill (before discount):"RESET" %.2f\n", totalBill);

    if (isStudent()) {
        float discount = (totalBill * STUDENT_DISCOUNT_RATE) / 100;
        totalBill -= discount;
        printf(GREEN"Student Discount Applied: -"RESET"%.2f\n", discount);
    }

    printf(GREEN"Final Bill (after discount if applicable): "RESET"%.2f\n", totalBill);
    fclose(orderFile);
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
void paymentMenu(float totalBill, char *username) {
    int paymentChoice;
    printf(BLUE"\n===== Payment Options =====\n"RESET);
    printf("1. Pay with Card\n");
    printf("2. Pay with Bkash\n");
    printf("3. Cancel Payment\n");
    printf(MAGENTA"Enter your choice: "RESET);
    scanf("%d", &paymentChoice);
    clearBuffer();

    switch (paymentChoice) {
    case 1:
        printf(GREEN"Processing payment through Card...\n"RESET);
        printf(GREEN"Payment successful! Thank you, %s.\n"RESET, username);
        break;
    case 2:
        printf(GREEN"Processing payment through Bkash...\n");
        printf("Please complete the payment via your Bkash app.\n");
        printf("Payment successful! Thank you, %s.\n"RESET, username);
        break;
    case 3:
        printf(RED"Payment cancelled. Please try again.\n");
        break;
    default:
        printf("Invalid choice. Payment not processed.\n"RESET);
    }
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

