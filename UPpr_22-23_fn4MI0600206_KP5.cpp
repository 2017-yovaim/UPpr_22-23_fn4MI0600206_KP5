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
const int FAIL_TO_OPEN_FILE = -2;
const int STUDENT_NOT_FOUND = -3;
const int FAIL_TO_CONSTRUCT_SUBJECT_VECTOR = -4;
const int FAIL_TO_CONSTRUCT_STUDENT_VECTOR = -5;

//grade constants
const int LOWEST_GRADE = 2;
const int HIGHEST_GRADE = 6;

//group constants
const int FIRST_GROUP = 1;
const int LAST_GROUP = 8;

//command constants
const int END = 0;
const int ADD_STUDENT = 1;
const int DELETE_STUDENT = 2;
const int PRINT = 3;
const int SORT = 4;
const int ASCENDING_FACULTY_NUMBER = 5;
const int DESCENDING_FACULTY_NUMBER = 6;
const int ASCENDING_AVERAGE_GRADE = 7;
const int DESCENDING_AVERAGE_GRADE = 8;


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
    if (linesInFile == 0)
    {
        return SUCCESS; //not a mistake - no info in file is allowed, vector will remain empty
    }
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

//rewrites the info from group into the file
int updateFile(vector<student> group, string filePath)
{
    if (filePath == "")
        return INVALID_DATA;

    //delete all contents from the file by opening it in truncate mode
    ofstream deleteFileContents(filePath, ios::trunc);
    if (!deleteFileContents.is_open())
    {
        return FAIL_TO_OPEN_FILE;
    }
    deleteFileContents.close();

    //write each student in the file anew using addStudent
    int groupSize = group.size();
    for (int i = 0; i < groupSize; i++)
    {
        addStudent(group[i], filePath);
    }

}

int deleteStudent(string facultyNumber, vector<student>& group, string filePath)
{
    if (facultyNumber == "" || filePath == "" || group.empty())
        return INVALID_DATA;

    //look for the student
    int groupSize = group.size();
    int toDeleteIndex = INVALID_DATA;

    for (int i = 0; i < groupSize; i++)
    {
        if (facultyNumber.compare(group[i].falcultyNumber) == 0)
            toDeleteIndex = i;
    }

    if (toDeleteIndex == INVALID_DATA)
        return STUDENT_NOT_FOUND;

    //delete student from vector
    group.erase(group.begin() + toDeleteIndex);

    //write new vector info into the file
    updateFile(group, filePath);

    return SUCCESS;
}

void swapStudents(student& s1, student& s2)
{
    student temp = s1;
    s1 = s2;
    s2 = temp;
}

//sorts the vector in descending order by faculty number
int sortInDescendingOrderByFacultyNumber(vector<student>& group)
{
    if (group.empty())
        return INVALID_DATA;

    //sort the vector
    int groupSize = group.size();
    for (int i = 0; i < groupSize - 1; i++)
    {
        for (int j = 0; j < groupSize - i - 1; j++)
        {
            if (group[j].falcultyNumber.compare(group[j + 1].falcultyNumber) < 0)
            {
                swapStudents(group[j], group[j + 1]);
            }
        }
    }

    return SUCCESS;
}

//sorts the vector in ascending order by faculty number
int sortInAscendingOrderByFacultyNumber(vector<student>& group)
{
    if (group.empty())
    {
        return INVALID_DATA;
    }

    //sort the vector
    int groupSize = group.size();
    for (int i = 0; i < groupSize - 1; i++)
    {
        for (int j = 0; j < groupSize - i - 1; j++)
        {
            if (group[j].falcultyNumber.compare(group[j + 1].falcultyNumber) > 0)
            {
                swapStudents(group[j], group[j + 1]);
            }
        }
    }

    return SUCCESS;
}

//calculates a student's average grade
double calculateAverageGrade(student s)
{
    double numberOfSubjects = s.subjects.size();
    if (numberOfSubjects < 1)
        return INVALID_DATA;

    double sum = 0;
    double average = 0;

    for (int i = 0; i < numberOfSubjects; i++)
    {
        sum += (double)s.subjects[i].grade;
    }

    average = sum / numberOfSubjects;
    return average;
}

