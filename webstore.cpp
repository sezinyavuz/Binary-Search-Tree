#include <iostream>
#include <vector>
#include "util.h"
#include "webstore.hpp"
#include "tree.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
int main(int argc, char **argv) {
	char *input_file = argv[1];
  	char *output1_file = argv[2]; 
	char *output2_file = argv[3]; 
	std::fstream fp;
	std::ofstream* file1_out = new std::ofstream();
	std::ofstream* file2_out = new std::ofstream();
	fp.open(input_file, std::ios::in);
	file1_out->open(output1_file, std::ios_base::out);
	file2_out->open(output2_file, std::ios_base::out);
	if(fp.fail()) error_exit("Failed to open input file \"%s\"\n", input_file);
	if(file1_out->fail()) error_exit("Failed to open output file \"%s\"\n", output1_file);
	if(file2_out->fail()) error_exit("Failed to open output file \"%s\"\n", output2_file);
	std::string line; 
	InputLine input;
	Item new_item;
	int ch;
	TwoPhaseBST tree;
	TwoPhaseBST rbl_tree;
	tree.root = NULL;
	tree.tree_type = 0;
	rbl_tree.root = NULL;
	rbl_tree.tree_type = 1;
	while((ch = fp.get()) != EOF){ // satır satır oku
		std::getline(fp, line);
		std::istringstream in(line);  //satır için stream yarat
		std::vector<std::string> vect = StringToVector(line, '\t');
		input.command = vect[0];
		if(vect.size() > 1)
			input.category = vect[1];
		if(vect.size() > 2)
			input.name = vect[2];
		if(vect.size() > 3)
			input.price = std::stoi(vect[3]);
		new_item.category = input.category;
		new_item.name = input.name;
		new_item.price = input.price;
		if(input.command == "nsert"){
			tree.root = tree.insert(tree.root, new_item);
			rbl_tree.root = rbl_tree.insert(rbl_tree.root,new_item);
			continue;
			//insert
		}
		else if(input.command == "emove"){
			tree.remove(tree.root, input.category, input.name);
			rbl_tree.remove(rbl_tree.root, input.category, input.name);
			continue;
		}
		else if(input.command == "rintAllItems"){
			//printAllItems
			*file1_out<<"command:printAllItems"<<std::endl<<"{"<<std::endl;
			tree.printAllItems(tree.root,file1_out);
			*file1_out<<"}"<<std::endl;
			*file2_out<<"command:printAllItems"<<std::endl<<"{"<<std::endl;
			rbl_tree.printAllItems(tree.root,file2_out);
			*file2_out<<"}"<<std::endl;
		}
		else if(input.command == "rintAllItemsInCategory"){
			//printAllItemsInCategory
			*file1_out<<"command:printAllItemsInCategory\t"<<input.category<<std::endl<<"{"<<std::endl;
			tree.printAllItemsInCategory(input.category, file1_out);
			*file1_out<<"}"<<std::endl;
			*file2_out<<"command:printAllItemsInCategory\t"<<input.category<<std::endl<<"{"<<std::endl;
			rbl_tree.printAllItemsInCategory(input.category, file2_out);
			*file2_out<<"}"<<std::endl;
		}
		else if(input.command == "rintItem"){
			//printItem
			int success;
			*file1_out<<"command:printItem\t"<<input.category<<"\t"<<input.name<<std::endl;
			success == tree.printItem(input.category,input.name, file1_out);
			if(success == 0){
				*file1_out<<"}"<<std::endl;
			}
			else
				*file1_out<<"{}"<<std::endl;
			*file2_out<<"command:printItem\t"<<input.category<<"\t"<<input.name<<std::endl;
			rbl_tree.printItem(input.category,input.name, file2_out);
			if(success == 0){
				*file2_out<<"}"<<std::endl;
			}
			else
				*file2_out<<"{}"<<std::endl;
		}
		else if(input.command == "ind"){
			//find
			int success;
			*file1_out<<"command:find\t"<<input.category<<"\t"<<input.name<<std::endl;
			searchResult* result;
			result = tree.search(tree.root,input.category,input.name);
			if (result == NULL || result->primaryNode == NULL || result->secondaryNode == NULL){
				*file1_out << "{}" << std::endl;
			}
			else{
				success = tree.printItem(result->primaryNode->key,result->secondaryNode->key,file1_out);
				if(success == 0)
					*file1_out<<"}"<<std::endl;
				else
					*file1_out<<"{}"<<std::endl;
			}
			*file2_out<<"command:find\t"<<input.category<<"\t"<<input.name<<std::endl;
			result = rbl_tree.search(tree.root,input.category,input.name);
			if (result == NULL || result->primaryNode == NULL || result->secondaryNode == NULL){
				*file2_out << "{}" << std::endl;
			}
			else{
				*file2_out<<"{"<<std::endl;
				success = rbl_tree.printItem(result->primaryNode->key,result->secondaryNode->key,file2_out);
				if(success == 0)
					*file2_out<<"}"<<std::endl;
				else
					*file2_out<<"{}"<<std::endl;
			}
		}
		else if(input.command == "pdateData"){
			tree.update(tree.root, input.category, input.name,input.price);
			rbl_tree.update(rbl_tree.root, input.category, input.name,input.price);
		}
		else{
			error_exit("Invalid command \"%s\" in input file \"%s\"\n", input.command.c_str(), input_file);
		}
	}
	
	fp.close();
	file1_out->close();
	file2_out->close();
	return 0;
}
