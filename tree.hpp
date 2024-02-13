#pragma once
#include "webstore.hpp"
#include <queue>
#include "util.h"
#include <fstream>
#include <iostream>
class TwoPhaseBST {
    private:
    template <typename T>
		T *rightRotate(T *y){
			T *x = y->left;
			T *T2 = x->right;

			// Perform rotation
			x->right = y;
			y->left = T2;

			// Update heights
			y->height = max(y->left->getHeight(),y->right->getHeight()) + 1;
			x->height = max(x->left->getHeight(),x->right->getHeight()) + 1;

			// Return new root
			return x;
		};
		template <typename T>
		T *leftRotate(T *x){
			T *y = x->right;
			T *T2 = y->left;

			// Perform rotation
			y->left = x;
			x->right = T2;

			// Update heights
			y->height = max(y->left->getHeight(),y->right->getHeight()) + 1;
			x->height = max(x->left->getHeight(),x->right->getHeight()) + 1;

			// Return new root
			return y;
		};
		PrimaryNode* balanceTreeAvl(PrimaryNode *node, Item new_item){
			int balance = node->getBalance();
			if (balance > 1 && (compareLexicographically(new_item.category, node->left->key) == -1))
				return rightRotate(node);

			// Right Right Case
			if (balance < -1 && (compareLexicographically(new_item.category, node->right->key) == 1))
				return leftRotate(node);

			// Left Right Case
			if (balance > 1 && (compareLexicographically(new_item.category, node->left->key) == 1)){
				node->left = leftRotate(node->left);
				return rightRotate(node);
			}

			// Right Left Case
			if (balance < -1 && (compareLexicographically(new_item.category, node->right->key) == -1)){
				node->right = rightRotate(node->right);
				return leftRotate(node);
			}
			return node;
		};
		SecondaryNode* balanceSecondaryTreeAvl(SecondaryNode *node, Item new_item){
			int balance = node->getBalance();
			if (balance > 1 && (compareLexicographically(new_item.name, node->left->key) == -1))
				return rightRotate(node);

			// Right Right Case
			if (balance < -1 && (compareLexicographically(new_item.name, node->right->key) == 1))
				return leftRotate(node);

			// Left Right Case
			if (balance > 1 && (compareLexicographically(new_item.name, node->left->key) == 1)){
				node->left = leftRotate(node->left);
				return rightRotate(node);
			}

			// Right Left Case
			if (balance < -1 && (compareLexicographically(new_item.name, node->right->key) == -1)){
				node->right = rightRotate(node->right);
				return leftRotate(node);
			}
			return node;
		};
		template <typename T>
		T* balanceTreeRbl(T *node){
			// case 1.
			// when right child is Red but left child is Black or doesn't exist.
			if (isRed(node -> right) &&!isRed(node -> left)){
				// Left rotate the node to make it into valid structure.
				node = leftRotate(node);
				// Swap the colors as the child node should always be red
				swapColors(node, node -> left);
			}
			
			// case 2
			// when left child as well as left grand child in Red
			if (isRed(node -> left) &&isRed(node -> left -> left)){  
				// Right rotate the current node to make it into a valid structure.
				node = rightRotate(node);
				swapColors(node, node -> right);
			}
			// case 3
			// when both left and right child are Red in color.
			if (isRed(node -> left) && isRed(node -> right)){
				// Invert the color of node as well
				// it's left and right child.
				node -> color = !node -> color;
				// Change the color to black.
				node -> left -> color = false;
				node -> right -> color = false;
			}
			return node;
		};
		// Helper function to insert a new key in the subtree rooted at node.
		// Returns the new root of the subtree.
		// Utility function to check whether
		// node is red in color or not.
		template <typename T>
		int isRed(T *myNode){
			if (myNode == NULL)return 0;
			return (myNode -> color == true);
		}
		// Utility function to swap color of two
		// nodes.
		template <typename T>
		void swapColors(T *node1, T *node2){
			bool temp = node1 -> color;
			node1 -> color = node2 -> color;
			node2 -> color = temp;
		}
	public:
		PrimaryNode* root;
		bool tree_type; // 0 => avl, 1 => rbl
		// Helper function to insert a new key in the subtree rooted at node.
		// Returns the new root of the subtree.		
		PrimaryNode* insert(PrimaryNode* node, Item new_item) {
			// If the subtree is empty, create a new node for the key
			if (node == NULL) return new PrimaryNode(new_item);

			// Otherwise, recur down the tree

			if(compareLexicographically(new_item.category, node->key) == -1) {
				node->left = insert(node->left, new_item);
			} 
			else if(compareLexicographically(new_item.category, node->key) == 1) {
				node->right = insert(node->right, new_item);
			}
			else{
				// Same category already exists
				node->secondary_root = insertSecondaryTree(node->secondary_root, new_item);
				return node;
			}

			// Update the height of the ancestor node
			node->height = 1 + max(node->left->getHeight(), node->right->getHeight());
			if(this->tree_type == 0)
				return balanceTreeAvl(node, new_item);
			else 
				return balanceTreeRbl(node);
		}
		SecondaryNode* insertSecondaryTree(SecondaryNode* node, Item new_item) {
			// If the subtree is empty, create a new node for the key
			if (node == NULL) return new SecondaryNode(new_item);

			// Otherwise, recur down the tree

			if(compareLexicographically(new_item.name, node->key) == -1) {
				node->left = insertSecondaryTree(node->left, new_item);
			} 
			else if(compareLexicographically(new_item.name, node->key) == 1) {
				node->right = insertSecondaryTree(node->right, new_item);
			}
			
			// Update the height of the ancestor node
			node->height = 1 + max(node->left->getHeight(), node->right->getHeight());
			if(this->tree_type == 0)
				return balanceSecondaryTreeAvl(node, new_item);
			else
				return balanceTreeRbl(node);
		}
	PrimaryNode* update(PrimaryNode* node, std::string category, std::string name, int data){
		// If the subtree is empty, the key does not exist in the tree
		if (node == NULL) return NULL;
		// recur down the tree
		if(compareLexicographically(category, node->key) == -1) {
			node->left = update(node->left, category,name,data);
		} 
		else if(compareLexicographically(category, node->key) == 1) {
			node->right = update(node->right, category,name,data);
		}
		else{
			//  found
			node->secondary_root = updateSecondaryTree(node->secondary_root, name,data);
		}
		return node;
	};
	SecondaryNode* updateSecondaryTree(SecondaryNode* node, std::string name,int data){
		if (node == NULL) return NULL;
		
		// recur down the tree
		if(compareLexicographically(name, node->key) == -1) {
			node->left = updateSecondaryTree(node->left,name,data);
		} 
		else if(compareLexicographically(name, node->key) == 1) {
			node->right = updateSecondaryTree(node->right,name,data);
		}
		else{
			//  found
			node->data = data;
		}
		return node;
	};
	PrimaryNode* remove(PrimaryNode* node, std::string category, std::string name){
		// If the subtree is empty, the key does not exist in the tree
		if (node == NULL) return NULL;
		// recur down the tree
		if(compareLexicographically(category, node->key) == -1) {
			node->left = remove(node->left, category,name);
		} 
		else if(compareLexicographically(category, node->key) == 1) {
			node->right = remove(node->right, category,name);
		}
		else{
			//  found
			node->secondary_root = removeSecondaryTree(node->secondary_root, name);
		}
		return node;
	};
	SecondaryNode* removeSecondaryTree(SecondaryNode* node, std::string name){
		if (node == NULL) return NULL;
		
		// recur down the tree
		if(compareLexicographically(name, node->key) == -1) {
			node->left = removeSecondaryTree(node->left,name);
		} 
		else if(compareLexicographically(name, node->key) == 1) {
			node->right = removeSecondaryTree(node->right,name);
		}
		else{
			//  found
			// If the secondary BST of the root has only one node, simply remove it
			if (node->left == NULL && node->right == NULL)
			{
				delete node;
				node = NULL;
			}

			// If the secondary BST of the root has two children, find the inorder successor and remove it
			else if (node->left != NULL && node->right != NULL)
			{
				SecondaryNode* successor = node->right;
				while (successor->left != NULL)
					successor = successor->left;

				node->key = successor->key;
				delete successor;
			}

			// If the secondary BST of the root has only one child, remove the root and replace it with its child
			else {
				SecondaryNode* child;
				if(node->left != NULL){
					child = node->left;
				}else {
					child = node->right;
				}
				delete node;
				node = child;
			}	
		}
		return node;
	}
	

