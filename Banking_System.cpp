#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <vector>

using namespace std;

class BankAccount {
private:
    int accountNumber;
    char name[50];
    char address[60];
    double balance;
    char accountType; // 'S' for Savings, 'C' for Current

public:
    BankAccount() {
        accountNumber = 0;
        balance = 0.0;
        accountType = 'S';
        name[0] = '\0';
        address[0] = '\0';
    }

    // Getters
    int getAccountNumber() const { return accountNumber; }
    string getName() const { return string(name); }
    string getAddress() const { return string(address); }
    double getBalance() const { return balance; }
    char getAccountType() const { return accountType; }

    // Setters & Modifiers
    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            return false;
        }
        balance -= amount;
        return true;
    }

    // Create a new account
    void createAccount() {
        system("cls"); // Use "clear" if running on Linux/macOS
        cout << "\n\t=== OPEN A NEW BANK ACCOUNT ===\n";
        cout << "Enter Account Number: ";
        cin >> accountNumber;
        cin.ignore();
        
        cout << "Enter Full Name: ";
        cin.getline(name, 50);
        
        cout << "Enter Address: ";
        cin.getline(address, 60);
        
        cout << "Enter Account Type (S for Savings / C for Current): ";
        cin >> accountType;
        accountType = toupper(accountType);
        
        cout << "Enter Initial Deposit Amount (Min. 1000): ";
        cin >> balance;
        
        if (balance < 1000) {
            cout << "\nInitial deposit must be at least 1000. Defaulting balance to 1000.\n";
            balance = 1000.0;
        }
        cout << "\nAccount Created Successfully!\n";
    }

    // Display account details
    void showAccount() const {
        cout << "\nAccount No.    : " << accountNumber;
        cout << "\nAccount Holder : " << name;
        cout << "\nAddress        : " << address;
        cout << "\nAccount Type   : " << (accountType == 'S' ? "Savings" : "Current");
        cout << "\nBalance        : Rs. " << fixed << setprecision(2) << balance << "\n";
    }

    // Modify account details
    void modifyAccount() {
        cin.ignore();
        cout << "\nModify Account Holder Name: ";
        cin.getline(name, 50);
        cout << "Modify Address: ";
        cin.getline(address, 60);
        cout << "Modify Account Type (S/C): ";
        cin >> accountType;
        accountType = toupper(accountType);
    }
};

// Function prototypes for file operations
void writeAccount();
void displaySpecific(int);
void modifyAccount(int);
void deleteAccount(int);
void displayAll();
void depositWithdraw(int, int);

int main() {
    char choice;
    int num;

    do {
        system("cls"); // Use "clear" for Linux/macOS
        cout << "\n\n\t===============================";
        cout << "\n\t    MULTIFUNCTIONAL BANK SYSTEM";
        cout << "\n\t===============================";
        cout << "\n\t1. CREATE NEW ACCOUNT";
        cout << "\n\t2. DEPOSIT AMOUNT";
        cout << "\n\t3. WITHDRAW AMOUNT";
        cout << "\n\t4. BALANCE ENQUIRY";
        cout << "\n\t5. ALL ACCOUNT HOLDER LIST";
        cout << "\n\t6. CLOSE AN ACCOUNT";
        cout << "\n\t7. UPDATE AN ACCOUNT";
        cout << "\n\t8. EXIT";
        cout << "\n\t===============================";
        cout << "\n\tSelect Your Option (1-8): ";
        cin >> choice;

        switch (choice) {
        case '1':
            writeAccount();
            break;
        case '2':
            system("cls");
            cout << "\n\n\tEnter The Account No. : ";
            cin >> num;
            depositWithdraw(num, 1);
            break;
        case '3':
            system("cls");
            cout << "\n\n\tEnter The Account No. : ";
            cin >> num;
            depositWithdraw(num, 2);
            break;
        case '4':
            system("cls");
            cout << "\n\n\tEnter The Account No. : ";
            cin >> num;
            displaySpecific(num);
            break;
        case '5':
            displayAll();
            break;
        case '6':
            system("cls");
            cout << "\n\n\tEnter The Account No. to Delete: ";
            cin >> num;
            deleteAccount(num);
            break;
        case '7':
            system("cls");
            cout << "\n\n\tEnter The Account No. to Update: ";
            cin >> num;
            modifyAccount(num);
            break;
        case '8':
            cout << "\n\tThank you for using the Banking System!\n";
            exit(0);
        default:
            cout << "\n\tInvalid Choice! Please try again.";
        }
        cout << "\n\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    } while (choice != '8');

    return 0;
}

// Write record to binary file
void writeAccount() {
    BankAccount acc;
    ofstream outFile("bank_records.dat", ios::binary | ios::app);
    acc.createAccount();
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(BankAccount));
    outFile.close();
}

