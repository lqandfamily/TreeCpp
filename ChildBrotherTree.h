#ifndef CHILD_BROTHER_TREE_H
#define CHILD_BROTHER_TREE_H

/***********************************************************
 * 孩子兄弟链表作底层的树的类声明
 ***********************************************************/

#include <string>

#include "ParentTree.h"

/**
 * 节点:孩子兄弟链表
 * @tparam T 元素的数据类型
*/

using namespace std;

namespace tree {
    /**
     * 前置声明，因为ParentTree.h和ChildBrotherTree.h头文件互相包含了
     * emmm....其实我也不清楚这是啥子问题
     * @tparam T
     */
    template<typename T>
    class ParentTree;

    template<typename T>
    class ChildBrotherNode {
    private:
        T data;
        ChildBrotherNode *firstChild;
        ChildBrotherNode *nextBrother;
    public:
        friend class ParentTree<T>;

        ChildBrotherNode(T data) {
            this->data = data;
            this->firstChild = NULL;
            this->nextBrother = NULL;
        }

        ChildBrotherNode() {};


        T getData() {
            return this->data;
        }

        ChildBrotherNode<T> *getFirstChild() {
            return firstChild;
        }

        void setFirstChild(ChildBrotherNode<T> *firstChild) {
            this->firstChild = firstChild;
        }

        ChildBrotherNode<T> *getNextBrother() {
            return nextBrother;
        }

        void setNextBrother(ChildBrotherNode<T> *nextBrother) {
            this->nextBrother = nextBrother;
        }
    };


/**
 * 本例的核心表示方法是孩子兄弟链表法
 * @tparam T
 */
    template<typename T>
    class ChildBrotherTree {
    private:
        ChildBrotherNode<T> *root;

    public:
        friend class ParentTree<T>;

        ChildBrotherTree() {
            root = NULL;
        }

        ChildBrotherTree(std::string fileFullName);

        ~ChildBrotherTree() {};

        ChildBrotherNode<T> *getRoot() {
            return root;
        }

        static ChildBrotherTree *creatChildBrotherTreeFromFile(string fileFullName);

        void setRoot(ChildBrotherNode<T> *root) {
            this->root = root;
        }

    };


/************************************************************
 * 函数实现
 ************************************************************/
    template<typename T>
    ChildBrotherTree<T>::ChildBrotherTree(std::string fileFullName) {
        /**
         *！！！选用构造器是个巨大的错误
         */

    }

    template<typename T>
    ChildBrotherTree<T> *ChildBrotherTree<T>::creatChildBrotherTreeFromFile(std::__1::string fileFullName) {
        ParentTree<T> tree(fileFullName);
        ChildBrotherTree *childBrotherTree = tree.createChildBrotherTreeFromFile(fileFullName);
        return childBrotherTree;
    }
}


#endif
