#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Block {
    int blockNumber;
    string sender, receiver, description;
    double amount;
    string previousHash, hash;
    Block* next;

    Block(int n, string s, string r, double a, string d, string prev)
        : blockNumber(n), sender(s), receiver(r), amount(a),
          description(d), previousHash(prev), next(nullptr) {}
};

string calculateHash(Block* b) {
    string data = to_string(b->blockNumber) + b->sender + b->receiver +
                  to_string(b->amount) + b->description + b->previousHash;

    unsigned long long h = 0;
    for (char c : data)
        h = h * 31 + static_cast<unsigned char>(c);

    return to_string(h);
}

void addTransaction(Block*& head, int& count) {
    string sender, receiver, description;
    double amount;

    cout << "\nSender: ";
    cin >> sender;
    cout << "Receiver: ";
    cin >> receiver;
    cout << "Amount: ";
    cin >> amount;
    cin.ignore();
    cout << "Description: ";
    getline(cin, description);

    string previousHash = "000000";
    Block* last = head;

    if (last) {
        while (last->next)
            last = last->next;
        previousHash = last->hash;
    }

    Block* b = new Block(++count, sender, receiver, amount,
                         description, previousHash);

    b->hash = calculateHash(b);

    if (!head)
        head = b;
    else
        last->next = b;

    cout << "\nTransaction added. Hash: " << b->hash << endl;
}

void displayLedger(Block* head) {
    if (!head) {
        cout << "\nLedger is empty.\n";
        return;
    }

    cout << "\n========== LEDGER ==========\n";

    while (head) {
        cout << "\nBlock #" << head->blockNumber
             << "\n" << head->sender << " -> " << head->receiver
             << "\nAmount: Rs. " << head->amount
             << "\nDescription: " << head->description
             << "\nPrevious Hash: " << head->previousHash
             << "\nHash: " << head->hash << "\n";

        head = head->next;
    }
}

void showFinalBalances(Block* head) {
    if (!head) {
        cout << "\nLedger is empty.\n";
        return;
    }

    vector<string> people;
    vector<double> balance;

    while (head) {
        int s = -1, r = -1;

        for (int i = 0; i < people.size(); i++) {
            if (people[i] == head->sender) s = i;
            if (people[i] == head->receiver) r = i;
        }

        if (s == -1) {
            people.push_back(head->sender);
            balance.push_back(0);
            s = people.size() - 1;
        }

        if (r == -1) {
            people.push_back(head->receiver);
            balance.push_back(0);
            r = people.size() - 1;
        }

        balance[s] -= head->amount;
        balance[r] += head->amount;

        head = head->next;
    }

    cout << "\n====== FINAL BALANCES ======\n";

    for (int i = 0; i < people.size(); i++) {
        if (balance[i] < 0)
            cout << people[i] << " owes Rs. " << -balance[i] << endl;
        else if (balance[i] > 0)
            cout << people[i] << " should receive Rs. " << balance[i] << endl;
        else
            cout << people[i] << " is settled.\n";
    }
}

void deleteLedger(Block* head) {
    while (head) {
        Block* next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    Block* head = nullptr;
    int count = 0;

    while (true) {
        cout << "\n\n===== TAMPER-PROOF LEDGER =====\n";
        cout << "1. Add Transaction\n";
        cout << "2. View Ledger\n";
        cout << "3. Show Final Balances\n";
        cout << "4. Exit\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 1)
            addTransaction(head, count);
        else if (choice == 2)
            displayLedger(head);
        else if (choice == 3)
            showFinalBalances(head);
        else if (choice == 4) {
            deleteLedger(head);
            break;
        }
        else
            cout << "Invalid choice.\n";
    }

    return 0;
}