// Read specific record from binary file
void displaySpecific(int n) {
    BankAccount acc;
    bool flag = false;
    ifstream inFile("bank_records.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be opened! Press any key...";
        return;
    }

    cout << "\n\nBALANCE DETAILS\n";
    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(BankAccount))) {
        if (acc.getAccountNumber() == n) {
            acc.showAccount();
            flag = true;
        }
    }
    inFile.close();
    if (!flag) {
        cout << "\n\nAccount number does not exist!";
    }
}

// Deposit or Withdraw functionality
void depositWithdraw(int n, int option) {
    double amt;
    bool found = false;
    BankAccount acc;
    fstream file("bank_records.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nFile could not be opened! Press any key...";
        return;
    }

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(BankAccount))) {
        if (acc.getAccountNumber() == n) {
            acc.showAccount();
            if (option == 1) {
                cout << "\n\nTO DEPOSIT AMOUNT";
                cout << "\nEnter amount to be deposited: ";
                cin >> amt;
                acc.deposit(amt);
                cout << "\nDeposit Successful!";
            } else if (option == 2) {
                cout << "\n\nTO WITHDRAW AMOUNT";
                cout << "\nEnter amount to be withdrawn: ";
                cin >> amt;
                if (!acc.withdraw(amt)) {
                    cout << "\nInsufficient Balance!";
                    file.close();
                    return;
                }
                cout << "\nWithdrawal Successful!";
            }
            int pos = (-1) * static_cast<int>(sizeof(BankAccount));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(BankAccount));
            found = true;
            break;
        }
    }
    file.close();
    if (!found) {
        cout << "\n\nRecord Not Found!";
    }
}

// Display all accounts list
void displayAll() {
    system("cls");
    BankAccount acc;
    ifstream inFile("bank_records.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be opened! Press any key...";
        return;
    }

    cout << "\n\n\t\t=== ACCOUNT HOLDER'S LIST ===\n\n";
    cout << "=" << string(65, '=') << "\n";
    cout << left << setw(15) << "A/c No." << setw(25) << "Name" << setw(15) << "Type" << setw(10) << "Balance" << "\n";
    cout << "=" << string(65, '=') << "\n";

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(BankAccount))) {
        cout << left << setw(15) << acc.getAccountNumber() 
             << setw(25) << acc.getName() 
             << setw(15) << (acc.getAccountType() == 'S' ? "Savings" : "Current") 
             << "Rs. " << fixed << setprecision(2) << acc.getBalance() << "\n";
    }
    inFile.close();
}

// Delete an account from file storage
void deleteAccount(int n) {
    BankAccount acc;
    ifstream inFile("bank_records.dat", ios::binary);
    if (!inFile) {
        cout << "\nFile could not be opened!";
        return;
    }
    ofstream outFile("temp.dat", ios::binary | ios::app);
    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(BankAccount))) {
        if (acc.getAccountNumber() != n) {
            outFile.write(reinterpret_cast<char*>(&acc), sizeof(BankAccount));
        }
    }
    inFile.close();
    outFile.close();
    remove("bank_records.dat");
    rename("temp.dat", "bank_records.dat");
    cout << "\n\nRecord Deleted Successfully!";
}

// Update existing account details
void modifyAccount(int n) {
    bool found = false;
    BankAccount acc;
    fstream file("bank_records.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "\nFile could not be opened!";
        return;
    }
    while (file.read(reinterpret_cast<char*>(&acc), sizeof(BankAccount))) {
        if (acc.getAccountNumber() == n) {
            acc.showAccount();
            cout << "\n\nEnter The New Details Below:\n";
            acc.modifyAccount();
            int pos = (-1) * static_cast<int>(sizeof(BankAccount));
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(BankAccount));
            cout << "\n\nRecord Updated Successfully!";
            found = true;
            break;
        }
    }
    file.close();
    if (!found) {
        cout << "\n\nRecord Not Found!";
    }
}