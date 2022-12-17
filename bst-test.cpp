#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    // BinarySearchTree<char,int> bt;
    // bt.insert(std::make_pair('f',2));
    // bt.insert(std::make_pair('b',1));
    // bt.insert(std::make_pair('g',3));
    // bt.insert(std::make_pair('a',3));
    // bt.insert(std::make_pair('d',3));
    // bt.insert(std::make_pair('i',3));
    // bt.insert(std::make_pair('c',3));
    // bt.insert(std::make_pair('e',3));
    // bt.insert(std::make_pair('h',3));

    BinarySearchTree<int, double> bt;
	bt.insert(std::make_pair(3, 1.0));
	bt.insert(std::make_pair(2, 1.0));
	bt.insert(std::make_pair(1, 1.0));
	//bst.remove(3);
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<int,double>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find(3) != bt.end()) {
        cout << "Found f" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    bt.remove(3);

    // // AVL Tree Tests
    // AVLTree<char,int> at;
    // at.insert(std::make_pair('a',1));
    // at.insert(std::make_pair('b',2));

    // cout << "\nAVLTree contents:" << endl;
    // for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
    //     cout << it->first << " " << it->second << endl;
    // }
    // if(at.find('b') != at.end()) {
    //     cout << "Found b" << endl;
    // }
    // else {
    //     cout << "Did not find b" << endl;
    // }
    // cout << "Erasing b" << endl;
    // at.remove('b');

    return 0;
}
