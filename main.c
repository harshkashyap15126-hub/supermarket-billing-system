#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_ITEMS 50

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define WHITE   "\033[1;37m"

typedef struct {
    char name[50];
    float price;
    int quantity;
    float total;
} Product;

void line() {
    printf(CYAN "============================================================\n" RESET);
}

void header() {
    line();
    printf(YELLOW "             SUPERMARKET BILLING SYSTEM\n" RESET);
    printf(GREEN "                  SMART BILLING\n" RESET);
    line();
}

int readInt(const char *message) {
    char input[100];
    int value;

    while (1) {
        printf("%s", message);
        if (fgets(input, sizeof(input), stdin) != NULL &&
            sscanf(input, "%d", &value) == 1 && value >= 0) {
            return value;
        }
        printf(RED "Invalid input! Enter a valid number.\n" RESET);
    }
}

float readFloat(const char *message) {
    char input[100];
    float value;
    char extra;

    while (1) {
        printf("%s", message);
        if (fgets(input, sizeof(input), stdin) != NULL &&
            sscanf(input, "%f %c", &value, &extra) == 1 &&
            value >= 0) {
            return value;
        }
        printf(RED "Invalid amount! Please try again.\n" RESET);
    }
}

void readText(const char *message, char *output, int size) {
    printf("%s", message);
    if (fgets(output, size, stdin) != NULL) {
        output[strcspn(output, "\n")] = '\0';
    }
}

void createBill() {
    Product items[MAX_ITEMS];
    char customer[50];
    char phone[20];
    int count = 0;
    int choice;
    float subtotal = 0, discount = 0;
    float gst, finalAmount;

    time_t now = time(NULL);
    struct tm *date = localtime(&now);

    printf("\n" YELLOW "              CREATE NEW CUSTOMER BILL\n" RESET);
    line();

    readText("Enter Customer Name: ", customer, sizeof(customer));
    readText("Enter Phone Number: ", phone, sizeof(phone));

    printf("\n" GREEN "Enter Product Details\n" RESET);
    line();

    do {
        if (count >= MAX_ITEMS) {
            printf(RED "Maximum product limit reached!\n" RESET);
            break;
        }

        printf("\n" CYAN "--- Product %d ---\n" RESET, count + 1);

        readText("Product Name: ", items[count].name,
                 sizeof(items[count].name));

        items[count].price = readFloat("Product Price (Rs.): ");
        
        do {
            items[count].quantity = readInt("Quantity: ");
            if (items[count].quantity <= 0)
                printf(RED "Quantity must be greater than zero.\n" RESET);
        } while (items[count].quantity <= 0);

        items[count].total =
            items[count].price * items[count].quantity;

        subtotal += items[count].total;
        count++;

        printf(GREEN "\nProduct added successfully!\n" RESET);

        if (count < MAX_ITEMS)
            choice = readInt("\nAdd another product? (1 = Yes, 0 = No): ");
        else
            choice = 0;

    } while (choice == 1);

    if (count == 0) {
        printf(RED "\nNo products entered. Bill cancelled.\n" RESET);
        return;
    }

    /* Demonstration discount: 5% for bills of Rs. 2000 or more */
    if (subtotal >= 2000)
        discount = subtotal * 0.05f;

    /* Demonstration GST rate: 5% */
    gst = (subtotal - discount) * 0.05f;
    finalAmount = subtotal - discount + gst;

    printf("\n\n");
    line();
    printf(YELLOW "                    CUSTOMER INVOICE\n" RESET);
    printf(GREEN "               SMART SUPERMARKET STORE\n" RESET);
    line();

    printf(WHITE "Customer Name : %s\n", customer);
    printf("Phone Number  : %s\n", phone);
    printf("Bill Date     : %02d-%02d-%04d\n",
           date->tm_mday, date->tm_mon + 1,
           date->tm_year + 1900);
    printf("Bill Time     : %02d:%02d:%02d\n",
           date->tm_hour, date->tm_min, date->tm_sec);
    line();

    printf("%-20s %8s %6s %12s\n",
           "PRODUCT", "PRICE", "QTY", "TOTAL");
    line();

    for (int i = 0; i < count; i++) {
        printf("%-20.20s %8.2f %6d %12.2f\n",
               items[i].name,
               items[i].price,
               items[i].quantity,
               items[i].total);
    }

    line();
    printf("%-35s Rs. %10.2f\n", "Subtotal:", subtotal);
    printf("%-35s Rs. %10.2f\n", "Discount (5% if eligible):", discount);
    printf("%-35s Rs. %10.2f\n", "GST (Demo 5%):", gst);

    line();
    printf(GREEN "FINAL AMOUNT:                 Rs. %.2f\n" RESET,
           finalAmount);
    line();

    printf(YELLOW "\n       THANK YOU FOR SHOPPING WITH US!\n" RESET);
    printf(CYAN "          VISIT AGAIN - HAVE A NICE DAY!\n" RESET);
    line();
}

void aboutProject() {
    printf("\n" YELLOW "              ABOUT THE PROJECT\n" RESET);
    line();
    printf(WHITE "Project Name : Supermarket Billing Management System\n");
    printf("Language     : C Programming\n");
    printf("Compiler     : GCC\n");
    printf("Editor       : Visual Studio Code\n");
    printf("Purpose      : Easy and accurate supermarket billing\n");
    line();
}

int main() {
    int option;

    while (1) {
        printf("\n\n");
        header();

        printf(BLUE "\n             MAIN MENU\n" RESET);
        printf("\n");
        printf(GREEN "  1. Create New Bill\n" RESET);
        printf(CYAN  "  2. About Project\n" RESET);
        printf(RED   "  3. Exit\n" RESET);

        line();

        option = readInt("Enter Your Choice: ");

        switch (option) {
            case 1:
                createBill();
                break;

            case 2:
                aboutProject();
                break;

            case 3:
                printf(GREEN "\nThank you for using Smart Billing System!\n" RESET);
                return 0;

            default:
                printf(RED "\nInvalid choice! Please select 1, 2 or 3.\n" RESET);
        }

        printf("\nPress ENTER to return to Main Menu...");
        getchar();
    }

    return 0;
}