//sorts the vector in descending order by average grade
int sortInDescendingOrderByAverageGrade(vector<student>& group)
{
    if (group.empty())
        return INVALID_DATA;

    int groupSize = group.size();

    //sort the vector
    for (int i = 0; i < groupSize - 1; i++)
    {
        for (int j = 0; j < groupSize - i - 1; j++)
        {
            if (calculateAverageGrade(group[j]) < calculateAverageGrade(group[j + 1]))
            {
                swapStudents(group[j], group[j + 1]);
            }
        }
    }

    return SUCCESS;
}

//sorts the vector in ascending order by average grade
int sortInAscendingOrderByAverageGrade(vector<student>& group)
{
    if (group.empty())
        return INVALID_DATA;

    int groupSize = group.size();

    //sort the vector
    for (int i = 0; i < groupSize - 1; i++)
    {
        for (int j = 0; j < groupSize - i - 1; j++)
        {
            if (calculateAverageGrade(group[j]) > calculateAverageGrade(group[j + 1]))
            {
                swapStudents(group[j], group[j + 1]);
            }
        }
    }

    return SUCCESS;
}

//control function - calls the appropriate sort function based on user input
//and then updates the file info
int sort(vector<student>& group, string filePath)
{
    if (group.empty() || filePath == "")
        return INVALID_DATA;

    cout << "Choose your sort. Please enter " 
        << endl << ASCENDING_FACULTY_NUMBER << " for ascending order by faculty number, "
        << endl << DESCENDING_FACULTY_NUMBER << " for descending order by faculty number, "
        << endl << ASCENDING_AVERAGE_GRADE << " for ascending order by average grade, or "
        << endl << DESCENDING_AVERAGE_GRADE << " for descending order by average grade.";

    int action;
    cin >> action;

    while (action != ASCENDING_AVERAGE_GRADE && action != DESCENDING_AVERAGE_GRADE && action != DESCENDING_FACULTY_NUMBER
        && action != ASCENDING_FACULTY_NUMBER)
    {
        cout << "This is not a valid sorting command. Please enter it again: " << endl;
        cin >> action;
    }

    int result;

    switch (action)
    {
    case ASCENDING_FACULTY_NUMBER:
        result = sortInAscendingOrderByFacultyNumber(group);
        break;
    case DESCENDING_FACULTY_NUMBER:
        result = sortInDescendingOrderByFacultyNumber(group);
        break;
    case ASCENDING_AVERAGE_GRADE:
        result = sortInAscendingOrderByAverageGrade(group);
        break;
    case DESCENDING_AVERAGE_GRADE:
        result = sortInDescendingOrderByAverageGrade(group);
        break;
    default:
        result = INVALID_DATA;
        break;
    }

    //do not update file if faulty result
    if (result != SUCCESS)
        return result;

    updateFile(group, filePath);
    return result;
}

