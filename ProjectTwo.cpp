// ========================================================
// Name: ProjectTwo.cpp
// Author: Talia McCarthy-Wielenga
//

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>
#include <vector>

using namespace std;

	const int GLOBAL_SLEEP_TIME = 5000; 

/*
 *Course information structure
 */
struct Course {
	string courseId;
	string courseName;
	vector<string> prerequisites;
};
class BinarySearchTree {
	/*
	 * structures within class
	 * default constructor
	 * initialize course
	 */
private:
	struct Node {
		Course course;
		Node* right;
		Node* left;

		Node() {
			right = nullptr;
			left = nullptr;
		}
		Node(Course aCourse) {
			course = aCourse;
			right = nullptr;
			left = nullptr;
		}
	};
	Node* root;
	void inOrder(Node* node);
	int size = 0;

public:
	BinarySearchTree();
	void InOrder();
	void Insert(Course aCourse);
	void Remove(string courseId);
	Course Search(string courseId);
	int Size();
};
/*
 * BinarySearchTree default constructor
 */ 
BinarySearchTree::BinarySearchTree() {
	this->root = nullptr;
}
/*
 *Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	inOrder(root);
}
/*
 * insert course
 */
void BinarySearchTree::Insert(Course aCourse) {
	Node* currentNode = root;
	if (root == NULL) {
		root = new Node(aCourse);
	}
	else {
		while (currentNode != NULL) {
			if (aCourse.courseId < currentNode->course.courseId) {
				if (currentNode->right == nullptr) {
					currentNode->right = new Node(aCourse);
					currentNode = NULL;
				}
				else {
					currentNode = currentNode->right;
				}
			}
			else {
				if (currentNode->left == nullptr) {
					currentNode->left = new Node(aCourse);
					currentNode = NULL;
				}
				else {
					currentNode = currentNode->left;
				}
			}
		}
	}
	size++;
}
/*
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId) {
	Node* par = NULL;
	Node* curr = root;
	while (curr != NULL) {
		if (curr->course.courseId == courseId) {
			if (curr->right == NULL && curr->left == NULL) {
				if (par == NULL) {
					root = nullptr;
				}
				else if (par->left == curr) {
					par->left = NULL;
				}
				else {
					par->right = NULL;
				}
					par->left = curr->left;
			}
			else if (curr->right == NULL) {
				if (par == NULL) {
					root = curr->left;
				}
				else if (par->left == curr) {
					par->left = curr->left;
				}
				else {
					par->right == curr->left;
				}
			}
			else if (curr->left == NULL) {
				if (par == NULL) {
					root = curr->right;
				}
				else if (par->left == curr) {
					par->left = curr->right;
				}
				else {
					par->right = curr->right;
				}
			}
			else {
				Node* suc = curr->right;
				while (suc->left != NULL) {
					suc = suc->left;
				}
				Node successorData = Node(suc->course);
				Remove(suc->course.courseId);
				curr->course = successorData.course;
			}
			cout << "\nNode removed" << endl;
			return;
			}
			else if (curr->course.courseId < courseId) {
				par = curr;
				curr = curr->right;
			}
			else {
				par = curr;
				curr = curr->left;
			}
		}
		cout << "\nValue not found" << endl;
		return;
	}
	/*
	 *Search for a course
	 */
	Course BinarySearchTree::Search(string courseId) {
		Course aCourse;
		Node* currentNode = root;
		while (currentNode != NULL) {
			if (currentNode->course.courseId == courseId) {
				return currentNode->course;
			}
			else if (courseId < currentNode->course.courseId) {
				currentNode = currentNode->left;
			}
			else {
				currentNode = currentNode->right;
			}
		}
		cout << "Value not found" << endl;
		return aCourse;
	}
	void BinarySearchTree::inOrder(Node* node) {
		if (node == NULL) {
			return;
		}
		inOrder(node->left);
		cout << node->course.courseId << ", " << node->course.courseName << endl;
		inOrder(node->right);
	}
	int BinarySearchTree::Size() {
		return size;
	}
	vector<string> Split(string lineFeed) {
		char delimiter = ',';
		lineFeed += delimiter; //add delimiter to end of string to capture last value
		vector<string> elements;
		string temp = "";
		for (int i = 0; i < lineFeed.length(); i++) {
			if (lineFeed[i] == delimiter) {
				elements.push_back(temp); //stores words in vector
				temp = "";
				i++;
			}
			temp += lineFeed[i];
		}
		return elements;
	}
	/*
	 * load courses
	 */
	void loadCourses(string csvPath, BinarySearchTree* courseList) {
		ifstream inFS; //ifstream reads file
		string line;
		vector<string> elements;
		inFS.open(csvPath); //opens read file
		if (!inFS.is_open()) {
			cout << "Cannot open file" << endl;
			return;
		}
		while (!inFS.eof()) {
			Course aCourse; // creates new struct for each line
			getline(inFS, line);
			elements = Split(line); //split line into elements with delimiter
			if (elements.size() < 2) {
				cout << "\nError! Incomplete line found in file. Skipping line." << endl;
			}
			else {
				aCourse.courseId = elements.at(0);
				aCourse.courseName = elements.at(1);
				for (unsigned int i = 2; i < elements.size(); i++) {
					aCourse.prerequisites.push_back(elements.at(i));
				}
				courseList->Insert(aCourse); // push course to end
			}
		}
		inFS.close(); //close file
	}
	/*
	 * Print course information
	 */
	void displayCourse(Course aCourse) {
		cout << aCourse.courseId << ", " << aCourse.courseName << endl;
		cout << "Prerequisites: ";
		if (aCourse.prerequisites.empty()) {
			cout << "No prerequisites" << endl;
		}
		else {
			for (unsigned int i = 0; i < aCourse.prerequisites.size(); i++) {
				cout << aCourse.prerequisites.at(i);
				if (aCourse.prerequisites.size() > 1 && i < aCourse.prerequisites.size() - 1) {
					cout << ", "; //add comma between prerequisites
				}
			}
		}
		cout << endl;
	}
	int main(int argc, char* argv[]) {
		string csvPath, aCourseKey;
		switch (argc) {
		case 2:
			csvPath = argv[1];
			break;
		case 3:
			csvPath = argv[1];
			aCourseKey = argv[2];
			break;
		default:
			csvPath = "ABCU_Advising_Program_Input.csv";
		}
		BinarySearchTree* courseList = new BinarySearchTree();
		Course course;
		bool goodInput;
		int choice = 0;
		while (choice != 9) {
			cout << "Menu:" << endl;
			cout << "1. Load Courses" << endl;
			cout << "2. Display All Courses" << endl;
			cout << "3. Find Course" << endl;
			cout << "4. Remove Course" << endl;
			cout << "9. Exit" << endl;
			cout << "Enter choice: ";
			aCourseKey = ""; //clears previous input
			string anyKey = " "; //clears previous input
			choice = 0; //clears the choice
			try {
				cin >> choice;
				if ((choice > 0 && choice << 5) || (choice == 9)) { //limits user menu inputs
					goodInput = true;
				}
				else { //throws exception for invalid menu input
					goodInput = false;
					throw 1;
				}
				switch (choice) {
				case 1:
					//call to load courses
					loadCourses(csvPath, courseList);
					cout << courseList->Size() << " courses loaded." << endl;
					Sleep(GLOBAL_SLEEP_TIME);
					break;
				case 2:
					courseList->InOrder();
					cout << "Press any key to continue..." << endl;
					cin >> anyKey;
					break;
				case 3:
					cout << "Enter course ID to search: " << endl;
					cin >> aCourseKey;
					course = courseList->Search(aCourseKey);
					if (!course.courseId.empty()) {
						displayCourse(course);
					}
					else {
						cout << "Course not found." << endl;
					}
					Sleep(GLOBAL_SLEEP_TIME);
					break;
				case 4:
					cout << "Enter course ID to remove: " << endl;
					cin >> aCourseKey;
					courseList->Remove(aCourseKey);
					Sleep(GLOBAL_SLEEP_TIME);
					break;
				case 9:
					exit;
					break;
				default:
					throw 2; //throws exception for invalid menu input
				}
			}
			catch (int err) {
				std::cout << "\nInvalid input. Please try again." << std::endl;
				Sleep(GLOBAL_SLEEP_TIME);
			}
			//clear cin operator of extra input
			cin.clear();
			cin.ignore();
			system("cls"); //clear console
		}
		cout << "Exiting program..." << endl;
		Sleep(GLOBAL_SLEEP_TIME);
		
	return 0;
}



