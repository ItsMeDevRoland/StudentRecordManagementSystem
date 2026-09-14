#include <iostream>
#include <string>
#include "PainFluxDB/main.hpp"
using namespace std;
int main();

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
    }
}
}

//Search Student
void searchstudent() {
    cout << "SEARCH STUDENT: ";
    cin >> 
}

//Update Student
void updatestudent() {

}

//Delete Student 
void deletestudent(){

}

};

//Would this even be needed? I should make it better
int main() {
    
    cout << "Welcome to the STUWART™ record management system!'\n'";
    cout << "════════════════════════════════════════════════════════════════════════'\n'";

return 0;
}