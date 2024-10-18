#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>  // for numeric_limits

using namespace std;

// Course structure to hold course details
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    // Constructor
    Course(string number = "", string title = "", vector<string> prereqs = {})
        : courseNumber(number), courseTitle(title), prerequisites(prereqs) {}
};

// Node structure for the Binary Search Tree
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course course) : course(course), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class to store and manage courses
class BinarySearchTree {
private:
    Node* root;

    // In-order traversal to print courses in alphanumeric order
    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        inOrder(node->right);
    }

    // Helper function to insert a course into the tree
    Node* insert(Node* node, Course course) {
        if (node == nullptr) return new Node(course);

        if (course.courseNumber < node->course.courseNumber)
            node->left = insert(node->left, course);
        else
            node->right = insert(node->right, course);

        return node;
    }

    // Helper function to search for a course in the tree
    Node* search(Node* node, const string& courseNumber) {
        if (node == nullptr || node->course.courseNumber == courseNumber)
            return node;

        if (courseNumber < node->course.courseNumber)
            return search(node->left, courseNumber);
        else
            return search(node->right, courseNumber);
    }

public:
    // Constructor
    BinarySearchTree() : root(nullptr) {}

    // Insert a course into the tree
    void insert(Course course) {
        root = insert(root, course);
    }

    // Search for a course by course number
    Course search(const string& courseNumber) {
        Node* node = search(root, courseNumber);
        if (node == nullptr) {
            return Course();
        }
        return node->course;
    }

    // Print all courses in alphanumeric order
    void printAllCourses() {
        inOrder(root);
    }
};

// Function to load courses from file
void loadCourses(BinarySearchTree& bst, const string& fileName) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Could not open file " << fileName << endl;
        return;
    }

    string line;
    int lineCount = 0;  // Track lines to catch issues
    while (getline(file, line)) {
        lineCount++;
        stringstream ss(line);
        string courseNumber, courseTitle, prereq;
        vector<string> prerequisites;

        if (!getline(ss, courseNumber, ',')) {
            cout << "Error on line " << lineCount << ": missing course number." << endl;
            continue;
        }

        if (!getline(ss, courseTitle, ',')) {
            cout << "Error on line " << lineCount << ": missing course title." << endl;
            continue;
        }

        while (getline(ss, prereq, ',')) {
            prerequisites.push_back(prereq);
        }

        Course course(courseNumber, courseTitle, prerequisites);
        bst.insert(course);
    }

    cout << "Courses loaded successfully." << endl;
    file.close();
}

// Function to print a course's information
void printCourseInformation(BinarySearchTree& bst, const string& courseNumber) {
    Course course = bst.search(courseNumber);
    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseTitle << endl;
    if (!course.prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            if (i > 0) cout << ", ";
            cout << course.prerequisites[i];
        }
        cout << endl;
    } else {
        cout << "No prerequisites." << endl;
    }
}

// Menu display function
void displayMenu() {
    cout << "\nWelcome to the course planner." << endl;
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit." << endl;
}

int main() {
    BinarySearchTree bst;
    string fileName, courseNumber;
    int choice = 0;

    while (choice != 9) {
        displayMenu();
        cout << "What would you like to do? ";
        
        // Input validation for the menu
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 9." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enter the file name: ";
            cin.ignore();  // Clear the newline from previous input
            getline(cin, fileName);  // Use getline to handle file names with spaces
            loadCourses(bst, fileName);
            break;

        case 2:
            cout << "Here is a sample schedule:" << endl;
            bst.printAllCourses();
            break;

        case 3:
            cout << "What course do you want to know about? ";
            cin.ignore();  // Clear the newline from previous input
            getline(cin, courseNumber);  // Use getline to handle course names with spaces
            printCourseInformation(bst, courseNumber);
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option. Please enter a number between 1 and 9." << endl;
        }
    }

    return 0;
}

