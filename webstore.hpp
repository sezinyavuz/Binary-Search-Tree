#pragma once
#include <string>

// A balanced two-phase binary tree node
struct Item {
	std::string category;
	std::string name;
	int price;
};
struct InputLine {
	std::string command;
	std::string category;
	std::string name;
	int price;
};

struct SecondaryNode{
	std::string key;
	SecondaryNode *left;
	SecondaryNode *right;
	int data;
	int height;
	// red ==> true, black ==> false
    bool color;
	SecondaryNode(Item item) : left(NULL), right(NULL),height(1), color(true){
		key = item.name;
		data = item.price;
	}
	int getHeight(){
		if (this == NULL)
			return 0;
		return this->height;
	};
	int getBalance() {
		if (this == NULL)
			return 0;
		return this->left->getHeight() - this->right->getHeight();
	};

};
struct PrimaryNode {
	std::string key;
	PrimaryNode *left;
	PrimaryNode *right;
	int height;
	// red ==> true, black ==> false
    bool color;
	SecondaryNode *secondary_root;
	PrimaryNode(Item new_item) : left(NULL), right(NULL),height(1), color(true){
		key = new_item.category;
		secondary_root = new SecondaryNode(new_item);
	}
	int getHeight(){
		if (this == NULL)
			return 0;
		return this->height;
	};
	int getBalance() {
		if (this == NULL)
			return 0;
		return this->left->getHeight() - this->right->getHeight();
	};
};
struct searchResult{
	PrimaryNode* primaryNode;
	SecondaryNode* secondaryNode;
};
/*
struct Node{
	public:
	std::string key;
	Node *left;
	Node *right;
	int height;
	Node(Item item) : left(nullptr), right(nullptr),height(1){
		key = item.name;
	}
	int getHeight(){
		if (this == NULL)
			return 0;
		return this->height;
	};
	int getBalance() {
		if (this == NULL)
			return 0;
		return this->left->getHeight() - this->right->getHeight();
	};
};
struct SecondaryNode : public Node{
	int data;
	SecondaryNode(Item item) : Node(item){
		data = item.price;
	}
};
struct PrimaryNode : public Node {
	SecondaryNode *secondary_root;
	PrimaryNode(Item new_item) : Node(new_item){
		secondary_root = new SecondaryNode(new_item);
	}
};*/