  searchResult* search(PrimaryNode* node, std::string category,std::string name) {
    // If the subtree is empty, the key does not exist in the tree
    if (node == NULL) return NULL;
		searchResult* result = new(searchResult);
    // If the key is at the root, we have found it
    if ((compareLexicographically(category, node->key) == 0)) {
			result->primaryNode = node;
			result->secondaryNode = searchSecondTree(node->secondary_root, name);
			return result;
		}
    // Otherwise, recur down the correct subtree
    if ((compareLexicographically(category, node->key) == -1)) {
      return search(node->left, category,name);
    } else {
      return search(node->right, category,name);
    }
    }
    PrimaryNode* searchCategory(PrimaryNode* node, std::string category){
        if(node == NULL) return NULL;
        if ((compareLexicographically(category, node->key) == 0)) {
            return node;
        }
        else if ((compareLexicographically(category, node->key) == -1)) {
            return searchCategory(node->left, category);
        } else {
            return searchCategory(node->right, category);
        }
    } 
	SecondaryNode* searchSecondTree(SecondaryNode* node, std::string key) {
		// If the subtree is empty, the key does not exist in the tree
		if (node == NULL ) return NULL;

		// If the key is at the root, we have found it
		if ((compareLexicographically(key, node->key) == 0)) {
			return node;
		}

		// Otherwise, recur down the correct subtree
		if ((compareLexicographically(key, node->key) == -1)) {
			return searchSecondTree(node->left, key);
		} else {
			return searchSecondTree(node->right, key);
		}
	}
	
