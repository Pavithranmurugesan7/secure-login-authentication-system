#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stack>
#include <sstream>
#include <functional>

using namespace std;

struct User {
    string passwordHash;
    int attempts;
    bool locked;
};

unordered_map<string, User> users;
stack<string> loginHistory;

string hashPassword(string password) {
    hash<string> hasher;
    return to_string(hasher(password));
}

// ---------------- Load Users ----------------
void loadUsers() {
    ifstream file("users.txt");
    string username, hash;
    int attempts;
    bool locked;

    while (file >> username >> hash >> attempts >> locked) {
        users[username] = {hash, attempts, locked};
    }
    file.close();
}

// ---------------- Save Users ----------------
void saveUsers() {
    ofstream file("users.txt");

    for (auto &u : users) {
        file << u.first << " "
             << u.second.passwordHash << " "
             << u.second.attempts << " "
             << u.second.locked << endl;
    }
    file.close();
}

// ---------------- Register ----------------
void registerUser() {
    string username, password;

    cout << "Enter Username: ";
    cin >> username;

    if (users.count(username)) {
        cout << "User already exists!\n";
        return;
    }

    cout << "Enter Password: ";
    cin >> password;

    users[username] = {hashPassword(password), 0, false};

    saveUsers();
    cout << "Registration Successful!\n";
}

// ---------------- Login ----------------
void loginUser() {
    string username, password;

    cout << "Enter Username: ";
    cin >> username;

    if (!users.count(username)) {
        cout << "User not found!\n";
        return;
    }

    if (users[username].locked) {
        cout << "Account Locked!\n";
        return;
    }

    cout << "Enter Password: ";
    cin >> password;

    if (hashPassword(password) == users[username].passwordHash) {
        cout << "Login Successful!\n";
        users[username].attempts = 0;
        loginHistory.push(username);
    }
    else {
        users[username].attempts++;
        cout << "Wrong Password!\n";

        if (users[username].attempts >= 3) {
            users[username].locked = true;
            cout << "Account Locked after 3 failed attempts!\n";
        }
    }

    saveUsers();
}

// ---------------- Login History ----------------
void showHistory() {
    if (loginHistory.empty()) {
        cout << "No login history.\n";
        return;
    }

    stack<string> temp = loginHistory;

    cout << "\nLogin History:\n";
    while (!temp.empty()) {
        cout << temp.top() << endl;
        temp.pop();
    }
}

// ---------------- Menu ----------------
void menu() {
    int choice;

    do {
        cout << "\n===== Secure Login System =====\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Login History\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: showHistory(); break;
        }

    } while (choice != 4);
}

int main() {
    loadUsers();
    menu();
    return 0;
}