#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TreeNode {
	int val;
	TreeNode* left;
	TreeNode* right;
	int balanceVal;
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
public:
	TreeNode(int rootvalue) {
		val = rootvalue;
	}
	int getVal() {
		return val;
	}
	TreeNode* getLeftNode() {
		return left;
	}
	void setLeftNode(TreeNode* leftNode) {
		left = leftNode;
	}
	TreeNode* getRightNode() {
		return right;
	}
	void setRightNode(TreeNode* rightNode) {
		right = rightNode;
	}
	TreeNode* insert(TreeNode* root, int data)
	{
		if (root == nullptr)
			return new TreeNode(data);
		if (root->val < data)
			root->right = insert(root->right, data);
		else
			root->left = insert(root->left, data);
		return root;
	}
	void destruct(TreeNode* root)
	{
		if (root->left)
			destruct(root->left);
		if (root->right)
			destruct(root->right);
		delete root;
	}
};

/*
class TreeNode
{
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// This will be called in main to construct the tree
TreeNode* insert(TreeNode* root, int data)
{
	if (root == nullptr)
		return new TreeNode(data);
	if (root->val < data)
		root->right = insert(root->right, data);
	else
		root->left = insert(root->left, data);
	return root;
}

// This will be called in main to deallocate memory
void destruct(TreeNode* root)
{
	if (root->left)
		destruct(root->left);
	if (root->right)
		destruct(root->right);
	delete root;
}


*/