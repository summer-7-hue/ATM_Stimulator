#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int loadPIN();
void savePIN(int pin);
float getBalance();
void saveBalance(float balance);
void addHistory(const char *type, float amount);
void showHistory();
void welcome();
void line();

int main() {
    int pin = loadPIN();
    int enteredPin, attempts = 0;
    int choice, newPIN;
    float amount, balance;
    char again;

    system("color 0A");
    welcome();

    while (attempts < 3) {
        printf("Enter PIN: ");
        scanf("%d", &enteredPin);

        if (enteredPin == pin) {
            printf("Login successful.\n");
            Beep(1000, 200);
            break;
        } else {
            printf("Wrong PIN. Try again.\n");
            Beep(600, 300);
            attempts++;
        }
    }

    if (attempts == 3) {
        printf("Too many wrong attempts. Exiting.\n");
        return 0;
    }

    do {
        line();
        printf("ATM Menu\n");
        line();
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. View History\n");
        printf("5. Change PIN\n");
        printf("6. Exit\n");
        printf("Select option (1-6): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                balance = getBalance();
                printf("Current Balance: Rs %.2f\n", balance);
                break;

            case 2:
                printf("Enter amount to deposit: ");
                scanf("%f", &amount);
                if (amount > 0) {
                    balance = getBalance();
                    balance += amount;
                    saveBalance(balance);
                    addHistory("Deposited", amount);
                    printf("Deposited: Rs %.2f | New Balance: Rs %.2f\n", amount, balance);
                    Beep(1200, 150);
                } else {
                    printf("Invalid amount.\n");
                }
                break;

            case 3:
                printf("Enter amount to withdraw: ");
                scanf("%f", &amount);
                balance = getBalance();
                if (amount > 0 && amount <= balance) {
                    balance -= amount;
                    saveBalance(balance);
                    addHistory("Withdrawn", amount);
                    printf("Withdrawn: Rs %.2f | New Balance: Rs %.2f\n", amount, balance);
                    Beep(1000, 200);
                } else {
                    printf("Insufficient balance or invalid amount.\n");
                }
                break;

            case 4:
                showHistory();
                break;

            case 5:
                printf("Enter new PIN: ");
                scanf("%d", &newPIN);
                savePIN(newPIN);
                pin = newPIN;
                printf("PIN updated successfully.\n");
                break;

            case 6:
                printf("Thank you for using the ATM.\n");
                again = 'n';
                continue;

            default:
             printf("Invalid option. Choose between 1 to 6.\n");
        }

        printf("Do you want another operation? (y/n): ");
        scanf(" %c", &again);

    } while (again == 'y' || again == 'Y');
            printf("Thank you for using the ATM.\n");

    return 0;
}

void welcome() {
    line();
    printf("      ATM SIMULATOR\n");
    line();
}

void line() {
    printf("================================\n");
}

int loadPIN() {
    FILE *file = fopen("pin.txt", "r");
    int pin;

    if (file == NULL) {
        pin = 1234;
        savePIN(pin);
    } else {
        fscanf(file, "%d", &pin);
        fclose(file);
    }

    return pin;
}

void savePIN(int pin) {
    FILE *file = fopen("pin.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", pin);
        fclose(file);
    }
}

float getBalance() {
    FILE *file = fopen("balance.txt", "r");
    float bal = 0;

    if (file != NULL) {
        fscanf(file, "%f", &bal);
        fclose(file);
    } else {
        saveBalance(0);
    }

    return bal;
}

void saveBalance(float balance) {
    FILE *file = fopen("balance.txt", "w");
    if (file != NULL) {
        fprintf(file, "%.2f", balance);
        fclose(file);
    }
}

void addHistory(const char *type, float amount) {
    FILE *file = fopen("history.txt", "a");
    time_t t;
    time(&t);

    if (file != NULL) {
        fprintf(file, "%s Rs %.2f on %s", type, amount, ctime(&t));
        fclose(file);
    }
}

void showHistory() {
    FILE *file = fopen("history.txt", "r");
    char line[256];

    if (file == NULL) {
        printf("No transaction history found.\n");
        return;
    }

    printf("\nTransaction History:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    printf("\n");
    fclose(file);
}
