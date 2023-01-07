/**
*
* Solution to course project #5
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Yova Mitova
* @idnumber 4MI0600206
* @compiler VC
*
* Main programme
*
*/


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

//paths to the files
const string PATH_TO_FILE_1 = "group1.txt";
const string PATH_TO_FILE_2 = "group2.txt";
const string PATH_TO_FILE_3 = "group3.txt";
const string PATH_TO_FILE_4 = "group4.txt";
const string PATH_TO_FILE_5 = "group5.txt";
const string PATH_TO_FILE_6 = "group6.txt";
const string PATH_TO_FILE_7 = "group7.txt";
const string PATH_TO_FILE_8 = "group8.txt";

//error codes
const int SUCCESS = 0;
const int FAIL_TO_OPEN_FILE = 1;
const int INVALID_DATA = -1;
const int STUDENT_NOT_FOUND = 2;

//grade constants
const int LOWEST_GRADE = 2;
const int HIGHEST_GRADE = 6;

struct subject
{
    string subjectName;
    int grade;
};

struct student
{
    string firstName;
    string middleName;
    string lastName;
    string falcultyNumber;
    vector<subject> subjects;
};

//prints a vector of subjects
void printSubjects(vector<subject> subjects)
{
    int subjectsSize = subjects.size();
    for (int i = 0; i < subjectsSize; i++)
    {
        cout << subjects[i].subjectName << ": " << subjects[i].grade << endl;
    }
}

//prints the data from a member of struct student
void printStudent(student s)
{
    cout << s.falcultyNumber << ": " << s.firstName << " " << s.middleName << " " << s.lastName << endl;
    printSubjects(s.subjects);
}

//possible optimization - pass result by reference and return an int code message
char convertFromIntToChar(int grade)
{
    if (grade < LOWEST_GRADE || grade > HIGHEST_GRADE)
        return '\0'; //error
    char result = grade + '0';
    return result;
}

//--possible optimization - pass result by reference and return an int code message--
//converts a student's vector of subjects into a single string that will be written in the group file
string constructStringFromSubjectsVector(string result, vector<subject> subjects)
{
    result = "";
    //if subjects is empty return an error message
    int subjectsSize = subjects.size();
    for (int i = 0; i < subjectsSize; i++)
    {
        char charGrade = convertFromIntToChar(subjects[i].grade);
        if (i == subjectsSize - 1)
            result = result + subjects[i].subjectName + " " + charGrade;
        else
            result = result + subjects[i].subjectName + " " + charGrade + " ";
    }

    return result;
}

//adds the data from s to the file with path filePath
int addStudent(student s, string filePath)
{
    ofstream toAddIn(filePath, ios::app);
    if (!toAddIn.is_open())
    {
        return FAIL_TO_OPEN_FILE;
    }

    toAddIn << s.firstName;
    toAddIn << endl;
    toAddIn << s.middleName;
    toAddIn << endl;
    toAddIn << s.lastName;
    toAddIn << endl;
    toAddIn << s.falcultyNumber;
    toAddIn << endl;

    //all the subjects of a student are converted into a single string
    string subjectsString;
    subjectsString = constructStringFromSubjectsVector(subjectsString, s.subjects);
    toAddIn << subjectsString;
    toAddIn << endl;

    toAddIn.close();
    return SUCCESS;

}

int main()
{
    //for testing purposes only - will be deleted in the complted project
    subject sub1 = { "Calculus", 4 };
    subject sub2 = { "English", 6 };
    subject sub3 = { "Linear Algebra", 5 };

    student s1;
    s1.firstName = "Nathan";
    s1.middleName = "Byrne";
    s1.lastName = "Edge";
    s1.falcultyNumber = "2MI0300200"; //sorry if it's somebody's actual falculty number = it's not intentional
    s1.subjects.push_back(sub1);
    s1.subjects.push_back(sub2);
    s1.subjects.push_back(sub3);

    printStudent(s1);

    addStudent(s1, PATH_TO_FILE_1);

    subject sub4 = { "English", 5 };
    subject sub5 = { "Discreet Structures", 6 };
    subject sub6 = { "Calculus", 5 };
    subject sub7 = { "Linear Algebra", 3 };

    student s2;
    s2.firstName = "Annalise";
    s2.middleName = "Lilly";
    s2.lastName = "O'Brien";
    s2.falcultyNumber = "2MI0300201";
    s2.subjects.push_back(sub4);
    s2.subjects.push_back(sub5);
    s2.subjects.push_back(sub6);
    s2.subjects.push_back(sub7);

    printStudent(s2);
    addStudent(s2, PATH_TO_FILE_1);



    return SUCCESS;

}
