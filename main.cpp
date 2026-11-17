#include <iostream>
#include <string>
#include "PainFluxDB/DB.hpp"
using namespace std;

struct StudentRecord {
    int id_num;
    double gwa;
    string name;
};

//Add students 
void addstudent() {
    StudentRecord a;
    cout << "Please input the Student's ID-#, Name, and GWA(General Weighted Average).'\n'";
    cout << "════════════════════════════════════════════════════════════════════════'\n'";
    cout << "Student ID-#: ";
    cin >> a.id_num;

    cout << "Student Name: ";
    getline(cin >> ws, a.name);

    cout << "Student GWA: ";
    cin >> a.gwa;


    if (addstudent(a)){
        cout << "Student Record successfully added!\n";
    } else {
        cout << "ERROR: Student Record with ID-# " << a.id_num << " already exists.\n";
        }
    }


//Display students 
void displaystudent() {
    char display;
    bool end_display = false;

    while (end_display == false){

    
    cout << "Do you wish to display all students in the system? (Y/N)";
    cin >> display;

    switch(display) {
        case 'Y':
        case 'y':
            showAllDB();
            break;
        case 'N':
        case 'n':
            end_display = true;
            break;
        default:
            cout << "Invalid input. Please type (Y/N) only.'\n'";
            continue;
        };
    };
}


//Search Student
void searchstudent() {
    StudentRecord a;
    int id_search;
    cout << "Enter Student ID-# to search: ";
    cin >> id_search;

    if (searchstudent(id_search, a)){
        cout << "ID-#: " << a.id_num << "\n";
        cout << "Student Name: " << a.name << "\n";
        cout << "GWA: " << a.gwa << "\n"; 
    return;
    }
    cout << "ID-# " << id_search << " not found in system. Please input again.\n"; 
    }

//Update Student
void updatestudent(){
    StudentRecord a;
    int id_update;
    string name_update;
    double gwa_update;


    while (true){
        cout << "Enter Student ID-# to update student record (Press 0 to cancel update): ";
        cin >> id_update;

        if (id_update == 0) return;

        if (searchstudent(id_update, a)){
            cout << "(CURRENT NAME: " << a.name << ")\n";
            cout << "Enter updated student name: ";
            getline(cin >> ws, name_update);

            cout << "(CURRENT GWA: " << a.gwa << ")\n";
            cout << "Enter updated GWA (General Weighted Average): ";
            cin >> gwa_update;
        }

        if (updateDB(id_update, name_update, gwa_update)){
        if (updatestudent(id_update, name_update, gwa_update)){
            cout << "Student Record updated!\n";
        } else {
            cout << "Student record not found, please try again.\n";
                }
            }
        }
    }


//Delete Student 
void deletestudent(){
    StudentRecord a;
    int id_delete;
    char opt;

    while (true) {
        cout << "Enter Student ID-# to delete record (or 0 to cancel): ";
        cin >> id_delete;

        if (id_delete == 0) return;


        if (searchStudent(id_delete, a)){
            cout << "\nRECORD:\n";
            cout << "STUDENT ID-#: " << a.id_num << "\n";
            cout << "NAME: " << a.name << "\n";
            cout << "STUDENT GWA: " << a.gwa << "\n";
            
            cout << "ARE YOU SURE YOU WISH TO DELETE THIS RECORD? (Y/N): ";
            cin >> opt;

            switch (opt){
                case 'Y':
                case 'y':
                    if (deleteDB(id_delete)) {
                    if (deleteStudent(id_delete)) {
                        cout << "STUDENT RECORD SUCCESSFULLY DELETED.\n";
                    }
                    return;
                case 'N':
                case 'n':
                    cout << "CANCELLED.\n";
                    return;
                default:
                    cout << "Invalid Input. Please enter only Y or N.\n";
                    break;
            }
        } else {
            cout << "Student ID-# not found in database. Please enter again.\n";
                }
            }
        }
    }

//Would this even be needed? I should make it better
int main(){

    string welcometext1 = R"(
     _       __     __                            __           __  __       
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

    string welcometext3 = R"(                                       __                     __                 __
       ____ ___  ____ _____  ____ _____ ____  ____ ___  ___  ____  / /_   _______  _______/ /____  ____ ___  / /
      / __ `__ \/ __ `/ __ \/ __ `/ __ `/ _ \/ __ `__ \/ _ \/ __ \/ __/  / ___/ / / / ___/ __/ _ \/ __ `__ \/ / 
     / / / / / / /_/ / / / / /_/ / /_/ /  __/ / / / / /  __/ / / / /_   (__  ) /_/ (__  ) /_/  __/ / / / / /_/  
    /_/ /_/ /_/\__,_/_/ /_/\__,_/\__, /\___/_/ /_/ /_/\___/_/ /_/\__/  /____/\__, /____/\__/\___/_/ /_/ /_(_)   
                                /____/                                      /____/                              
    )";

    int choice = 0;

    cout << "· · ──────────────────────────────────────────────────────────── ꒰ঌ·✦·໒꒱ ──────────────────────────────────────────────────────────── · ·'\n'";
    cout << welcometext1 << endl;
    cout << welcometext2 << endl;
    cout << welcometext3 << endl;
    cout << "⊰═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════⊱ \n";
    cout << "By Group ____" << endl;
    cout << " " << endl;
    cout << "Members:";
    cout << "Roland Sibal" << endl;
    cout << "Nashe Ortega" << endl;
    cout << "James Daryll Valiente" << endl;
    cout << "James Ivan Trumata" << endl;
    cout << "Seigfreid Catindoy" << endl;
    cout << "⊰═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════⊱ \n";
    do { 
        cout << "[1] Add Student Record\n";
        cout << "[2] Display All Students\n";
        cout << "[3] Search Student Record\n";
        cout << "[4] Update Student Record\n";
        cout << "[5] Delete Student Record\n";
        cout << "[6] Sort Student Record\n";
        cout << "[7] Stack Operations\n";
        cout << "[8] Queue Operations\n";
        cout << "[9] Student Statistics\n";
        cout << "[10] Exit\n";

        cout << "Enter your choice [1-6]: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addstudent();
                break;
            case 2:
                displaystudent();
                break;
            case 3:
                searchstudent();
                break;
            case 4:
                updatestudent();
                break;
            case 5:
                deletestudent();
                break;
            case 6:
                cout << "[EXITING SYSTEM. GOODBYE!]\n";
                break;
            default:
                cout << "Invalid choice. Please enter a valid choice from 1 to 6.\n";
                break;
        }
    } while (choice != 6);
    

return 0;
}