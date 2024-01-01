#pragma once
#include <vector>
#include <stdexcept>
#include <iostream>
using namespace std;
template <typename T>
class LinkedList {
private:
	unsigned int nodeCount = 0;
public:
	struct Node {
		T data;
		Node* next = nullptr;
		Node* prev = nullptr;
	};
	Node* head;
	Node* tail;
	LinkedList();
	~LinkedList();
	LinkedList(const LinkedList<T>& list);
	LinkedList<T>& operator=(const LinkedList<T>& rhs);
	void AddHead(const T& data);
	void AddTail(const T& data);
	void AddNodesHead(const T* data, unsigned int count);
	void AddNodesTail(const T* data, unsigned int count);
	void InsertAfter(Node* node, const T& data);
	void InsertBefore(Node* node, const T& data);
	void InsertAt(const T& data, unsigned int index);
	bool RemoveHead();
	bool RemoveTail();
	unsigned int Remove(const T& data);
	bool RemoveAt(unsigned int index);
	void Clear();
	unsigned int NodeCount() const;
	void FindAll(vector<Node*>& outData, const T& value) const;
	const Node* Find(const T& data) const;
	Node* Find(const T& data);
	const Node* GetNode(unsigned int index) const;
	Node* GetNode(unsigned int index);
	Node* Head();
	const Node* Head() const;
	Node* Tail();
	const Node* Tail() const;
	void PrintForward();
	void PrintReverse();
	void PrintForwardRecursive(const Node* node) const;
	void PrintReverseRecursive(const Node* node) const;
	//Operators
	const T& operator[](unsigned int index) const;
	T& operator[](unsigned int index);
	bool operator==(const LinkedList<T>& rhs) const;
};
/*====Construction / Destruction / Big Three====*/
template <typename T>
LinkedList<T>::LinkedList() {
	head = nullptr;
	tail = nullptr;
	nodeCount = 0;
}
template <typename T>
LinkedList<T>::~LinkedList() {
	Node* tempVar;
	Node* nextPoint;
	tempVar = head;
	while (tempVar != nullptr) {
		nextPoint = tempVar->next;
		delete tempVar;
		tempVar = nextPoint;
	}
}



/*====NEW====*/
template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) {
	//THIS IS A SHALLOW COPY,.,., BUT WHAT DO I DO... TwT
	//this->head->data = list.head->data;
	//this->tail = list.head;
	//this->nodeCount = list.nodeCount;

	//the thing is unititialized and you want to copy the DATA of the already intialized!
	Node* tempNode = list.head;
	for (unsigned int i = 0; i < list.nodeCount; i++) {
		AddTail(tempNode->data);
		tempNode = tempNode->next;
	}
	//maybe this will work?>?>?>?>?>
	//for (unsigned int i = 0; i < nodeCount; i++) {
		//AddTail(list.GetNode(i)->data);
	//}
	//this->nodeCount = list.nodeCount;

	//Node* tempVar;
	//Node* nextPoint;
	//tempVar->data = head->data;
	//this->head = tempVar;
	//while (tail->next != nullptr) {
		//nextPoint->data = tail->next->data;
		//tail = tail->next;
		//tempVar->next->data = nextPoint->data;
		//tempVar = tempVar->next;
		//tempVar->prev = nextPoint;
	//}
	//this->tail = tempVar;
	//delete tempVar;
	//delete nextPoint;
	/*
	Node* tempNode;
	tempNode->data = head->data;
	head = tempNode;
	for (unsigned int i = 0; i < nodeCount; i++) {
		if (list.GetNode(i + 1) != nullptr) {
			tempNode->next->data = list.GetNode(i + 1)->data;
			tempNode = tempNode->next;
			tail = tempNode;
		}
		//do i have to add previous? maybe introduce a pointer that can keep its memory locaiton
		//maybe that can be acomplished with tail?
	}
	*/
	/*
	Node* tempNode = list.head;
	this->head->data = tempNode->data;
	this->tail = head;
	this->tail->next = nullptr;

	Node* previousNode = tempNode->prev;
	tempNode = list.head->next;
	while (tempNode != nullptr) {
		this->tail->next->data = tempNode->data;
		this->tail = this->tail->next;
		//i think this next line is WRONG becasue pointers are shared but idk...
		this->tail->prev = previousNode;
		tempNode = tempNode->next;
		previousNode = tempNode->prev;
	}
	this->tail->next = nullptr;
	*/
}
template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs) {
	//same as copy constructor but with rhs instea dof list, also return *this
	//since the original linked lsit would already hav e anodesCOunt, you need to reset it

	/*
	Node* unassignedNode = head;
	for (unsigned int i = 0; i < nodeCount; i++) {
		delete unassignedNode->next;
		unassignedNode = unassignedNode->next;
	}
	delete head;
	*/
	Node* _tempNode = head;
	while (_tempNode != nullptr) {
		head = head->next;
		delete _tempNode;
		_tempNode = head;
	}
	head = tail = nullptr;
	nodeCount = 0;
	Node* tempNode = rhs.head;
	for (unsigned int i = 0; i < rhs.nodeCount; i++) {
		AddTail(tempNode->data);
		tempNode = tempNode->next;
	}
	return *this;
	// TODO: insert return statement here
}




