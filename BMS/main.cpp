#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <sstream>

using namespace std;

// Transaction class to store transaction history
class Transaction {
private:
    string transactionId;
    string type;
    double amount;
    string date;
    double balanceAfter;

public:
    Transaction(string id, string t, double amt, string d, double bal) 
        : transactionId(id), type(t), amount(amt), date(d), balanceAfter(bal) {}
    
    void displayTransaction() const {
        cout << left << setw(15) << transactionId 
             << setw(12) << type 
             << setw(12) << fixed << setprecision(2) << amount
             << setw(12) << date
             << setw(12) << balanceAfter << endl;
    }
    
    string getTransactionData() const {
        return transactionId + "," + type + "," + to_string(amount) + "," + date + "," + to_string(balanceAfter);
    }
};

// Bank Account class
class BankAccount {
private:
    long accountNumber;
    string accountHolderName;
    string accountType;
    double balance;
    string dateCreated;
    string phoneNumber;
    string address;
    vector<Transaction> transactions;
    
    string getCurrentDate() {
        time_t now = time(0);
        char* dt = ctime(&now);
        string date(dt);
        date.pop_back(); // Remove newline character
        return date;
    }
    
    string generateTransactionId() {
        static int counter = 1000;
        return "TXN" + to_string(++counter);
    }

public:
    // Constructor
    BankAccount() : accountNumber(0), balance(0.0) {}
    
    BankAccount(long accNum, string name, string type, double initialDeposit, string phone, string addr) 
        : accountNumber(accNum), accountHolderName(name), accountType(type), 
          balance(initialDeposit), phoneNumber(phone), address(addr) {
        dateCreated = getCurrentDate();
        if (initialDeposit > 0) {
            transactions.push_back(Transaction(generateTransactionId(), "DEPOSIT", initialDeposit, dateCreated, balance));
        }
    }
    
    // Getter methods
    long getAccountNumber() const { return accountNumber; }
    string getAccountHolderName() const { return accountHolderName; }
    string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }
    string getDateCreated() const { return dateCreated; }
    string getPhoneNumber() const { return phoneNumber; }
    string getAddress() const { return address; }
    
    // Setter methods
    void setAccountHolderName(string name) { accountHolderName = name; }
    void setAccountType(string type) { accountType = type; }
    void setPhoneNumber(string phone) { phoneNumber = phone; }
    void setAddress(string addr) { address = addr; }
    
    // Banking operations
    bool deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount!" << endl;
            return false;
        }
        balance += amount;
        transactions.push_back(Transaction(generateTransactionId(), "DEPOSIT", amount, getCurrentDate(), balance));
        cout << "Amount deposited successfully!" << endl;
        cout << "New balance: $" << fixed << setprecision(2) << balance << endl;
        return true;
    }
    
    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount!" << endl;
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient funds!" << endl;
            cout << "Available balance: $" << fixed << setprecision(2) << balance << endl;
            return false;
        }
        balance -= amount;
        transactions.push_back(Transaction(generateTransactionId(), "WITHDRAWAL", amount, getCurrentDate(), balance));
        cout << "Amount withdrawn successfully!" << endl;
        cout << "New balance: $" << fixed << setprecision(2) << balance << endl;
        return true;
    }
    
    bool transfer(double amount, BankAccount& toAccount) {
        if (amount <= 0) {
            cout << "Invalid transfer amount!" << endl;
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient funds for transfer!" << endl;
            return false;
        }
        
        balance -= amount;
        toAccount.balance += amount;
        
        string date = getCurrentDate();
        transactions.push_back(Transaction(generateTransactionId(), "TRANSFER_OUT", amount, date, balance));
        toAccount.transactions.push_back(Transaction(toAccount.generateTransactionId(), "TRANSFER_IN", amount, date, toAccount.balance));
        
        cout << "Transfer successful!" << endl;
        cout << "Your new balance: $" << fixed << setprecision(2) << balance << endl;
        return true;
    }
    
    void displayAccountDetails() const {
        cout << "\n========== ACCOUNT DETAILS ==========" << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "Date Created: " << dateCreated << endl;
        cout << "Phone Number: " << phoneNumber << endl;
        cout << "Address: " << address << endl;
        cout << "=====================================" << endl;
    }
    
    void displayTransactionHistory() const {
        cout << "\n========== TRANSACTION HISTORY ==========" << endl;
        cout << left << setw(15) << "Transaction ID" 
             << setw(12) << "Type" 
             << setw(12) << "Amount"
             << setw(12) << "Date"
             << setw(12) << "Balance" << endl;
        cout << string(63, '-') << endl;
        
        for (const auto& transaction : transactions) {
            transaction.displayTransaction();
        }
        cout << "=========================================" << endl;
    }
    
    // File operations
    string getAccountData() const {
        stringstream ss;
        ss << accountNumber << "," << accountHolderName << "," << accountType << "," 
           << balance << "," << dateCreated << "," << phoneNumber << "," << address;
        return ss.str();
    }
    
    void setAccountData(const string& data) {
        stringstream ss(data);
        string item;
        
        getline(ss, item, ','); accountNumber = stol(item);
        getline(ss, accountHolderName, ',');
        getline(ss, accountType, ',');
        getline(ss, item, ','); balance = stod(item);
        getline(ss, dateCreated, ',');
        getline(ss, phoneNumber, ',');
        getline(ss, address, ',');
    }
};

