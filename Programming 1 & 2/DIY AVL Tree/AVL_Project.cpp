/*
I used information learned in lecture and discussion to help me with the Node class object, the roations and the balance factors
I used Project 1 breakdown slides for the parsing notation
I got get from GeeksforGeeks for the removenode functionality- specifically the 2 children removal but,
I was able to tweak it to work for the specifications provided by this project.
The link is found here:
	https://www.geeksforgeeks.org/avl-tree-set-2-deletion/
*/
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
//#include "TreeNode.h"
using namespace std;
class TreeNode {
	int val;
	string name;
	string parseInt;
	TreeNode* left;
	TreeNode* right;
	int height;
public:
	TreeNode() {
		val;
		name = "";
		parseInt = "";
		left = nullptr;
		right = nullptr;
	}
	TreeNode(int value, string idName) {
		val = value;
		name = idName;
		height = 1; //(assuming 1 based height- all leafs are height 1!)
		//string uid
		left = nullptr;
		right = nullptr;
	}
	int getHeight(TreeNode* node) {
		if (node != nullptr) {
			return node->height;
		}
		else {
			return 0;
		}
	}
	void setHeight(TreeNode* root) {
		if (root == nullptr) {
			return;
		}
		if (getHeight(root->left) >= getHeight(root->right)) {
			root->height = 1 + getHeight(root->left);
		}
		else {
			root->height = 1 + getHeight(root->right);
		}
	}
	int findBalance(TreeNode* root) {
		if (root == nullptr) {
			return 0;
		}
		return getHeight(root->left) - getHeight(root->right);
	}
	TreeNode* rotateLeft(TreeNode* node) {
		//taken from lecture and discussion
		TreeNode* newParent = node->right;
		TreeNode* grandchild = newParent->left;
		newParent->left = node;
		node->right = grandchild;
		////now after rotating, you have to update the height values for all the nodes
		////height of node, and node->right did not change. Only the values for newParent and grandchild changed
		//update height values in backwards order bc the parents height is DEPENDANT on the CHILDS height

		setHeight(grandchild);
		setHeight(node);
		setHeight(newParent);
		return newParent;
	}
	TreeNode* rotateRight(TreeNode* node) {
		//taken from lecture and discussion
		TreeNode* newParent = node->left;
		TreeNode* grandchild = newParent->right;
		newParent->right = node;
		node->left = grandchild;
		//update height values in backwards order bc the parents height is DEPENDANT on the CHILDS height
		setHeight(grandchild);
		setHeight(node);
		setHeight(newParent);
		return newParent;
	}
	TreeNode* rotateRightLeft(TreeNode* node) {
		//taken from lecture and discussion
		node->right = rotateRight(node->right);
		TreeNode* newParent = rotateLeft(node);
		return newParent;
	}
	TreeNode* rotateLeftRight(TreeNode* node) {
		//taken from lecture and discussion
		node->left = rotateLeft(node->left);
		TreeNode* newParent = rotateRight(node);
		return newParent;
	}
	TreeNode* insert(TreeNode* root, int data, string idName)
	{
		if (root == nullptr) {
			cout << "successful" << endl;
			return new TreeNode(data, idName);
		}
		if (root->val == data) {
			cout << "unsuccessful" << endl;
			return root;
		}
		if (root->val < data)
			root->right = insert(root->right, data, idName);
		else
			root->left = insert(root->left, data, idName);
		//must check for BALANCE because its an AVL tree!<3
		//1st step of checking balance is UPDATING the height (does not always chenge- depends on the max left/right node)
		setHeight(root);
		//check balance factor after updating the heights
		int bf = findBalance(root); // this will return an int- if its between -1 and 1 then its still balanced, otherwise, we must redistribute it
		
		//right heavy
		if (bf < -1) {
			//discussion discussed the 1st case you should check is the double rotations, which using the chart provided on parent and child balance factors, I was able to insert
			if (findBalance(root->right) == 1){
				return rotateRightLeft(root);
			}
			else {
				return rotateLeft(root);
			}
		}
		else if (bf > 1) {
			if (findBalance(root->left) == -1){
				return rotateLeftRight(root);
			}
			else {
				return rotateRight(root);
			}
		}
		//cout << "CURRENT ROOT" << root->val << endl;
		return root;
	}
	void destruct(TreeNode* root)
	{
		//im not sure if we would need more here but I think this is enough
		delete root;
	}
	void destructTree(TreeNode* root) {
		if (root == nullptr) {
			return;
		}
		destructTree(root->left);
		destructTree(root->right);
		delete root;
	}
	void inOrderTraversalInt(TreeNode* root, vector<int>& list) {
		//similar to the ones that follow- this was created as a helper funciton to aid in teh process of deleting inorder node at N
		if (root == nullptr) {
			return;
		}
		inOrderTraversalInt(root->left, list);
		list.push_back(root->val);
		inOrderTraversalInt(root->right, list);
	}
	void inOrderTraversalStr(TreeNode* root, vector<string>& list) {
		//this code was adopted from lecture/ discussion but was tweaked by me to help with the the prinint of the names with commas on all excpet the last one!
		if (root == nullptr) {
			return;
		}
		inOrderTraversalStr(root->left, list);
		list.push_back(root->name);
		inOrderTraversalStr(root->right, list);
	}
	void preOrderTraversalStr(TreeNode* root, vector<string>& list) {
		//this code was adopted from lecture/ discussion but was tweaked by me to help with the the prinint of the names with commas on all excpet the last one!
		if (root == nullptr) {
			return;
		}
		list.push_back(root->name);
		preOrderTraversalStr(root->left, list);
		preOrderTraversalStr(root->right, list);
	}
	void printTraversal(vector<string>& list) {
		//this prints any of the traversal lists with commas btw all elements except for the final one
		int i;
		for (i = 0; i < (list.size() - 1); i++) {
			cout << list[i] << ", ";
		}
		cout << list[i] << endl;
	}
	void postOrderTraversalStr(TreeNode* root, vector<string>& list) {
		//this code was adopted from lecture/ discussion but was tweaked by me to help with the the prinint of the names with commas on all excpet the last one!
		if (root == nullptr) {
			return;
		}
		postOrderTraversalStr(root->left, list);
		postOrderTraversalStr(root->right, list);
		list.push_back(root->name);
	}
	void checkChildren(TreeNode* root) {
		//this was just a helper function while i was coding the project to being with and is not used in the finanl solution
		cout << root->val << endl;
		cout << "Left Child: " << root->left->val << endl;
		cout << "Right Child: " << root->right->val << endl;
	}
	TreeNode* findSuccessor(TreeNode* node) {
		if (node->left == nullptr) {
			return node;
		}
		else {
			return findSuccessor(node->left);
		}
		//this is simple code that takes in the right subtree's root node and finds its leftmost child!
	}
	TreeNode* removeNode(TreeNode* root, int idVal) {
		//the tricky part with remove is that you need to have a pointer to the PARENT value for the double rotation values. So you need to recsively pass in the new child (after rotation) to the parents pointer val
		if (root == nullptr) {
			return root;
		}
		if (idVal > root->val) {
			root->right = removeNode(root->right, idVal);
		}
		else if (idVal < root->val) {
			root->left = removeNode(root->left, idVal);
		}
		else {
			//else here means a maching node was found
			//if the subtree's root val is the SAME as the val and has NO children
			if (root->left == nullptr && root->right == nullptr) {
				//cout << "successful" << endl;
				root = nullptr;
				//the child of the parent is now a nullptr
			}
			else if (root->right == nullptr) {
				//if the sbutree's root has ONLY a left child (make the return value the left subtree's root node so the grandparent will point to it)
				TreeNode* newChild = root->left;
				root->val = newChild->val;
				root->name = newChild->name;
				root->left = removeNode(root->left, newChild->val);
				//cout << "successful" << endl;
			}
			else if (root->left == nullptr) {
				//same process but for the RIGHT subtree assuming there is no left subtree
				TreeNode* newChild = root->right;
				root->val = newChild->val;
				root->name = newChild->name;
				root->right = removeNode(root->right, newChild->val);
				//cout << "successful" << endl;
			}
			else {
				//the final case if the child has 2 children. Which according to the instructions, we need to prioritize the sucessor (which means we only need to account for the successor bc it will ALWAYS be there in the case 2 children are present)
				TreeNode* newRoot = findSuccessor(root->right);
				//we must replace the root with successor and then delete the successor
				//I could not think of how to delete the node and point to the succcessor so this is where I got help from an outside source (geeks for geeks) that suggested you copy the data into the root and then delete the successors occurence in the subtree so its not shown twice!
				root->val = newRoot->val;
				root->name = newRoot->name;
				//directly replacing the root by changing its values
				//deleting 7: this doesnt delete the 7 replaced bc it is only deleting it from teh SUBTREE
				root->right = removeNode(root->right, root->val);
				//cout << "successful" << endl;
			}
		}
		//although we are not required to rebalance the deleted node tree- you must reset the height of all the nodes to reflect teh new height
		//that way, when you insert a noode, it is as if the deleted node was never there to beign with
		setHeight(root);
		return root;
	}
	void searchIDNum(TreeNode* root, int val) {
		if (root == nullptr) {
			cout << "unsuccessful" << endl;
			return;
		}
		if (root->val == val) {
			cout << root->name << endl;
			return;
		}
		if (root->val < val)
			searchIDNum(root->right, val);
		else if (root->val > val)
			searchIDNum(root->left, val);
		//just normal traversal comparing values though ALL nodes (O(n))
	}
	bool searchName(TreeNode* root, string name) {
		//bool contained = false;
		////considering theres is no way to cmoparae the name values by searching through the tree as we can with numbers- 
		////the whole tree must be seaerched through!
		bool found = false;
		if (root == nullptr) {
			return false;
		}
		if (name.compare(root->name) == 0) {
			//if the strings match- printout the sucecss message
			//as seen on slack inputs of 00000001 will read as 1 for, so when we print out it MUST be 00000001 bc ID's are of size 7 ALWAYS
			//using setfill sets PADDING to be 0, setw makes the width of the output be 8 ints long- which was specified in teh isntructions
			cout << setfill('0') << setw(8) << root->val << endl;
			found = true;
		}
		bool left = searchName(root->left, name);
		bool right = searchName(root->right, name);
		
		//prints out all names FIRST then returns the value after- if its found in the roots left OR right subtrees, then it was found and there is no need to prinnt 'unsuccessful'
		if (left || right || found) {
			return true;
		}
		return false;
	}
	void levelCount(TreeNode* root) {
		cout << getHeight(root) << endl;
		return;
	}
	void removeInorder(TreeNode* root, vector<int>& inorderList, int removeVal) {
		for (int i = 0; i < inorderList.size(); i++) {
			if (i == removeVal) {
				root = removeNode(root, inorderList[i]);
				cout << "successful" << endl;
				return;
			}
		}
		cout << "unsuccessful" << endl;
		return;
	}
	bool stringDig(string& string) {
		for (int i = 0; i < string.size(); i++) {
			if (isdigit(string[i]) == 0) {
				return false;
			}
		}
		return true;
	}
	void commands() {
		TreeNode* root = nullptr;
		string parse;
		string name;
		int val;
		int numCommands;
		//get number of operations!
		getline(cin, parse);
		numCommands = stoi(parse);
		for (int i = 0; i < numCommands; i++) { 
			//this was seen in the Project 1 breakdown slides and I used them to work with the parsing!
			getline(cin, parse);
			istringstream in(parse);
			string command;
			in >> command;
			if (command.compare("insert") == 0) {
				string next;
				getline(in, next, '\"');
				getline(in, next, '\"');
				name = next;
				bool acceptable_name = true;
				if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != std::string::npos) {
					cout << "unsuccessful" << endl;
					acceptable_name = false;
				}
				getline(in, next, ' ');
				getline(in, next);
				if (next.size() != 8) {
					cout << "unsuccessful" << endl;
				}
				else if (acceptable_name == true) {
					bool number = stringDig(next);
					if (number == true) {
						val = stoi(next);
						root = insert(root, val, name);
					}
					else {
						cout << "unsuccessful" << endl;
					}
				}
			}
			else if (command.compare("remove") == 0) {
				string next;
				getline(in, next);
				vector<int> inorderList;
				bool valueFound = false;
				inOrderTraversalInt(root, inorderList);
				val = stoi(next);
				for (int i = 0; i < inorderList.size(); i++) {
					if (inorderList[i] == val) {
						valueFound = true;
					}
				}
				if (valueFound == true) {
					removeNode(root, val);
					cout << "successful" << endl;
				}
				else {
					cout << "unsuccessful" << endl;
				}
				//cout << "Root lvl count: ";
				//levelCount(root);
				//cout << endl;
			}
			else if (command.compare("search") == 0) {
				string next;
				getline(in, next);
				if (next.find_first_of('\"') != std::string::npos) {
					string name;
					name = next.substr((next.find_first_of('\"') + 1), (next.length() - 3));
					if (searchName(root, name) == false) {
						cout << "unsuccessful" << endl;
					}
				}
				else {
					getline(in, next);
					val = stoi(next);
					searchIDNum(root, val);
				}
			}
			else if (command.compare("printInorder") == 0) {
				vector<string> inorderList;
				inOrderTraversalStr(root, inorderList);
				printTraversal(inorderList);
			}
			else if (command.compare("printPreorder") == 0) {
				vector<string> preorderList;
				preOrderTraversalStr(root, preorderList);
				printTraversal(preorderList);
			}
			else if (command.compare("printPostorder") == 0) {
				vector<string> postorderList;
				postOrderTraversalStr(root, postorderList);
				printTraversal(postorderList);
			}
			else if (command.compare("printLevelCount") == 0) {
				levelCount(root);
			}
			else if (command.compare("removeInorder") == 0) {
				vector<int> inorderList;
				inOrderTraversalInt(root, inorderList);
				string next;
				getline(in, next);
				val = stoi(next);
				removeInorder(root, inorderList, val);
			}
			else {
				cout << "unsuccessful" << endl;
			}
		}
		//delete tree at the end because its dynamic memory
		destructTree(root);
	}
};


int main()
{
	//I KNOW this main looks quite... empty- though I discussed it with Prof. Aman and he said it was fine.
	//Is there any drawbacks to having main be so empty?
	TreeNode tree;
	tree.commands();
}