/*====Insertion====*/
template <typename T>
void LinkedList<T>::AddHead(const T& data) {
	if (nodeCount == 0) {
		Node* firstNode = new Node;
		firstNode->data = data;
		firstNode->next = nullptr;
		head = firstNode;
		//strange to have them both be the same but if its the only element then you must!
		tail = firstNode;
		nodeCount++;
	}
	else {
		Node* tempNode = new Node;
		tempNode->data = data;
		//make the head this new data and dix all the alignements!
		tempNode->next = head;
		head->prev = tempNode;
		//the next of the previous head stay the same! so no need ot update it 
		head = tempNode;
		nodeCount++;
	}

}
template <typename T>
void LinkedList<T>::AddTail(const T& data) {
	//if there is nothing in the array!
	if (nodeCount == 0) {
		Node* firstNode = new Node;
		firstNode->data = data;
		firstNode->next = nullptr;
		head = firstNode;
		//strange to have them both be the same but if its the only element then you must!
		tail = firstNode;
		nodeCount++;
	}
	else {
		//tail's next is nullptr
		//if node already exists
		Node* tempNode = new Node;
		tempNode->data = data;
		//make the current tial's 'next' to the intialized  (adjust connections!)
		tail->next = tempNode;
		tempNode->prev = tail;
		tempNode->next = nullptr;
		//now set teh new element to the new tail
		tail = tempNode;
		nodeCount++;
	}

}
template <typename T>
void LinkedList<T>::AddNodesHead(const T* data, unsigned int count) {
	// if (or while CUrrent node != nullptr)currentNode - currentNide-> next
	//for each count!
	for (unsigned int i = count; i > 0; i--) {
		AddHead(data[i - 1]);
	}
}
template <typename T>
void LinkedList<T>::AddNodesTail(const T* data, unsigned int count) {
	for (unsigned int i = 0; i < count; i++) {
		AddTail(data[i]);
	}
}
template<typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& data) {
	Node* insertNode = new Node;
	insertNode->data = data;
	//node next is the new node, new node prev is node, new node next is node next, node next
	insertNode->next = node->next;
	insertNode->next->prev = insertNode;
	insertNode->prev = node;
	node->next = insertNode;

	nodeCount++;
}
template<typename T>
void LinkedList<T>::InsertBefore(Node* node, const T& data) {
	Node* insertNode = new Node;
	insertNode->data = data;
	insertNode->next = node;
	insertNode->prev = node->prev;
	node->prev = insertNode;
	insertNode->prev->next = insertNode;
	nodeCount++;
}
template<typename T>
inline void LinkedList<T>::InsertAt(const T& data, unsigned int index) {
	if (index > nodeCount) {
		throw out_of_range("whoops");
	}
	if (index == nodeCount) {
		AddTail(data);
	}
	else if (index == 0) {
		AddHead(data);
	}
	else {
		Node* insert = GetNode(index);
		InsertBefore(insert, data);
	}
}




/*====Removal====*/
template<typename T>
bool LinkedList<T>::RemoveHead()
{
	if (head == nullptr) {
		return false;
	}
	else {
		Node* tempNode = head;
		if (head->next == nullptr) {
			head = tail = nullptr;
		}
		else {
			head = head->next;
			head->prev = nullptr;
		}
		delete tempNode;
		nodeCount--;
		return true;
	}
}
template<typename T>
bool LinkedList<T>::RemoveTail()
{
	if (tail == nullptr) {
		return false;
	}
	else {
		Node* tempNode = tail;
		if (tail->prev == nullptr) {
			head = tail = nullptr;
		}
		else {
			tail = tail->prev;
			tail->next = nullptr;
		}
		delete tempNode;
		nodeCount--;
		return true;
	}
}
template<typename T>
unsigned int LinkedList<T>::Remove(const T& data)
{
	unsigned int removalCount = 0;
	Node* tempNode = head;
	Node* nextNode;
	for (unsigned int i = 0; i < nodeCount; i++) {
		nextNode = tempNode->next;
		if (tempNode->data == data) {
			if (tempNode->prev == nullptr) {
				tempNode->next->prev = nullptr;
			}
			else if (tempNode->next == nullptr) {
				tempNode->prev->next = nullptr;
			}
			else {
				tempNode->next->prev = tempNode->prev;
				tempNode->prev->next = tempNode->next;
			}
			delete tempNode;
			removalCount++;
		}
		tempNode = nextNode;
	}
	nodeCount = nodeCount - removalCount;
	return removalCount;
}
template<typename T>
//FINAL DUNCTION TO FIX!!! WOOHOO GOOD JOB ON GETTING THIS FAR<<<<
bool LinkedList<T>::RemoveAt(unsigned int index)
{
	if (index > nodeCount) {
		return false;
	}
	else if (index == 0) {
		RemoveHead();
		return true;
	}
	else if (index == nodeCount-1) {
		RemoveTail();
		return true;
	}
	else {
		Node* tempNode = head;
		for (unsigned int i = 0; i < index; i++) {
			tempNode = tempNode->next;
		}
		tempNode->next->prev = tempNode->prev;
		tempNode->prev->next = tempNode->next;
		delete tempNode;
		nodeCount--;
		return true;
	}
}
template<typename T>
void LinkedList<T>::Clear(){
	//same as beggining of copy assingement operator! -thank you ta
	Node* _tempNode = head;
	while (_tempNode != nullptr) {
		head = head->next;
		delete _tempNode;
		_tempNode = head;
	}
	head = tail = nullptr;
	nodeCount = 0;
}