	void printAllItems(PrimaryNode* node, std::ofstream* file_out){
		if (node == NULL) return;
		std::queue<PrimaryNode*> q;
		q.push(node);
		while (!q.empty()) {
			int levelSize = q.size();
			for (int i = 0; i < levelSize; i++) {
				PrimaryNode* curr = q.front();
				q.pop();
				if (curr->secondary_root != NULL){
					*file_out <<"\""<< curr->key << "\":" << std::endl;
				}
				else{
					*file_out <<"\""<< curr->key << "\":" << "{}" <<std::endl;
				}
				printSecondaryTree(curr->secondary_root, file_out);
				if (curr->left) {
					q.push(curr->left);
				}
				if (curr->right) {
					q.push(curr->right);
				}
			}
		}
	};
	// Mark all the vertices as not visited
	void printSecondaryTree(SecondaryNode* node, std::ofstream *file_out){
		if (node == NULL) return;
		std::queue<SecondaryNode*> q;
		q.push(node);
		while (!q.empty()) {
			int levelSize = q.size();
			for (int i = 0; i < levelSize; i++) {
				SecondaryNode* curr = q.front();
				q.pop();
				*file_out <<"\t\""<< curr->key << "\":\"" << curr->data << "\"";
				if (curr->left) {
					q.push(curr->left);
				}
				if (curr->right) {
					q.push(curr->right);
				}
			}
			*file_out << std::endl;
		}
	};
	void printAllItemsInCategory(std::string category, std::ofstream* file_out){
		PrimaryNode* node = searchCategory(root, category);
		if (node == NULL) return;
		if(node->secondary_root == NULL){
			*file_out <<"\""<< node->key << "\": {}" << std::endl;
		}
		else{
			*file_out <<"\""<< node->key << "\":" << std::endl;
			printSecondaryTree(node->secondary_root, file_out);
		}
		
	}
	
	int printItem(std::string category,std::string name, std::ofstream* file_out ){
		if(category == "" || name == "") return 0;
		searchResult* result = search(root, category, name);
		if (result == NULL || result->primaryNode == NULL || result->secondaryNode == NULL) return 0;
		*file_out <<"{"<<std::endl<<"\""<< category << "\":" << std::endl;
		*file_out <<"\t\""<< name<< "\":\"" << result->secondaryNode->data << "\""<< std::endl; ;
		return 1;
	};
	
};