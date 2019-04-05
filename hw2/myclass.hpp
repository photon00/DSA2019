#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <deque>
#include <tuple>
#include <algorithm>

#define OFFSET 0x61


class Node{
public:
    Node();
    Node(char c, Node *p);
    ~Node();
    int getLevel(){ return _level; }
    char getCh(){ return _c; }
    Node* getParent(){ return _ancestors.empty() ? NULL : _ancestors[0]; };
	
    void show();
    void showAncestors();

    int                 _index;
    std::vector<Node*>  _children;
    std::vector<Node*>  _children_stack;
    std::vector<Node*>  _ancestors;

private:
    char    _c;
    int     _level;
};

class Container{
public:
    Container(int n);
    ~Container();
    void insert(int idx, char c);
    void init();
    int getLCP(int i, int j);

    void showTries();
    void showAll();
    void showAncestors();

private:
    void dfs(Node* n);
    std::vector<Node*>              _indexes;
    int                             _num_node;
    std::vector< std::vector<int> > _DParray;
};
