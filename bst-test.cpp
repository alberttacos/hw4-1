#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // AVL Tree Tests
	AVLTree<uint16_t, uint16_t> testTree;
    testTree.insert(std::make_pair(6, 8));
		testTree.insert(std::make_pair(5, 9));
		testTree.insert(std::make_pair(4, 8));
    // testTree.insert(std::make_pair(3, 8));
    // testTree.insert(std::make_pair(2, 8));
    // testTree.insert(std::make_pair(1, 8));
    
	
	

    cout << "\nAVLTree contents:" << endl;
    
    for( AVLTree<uint16_t, uint16_t>::iterator it = testTree.begin(); it != testTree.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    testTree.print();
    return 0;
}