//control function - creates a new student from user input and writes it in the appropriate group vector
//then calls addStudent to the appropriate file
int addStudentMenu(vector<student>& group1, vector<student>& group2, vector<student>& group3, vector<student>& group4,
    vector<student>& group5, vector<student>& group6, vector<student>& group7, vector<student>& group8)
{
    student s;
    subject tempSubject;
    cout << "Please enter the student's first name: " << endl;
    getline(cin, s.firstName);
    cout << "Please enter the student's middle name: " << endl;
    getline(cin, s.middleName);
    cout << "Please enter the student's last name: " << endl;
    getline(cin, s.lastName);
    cout << "Please enter the student's faculty number: " << endl;
    getline(cin, s.falcultyNumber);

    cout << "Please enter subjects or enter 0 to stop inputting subjects." << endl;
    do 
    {
        cout << "Please enter your subject's name: " << endl;
        getline(cin, tempSubject.subjectName);
        if (tempSubject.subjectName.compare("") == 0)
        {
            cout << "That is not a valid subject name." << endl;
            continue;
        }
        if (tempSubject.subjectName.compare("0") == 0)
            break;
        cout << "Please enter your subject's grade: " << endl;
        cin >> tempSubject.grade;
        cin.ignore();
        if (tempSubject.grade < LOWEST_GRADE || tempSubject.grade > HIGHEST_GRADE)
        {
            cout << "That is not a valid subject grade." << endl;
            continue;
        }
        s.subjects.push_back(tempSubject);
    } while (tempSubject.subjectName.compare("0") != 0);

    int group;
    cout << "Please enter the number of the group in which you want to add the student: " << endl;
    cin >> group;

    while (group < 1 || group > 8)
    {
        cout << "That's not a valid group. Please enter again: " << endl;
        cin >> group;
    }

    int result = INVALID_DATA;

    switch (group)
    {
    case 1:
        group1.push_back(s);
        result = addStudent(group1.back(), PATH_TO_FILE_1);
        break;
    case 2:
        group2.push_back(s);
        result = addStudent(group2.back(), PATH_TO_FILE_2);
        break;
    case 3:
        group3.push_back(s);
        result = addStudent(group3.back(), PATH_TO_FILE_3);
        break;
    case 4:
        group4.push_back(s);
        result = addStudent(group4.back(), PATH_TO_FILE_4);
        break;
    case 5: 
        group5.push_back(s);
        result = addStudent(group5.back(), PATH_TO_FILE_5);
        break;
    case 6:
        group6.push_back(s);
        result = addStudent(group6.back(), PATH_TO_FILE_6);
        break;
    case 7:
        group7.push_back(s);
        result = addStudent(group7.back(), PATH_TO_FILE_7);
        break;
    case 8: 
        group8.push_back(s);
        result = addStudent(group8.back(), PATH_TO_FILE_8);
        break;
    default:
        result = INVALID_DATA;
        break;
    }

    return result;


}

//control function - asks the user for a group and calls the printStudentVector function for the group
int printMenu(vector<student> group1, vector<student> group2, vector<student> group3, vector<student> group4,
    vector<student> group5, vector<student> group6, vector<student> group7, vector<student> group8)
{
    cout << "Which group would you like to print on the console? Please enter its number: " << endl;
    int group;
    cin >> group;
    while (group < FIRST_GROUP || group > LAST_GROUP)
    {
        cout << "This is not a valid group number. Please enter it again: " << endl;
        cin >> group;
    }

    switch (group)
    {
    case 1:
        printStudentVector(group1);
        break;
    case 2:
        printStudentVector(group2);
        break;
    case 3:
        printStudentVector(group3);
        break;
    case 4:
        printStudentVector(group4);
        break;
    case 5:
        printStudentVector(group5);
        break;
    case 6:
        printStudentVector(group6);
        break;
    case 7:
        printStudentVector(group7);
        break;
    case 8:
        printStudentVector(group8);
        break;
    default:
        return INVALID_DATA;
        break;
    }

    return SUCCESS;
}

//control function - calls the sort function for a specific group inputted by the user
int sortMenu(vector<student>& group1, vector<student>& group2, vector<student>& group3, vector<student>& group4,
    vector<student>& group5, vector<student>& group6, vector<student>& group7, vector<student>& group8)
{
    cout << "Which group would you like to sort? Please enter its number: " << endl;
    int group;
    cin >> group;
    while (group < FIRST_GROUP || group > LAST_GROUP)
    {
        cout << "This is not a valid group number. Please enter it again: " << endl;
        cin >> group;
    }

    int result = INVALID_DATA;

    switch (group)
    {
    case 1:
        result = sort(group1, PATH_TO_FILE_1);
        break;
    case 2:
        result = sort(group2, PATH_TO_FILE_2);
        break;
    case 3:
        result = sort(group3, PATH_TO_FILE_3);
        break;
    case 4:
        result = sort(group4, PATH_TO_FILE_4);
        break;
    case 5:
        result = sort(group5, PATH_TO_FILE_5);
        break;
    case 6:
        sort(group6, PATH_TO_FILE_6);
        break;
    case 7:
        result = sort(group7, PATH_TO_FILE_7);
        break;
    case 8:
        result = sort(group8, PATH_TO_FILE_8);
        break;
    default:
        result = INVALID_DATA;
        break;
    }

    return result;
}

