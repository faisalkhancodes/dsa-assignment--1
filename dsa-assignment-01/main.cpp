#include <iostream>
#include <string>
#include <limits>

using namespace std;

// ============================================================
//  UTILITY INPUT FUNCTIONS (free functions, not tied to class)
// ============================================================

int getSafeInt(const string& msg) {
    int value;
    while (true) {
        cout << msg;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "  Invalid input! Please enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double getSafeDouble(const string& msg) {
    double value;
    while (true) {
        cout << msg;
        if (cin >> value && value > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "  Invalid input! Please enter a positive weight.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Supports multi-word strings (e.g. "Hazardous Material")
string getSafeString(const string& msg) {
    string value;
    cout << msg;
    getline(cin, value);
    return value;
}

// ============================================================
//  CARGO CLASS
// ============================================================

class Cargo {
public:
    int    cargoID;       // Unique identifier
    string type;          // Type/description of cargo
    string origin;        // Origin city
    string destination;   // Destination city
    double weight;        // Weight in tons
    int    priority;      // Dispatch priority (higher = more urgent)
    bool   isActive;      // True when slot is occupied

    Cargo() : cargoID(-1), weight(0.0), priority(0), isActive(false) {}
};

// ============================================================
//  MANIFEST CLASS
// ============================================================

class Manifest {
private:
    static const int MAX = 50;
    Cargo yard[MAX];   // Fixed-size array of cargo slots
    int   link[MAX];   // Simulated linked-list pointers
    int   head;        // Index of first cargo in list

    // Find first free slot; returns -1 if yard is full
    int findFreeSlot() const {
        for (int i = 0; i < MAX; i++)
            if (!yard[i].isActive) return i;
        return -1;
    }

    // Search by ID; returns slot index or -1 if not found
    // Private: internal helper only
    int locateCargo(int id) const {
        for (int i = 0; i < MAX; i++)
            if (yard[i].isActive && yard[i].cargoID == id)
                return i;
        return -1;
    }

    // Check if a Cargo ID is already taken
    bool idExists(int id) const {
        return locateCargo(id) != -1;
    }

    // Append slot index to end of linked list
    void appendToList(int slot) {
        if (head == -1) {
            head = slot;
        } else {
            int temp = head;
            while (link[temp] != -1)
                temp = link[temp];
            link[temp] = slot;
        }
    }

    // Print a single cargo record
    void printCargo(int slot) const {
        cout << "  Slot     : " << slot                    << "\n"
             << "  ID       : " << yard[slot].cargoID      << "\n"
             << "  Type     : " << yard[slot].type         << "\n"
             << "  Origin   : " << yard[slot].origin       << "\n"
             << "  Dest     : " << yard[slot].destination  << "\n"
             << "  Weight   : " << yard[slot].weight << " tons\n"
             << "  Priority : " << yard[slot].priority     << "\n"
             << "  " << string(30, '-') << "\n";
    }

public:

    Manifest() : head(-1) {
        for (int i = 0; i < MAX; i++)
            link[i] = -1;
    }

    // --------------------------------------------------------
    //  1. LOAD CARGO
    //     Auto-assigns the next free slot; validates duplicate IDs
    // --------------------------------------------------------
    void Load_Cargo() {
        int slot = findFreeSlot();
        if (slot == -1) {
            cout << "  ERROR: Yard is full! Dispatch cargo before loading more.\n";
            return;
        }

        int id = getSafeInt("  Enter Cargo ID       : ");
        if (idExists(id)) {
            cout << "  ERROR: Cargo ID " << id << " already exists in the yard.\n";
            return;
        }

        yard[slot].cargoID     = id;
        yard[slot].type        = getSafeString("  Enter Cargo Type     : ");
        yard[slot].origin      = getSafeString("  Enter Origin City    : ");
        yard[slot].destination = getSafeString("  Enter Destination    : ");
        yard[slot].weight      = getSafeDouble("  Enter Weight (tons)  : ");
        yard[slot].priority    = getSafeInt   ("  Enter Priority       : ");
        yard[slot].isActive    = true;

        appendToList(slot);

        cout << "  SUCCESS: Cargo loaded into Slot " << slot << ".\n";
    }

    // --------------------------------------------------------
    //  2. DISPATCH CARGO  (by Cargo ID)
    // --------------------------------------------------------
    void Dispatch_Cargo() {
        int id   = getSafeInt("  Enter Cargo ID to dispatch: ");
        int slot = locateCargo(id);

        if (slot == -1) {
            cout << "  ERROR: No cargo with ID " << id << " found.\n";
            return;
        }

        // Unlink from list
        if (head == slot) {
            head = link[slot];
        } else {
            int prev = head;
            while (prev != -1 && link[prev] != slot)
                prev = link[prev];
            if (prev != -1)
                link[prev] = link[slot];
        }

        yard[slot].isActive = false;
        link[slot]          = -1;

        cout << "  SUCCESS: Cargo ID " << id << " dispatched from Slot " << slot << ".\n";
    }

    // --------------------------------------------------------
    //  3. SHOW MANIFEST
    // --------------------------------------------------------
    void Show_Manifest() const {
        if (head == -1) {
            cout << "  Yard is empty.\n";
            return;
        }

        cout << "\n  ========== CARGO MANIFEST ==========\n";
        int curr = head;
        while (curr != -1) {
            printCargo(curr);
            curr = link[curr];
        }
    }

    // --------------------------------------------------------
    //  4. SEARCH CARGO  (by Cargo ID)
    // --------------------------------------------------------
    void Search_Cargo() const {
        int id   = getSafeInt("  Enter Cargo ID to search: ");
        int slot = locateCargo(id);

        if (slot == -1)
            cout << "  Cargo ID " << id << " not found.\n";
        else {
            cout << "\n  Cargo found:\n";
            printCargo(slot);
        }
    }

    // --------------------------------------------------------
    //  5. UPDATE CARGO  (destination, weight, priority)
    // --------------------------------------------------------
    void Update_Cargo() {
        int id   = getSafeInt("  Enter Cargo ID to update: ");
        int slot = locateCargo(id);

        if (slot == -1) {
            cout << "  ERROR: Cargo ID " << id << " not found.\n";
            return;
        }

        cout << "  (Press Enter to keep current value)\n";

        cout << "  Current Destination [" << yard[slot].destination << "]: ";
        string newDest;
        getline(cin, newDest);
        if (!newDest.empty()) yard[slot].destination = newDest;

        yard[slot].weight   = getSafeDouble("  New Weight (tons)  : ");
        yard[slot].priority = getSafeInt   ("  New Priority       : ");

        cout << "  SUCCESS: Cargo ID " << id << " updated.\n";
    }

    // --------------------------------------------------------
    //  6. SORT BY WEIGHT  (ascending — lightest first)
    //     Bubble-sort on the linked list
    // --------------------------------------------------------
    void Rearrange_By_Weight() {
        if (head == -1 || link[head] == -1) return;  // 0 or 1 item

        bool swapped;
        do {
            swapped   = false;
            int curr  = head;
            int prev  = -1;

            while (curr != -1 && link[curr] != -1) {
                int next = link[curr];

                // FIX: swap when current is HEAVIER than next (ascending order)
                if (yard[curr].weight > yard[next].weight) {
                    // Swap nodes in the list
                    if (prev == -1)
                        head = next;
                    else
                        link[prev] = next;

                    link[curr] = link[next];
                    link[next] = curr;

                    prev    = next;
                    swapped = true;
                } else {
                    prev = curr;
                    curr = link[curr];
                }
            }
        } while (swapped);  // Repeat until no swaps needed

        cout << "  Manifest sorted by weight (lightest first).\n";
    }

    // --------------------------------------------------------
    //  7. SORT BY PRIORITY  (descending — highest priority first)
    // --------------------------------------------------------
    void Rearrange_By_Priority() {
        if (head == -1 || link[head] == -1) return;

        bool swapped;
        do {
            swapped  = false;
            int curr = head;
            int prev = -1;

            while (curr != -1 && link[curr] != -1) {
                int next = link[curr];

                if (yard[curr].priority < yard[next].priority) {
                    if (prev == -1)
                        head = next;
                    else
                        link[prev] = next;

                    link[curr] = link[next];
                    link[next] = curr;

                    prev    = next;
                    swapped = true;
                } else {
                    prev = curr;
                    curr = link[curr];
                }
            }
        } while (swapped);

        cout << "  Manifest sorted by priority (highest first).\n";
    }

    // --------------------------------------------------------
    //  8. ROUTE FILTER  (show all cargo for a destination)
    // --------------------------------------------------------
    void Divide_By_Route() const {
        string city = getSafeString("  Enter destination city: ");

        int  curr  = head;
        bool found = false;

        cout << "\n  Shipments destined for \"" << city << "\":\n";
        cout << "  " << string(30, '-') << "\n";

        while (curr != -1) {
            if (yard[curr].destination == city) {
                printCargo(curr);
                found = true;
            }
            curr = link[curr];
        }

        if (!found)
            cout << "  No shipments found for that destination.\n";
    }

    // --------------------------------------------------------
    //  9. RESET YARD
    // --------------------------------------------------------
    void Reset_Yard() {
        char confirm;
        cout << "  WARNING: This will clear all cargo. Confirm? (y/n): ";
        cin  >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (confirm != 'y' && confirm != 'Y') {
            cout << "  Reset cancelled.\n";
            return;
        }

        head = -1;
        for (int i = 0; i < MAX; i++) {
            yard[i] = Cargo();  // Reset to default
            link[i] = -1;
        }
        cout << "  Yard reset successfully.\n";
    }

    // --------------------------------------------------------
    //  Yard stats (capacity)
    // --------------------------------------------------------
    void Show_Stats() const {
        int used = 0;
        for (int i = 0; i < MAX; i++)
            if (yard[i].isActive) used++;

        cout << "  Slots used : " << used << " / " << MAX << "\n";
        cout << "  Slots free : " << (MAX - used) << "\n";
    }
};

// ============================================================
//  MAIN
// ============================================================

int main() {
    Manifest system;
    int choice;

    while (true) {
        cout << "\n  ==============================\n";
        cout << "    RAILWAY CARGO SYSTEM\n";
        cout << "  ==============================\n";
        cout << "  1.  Load Cargo\n";
        cout << "  2.  Dispatch Cargo\n";
        cout << "  3.  Show Manifest\n";
        cout << "  4.  Search Cargo\n";
        cout << "  5.  Update Cargo\n";
        cout << "  6.  Sort by Weight\n";
        cout << "  7.  Sort by Priority\n";
        cout << "  8.  Route Filter\n";
        cout << "  9.  Yard Stats\n";
        cout << "  10. Reset Yard\n";
        cout << "  11. Exit\n";
        cout << "  ------------------------------\n";

        choice = getSafeInt("  Choose option: ");

        cout << "\n";
        switch (choice) {
            case  1: system.Load_Cargo();          break;
            case  2: system.Dispatch_Cargo();      break;
            case  3: system.Show_Manifest();       break;
            case  4: system.Search_Cargo();        break;
            case  5: system.Update_Cargo();        break;
            case  6: system.Rearrange_By_Weight(); break;
            case  7: system.Rearrange_By_Priority(); break;
            case  8: system.Divide_By_Route();     break;
            case  9: system.Show_Stats();          break;
            case 10: system.Reset_Yard();          break;
            case 11:
                cout << "  Goodbye!\n";
                return 0;
            default:
                cout << "  Invalid option. Please choose 1-11.\n";
        }
    }
}
