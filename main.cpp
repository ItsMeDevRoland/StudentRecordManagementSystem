#include <iostream>
#include <string>
#include "PainFluxDB/main.hpp"
using namespace std;

struct StudentRecord {
    int id_num, gwa;
    string name;
    char display;


//Add students 
void addstudent() {
    StudentRecord a;
    cout << "Please input the Student's ID-#, Name, and GWA(General Weighted Average).'\n'";
    cout << "════════════════════════════════════════════════════════════════════════'\n'";
    cout << "Student ID-#: ";
    cin >> a.id_num;
    cout << "Student Name: ";
    cin >> a.name;
    cout << "Student GWA: ";
    cin >> a.gwa;
}

//Display students 
void displaystudent() {
    bool end_display = false;

    while (end_display == false){

    
    cout << "Do you wish to display all students in the system? (Y/N)";
    cin >> display;

    switch(display) {
        case 'Y':
            showAllDB();
            break;
        case 'N':
            end_display = true;
            break;
        default:
            cout << "Invalid input. Please type (Y/N) only.'\n'";
            continue;
    };
};
};

//Search Student
void searchstudent() {
    StudentRecord a;
    int id_search;
    cout << "Enter Student ID-# to search: ";
    cin >> id_search;

    if (a.id_num == id_search){
        cout << "ID-#: " << a.id_num << "\n";
        cout << "Student Name: " << a.name << "\n";
        cout << "GWA: " << a.gwa << "\n"; 
    return;
    }
    cout << "ID-# " << id_search << " not found in system. Please input again.\n"; 
}

//Update Student
void updatestudent() {
    StudentRecord a;
    string id_update;
    char opt;

    do {
    opt = ' ';

    cout << "Enter Student ID-# to update record: ";
    cin >> id_update;

    if (a.id_num == id_update) {
        cout << "ARE YOU SURE YOU WISH TO UPDATE THIS STUDENT RECORD? (Y/N): ";
        cin >> opt;

        switch(opt) {
        case 'Y':
            cout << "(CURRENT RECORD: " << a.name << ")\n";
            cout << "Enter student name: ";
            getline(cin, a.name);

            cout << "(CURRENT RECORD: " << a.gwa << ")\n";
            cout << "Enter new GWA (Grade Weighted Average): ";
            cin >> a.gwa;
            break;
        case 'N':
            cout << "Please enter Student ID-# again.";
            break;

        default:
            cout << "Invalid Input. Please enter input again.";
            continue;
        }
    } else {
        cout << "Student ID-# not found. Please enter again.";
        continue;
    }
    } while (opt == 'N');
}

//Delete Student 
void deletestudent(){
}

};

//Would this even be needed? I should make it better
int main() {

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
    cout << "⊰═════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════════⊱ \n";

return 0;
}