// Bank Management System class
class BankManagementSystem {
private:
    vector<BankAccount> accounts;
    const string ACCOUNTS_FILE = "accounts.txt";
    long nextAccountNumber;
    
    void loadAccountsFromFile() {
        ifstream file(ACCOUNTS_FILE);
        if (!file.is_open()) {
            nextAccountNumber = 100001;
            return;
        }
        
        string line;
        long maxAccountNum = 100000;
        
        while (getline(file, line)) {
            if (!line.empty()) {
                BankAccount account;
                account.setAccountData(line);
                accounts.push_back(account);
                if (account.getAccountNumber() > maxAccountNum) {
                    maxAccountNum = account.getAccountNumber();
                }
            }
        }
        
        nextAccountNumber = maxAccountNum + 1;
        file.close();
    }
    
    void saveAccountsToFile() {
        ofstream file(ACCOUNTS_FILE);
        if (!file.is_open()) {
            cout << "Error: Unable to save accounts to file!" << endl;
            return;
        }
        
        for (const auto& account : accounts) {
            file << account.getAccountData() << endl;
        }
        
        file.close();
    }
    
    BankAccount* findAccount(long accountNumber) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accountNumber) {
                return &account;
            }
        }
        return nullptr;
    }

public:
    BankManagementSystem() {
        loadAccountsFromFile();
    }
    
    ~BankManagementSystem() {
        saveAccountsToFile();
    }
    
    void createAccount() {
        string name, type, phone, address;
        double initialDeposit;
        
        cout << "\n========== CREATE NEW ACCOUNT ==========" << endl;
        cout << "Enter account holder name: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Enter account type (Savings/Checking): ";
        getline(cin, type);
        
        cout << "Enter phone number: ";
        getline(cin, phone);
        
        cout << "Enter address: ";
        getline(cin, address);
        
        cout << "Enter initial deposit amount: $";
        cin >> initialDeposit;
        
        if (initialDeposit < 0) {
            cout << "Initial deposit cannot be negative!" << endl;
            return;
        }
        
        BankAccount newAccount(nextAccountNumber++, name, type, initialDeposit, phone, address);
        accounts.push_back(newAccount);
        
        cout << "\nAccount created successfully!" << endl;
        cout << "Your account number is: " << newAccount.getAccountNumber() << endl;
        saveAccountsToFile();
    }
    
    void depositMoney() {
        long accountNumber;
        double amount;
        
        cout << "\n========== DEPOSIT MONEY ==========" << endl;
        cout << "Enter account number: ";
        cin >> accountNumber;
        
        BankAccount* account = findAccount(accountNumber);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        cout << "Enter deposit amount: $";
        cin >> amount;
        
        if (account->deposit(amount)) {
            saveAccountsToFile();
        }
    }
    
    void withdrawMoney() {
        long accountNumber;
        double amount;
        
        cout << "\n========== WITHDRAW MONEY ==========" << endl;
        cout << "Enter account number: ";
        cin >> accountNumber;
        
        BankAccount* account = findAccount(accountNumber);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        cout << "Enter withdrawal amount: $";
        cin >> amount;
        
        if (account->withdraw(amount)) {
            saveAccountsToFile();
        }
    }
    
    void transferMoney() {
        long fromAccount, toAccount;
        double amount;
        
        cout << "\n========== TRANSFER MONEY ==========" << endl;
        cout << "Enter your account number: ";
        cin >> fromAccount;
        
        BankAccount* fromAcc = findAccount(fromAccount);
        if (!fromAcc) {
            cout << "Source account not found!" << endl;
            return;
        }
        
        cout << "Enter destination account number: ";
        cin >> toAccount;
        
        BankAccount* toAcc = findAccount(toAccount);
        if (!toAcc) {
            cout << "Destination account not found!" << endl;
            return;
        }
        
        if (fromAccount == toAccount) {
            cout << "Cannot transfer to the same account!" << endl;
            return;
        }
        
        cout << "Enter transfer amount: $";
        cin >> amount;
        
        if (fromAcc->transfer(amount, *toAcc)) {
            saveAccountsToFile();
        }
    }
    
    void checkBalance() {
        long accountNumber;
        
        cout << "\n========== BALANCE INQUIRY ==========" << endl;
        cout << "Enter account number: ";
        cin >> accountNumber;
        
        BankAccount* account = findAccount(accountNumber);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        cout << "Account Balance: $" << fixed << setprecision(2) << account->getBalance() << endl;
    }
    
    void displayAccountDetails() {
        long accountNumber;
        
        cout << "\n========== ACCOUNT DETAILS ==========" << endl;
        cout << "Enter account number: ";
        cin >> accountNumber;
        
        BankAccount* account = findAccount(accountNumber);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        account->displayAccountDetails();
    }
    
    void displayAllAccounts() {
        if (accounts.empty()) {
            cout << "No accounts found!" << endl;
            return;
        }
        
        cout << "\n========== ALL ACCOUNT HOLDERS ==========" << endl;
        cout << left << setw(12) << "Acc Number" 
             << setw(25) << "Name" 
             << setw(12) << "Type"
             << setw(12) << "Balance" << endl;
        cout << string(61, '-') << endl;
        
        for (const auto& account : accounts) {
            cout << left << setw(12) << account.getAccountNumber()
                 << setw(25) << account.getAccountHolderName()
                 << setw(12) << account.getAccountType()
                 << setw(12) << fixed << setprecision(2) << account.getBalance() << endl;
        }
        cout << "=========================================" << endl;
    }
    
    void modifyAccount() {
        long accountNumber;
        
        cout << "\n========== MODIFY ACCOUNT ==========" << endl;
        cout << "Enter account number: ";
        cin >> accountNumber;
        
        BankAccount* account = findAccount(accountNumber);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        string name, type, phone, address;
        
        cout << "Current account holder name: " << account->getAccountHolderName() << endl;
        cout << "Enter new name (or press Enter to keep current): ";
        cin.ignore();
        getline(cin, name);
        if (!name.empty()) {
            account->setAccountHolderName(name);
        }
        
        cout << "Current account type: " << account->getAccountType() << endl;
        cout << "Enter new type (or press Enter to keep current): ";
        getline(cin, type);
        if (!type.empty()) {
            account->setAccountType(type);
        }
        
        cout << "Current phone number: " << account->getPhoneNumber() << endl;
        cout << "Enter new phone (or press Enter to keep current): ";
        getline(cin, phone);
        if (!phone.empty()) {
            account->setPhoneNumber(phone);
        }
        
        cout << "Current address: " << account->getAddress() << endl;
        cout << "Enter new address (or press Enter to keep current): ";
        getline(cin, address);
        if (!address.empty()) {
            account->setAddress(address);
        }
        
        cout << "Account modified successfully!" << endl;
        saveAccountsToFile();
    }
    
    void closeAccount() {
        long accountNumber;
        
        cout << "\n========== CLOSE ACCOUNT ==========" << endl;
        cout << "Enter account number: ";
        cin >> accountNumber;
        
        auto it = find_if(accounts.begin(), accounts.end(), 
                         [accountNumber](const BankAccount& acc) {
                             return acc.getAccountNumber() == accountNumber;
                         });
        
        if (it == accounts.end()) {
            cout << "Account not found!" << endl;
            return;
        }
        
        char confirm;
        cout << "Are you sure you want to close this account? (y/n): ";
        cin >> confirm;
        
        if (confirm == 'y' || confirm == 'Y') {
            accounts.erase(it);
            cout << "Account closed successfully!" << endl;
            saveAccountsToFile();
        } else {
            cout << "Account closure cancelled." << endl;
        }
    }
    
    void displayTransactionHistory() {
        long accountNumber;
        
        cout << "\n========== TRANSACTION HISTORY ==========" << endl;
        cout << "Enter account number: ";
        cin >> accountNumber;
        
        BankAccount* account = findAccount(accountNumber);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        account->displayTransactionHistory();
    }
    
    void displayMenu() {
        cout << "\n========================================" << endl;
        cout << "    BANK MANAGEMENT SYSTEM" << endl;
        cout << "========================================" << endl;
        cout << "1. Create New Account" << endl;
        cout << "2. Deposit Money" << endl;
        cout << "3. Withdraw Money" << endl;
        cout << "4. Transfer Money" << endl;
        cout << "5. Check Balance" << endl;
        cout << "6. Display Account Details" << endl;
        cout << "7. Display All Accounts" << endl;
        cout << "8. Modify Account" << endl;
        cout << "9. Close Account" << endl;
        cout << "10. Transaction History" << endl;
        cout << "11. Exit" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice: ";
    }
    
    void run() {
        int choice;
        
        cout << "Welcome to Bank Management System!" << endl;
        
        while (true) {
            displayMenu();
            cin >> choice;
            
            switch (choice) {
                case 1:
                    createAccount();
                    break;
                case 2:
                    depositMoney();
                    break;
                case 3:
                    withdrawMoney();
                    break;
                case 4:
                    transferMoney();
                    break;
                case 5:
                    checkBalance();
                    break;
                case 6:
                    displayAccountDetails();
                    break;
                case 7:
                    displayAllAccounts();
                    break;
                case 8:
                    modifyAccount();
                    break;
                case 9:
                    closeAccount();
                    break;
                case 10:
                    displayTransactionHistory();
                    break;
                case 11:
                    cout << "Thank you for using Bank Management System!" << endl;
                    return;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
            
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
};

// Main function
int main() {
    BankManagementSystem bank;
    bank.run();
    return 0;
}
