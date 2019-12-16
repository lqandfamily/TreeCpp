#ifndef PARENT_TREE_H
#define PARENT_TREE_H
/***********************************************************
 * 孩子兄弟链表作底层的树的类声明
 ***********************************************************/

#include <string>
#include <fstream>
#include <iostream>

#include "ChildBrotherTree.h"

#define FILE_NODE_MAX 20

/**
 * 日志打印控制宏
 */
//普通信息打印，用于输出临时的，不重要的信息
//#define PRINT_GENERAL_INFO

/**
 * 测试函数控制宏
 */
#define TEST_FUN

namespace tree {
    using namespace std;

    /**
     * 节点：双亲表示
     * @tparam T
     */
    template<typename T>
    class ParentNode {
    private:
        T data;
        int parent;
    public:
        void setData(T data) {
            this->data = data;
        }

        T getData() {
            return this->data;
        }

        void setParent(int parent) {
            this->parent = parent;
        }

        int getParent() {
            return parent;
        }
    };

    /**
     * 双亲树
     * @tparam T
     */
    template<typename T>
    class ParentTree {

    private:
        ParentNode<T> *nodeArr;

        int nodeCount = 0;

        /**
         * 读取.tre文件并创建对应的双亲节点数组
         * @param fileFullNam .tre文件全路径(路径 + 文件名.tre)
         */
        bool readFileToParentNodeArr(string fileFullName);

        /**
         * 搜索节点对应的索引
         * @warning 请务必在所有节点都已经存入nodeArr的情况下调用，否则将出现不可预见错误！！！
         * @param parentData
         * @return 未找到索引返回-1
         */
        int getNodeIndex(T searchData);

        /**
         * 搜索当前节点的firstChild节点索引
         * @param curNodeIndex
         * @return 返回firstChild的索引，失败返回-1
        */
        int getFirstChildIndex(int curNodeIndex);

        /**
         * 搜索当前节点的兄弟节点索引
         * @param curNodeIndex
         * @param firstChildIndex 已知的firstChild孩子索引
         * @return 返回brother的索引，失败返回-1
         */
        int getBrotherIndex(int curNodeIndex, int firstChildIndex);

        /**
         * 搜索当前树的根节点的索引
         * @return 返回根节点的索引，失败返回-1
         */
        int getRootIndex();

        /**
         * 从双亲表示中创建一棵孩子兄弟链表表示的树
         * @param curNode 当前节点引用
         * @param curIndex 当前节点索引
         * @return
         */
        bool translateParentToChildAndBrother(ChildBrotherNode<T> *&curNode, int curIndex);

    public:
//        friend class ChildBrotherTree<T>;

        /**
         * 构造函数
         * @param nodeCount 该课树的节点总数，默认为10
         */
        ParentTree(int nodeCount = 10) {
            nodeArr = new ParentNode<T>[nodeCount];
        }

        ParentTree(string fileFullName) {
            nodeArr = new ParentNode<T>[FILE_NODE_MAX];
            readFileToParentNodeArr(fileFullName);
        }

        ~ParentTree() {
            delete nodeArr;
        }


        ChildBrotherTree<T> *createChildBrotherTreeFromFile(string &fileFullName);


#ifdef TEST_FUN

