//Gabrielle Maitland
//CS 300 Data Structures and Algorithms
//Project Two Code

#include <iostream>
#include <fstream>
#include <string>
#include "CSVparser.hpp"

using namespace std;

//This is a structure defined to hold course information
struct Course {
    string name; //course name
    string number; //course number
    string prerequisites; //course prerequisite
};

//This is a structure for a tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

//This is the default constructor of the nodes
Node() {
    left = nullptr;
    right = nullptr;
}

//Initialize the tree with a course
Node(Course aCourse) :
        Node() {
            course = aCourse;
        }
};

//Binary Search Tree Class Definition//
class BinarySearchTree
{
    private:
    Node *root;

    void addNode(Node *node, Course course);
    void inOrder(Node *node);
    void preOrder(Node *node);

    public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void inOrder();
    void preOrder();
    void Insert(Course course);
    Course Search(string number);
};

//Default constructor where we create an empty tree
BinarySearchTree::BinarySearchTree(){
    root = nullptr;
}

//Destructor that recurses from the root and deletes each node
BinarySearchTree::~BinarySearchTree(){
    Node *cur = root;
    if (cur != nullptr) {
        delete cur->left;
        delete cur->right;
    } else {
        return;
    }
}

//This function will traverse the tree in order when called
//In order means that the algorithm will visit every node
//in the tree from smallest to largest
void BinarySearchTree::inOrder() {
    inOrder(root);
}

//This function will traverse the tree in pre order when called
//Pre order means that the algorithm will start at the root
//and visit the left subtree then the right subtree
void BinarySearchTree::preOrder(){
    preOrder(root);
}

//This function will insert a course into the tree
void BinarySearchTree::Insert(Course course){
    if (root == nullptr) {
        root = new Node(course);
    } else {
        this->addNode(root, course);
    }
}

//Tihs function will allow users to search the tree for a particular course
//using the course number
Course BinarySearchTree::Search(string number){
    Node *cur = root;
    while (cur != nullptr) {
        if (cur->course.number.compare(number) == 0) {
            return cur->course;
        }
        else if (number.compare(cur->course.number) < 0) {
            cur = cur->left;
        }
        else {
            cur = cur->right;

        }
    }

    Course course;
    return course;
}

//This function will allow us to recursively add a course to some node
void BinarySearchTree::addNode(Node* node, Course course) {
    if (node != nullptr && node->course.number.compare(course.number)>0) {
        //check if lefthand node is empty
        if (node->left == nullptr) {
            node->left = new Node(course);
            return;
        } else {
            //If it is not empty, complete a recursive call to add a node to the left side
            this->addNode(node->left, course);
        }
    } else if (node != nullptr && node->course.number.compare(course.number)<0) {
                //check if righthand node is empty
        if (node->right == nullptr) {
            node->right = new Node(course);
            return;
        } else {
            //If it is not empty, complete a recursive call to add a node to the left side
            this->addNode(node->right, course);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    //If the node is not equal to null, print node information starting with the left side
    //Then recursively call to the rightside
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.number << " : " << node->course.name << endl;
        inOrder(node->right);
    }
}

void BinarySearchTree::preOrder(Node* node){
    cout << node->course.number << " : " << node->course.name << " | " << node->course.prerequisites << endl;
    if (node != nullptr) {
        preOrder(node->left);
        preOrder(node->right);
    }
}

//Display the course information in console
void displayCourses(Course course){
    if (course.prerequisites.size()<6) {
        cout << course.number << " : " << course.name << " | " << "Prerequisites: None!" << endl;
    } else {
        cout << course.number << " : " << course.name << " | " << "Prerequisites: " << course.prerequisites << endl;
    }
        return;
}

//Load a text file containing the course information into a container
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading File " << csvPath << endl;

    csv::Parser file = csv::Parser(csvPath);

    try {

        for (unsigned int i = 0; i < file.rowCount(); i++) {

            Course course;
            course.name = file[i][1];
            course.number = file[i][0];
            string prerequisites = "";
          	for(int j = 2; j < file[i].size(); j++) {
              prerequisites += file[i][j] + " ";
            }
            course.prerequisites = prerequisites;

            bst->Insert(course);
        }
    } catch (csv::Error &e) {
        cerr << e.what() << endl;
    }

}

//Driver Code/Main Method
int main(int argc, char* argv[]) {
    string courseInput;
    string csvPath;
    switch (argc) {
        default:
        csvPath = "ABCU_Advising_Program_Input.csv";
    }

    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int userInput = 0;
    while (userInput != 9) {

        cout << "Menu" << endl;
        cout << "1. Load Courses" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Search and Print Course" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter Choice: ";
        cin >> userInput;
        //if the user inputs a non integer value
        if (cin.fail()) {
            cout << "Error! You've broken me :( Please try again." << endl;
            break;
        }


        switch (userInput) {


            case 1:
            //Load the courses into the tree
                loadCourses(csvPath, bst);

                break;

            case 2:
            //Call the inorder function to print course list alphanumerically
                bst->inOrder();
                break;

            case 3:
            //Prompt the user to enter course number and accept it
                cout << "Enter Course Number (cAsE sensitive!)" << endl;
                cin >> courseInput;
            //Search the tree for the user's input
                course = bst->Search(courseInput);
            //If there is a course, display it. If not, tell user it wasnt found.
                if (!course.number.empty()) {
                    displayCourses(course);
                } else {
                    cout << "Course number " << courseInput << " not found." << endl;
                }
                break;

            case 9:
                //see the user out
                cout << "Goodbye~" << endl;
                break;

                //if the user inputs a number not accepted
                default:
                    cout << userInput << " is not an accepted choice. Please enter either 1, 2, 3, or 9." << endl;
        }
    }
    return 0;
}


//RESOURCES:
// https://stackoverflow.com/questions/18728754/checking-cin-input-stream-produces-an-integer
// BinarySearchTree Assignment from Module 4
// ZyBooks CS 300 Data Structures and Algorithms, Chapter 6 "Trees"