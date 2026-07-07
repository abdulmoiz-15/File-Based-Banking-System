#include <iostream>
#include <fstream>
#include <string>
// For remove and rename functions
#include <cstdio> 
// For Time function
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

// Global Variables
int fileCount = 0;
const string countFile = "E:/Files/count.txt";
string files[10] = {                   // Files stored in an array
    "E:/Files/1.txt", "E:/Files/2.txt", "E:/Files/3.txt",
    "E:/Files/4.txt", "E:/Files/5.txt", "E:/Files/6.txt",
    "E:/Files/7.txt", "E:/Files/8.txt", "E:/Files/9.txt",
    "E:/Files/10.txt"
};

string Time() {
    time_t now = time(0);
    tm ltm;
    localtime_s(&ltm, &now); // safer version for Visual Studio

    stringstream ss;
    ss << put_time(&ltm, "%d-%m-%Y %H:%M:%S");
    return ss.str();
}

void updateFileCount() {
    ifstream readFrom(countFile);
    readFrom >> fileCount;
    readFrom.close();
}

void writeFileCount() {
    ofstream writeTo(countFile);
    writeTo << fileCount;
    writeTo.close();
}

void updateLoanIndicesAfterDeletion(int deletedIndex) {
    ifstream readFrom("E:/Files/loans.txt");
    ofstream writeTo("E:/Files/temp_loans.txt");
    int fileIndex;
    string uname;
    long loanAmount;

    while (readFrom >> fileIndex >> uname >> loanAmount) {
        if (fileIndex != deletedIndex) {
            if (fileIndex > deletedIndex) { writeTo << fileIndex - 1; }
            else { writeTo << fileIndex; }
            writeTo << " " << uname << " " << loanAmount << "\n";
        }
    }

    readFrom.close();
    writeTo.close();
    if (remove("E:/Files/loans.txt") != 0) { perror(""); }
    if (rename("E:/Files/temp_loans.txt", "E:/Files/loans.txt") != 0) { perror(""); }
}

class User {
protected:
    string username;
    int pin;
public:
    User(string u,int p):username(u),pin(p){}
    virtual void login() = 0;
    virtual void interfaceMenu() = 0;
    virtual ~User() {}
};

class Customer : public User {
private:
    long balance;
    int fileNum;
public:
    Customer(int n, string user, int p, long b):User(user, p), balance(b), fileNum(n){}

    void login() override { interfaceMenu(); }

    void interfaceMenu() override {
        int choice;
        do {
            cout << "   |====================================================|\n";
            cout << "   |              ACCOUNT OPERATIONS MENU               |\n";
            cout << "   |====================================================|\n";
            cout << "   |     (1) Check Balance                              |\n";
            cout << "   |     (2) Deposit Money                              |\n";
            cout << "   |     (3) Withdraw Money                             |\n";
            cout << "   |     (4) Request Loan                               |\n";
            cout << "   |     (5) Check Transaction Log                      |\n";
            cout << "   |     (6) Edit Account Info                          |\n";
            cout << "   |     (7) Sign Out                                   |\n";
            cout << "   |====================================================|\n";
            cout << "                 Enter your choice: ";
            cin >> choice;
            cout << "\n";
            switch (choice) {
            case 1: cout << "   Balance: Rs." << balance << "\n\n"; break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: loans(); break;
            case 5: showHistory(); break;
            case 6: editacc(); break;
            }
        } while (choice != 7);
    }

    void deposit() {
        long amt;
        cout << "   Deposit amount: ";
        cin >> amt;
        balance += amt;
        updateFile();
        logTransaction("Deposited", amt);
        cout << "\n   [ Deposit successful ]\n   New balance: Rs." << balance << "\n\n";
    }

    void withdraw() {
        long amt;
        cout << "   Withdraw amount: ";
        cin >> amt;
        if (amt > balance) cout << "\n   [ Insufficient funds!\n\n";
        else {
            balance -= amt;
            updateFile();
            logTransaction("Withdrew", amt);
            cout << "\n   [ Withdrawal successful ]\n   New balance : Rs." << balance << "\n\n";
        }
    }

    void loans() {
        long loan;
        cout << "   Loan amount: ";
        cin >> loan;
        cout << "   Approval Pending...\n\n";
        ifstream readFrom(files[fileNum]); 
        ofstream writeTo("E:/Files/loans.txt", ios::app);
        string u;
        readFrom >> u;
        writeTo << fileNum << " " << u << " " << loan << "\n\n";
        readFrom.close();
        writeTo.close();
        logTransaction("Loan Requested", loan);
    }

