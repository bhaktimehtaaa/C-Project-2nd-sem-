#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

// Struct to hold nutritional data
struct FoodItem {
    string name;
    int calories;
    int protein; // in grams
};

class MacroLogger {
private:
    vector<FoodItem> database;
    const string DB_FILENAME = "food_database.csv";

    // Helper to get today's date as a string (e.g., "17-04-2026")
    string getTodayDateString() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream ss;
        ss << setfill('0') << setw(2) << ltm->tm_mday << "-"
           << setfill('0') << setw(2) << 1 + ltm->tm_mon << "-"
           << 1900 + ltm->tm_year;
        return ss.str();
    }

    // String manipulation helper to make searches case-insensitive
    string toLower(string str) {
        for (char &c : str) {
            c = tolower(c);
        }
        return str;
    }

public:
    // --- FILE HANDLING: INITIALIZE DATABASE ---
    void initializeDatabase() {
        ifstream file(DB_FILENAME);
        
        if (!file) {
            // File doesn't exist, create it with default data
            ofstream newFile(DB_FILENAME);
            if (!newFile) {
                cerr << "\n[CRITICAL ERROR] Cannot create '" << DB_FILENAME << "'.\n";
                cerr << "Check your folder permissions or VS Code working directory.\n";
                return; // Stop trying to load if we can't create it
            }
            
            newFile << "Chicken Breast,165,31\n";
            newFile << "Soya Chunk Sabzi,345,52\n";
            newFile << "Paneer,296,14\n";
            newFile << "Pintola Protein Oats,400,22\n";
            newFile << "Boiled Eggs (2),155,12\n";
            newFile.close();
            
            // Clear the fail state from the first 'ifstream' attempt and open the new file
            file.clear(); 
            file.open(DB_FILENAME);
        }
        // NOTE: If the file already existed, 'file' is already open and ready to read.

        // Load data from CSV into memory
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string nameStr, calStr, proStr;
            
            getline(ss, nameStr, ',');
            getline(ss, calStr, ',');
            getline(ss, proStr, ',');

            if (!nameStr.empty() && !calStr.empty() && !proStr.empty()) {
                database.push_back({nameStr, stoi(calStr), stoi(proStr)});
            }
        }
        file.close();
    }

    // --- FILE HANDLING: WRITE TO DB ---
    void addNewFood() {
        string name;
        int calories, protein;
        
        cout << "Enter food name: ";
        getline(cin, name);
        cout << "Enter calories per serving: ";
        cin >> calories;
        cout << "Enter protein (g) per serving: ";
        cin >> protein;

        // Append to CSV file
        ofstream file(DB_FILENAME, ios::app);
        if (file.is_open()) {
            file << name << "," << calories << "," << protein << "\n";
            database.push_back({name, calories, protein});
            cout << "Successfully added " << name << " to database.\n";
        } else {
            cerr << "\n[ERROR] Could not open '" << DB_FILENAME << "' to write.\n";
            cerr << "Ensure the file isn't open in another program (like Excel/Numbers).\n";
        }
    }

    // --- FILE HANDLING: APPEND TO DAILY LOG ---
    void logMeal() {
        string search;
        cout << "Enter food name to search: ";
        getline(cin, search);
        search = toLower(search);

        // Find matches
        vector<int> matches;
        for (size_t i = 0; i < database.size(); i++) {
            if (toLower(database[i].name).find(search) != string::npos) {
                matches.push_back(i);
            }
        }

        if (matches.empty()) {
            cout << "Food not found in database. Please add it first.\n";
            return;
        }

        cout << "Found the following:\n";
        for (size_t i = 0; i < matches.size(); i++) {
            cout << i + 1 << ". " << database[matches[i]].name << "\n";
        }

        int choice;
        cout << "Select item (or 0 to cancel): ";
        cin >> choice;

        if (choice > 0 && choice <= matches.size()) {
            float servings;
            cout << "Enter number of servings (e.g., 1.5): ";
            cin >> servings;

            FoodItem selected = database[matches[choice - 1]];
            int totalCal = selected.calories * servings;
            int totalPro = selected.protein * servings;

            // Write to today's log file
            string logFilename = "log_" + getTodayDateString() + ".txt";
            ofstream logFile(logFilename, ios::app);
            if (logFile) {
                logFile << selected.name << " (" << servings << "x)," << totalCal << "," << totalPro << "\n";
                cout << "Logged: " << selected.name << " | +" << totalCal << " kcal | +" << totalPro << "g protein\n";
            } else {
                cerr << "Error opening log file.\n";
            }
        }
    }

    // --- FILE HANDLING: READ DAILY LOG ---
    void viewDailyLog() {
        string logFilename = "log_" + getTodayDateString() + ".txt";
        ifstream logFile(logFilename);

        if (!logFile) {
            cout << "\nNo meals logged yet for today (" << getTodayDateString() << ").\n";
            return;
        }

        cout << "\n=== DAILY NUTRITION LOG (" << getTodayDateString() << ") ===\n";
        cout << left << setw(30) << "Food Item" << setw(15) << "Calories" << "Protein (g)\n";
        cout << string(55, '-') << "\n";

        string line;
        int dailyCalories = 0;
        int dailyProtein = 0;

        while (getline(logFile, line)) {
            stringstream ss(line);
            string name, calStr, proStr;
            
            getline(ss, name, ',');
            getline(ss, calStr, ',');
            getline(ss, proStr, ',');

            int cal = stoi(calStr);
            int pro = stoi(proStr);

            cout << left << setw(30) << name << setw(15) << cal << pro << "\n";
            
            dailyCalories += cal;
            dailyProtein += pro;
        }

        cout << string(55, '=') << "\n";
        cout << left << setw(30) << "TOTALS:" << setw(15) << dailyCalories << dailyProtein << "g\n\n";
    }
};

int main() {
    MacroLogger logger;
    logger.initializeDatabase();
    int choice;

    do {
        cout << "\n--- NUTRITIONAL MACRO LOGGER ---\n";
        cout << "1. Log a meal\n";
        cout << "2. View today's total macros\n";
        cout << "3. Add new food to database\n";
        cout << "4. Exit\n";
        cout << "Choose an option: ";
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(); // Clear newline from buffer

        switch (choice) {
            case 1:
                logger.logMeal();
                break;
            case 2:
                logger.viewDailyLog();
                break;
            case 3:
                logger.addNewFood();
                break;
            case 4:
                cout << "Keep pushing your limits. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}

//OUTPUT:
//--- NUTRITIONAL MACRO LOGGER ---
//1. Log a meal
//2. View today's total macros
//3. Add new food to database
//4. Exit
//Choose an option:
