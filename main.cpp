#include "PainFluxDB/DB.hpp"
#include "PainFluxDB/Row.hpp"
#include "PainFluxDB/facade.hpp"
#include "PainFluxDB/queue.hpp"
#include "PainFluxDB/search.hpp"
#include "PainFluxDB/sort.hpp"
#include "PainFluxDB/stack.hpp"
#include "PainFluxDB/stats.hpp"
#include "PainFluxDB/table.hpp"
#include "PainFluxDB/utility.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// ==========================================
// INPUT HELPERS (Prevents getline skipping)
// ==========================================
int getIntInput() {
  string line;
  while (true) {
    getline(cin, line);
    try {
      return stoi(line);
    } catch (...) {
      cout << "Invalid input. Enter a whole number: ";
    }
  }
}

double getDoubleInput() {
  string line;
  while (true) {
    getline(cin, line);
    try {
      return stod(line);
    } catch (...) {
      cout << "Invalid input. Enter a decimal number: ";
    }
  }
}

// ==========================================
// SEED SAMPLE DATA (Optional helper)
// ==========================================
/* void seedSampleStudents(Node *students) {
  struct Seed {
    const char *id;
    const char *first;
    const char *last;
    double gpa;
  };

  Seed samples[] = {{"2026-001", "Juan", "Dela Cruz", 1.75},
                    {"2026-002", "Ana", "Reyes", 1.50},
                    {"2026-003", "Pedro", "Cruz", 2.00},
                    {"2026-005", "Maria", "Santos", 1.25}};

  for (const auto &s : samples) {
    RowBuilder b(students);
    b.setString("ID", s.id);
    b.setString("FIRST", s.first);
    b.setString("LAST", s.last);
    b.setDouble("GPA", s.gpa);
    b.commit();
  }
}
*/

// ==========================================
// 1. ADD STUDENT (RowBuilder::commit)
// ==========================================
void handleAddStudent(Node *students) {
  cout << "\n--- ADD STUDENT ---\n";
  cout << "Enter Student ID (e.g., 2026-006): ";
  string id;
  getline(cin, id);

  if (FindRowStr(students, "ID", id) != nullptr) {
    cout << "Error: Student ID '" << id << "' already exists.\n";
    return;
  }

  cout << "Enter First Name: ";
  string first;
  getline(cin, first);

  cout << "Enter Last Name: ";
  string last;
  getline(cin, last);

  cout << "Enter GPA: ";
  double gpa = getDoubleInput();

  RowBuilder b(students);
  b.setString("ID", id);
  b.setString("FIRST", first);
  b.setString("LAST", last);
  b.setDouble("GPA", gpa);

  if (b.commit()) {
    cout << "Student added successfully!\n";
  } else {
    cout << "Failed to insert student record.\n";
  }
}

// ==========================================
// 2. DISPLAY ALL STUDENTS
// ==========================================
void handleDisplayAll(Node *students) {
  cout << "\n--- ALL STUDENTS ---\n";
  if (IsEmpty(students)) {
    cout << "(No student records found)\n";
    return;
  }
  DisplayAllRows(students);
}

// ==========================================
// 3. SEARCH STUDENT (Linear & Binary Search)
// ==========================================
void handleSearchStudent(Node *students) {
  if (IsEmpty(students)) {
    cout << "\nStudent table is empty.\n";
    return;
  }

  cout << "\n--- SEARCH STUDENT ---\n";
  cout << "1. Linear Search (O(n) - scans sequentially)\n";
  cout << "2. Binary Search (O(log n) - uses sorted snapshot)\n";
  cout << "Choose algorithm: ";
  int algo = getIntInput();

  if (algo != 1 && algo != 2) {
    cout << "Invalid search option.\n";
    return;
  }

  cout << "\nSearch by:\n";
  cout << "1. Student ID\n";
  cout << "2. Last Name\n";
  cout << "Choose field: ";
  int field = getIntInput();

  string colName;
  if (field == 1) {
    colName = "ID";
    cout << "Enter Student ID: ";
  } else if (field == 2) {
    colName = "LAST";
    cout << "Enter Last Name: ";
  } else {
    cout << "Invalid field.\n";
    return;
  }

  string query;
  getline(cin, query);

  Row *matches[MaxRowReturn];
  int totalHits = 0;

  if (algo == 1) {
    totalHits = LinearSearch(students, colName, makeString(query), matches,
                             MaxRowReturn);
    cout << "\n[Linear Search Results]\n";
  } else {
    totalHits = BinarySearch(students, colName, makeString(query), matches,
                             MaxRowReturn);
    cout << "\n[Binary Search Results]\n";
  }

  if (totalHits == 0) {
    cout << "No matching student found.\n";
  } else {
    int shown = min(totalHits, MaxRowReturn);
    DisplayRows(students, matches, shown);
    cout << "Total matches: " << totalHits << "\n";
  }
}

// ==========================================
// 4. UPDATE STUDENT (RowBuilder::update)
// ==========================================
void handleUpdateStudent(Node *students) {
  cout << "\n--- UPDATE STUDENT ---\n";
  cout << "Enter Student ID to update: ";
  string id;
  getline(cin, id);

  Row *target = FindRowStr(students, "ID", id);
  if (target == nullptr) {
    cout << "Student ID '" << id << "' not found.\n";
    return;
  }

  cout << "Current Record: ";
  DisplayRow(students, target);

  cout << "Enter new First Name (leave blank to keep '"
       << CellText(students, target, "FIRST") << "'): ";
  string first;
  getline(cin, first);

  cout << "Enter new Last Name (leave blank to keep '"
       << CellText(students, target, "LAST") << "'): ";
  string last;
  getline(cin, last);

  cout << "Update GPA? (1 = Yes, 0 = No): ";
  int changeGpa = getIntInput();

  RowBuilder patch(students);
  if (!first.empty())
    patch.setString("FIRST", first);
  if (!last.empty())
    patch.setString("LAST", last);
  if (changeGpa == 1) {
    cout << "Enter new GPA: ";
    patch.setDouble("GPA", getDoubleInput());
  }

  if (patch.filledCount() == 0) {
    cout << "No changes made.\n";
    return;
  }

  if (patch.update(target)) {
    cout << "Student record updated successfully!\n";
  } else {
    cout << "Failed to update student record.\n";
  }
}

// ==========================================
// 5. DELETE STUDENT (Auto-pushes to Stack)
// ==========================================
void printDeletedStack(Node *students) {
  Row *stackRows[MaxRowReturn];
  int count = DisplayStack(students, stackRows, MaxRowReturn);

  cout << "\nSTACK:\n";
  if (count == 0) {
    cout << "(Empty)\n";
    return;
  }
  cout << "Top\n";
  cout << " ↓\n";
  for (int i = 0; i < count; i++) {
    cout << CellText(students, stackRows[i], "ID") << " "
         << CellText(students, stackRows[i], "FIRST") << " "
         << CellText(students, stackRows[i], "LAST") << "\n";
  }
}

void handleDeleteStudent(Node *students) {
  cout << "\nEnter Student ID to delete: ";
  string id;
  getline(cin, id);

  Row *target = FindRowStr(students, "ID", id);
  if (target == nullptr) {
    cout << "Student ID '" << id << "' not found.\n";
    return;
  }

  string fullName = CellText(students, target, "FIRST") + " " +
                    CellText(students, target, "LAST");

  // DeleteRowStr automatically unlinks the row and pushes it onto the Deleted
  // Stack
  if (DeleteRowStr(students, "ID", id)) {
    cout << "\nDELETE STUDENT\n";
    cout << "------------------------------\n";
    cout << "Student: " << id << " - " << fullName << "\n";
    cout << "\nStudent added to Deleted Stack.\n";
    printDeletedStack(students);
  } else {
    cout << "Failed to delete student.\n";
  }
}

// ==========================================
// 6. SORT STUDENTS (Bubble, Selection, Insertion)
// ==========================================
void handleSortStudents(Node *students) {
  if (IsEmpty(students)) {
    cout << "\nStudent table is empty.\n";
    return;
  }

  cout << "\n--- SORT STUDENTS ---\n";
  cout << "Sort by:\n";
  cout << "1. Student ID\n";
  cout << "2. Last Name\n";
  cout << "3. GPA\n";
  cout << "Choose field: ";
  int field = getIntInput();

  string colName;
  if (field == 1)
    colName = "ID";
  else if (field == 2)
    colName = "LAST";
  else if (field == 3)
    colName = "GPA";
  else {
    cout << "Invalid field.\n";
    return;
  }

  cout << "\nSorting Algorithm:\n";
  cout << "1. Bubble Sort\n";
  cout << "2. Selection Sort\n";
  cout << "3. Insertion Sort\n";
  cout << "Choose algorithm: ";
  int algoChoice = getIntInput();

  SortAlgorithm algo;
  if (algoChoice == 1)
    algo = BUBBLE;
  else if (algoChoice == 2)
    algo = SELECTION;
  else if (algoChoice == 3)
    algo = INSERTION;
  else {
    cout << "Invalid algorithm.\n";
    return;
  }

  cout << "\nOrder (1 = Ascending, 2 = Descending): ";
  int orderChoice = getIntInput();
  bool ascending = (orderChoice != 2);

  if (SortRows(students, colName, algo, ascending)) {
    cout << "\nStudents sorted by " << colName << ":\n";
    DisplayAllRows(students);
  } else {
    cout << "Sort failed (table may exceed MaxRowReturn).\n";
  }
}