    void showHistory() {
        ifstream readFrom(files[fileNum]);
        string line;
        getline(readFrom, line); // Username
        getline(readFrom, line); // PIN
        while (getline(readFrom, line)) cout << line << endl;
        readFrom.close();
    }

    void editacc() {
        int enteredPin;
        cout << " ---Verify Account---\n";
        cout << "   Enter PIN: ";
        cin >> enteredPin;

        ifstream readFrom(files[fileNum]);
        string u;
        int p;
        long b;
        readFrom >> u >> p >> b;
        readFrom.close();

        if (p == enteredPin) {
            char ch;
            cout << "   Edit Username (y/n): ";
            cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                cout << "   Enter New Username: ";
                cin >> username;
            }
            else {
                username = u;
            }

            cout << "   Edit PIN (y/n): ";
            cin >> ch;
            if (ch == 'y' || ch == 'Y') {
                cout << "   Enter New PIN: ";
                cin >> pin;
            }
            else {
                pin = p;
            }

            updateFile();
            cout << "\n  [ Account info updated successfully! ]\n\n";
        }
        else {
            cout << "\n   [ Incorrect PIN! ]\n\n";
        }
    }

    void updateFile() {
        string temp = "E:/Files/temp.txt";

        // Step 1: Read from original file and write to temp file
        ifstream readFrom(files[fileNum]);
        ofstream writeTo(temp);
        string u;
        int p;
        readFrom >> u >> p;
        string skipline;
        getline(readFrom, skipline); // skip newline
        getline(readFrom, skipline); // skip old balance
        writeTo << username << " " << pin << "\n" << balance << "\n";
        string line;
        while (getline(readFrom, line)) {
            writeTo << line << "\n";
        }
        readFrom.close();
        writeTo.close();

        // Step 2: Overwrite original file with temp content
        ifstream tempFile(temp);
        ofstream origFile(files[fileNum], ios::trunc);
        while (getline(tempFile, line)) {
            origFile << line << "\n";
        }
        tempFile.close();
        origFile.close();

        // Step 3: Clear temp file
        ofstream tempClear(temp, ios::trunc);
        tempClear.close();
    }

    void logTransaction(const string& action, long amount) {
        ofstream logFile(files[fileNum], ios::app);
        logFile << "Rs." << amount << " " << action << " [ " << Time() << " ]" << "\n";
        logFile.close();
    }
};

class Admin : public User {
public:
    Admin():User("admin", 1111){}

    void login() override { interfaceMenu(); }

    void interfaceMenu() override {
        int choice;
        do {
            cout << "   |======================================================|\n";
            cout << "   |                   ADMIN OPERATIONS                   |\n";
            cout << "   |======================================================|\n";
            cout << "   |     (1) Approve Loans                                |\n";
            cout << "   |     (2) Check Details & Log                          |\n";
            cout << "   |     (3) Delete Account                               |\n";
            cout << "   |     (4) Sign Out                                     |\n";
            cout << "   |======================================================|\n";
            cout << "              Enter your choice: ";
            cin >> choice;
            cout << "\n";
            switch (choice) {
            case 1: approveloans(); break;
            case 2: history(); break;
            case 3: deleteacc(); break;
            }
        } while (choice != 4);
    }

    void approveloans() {
        ifstream loanFile("E:/Files/loans.txt");
        if (!loanFile.is_open()) {
            cout << "\n   [ No loan requests found ]\n\n";
            return;
        }

        ofstream newloanFile("E:/Files/temp_loans.txt");

        int filenum;
        string uname;
        long loan;
        char approve;

        while (loanFile >> filenum >> uname >> loan) {
            cout << "\n  -----Loan Request-----\n";
            cout << "   Username: " << uname << "\n";
            cout << "   Loan amount: " << loan << "\n";
            cout << "   Approve Loan (y/n): ";
            cin >> approve;
            cout << "\n";
            if (approve == 'y') {
                ifstream readFrom(files[filenum]);
                string u;
                int p;
                long b;
                readFrom >> u >> p >> b;
                readFrom.close();

                Customer cust(filenum, u, p, b + loan);
                cust.updateFile();
                logLoanStatus(filenum, loan, "Approved");
            }
            else {
                newloanFile << filenum << " " << uname << " " << loan << "\n";
                logLoanStatus(filenum, loan, "Refused");
            }
        }

        loanFile.close();
        newloanFile.close();

        if (remove("E:/Files/loans.txt") != 0) { perror(""); }
        if (rename("E:/Files/temp_loans.txt", "E:/Files/loans.txt") != 0) { perror(""); }
    }

    void logLoanStatus(int fileIndex, long loanAmount, const string& status) {
        ofstream appendLoan(files[fileIndex], ios::app);
        appendLoan << "Rs." << loanAmount << " Loan " << status << " [ " << Time() << " ]" << "\n";
        appendLoan.close();
    }

    void history() {
        int user;
        cout << "   Select Account (1-" << fileCount << "): ";
        cin >> user;
        ifstream readFrom(files[user - 1]);
        string line;
        readFrom >> line;
        cout << "---Account " << user << " Details & Log---" << endl;
        cout << "Name: " << line << endl;
        readFrom >> line; // Skip pin
        getline(readFrom, line); //Skip newline
        getline(readFrom, line);
        cout << "Balance: " << line << endl;       
        while (getline(readFrom, line)) { cout << line << endl; }
        readFrom.close();
    }

    void deleteacc() {
        int acc;
        cout << "   Select account (1-" << fileCount << "): ";
        cin >> acc;
        acc--;
        if (acc < 0 || acc >= fileCount) {
            cout << "\n   [ Invalid account number! ]\n";
            return;
        }

        for (int i = acc; i < fileCount - 1; i++) {
            ifstream readFrom(files[i + 1]);
            ofstream writeTo(files[i], ios::trunc);
            string line;
            while (getline(readFrom, line)) {
                writeTo << line << "\n";
            }
            readFrom.close();
            writeTo.close();
        }

        ofstream clear(files[fileCount - 1], ios::trunc);
        clear.close();

        fileCount--;
        writeFileCount();
        updateLoanIndicesAfterDeletion(acc);
        cout << "\n   [ Account deleted successfully! ]\n\n";
    }
};

class LoginSystem {
public:
    void run() {
        updateFileCount();
        int ch;
        do {
            cout << "   |===================================================|\n";
            cout << "   |                BANKING SYSTEM MENU                |\n";
            cout << "   |===================================================|\n";
            cout << "   |     (1) Log In                                    |\n";
            cout << "   |     (2) Create New Account                        |\n";
            cout << "   |     (3) Exit                                      |\n";
            cout << "   |===================================================|\n";
            cout << "                 Enter your choice: ";
            cin >> ch;
            if (ch == 1) {
                cout << "\n";
                loginUser();
            }
            else if (ch == 2) {
                cout << "\n";
                createAccount();
            }
        } while (ch != 3);
    }

    void createAccount() {
        if (fileCount >= 10) {
            cout << "\n   [ Account limit reached! ]\n";
            return;
        }
        string username;
        int pin;
        long balance = 5000;
        cout << "   Set Username: "; cin >> username;
        for (int i = 0; i < fileCount; i++) {
            string u;
            ifstream readFrom(files[i]);
            readFrom >> u;
            if (username == u) { cout << "\n   [ Username already exists! ]\n\n"; return; }
        }
        cout << "   Set 4-digit PIN: "; cin >> pin;

        ofstream writeTo(files[fileCount]);
        writeTo << username << " " << pin << "\n" << balance << "\n";
        writeTo.close();
        fileCount++;
        writeFileCount();
        cout << "\n   [ Account Created! ]\n\n";
    }

    void loginUser() {
        string username;
        int pin;
        cout << "   Username: "; cin >> username;
        cout << "   PIN: "; cin >> pin;
        cout << "\n";

        if (username == "admin" && pin == 1111) {
            Admin a; a.login(); return;
        }

        for (int i = 0; i < fileCount; i++) {
            ifstream readFrom(files[i]);
            string u;
            int p;
            long b;
            readFrom >> u >> p >> b;
            readFrom.close();
            if (u == username && p == pin) {
                Customer c(i, u, p, b);
                c.login(); return;
            }
        }
        cout << "\n   [ Login failed! ]\n\n";
    }
};

int main() {
    LoginSystem system;
    system.run();
    return 0;
}
