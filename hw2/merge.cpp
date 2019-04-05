#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <deque>
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
    std::vector<Node*>  _indexes;
};

/********************************
            Class　Node
*********************************/
Node::Node(){
    _c = 'R';
    _level = 0;
    _children.assign(26, NULL);
    _children_stack.reserve(16);
}

Node::Node(char c, Node* p){
    _c = c;
    _level = p->getLevel()+1;
    _children.assign(26, NULL);
    _children_stack.reserve(16);
    _ancestors.assign((int)ceil(log2(_level)) + 1, NULL);
    p->_children[(unsigned int)c-OFFSET] = this;
    p->_children_stack.push_back(this);
}

Node::~Node(){
    std::vector<Node*>::iterator it;
    for (it=_children_stack.begin(); it!=_children_stack.end(); ++it)
        delete *it;
}

void Node::show(){
	std::vector<char> stack;
	Node* tmp = this;
	while (true){
		stack.push_back(tmp->getCh());
		if (tmp->getParent() == NULL) break;
		tmp = tmp->getParent();
	}
	while (!stack.empty()){
		std::cout << stack.back();
		stack.pop_back();
	}
}

void Node::showAncestors(){
    show(); std::cout << ':';
    std::vector<Node*>::iterator it;
    for (it=_ancestors.begin(); it!=_ancestors.end(); ++it)
        std::cout << ' ' << (*it)->getCh();
    std::cout << std::endl;
    for (it=_children_stack.begin(); it!=_children_stack.end(); ++it)
        (*it)->showAncestors();
}


/*************************************
            Class　Container
**************************************/
Container::Container(int n){
    _indexes.reserve(n);
    _indexes.push_back(new Node);
}

Container::~Container(){
    //std::cout << "Calling Container destructor!\n";
    delete _indexes[0];
}

void Container::insert(int idx, char c){
    Node* parent_node = _indexes[idx];
    if (parent_node->_children[(unsigned int)c-OFFSET] != NULL){
        _indexes.push_back(parent_node->_children[(unsigned int)c-OFFSET]);
    }
    else {
        _indexes.push_back(new Node(c, parent_node));
    }
}

void Container::init(){
    dfs(_indexes[0]);
}

int Container::getLCP(int i, int j){
    Node *nodeA = _indexes[i], *nodeB = _indexes[j];
    if (nodeA->getLevel() > nodeB->getLevel()) std::swap(nodeA, nodeB);
    int nodeA_level = nodeA->getLevel();
    int it = (int)log2(nodeB->getLevel()-nodeA_level) + 1;
    for (; it>=0; --it){
        if (nodeB->getLevel() > nodeA_level && nodeB->_ancestors[it]->getLevel() >= nodeA_level){
            nodeB = nodeB->_ancestors[it];
            it = std::min((int)nodeB->_ancestors.size(), it);
        }
    }
    it = nodeA->_ancestors.size()-1;
    for (; it>=0; --it){
        if (nodeA->_ancestors[it] != nodeB->_ancestors[it]){
            nodeA = nodeA->_ancestors[it], nodeB = nodeB->_ancestors[it];
            it = std::min((int)nodeB->_ancestors.size(), it);
        }
    }
    if (nodeA != nodeB) return nodeA->_ancestors[0]->getLevel();
    return nodeA->getLevel();
}

void Container::showTries(){
    std::deque<Node*> tmpQue;
    tmpQue.push_back(_indexes[0]);
    int current_level = -1;
    Node* current_node;
    while(!tmpQue.empty()){
        current_node = tmpQue.front();
        tmpQue.pop_front();
        if (current_node->getLevel() != current_level){
            std::cout << "\nLevel " << ++current_level << ": ";
        }
		current_node->show();
		std::cout << ' ';
        std::vector<Node*>::iterator it;
        for (it=current_node->_children_stack.begin(); it!=current_node->_children_stack.end(); ++it)
            tmpQue.push_back(*it);
    }
    std::cout << std::endl;
}

void Container::showAll(){
    std::cout << "index, level, char\n";
    Node* tmp;
    for (int i=0; i<_indexes.size(); ++i){
        tmp = _indexes[i];
        std::cout << std::setw(5) << i << ", " << std::setw(5) << tmp->getLevel() << ", ";
		tmp->show();
		std::cout << '\n';
    }
    std::cout << std::endl;
}

void Container::showAncestors(){
    _indexes[0]->showAncestors();
}

void Container::dfs(Node* n){
    for (int i=1; i<n->_ancestors.size(); ++i){
        if (i-1 >= n->_ancestors[i-1]->_ancestors.size())
            n->_ancestors[i] = _indexes[0];
        else
            n->_ancestors[i] = n->_ancestors[i-1]->_ancestors[i-1];
    }
    std::vector<Node*>::iterator it;
    for (it=n->_children_stack.begin(); it!=n->_children_stack.end(); ++it){
        (*it)->_ancestors[0] = n;
        dfs(*it);
    }
}

int main(){
    using namespace std;
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, parentI;
    char c;
    cin >> n;
    Container myContainer(n);
    for (int i=0; i<n; ++i){
        cin >> parentI >> c;
        myContainer.insert(parentI, c);
    }
    myContainer.init();
    //myContainer.showTries();
    //myContainer.showAll();
    //myContainer.showAncestors();

    cin >> n;
    int a, b;
    for (int i=0; i<n; ++i){
        cin >> a >> b;
        cout << myContainer.getLCP(a, b) << '\n';
    }
    return 0;
}