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
const int INVALID_DATA = -1;
const int FAIL_TO_OPEN_FILE = 1;
const int STUDENT_NOT_FOUND = 2;
const int FAIL_TO_CONSTRUCT_SUBJECT_VECTOR = 3;
const int FAIL_TO_CONSTRUCT_STUDENT_VECTOR = 4;

//grade constants
const int LOWEST_GRADE = 2;
const int HIGHEST_GRADE = 6;

//miscellaneous
const int LINES_IN_FILE_PER_STUDENT = 5;

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

//effectively prints all the students of a group, aka the students from a file
void printStudentVector(vector<student> group)
{
    //not necessarily a mistake - a group can have no students yet
    if (group.empty())
        return;

    int groupSize = group.size();
    for (int i = 0; i < groupSize; i++)
    {
        printStudent(group[i]);
        cout << endl;
    }
}

//possible optimization - pass result by reference and return an int code message
char convertFromIntToChar(int grade)
{
    if (grade < LOWEST_GRADE || grade > HIGHEST_GRADE)
        return '\0'; //error
    char result = grade + '0';
    return result;
}

int convertFromCharToInt(char ch)
{
    if (ch < '0' || ch > '9')
        return INVALID_DATA;
    int result = ch - '0';
    return result;
}

bool isLetter(char ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool isNumber(char ch)
{
    return ch >= '0' && ch <= '9';
}

//--possible optimization - pass result by reference and return an int code message--
//converts a student's vector of subjects into a single string that will be written in the group file
string constructStringFromSubjectsVector(string result, vector<subject> subjects)
{
    result = "";
    //if subjects is empty return an error message
    int subjectsSize = subjects.size();

    //adds an extra space if the entry isn't the last
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

//counts how many lines are in a file
int countLinesInFile(string filePath)
{
    if (filePath == "")
        return INVALID_DATA;
    ifstream toRead(filePath);
    if (!toRead.is_open())
    {
        return FAIL_TO_OPEN_FILE;
    }

    int result = 0;
    char tempString[2045]; //just to move the file along
    while (!toRead.eof())
    {
        toRead.getline(tempString, 2045);
        result++;
    }

    toRead.close();

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

//takes a string with info about a student's subjects and converts it into a vector<subject>
int constructSubjectsVector(string subjectsString, vector<subject>& subjects)
{
    if (subjectsString == "")
    {
        return INVALID_DATA;
    }

    int subjectsStringSize = subjectsString.length();
    subject tempSubject;

    for (int i = 0; i < subjectsStringSize; i++)
    {
        //if current char is a letter 
        //or it's a space and the next char is a letter and the previous char is not a number
        //then the current char is part of the current subject name
        if (isLetter(subjectsString[i]) || (subjectsString[i] == ' ' && isLetter(subjectsString[i + 1]) 
            && !isNumber(subjectsString[i - 1])))
        {
            tempSubject.subjectName += subjectsString[i];
        }
        //if the current char is a space and the next is a number, then the next is the current grade
        //afterwards a new subject starts or the string ends
        //so the current subject is completed and is added to the vector
        else if (subjectsString[i] == ' ' && isNumber(subjectsString[i + 1]))
        {
            tempSubject.grade = convertFromCharToInt(subjectsString[i + 1]);
            subjects.push_back(tempSubject);
            tempSubject.subjectName = "";
            tempSubject.grade = 0;
            i++;
        }
        else if (subjectsString[i] == '\0')
        {
            break;
        }
           
    }
    return SUCCESS;
}

int constructStudentsVector(vector<student>& group, string filePath)
{
    //calculates how many iterations are necessary for the file to be read
    //functionally equal to the number of students in a file
    int linesInFile = countLinesInFile(filePath);
    linesInFile--; //there is always one extra line in the file as every new student ends their info with a new line
    if (linesInFile % LINES_IN_FILE_PER_STUDENT != 0)
        return INVALID_DATA;

    int iterationsNecessary = linesInFile / LINES_IN_FILE_PER_STUDENT;

    //in case of file not opening
    ifstream toRead(filePath);
    if (!toRead.is_open())
    {
        return FAIL_TO_OPEN_FILE;
    }

    student tempStudent;
    for (int i = 0; i < iterationsNecessary; i++)
    {
        if (toRead.eof())
            break;

        //fills in tempStudent with information from file
        getline(toRead, tempStudent.firstName);
        getline(toRead, tempStudent.middleName);
        getline(toRead, tempStudent.lastName);
        getline(toRead, tempStudent.falcultyNumber);

        //all the subjects of a student are stored in a single string
        //the info from it is written in a vector<subject> of struct student
        string subjectsString = "";
        getline(toRead, subjectsString);
        tempStudent.subjects.clear(); //removes info from previous students
        int subjectsResult = constructSubjectsVector(subjectsString, tempStudent.subjects);
        if (subjectsResult != SUCCESS)
        {
            return FAIL_TO_CONSTRUCT_SUBJECT_VECTOR;
        }
        group.push_back(tempStudent);
    }

    toRead.close();

    return SUCCESS;
}

int main()
{
    //for testing purposes only - will be deleted in the completed project
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

    //printStudent(s1);

   // addStudent(s1, PATH_TO_FILE_1);

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

    //printStudent(s2);
    //addStudent(s2, PATH_TO_FILE_1);

    vector<student> group1;
    cout << "constructStudentsVector = " << constructStudentsVector(group1, PATH_TO_FILE_1);
    printStudentVector(group1);
    cout << "Size of group 1 = " << group1.size() << endl;


    return SUCCESS;

}