        /**
         * 测试函数：遍历打印节点数据和对应的父节点
         */
        void testPrintNodeDataAndParent() {
            for (int i = 0; i < nodeCount; i++) {
                cout << "节点：" << nodeArr[i].getData() << "--->" << nodeArr[i].getParent()
                     << "==" << nodeArr[nodeArr[i].getParent()].getData() << endl;
            }
        }
    };

#endif

/************************************************************
 * 函数实现
 ************************************************************/
    template<typename T>
    bool ParentTree<T>::readFileToParentNodeArr(string fileFullName) {
        ifstream fileIn(fileFullName);
        char line[50];
        int i = 0;

        //跳过注释行
//        do {
//            fileIn.getline(line, 50);
//        } while (strstr(line, "//") != NULL);

        //读取所有节点数据
        char c[1];
        while (true) {
            fileIn.read(c, 1);
            if (c[0] == ' ') continue;
            if (c[0] == '\r') break;
            nodeArr[i].setData(c[0]);
            //默认父节点置为-1
            nodeArr[i].setParent(-1);
            i++;
#ifdef PRINT_GENERAL_INFO
            cout << "**节点数据" << c[0] << endl;
#endif
        }
        //设置节点总数
        nodeCount = i;

        //建立父节点索引
        T parentData;
        T childData;
        while (!fileIn.eof()) {
            fileIn.getline(line, 5);
            sscanf(line, "%c %c", &parentData, &childData);
            //搜索ChildData的位置并设置对应的父节点编号
            for (i = 0; i < nodeCount; i++) {
                if (nodeArr[i].getData() == childData) {
                    nodeArr[i].setParent(
                            getNodeIndex(parentData));
                }
            }
#ifdef PRINT_GENERAL_INFO
            //            cout << childData << "--->父节点：" << *(childData + i - 1) << endl;
#endif
        }

        fileIn.close();
    }

    template<typename T>
    int ParentTree<T>::getNodeIndex(T searchData) {
        for (int i = 0; i < nodeCount; i++) {
            if (nodeArr[i].getData() == searchData) {
                return i;
            }
        }
        return -1;
    }

    template<typename T>
    ChildBrotherTree<T> *ParentTree<T>::createChildBrotherTreeFromFile(string &fileFullName) {
        //创建双亲表示的树
//        ParentTree<T> parentTree(fileFullName);
        //从双亲表示转化为孩子兄弟链表
#ifdef TEST_FUN
        testPrintNodeDataAndParent();
#endif

        int rooIndex = getRootIndex();
        if (rooIndex == -1) return NULL;
        ChildBrotherTree<T> childBrotherTree;
        ChildBrotherTree<T> *childBrotherTreeP = &childBrotherTree;
        /**
         * 这里应该直接操作childBrotherTree的root，而不是getRoot()
         * 所以用到了友元
         */
        translateParentToChildAndBrother(childBrotherTreeP->root, rooIndex);
        return childBrotherTreeP;
    }

    template<typename T>
    bool ParentTree<T>::translateParentToChildAndBrother(ChildBrotherNode<T> *&curNode, int curIndex) {
        /**
         * 双亲表示转化为兄弟孩子链表表示
         * 1，找到根节点，从根节点开始创建
         * 2，递归创建孩子节点和兄弟节点
         * 2.1 创建过程中要从双亲表示中搜索到孩子节点和兄弟节点
         */
        curNode = new ChildBrotherNode<T>(nodeArr[curIndex].getData());

        int firstChildIndex = getFirstChildIndex(curIndex);
        if (firstChildIndex != -1) {
            translateParentToChildAndBrother(curNode->firstChild, firstChildIndex);
        }
        int brotherIndex = getBrotherIndex(curIndex, firstChildIndex);
        if (brotherIndex != -1) {
            translateParentToChildAndBrother(curNode->nextBrother, brotherIndex);
        }
    }

    template<typename T>
    int ParentTree<T>::getRootIndex() {
        for (int i = 0; i < nodeCount; i++) {
            if (nodeArr[i].getParent() == -1) {
                return i;
            }
        }
        return -1;
    }

    template<typename T>
    int ParentTree<T>::getFirstChildIndex(int curNodeIndex) {
        for (int i = 0; i < nodeCount; i++) {
            if (nodeArr[i].getParent() == curNodeIndex) {
                return i;
            }
        }
        return -1;
    }

    template<typename T>
    int ParentTree<T>::getBrotherIndex(int curNodeIndex, int firstChildIndex) {
        for (int i = firstChildIndex + 1; i < nodeCount; i++) {
            if (nodeArr[i].getParent() == firstChildIndex) {
                return i;
            }
        }
        return -1;
    }

}

#endif