/*====Accessros====*/
template <typename T>
unsigned int LinkedList<T>::NodeCount() const {
	return nodeCount;
}
/*====NEW====*/
template<typename T>
void LinkedList<T>::FindAll(vector<Node*>& outData, const T& value) const {
	Node* tempNode = head;
	for (unsigned int i = 0; i < nodeCount; i++) {
		if (tempNode->data == value) {
			outData.push_back(tempNode);
		}
		tempNode = tempNode->next;
	}
}
//^^^^complete for now!
template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) const {
	Node* tempNode = head;
	//use head function??? non ocnt to match it?
	for (unsigned int i = 0; i < nodeCount; i++) {
		if (tempNode->data == data) {
			return tempNode;
		}
		tempNode = tempNode->next;
	}

	return nullptr;
}
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) {
	Node* tempNode = head;
	//use head function?
	for (unsigned int i = 0; i < nodeCount; i++) {
		if (tempNode->data == data) {
			return tempNode;
		}
		tempNode = tempNode->next;
	}

	return nullptr;
}
//^^^^both complete for now! might need to change const functions!
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) {
	if (index >= nodeCount) {
		throw out_of_range("whoops");
	}
	else {
		Node* tempNode = head;
		for (unsigned int i = 0; i < index; i++) {
			tempNode = tempNode->next;
		}
		return tempNode;
	}
}
template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) const {
	if (index >= nodeCount) {
		throw out_of_range("whoops");
	}
	else {
		Node* tempNode = head;
		for (unsigned int i = 0; i < index; i++) {
			tempNode = tempNode->next;
		}
		return tempNode;
	}
}
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Head() {
	return head;
}
template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Head() const {
	return head;
}
template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::Tail() {
	return tail;
}
template <typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Tail() const {
	return tail;
}




/*====Behaviors====*/
template <typename T>
void LinkedList<T>::PrintForward() {
	//print the list from element 0 to last element using teh next keyword!
	Node* iterationPointer = head;
	Node* nextPoint;
	unsigned int count = 0;
	while (count < nodeCount) {
		nextPoint = iterationPointer->next;
		cout << iterationPointer->data << endl;
		iterationPointer = nextPoint;
		count++;
	}
}
template <typename T>
void LinkedList<T>::PrintReverse() {
	Node* iterationPointer = tail;
	Node* nextPoint;
	unsigned int count = nodeCount;

	while (count > 0) {
		nextPoint = iterationPointer->prev;
		cout << iterationPointer->data << endl;
		iterationPointer = nextPoint;
		count--;
	}
}
template<typename T>
void LinkedList<T>::PrintForwardRecursive(const Node* node) const{
	if (node == tail) {
		cout << node->data << endl;
	}
	else {
		cout << node->data << endl;
		PrintForwardRecursive(node->next);
	}
}
template<typename T>
inline void LinkedList<T>::PrintReverseRecursive(const Node* node) const{
	if (node == head) {
		cout << node->data << endl;
	}
	else {
		cout << node->data << endl;
		PrintReverseRecursive(node->prev);
	}
}



/*====NEW====*/
/*====Operators====*/
template <typename T>
const T& LinkedList<T>::operator[](unsigned int index) const {
	//almost the same as getNode???
	if (index >= nodeCount) {
		throw out_of_range("ahhh");
	}
	else {
		Node* tempNode = head;
		for (unsigned int i = 0; i < index; i++) {
			tempNode = tempNode->next;
		}
		return tempNode->data;
	}
}
template <typename T>
T& LinkedList<T>::operator[](unsigned int index) {
	if (index >= nodeCount) {
		throw out_of_range("ahhh");
	}
	else {
		Node* tempNode = head;
		for (unsigned int i = 0; i < index; i++) {
			tempNode = tempNode->next;
		}
		return tempNode->data;
	}



}
template<typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& rhs) const {
	bool matchData = true;
	Node* tempNode = head;
	for (unsigned int i = 0; i < nodeCount; i++) {
		if (tempNode->data != rhs.GetNode(i)->data) {
			matchData = false;
		}
		tempNode = tempNode->next;
	}
	return matchData;
}