// ==========================================
// 7. STACK OPERATIONS (Recently Deleted - LIFO)
// ==========================================
void handleStackOperations(Node *students) {
  int choice;
  do {
    cout << "\n--- STACK OPERATIONS (RECENTLY DELETED) ---\n";
    cout << "1. Push (Delete a Student into Stack)\n";
    cout << "2. Pop (Permanently Remove Top Deleted Student)\n";
    cout << "3. Restore (Undo Last Delete back to Table)\n";
    cout << "4. Peek (View Top Deleted Student)\n";
    cout << "5. Display Stack\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter choice: ";
    choice = getIntInput();

    switch (choice) {
    case 1:
      // Per engine design, DeleteRow pushes the row onto the stack
      handleDeleteStudent(students);
      break;

    case 2: {
      // Ownership rule: caller owns the Row* returned by PopDeleted() and must
      // delete it
      Row *popped = PopDeleted(students);
      if (popped == nullptr) {
        cout << "Stack Underflow! Deleted stack is empty.\n";
      } else {
        cout << "Popped & permanently discarded: "
             << CellText(students, popped, "ID") << " - "
             << CellText(students, popped, "FIRST") << " "
             << CellText(students, popped, "LAST") << "\n";
        delete popped;
      }
      break;
    }

    case 3:
      if (RestoreDeleted(students)) {
        cout << "Most recently deleted student restored to active table!\n";
      } else {
        cout << "Deleted stack is empty; nothing to restore.\n";
      }
      break;

    case 4: {
      Row *top = PeekDeleted(students);
      if (top == nullptr) {
        cout << "Deleted stack is empty.\n";
      } else {
        cout << "Top of Stack: " << CellText(students, top, "ID") << " - "
             << CellText(students, top, "FIRST") << " "
             << CellText(students, top, "LAST") << "\n";
      }
      break;
    }

    case 5:
      printDeletedStack(students);
      break;

    case 0:
      break;

    default:
      cout << "Invalid choice.\n";
    }
  } while (choice != 0);
}

// ==========================================
// 8. QUEUE OPERATIONS (Student Service - FIFO)
// ==========================================
void printServiceQueue(Node *students) {
  Row *qRows[MaxRowReturn];
  int count = DisplayQueue(students, qRows, MaxRowReturn);

  cout << "\nSTUDENT SERVICE QUEUE\n";
  cout << "------------------------------\n";
  if (count == 0) {
    cout << "(No students waiting in queue)\n";
    return;
  }
  for (int i = 0; i < count; i++) {
    cout << (i + 1) << ". " << CellText(students, qRows[i], "FIRST") << " "
         << CellText(students, qRows[i], "LAST") << "\n";
  }
}

void handleQueueOperations(Node *students) {
  int choice;
  do {
    cout << "\n--- QUEUE OPERATIONS (STUDENT SERVICE) ---\n";
    cout << "1. Enqueue (Add Student to Service Queue)\n";
    cout << "2. Dequeue (Serve Next Student)\n";
    cout << "3. Peek (View Front Student)\n";
    cout << "4. Display Queue\n";
    cout << "0. Back to Main Menu\n";
    cout << "Enter choice: ";
    choice = getIntInput();

    switch (choice) {
    case 1: {
      cout << "Enter Student ID to enqueue: ";
      string id;
      getline(cin, id);

      Row *target = FindRowStr(students, "ID", id);
      if (target == nullptr) {
        cout << "Student ID '" << id << "' not found in active table.\n";
      } else if (Enqueue(students, target)) {
        cout << CellText(students, target, "FIRST") << " "
             << CellText(students, target, "LAST")
             << " added to the service queue.\n";
      } else {
        cout << "Could not enqueue student.\n";
      }
      break;
    }

    case 2: {
      // Ownership rule: Queue only borrows rows; NEVER delete the dequeued Row*
      Row *served = Dequeue(students);
      if (served == nullptr) {
        cout << "Queue is empty! No students waiting.\n";
      } else {
        cout << "Now serving: " << CellText(students, served, "ID") << " - "
             << CellText(students, served, "FIRST") << " "
             << CellText(students, served, "LAST") << "\n";
      }
      break;
    }

    case 3: {
      Row *front = PeekQueue(students);
      if (front == nullptr) {
        cout << "Queue is empty!\n";
      } else {
        cout << "Next in line: " << CellText(students, front, "ID") << " - "
             << CellText(students, front, "FIRST") << " "
             << CellText(students, front, "LAST") << "\n";
      }
      break;
    }

    case 4:
      printServiceQueue(students);
      break;

    case 0:
      break;

    default:
      cout << "Invalid choice.\n";
    }
  } while (choice != 0);
}

