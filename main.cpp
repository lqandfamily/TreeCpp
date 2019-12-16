#include <iostream>

#include "ParentTree.h"

using namespace tree;

void testCreateParentTreeFromFile() {
    ParentTree<char> parentTree("/Users/blue/CLionProjects/TreeCpp/tData/test.tre");
    parentTree.testPrintNodeDataAndParent();
}

void testCreateChildBrotherTreeFromFile() {
    string fileUrl = "/Users/blue/CLionProjects/TreeCpp/tData/test.tre";
    ChildBrotherTree<char> *tree = ChildBrotherTree<char>::creatChildBrotherTreeFromFile(fileUrl);
    cout << "test" << endl;
}

int main() {
//    testCreateParentTreeFromFile();
    testCreateChildBrotherTreeFromFile();
    return 0;
}