//control function - calls the deleteStudent function for a specific faculty number and group entered by the user
int deleteMenu(vector<student>& group1, vector<student>& group2, vector<student>& group3, vector<student>& group4,
    vector<student>& group5, vector<student>& group6, vector<student>& group7, vector<student>& group8)
{
    cout << "Please enter the faculty number of the student you'd like to delete: " << endl;
    string facultyNumber;
    getline(cin, facultyNumber);
    while (facultyNumber.compare("") == 0)
    {
        cout << "This is not a valid faculty number. Please enter it again: " << endl;
        getline(cin, facultyNumber);
    }

    cout << "Please enter the group of the student: " << endl;
    int group;
    cin >> group;
    while (group < FIRST_GROUP || group > LAST_GROUP)
    {
        cout << "This is not a valid group number. Please enter it again: " << endl;
        cin >> group;
    }

    int result = INVALID_DATA;

    switch (group)
    {
    case 1:
        result = deleteStudent(facultyNumber, group1, PATH_TO_FILE_1);
        break;
    case 2:
        result = deleteStudent(facultyNumber, group2, PATH_TO_FILE_2);
        break;
    case 3:
        result = deleteStudent(facultyNumber, group3, PATH_TO_FILE_3);
        break;
    case 4:
        result = deleteStudent(facultyNumber, group4, PATH_TO_FILE_4);
        break;
    case 5:
        result = deleteStudent(facultyNumber, group5, PATH_TO_FILE_5);
        break;
    case 6:
        result = deleteStudent(facultyNumber, group6, PATH_TO_FILE_6);
        break;
    case 7:
        result = deleteStudent(facultyNumber, group7, PATH_TO_FILE_7);
        break;
    case 8:
        result = deleteStudent(facultyNumber, group8, PATH_TO_FILE_8);
        break;
    default:
        result = INVALID_DATA;
        break;
    }

    return result;
}


//control function - calls to the appropriate action based on user input
int menu(vector<student>& group1, vector<student>& group2, vector<student>& group3, vector<student>& group4,
    vector<student>& group5, vector<student>& group6, vector<student>& group7, vector<student>& group8)
{
    cout << "Welcome to student information! Choose what you would like to do:" << endl;
    int action;
    do
    {
        cout << "Please enter " << ADD_STUDENT << " to add a new student, " 
            << endl << DELETE_STUDENT << " to delete a student, "
            << endl << PRINT << " to print a group of students on the console, " 
            << endl << SORT << " to sort a group of students, or "
            << endl << END << " to end the programme." << endl;
        cin >> action;
        if (action == END)
            break;

        int result = INVALID_DATA;
        cin.ignore();
        
        switch (action)
        {
        case ADD_STUDENT:
            result = addStudentMenu(group1, group2, group3, group4, group5, group6, group7, group8);
            break;
        case DELETE_STUDENT:
            result = deleteMenu(group1, group2, group3, group4, group5, group6, group7, group8);
            break;
        case SORT:
            result = sortMenu(group1, group2, group3, group4, group5, group6, group7, group8);
            break;
        case PRINT:
            result = printMenu(group1, group2, group3, group4, group5, group6, group7, group8);
            break;
        default:
            cout << "Invalid action" << endl;
            break;
        }

        if (result == SUCCESS)
        {
            cout << "Task completed successfully." << endl;
        }
        else
        {
            cout << "The programme has encountered a problem." << endl;
        }

        result = INVALID_DATA;
    } while (action != END);

    cout << "Thank you for using this programme! Have a good day!" << endl;
    return SUCCESS;
}



int main()
{
    //startup
    vector<student> group1;
    vector<student> group2;
    vector<student> group3;
    vector<student> group4;
    vector<student> group5;
    vector<student> group6;
    vector<student> group7;
    vector<student> group8;

    constructStudentsVector(group1, PATH_TO_FILE_1);
    constructStudentsVector(group2, PATH_TO_FILE_2);
    constructStudentsVector(group3, PATH_TO_FILE_3);
    constructStudentsVector(group4, PATH_TO_FILE_4);
    constructStudentsVector(group5, PATH_TO_FILE_5);
    constructStudentsVector(group6, PATH_TO_FILE_6);
    constructStudentsVector(group7, PATH_TO_FILE_7);
    constructStudentsVector(group8, PATH_TO_FILE_8);

    menu(group1, group2, group3, group4, group5, group6, group7, group8);

    return SUCCESS;
}