// ==========================================
// 9. STUDENT STATISTICS (ComputeStats)
// ==========================================
void handleStatistics(Node *students) {
  cout << "\n========================================\n";
  cout << "          STUDENT STATISTICS\n";
  cout << "========================================\n";

  ColumnStats gpaStats = ComputeStats(students, "GPA");

  cout << "Total Active Students   : " << gpaStats.count << "\n";
  cout << "Recently Deleted (Stack): " << students->StackCount << "\n";
  cout << "Waiting in Service Queue: " << students->QueueCount << "\n";

  if (gpaStats.count > 0) {
    cout << fixed << setprecision(2);
    cout << "Average GPA             : " << gpaStats.avg << "\n";
    cout << "Lowest Numeric GPA      : " << gpaStats.min << "\n";
    cout << "Highest Numeric GPA     : " << gpaStats.max << "\n";
  }
}

// ==========================================
// MAIN ENTRY POINT
// ==========================================
int main() {
  // 1. Initialize Database (fetch pointer AFTER creation)
  if (!headlessCreateNewDatabase("SCHOOL")) {
    cerr << "Failed to create database.\n";
    return 1;
  }
  Database *db = returnQueryADB("SCHOOL");
  if (db == nullptr) {
    cerr << "Database pointer is null.\n";
    return 1;
  }

  // 2. Setup Table Schema via Facade
  TableSpec spec[] = {{"ID", COL_STRING},
                      {"FIRST", COL_STRING},
                      {"LAST", COL_STRING},
                      {"GPA", COL_DOUBLE}};

  if (!SetupTable(db, "STUDENTS", spec, 4)) {
    cerr << "Failed to setup STUDENTS table.\n";
    return 1;
  }

  Node *students = SearchTable(db, "STUDENTS");
  if (students == nullptr) {
    cerr << "Failed to locate STUDENTS table.\n";
    return 1;
  }

  // Load initial sample records so you can immediately test
  // search/sort/queue/stack
  // seedSampleStudents(students);

  string welcometext1 = R"(
     _       __     __                             __           __  __       
    | |     / /__  / /________  ____ ___  ___     / /_____     / /_/ /_  ___ 
    | | /| / / _ \/ / ___/ __ \/ __ `__ \/ _ \   / __/ __ \   / __/ __ \/ _ \
    | |/ |/ /  __/ / /__/ /_/ / / / / / /  __/  / /_/ /_/ /  / /_/ / / /  __/
    |__/|__/\___/_/\___/\____/_/ /_/ /_/\___/   \__/\____/   \__/_/ /_/\___/                                                                    
    )";

  string welcometext2 = R"(
      _____ _______ _    ___          __     _____ _______                              _ 
     / ____|__   __| |  | \ \        / /\   |  __ \__   __|TM                          | |
    | (___    | |  | |  | |\ \  /\  / /  \  | |__) | | |     _ __ ___  ___ ___  _ __ __| |
     \___ \   | |  | |  | | \ \/  \/ / /\ \ |  _  /  | |    | '__/ _ \/ __/ _ \| '__/ _` |
     ____) |  | |  | |__| |  \  /\  / ____ \| | \ \  | |    | | |  __/ (_| (_) | | | (_| |
    |_____/   |_|   \____/    \/  \/_/    \_\_|  \_\ |_|    |_|  \___|\___\___/|_|  \__,_|
    )";

  string welcometext3 =
      R"(                                                           __                     __                 __
       ____ ___  ____ _____  ____ _____ ____  ____ ___  ___  ____  / /_   _______  _______/ /____  ____ ___  / /
      / __ `__ \/ __ `/ __ \/ __ `/ __ `/ _ \/ __ `__ \/ _ \/ __ \/ __/  / ___/ / / / ___/ __/ _ \/ __ `__ \/ / 
     / / / / / / /_/ / / / / /_/ / /_/ /  __/ / / / / /  __/ / / / /_   (__  ) /_/ (__  ) /_/  __/ / / / / /_/  
    /_/ /_/ /_/\__,_/_/ /_/\__,_/\__, /\___/_/ /_/ /_/\___/_/ /_/\__/  /____/\__, /____/\__/\___/_/ /_/ /_(_)   
                                /____/                                      /____/                              
    )";

  int choice = 0;

  cout
      << "· · ──────────────────────────────────────────────────────────── "
         "꒰ঌ·✦·໒꒱ "
         "──────────────────────────────────────────────────────────── · ·'\n'";
  cout << welcometext1 << endl;
  cout << welcometext2 << endl;
  cout << welcometext3 << endl;
  cout << "⊰═══════════════════════════════════════════════════════════════════"
          "════════════════════════════════════════════════════════════════════"
          "══⊱ \n";
  cout << "By Group ____" << endl;
  cout << " " << endl;

  cout << "꧁𓊈𒆜MEMBERS𒆜𓊉꧂" << endl;
  cout << " " << endl;

  cout << "𓆩❤︎𓆪 Roland Sibal 𓆩❤︎𓆪" << endl;
  cout << "✮⋆˙ Nashe Ortega ˙⋆✮" << endl;
  cout << "ꨄ︎ James Daryll Valiente ꨄ︎" << endl;
  cout << "ꨄ︎ James Ivan Trumata ꨄ︎" << endl;
  cout << "ꨄ︎ Seigfreid Catindoy ꨄ︎" << endl;
  cout << "⊰═══════════════════════════════════════════════════════════════════"
          "════════════════════════════════════════════════════════════════════"
          "══⊱ \n";
  do {
    cout << "[1] Add Student Record\n";
    cout << "[2] Display All Students\n";
    cout << "[3] Search Student Record  \n";
    cout << "[4] Update Student Record\n";
    cout << "[5] Delete Student Record\n";
    cout << "[6] Sort Student Record\n";
    cout << "[7] Stack Operations\n";
    cout << "[8] Queue Operations\n";
    cout << "[9] Student Statistics\n";
    cout << "[10] Exit\n";

    cout << " " << endl;
    cout << "⤷Enter your choice [1-10]: ";
    choice = getIntInput();

    switch (choice) {
    case 1:
      handleAddStudent(students);
      break;
    case 2:
      handleDisplayAll(students);
      break;
    case 3:
      handleSearchStudent(students);
      break;
    case 4:
      handleUpdateStudent(students);
      break;
    case 5:
      handleDeleteStudent(students);
      break;
    case 6:
      handleSortStudents(students);
      break;
    case 7:
      handleStackOperations(students);
      break;
    case 8:
      handleQueueOperations(students);
      break;
    case 9:
      handleStatistics(students);
      break;
    case 10:
      cout << "⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘" << endl;
      cout
          << R"(________   _______ _______ _____ _   _  _____     _______     _______ _______ ______ __  __       _____  ____   ____  _____  ______     ________ _ 
              |  ____\ \ / /_   _|__   __|_   _| \ | |/ ____|   / ____\ \   / / ____|__   __|  ____|  \/  |     / ____|/ __ \ / __ \|  __ \|  _ \ \   / /  ____| |
              | |__   \ V /  | |    | |    | | |  \| | |  __   | (___  \ \_/ / (___    | |  | |__  | \  / |    | |  __| |  | | |  | | |  | | |_) \ \_/ /| |__  | |
              |  __|   > <   | |    | |    | | | . ` | | |_ |   \___ \  \   / \___ \   | |  |  __| | |\/| |    | | |_ | |  | | |  | | |  | |  _ < \   / |  __| | |
              | |____ / . \ _| |_   | |   _| |_| |\  | |__| |   ____) |  | |  ____) |  | |  | |____| |  | |_   | |__| | |__| | |__| | |__| | |_) | | |  | |____|_|
              |______/_/ \_\_____|  |_|  |_____|_| \_|\_____|  |_____/   |_| |_____/   |_|  |______|_|  |_(_)   \_____|\____/ \____/|_____/|____/  |_|  |______(_)
       )" << endl;
      cout << "⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘⫘" << endl;
      break;
    default:
      cout << "──────────────────────────────── ⭑ ☆ ⭑ "
              "────────────────────────────────"
           << endl;
      cout << "⚠︎ INVALID CHOICE. ⚠︎" << endl;
      cout << "【 Please enter a valid choice from 1 to 10. 】 \n";
      break;
    }
  } while (choice != 10);

  // 3. Clean up memory before exit
  headlessDeleteDatabase("SCHOOL");
  return 0;
}