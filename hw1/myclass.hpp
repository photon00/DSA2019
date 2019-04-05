#include <iostream>
#include <cstdio>
#include <climits>
#include <cstdlib>

class heap_node;

void swap(heap_node &a, heap_node &b);
class node {
public:
	node();
	node(int value);

	node *nxt_node;
	heap_node *p2heap_node;

	int value;
};


class queue {
public:
	queue();
	~queue();
	node *enqueue(int value);
	void dequeue();
	node *getHead(){ return _head; }
	unsigned int get_size(){ return _size; };
	node *get_head(){ return _head; };
	node *get_tail(){ return _tail; };

	void show();
	int check_heap();

	unsigned int _size;
	node *_head;
	node *_tail;
};

class heap_node {
public:
	heap_node();
	int value;
	node *p2node;
};

class maxHeap {
public:
	maxHeap(int size, heap_node *root);
	~maxHeap();
	unsigned int get_size(){ return _size; }
	int getMax(){ return _root[0].value; }
	void maxHeapify(int idx);
	void insert(node *new_node);
	heap_node *erase(int idx);
	void exchange(int idx, node *new_node);

	void show();
	int check();

	heap_node *extract_max();
	void increase(int idx, int new_value);

	unsigned int _size;
	heap_node *_root;
};

class minHeap {
public:
	minHeap(int size, heap_node *root);
	~minHeap();
	unsigned int get_size(){ return _size; }
	int getMin(){ return _root[0].value; }
	void minHeapify(int idx);
	void insert(node *new_node);
	heap_node *erase(int idx);
	void exchange(int idx, node *new_node);

	void show();
	int check();

	void decrease(int idx, int new_value);
	heap_node *extract_min();

	unsigned int _size;
	heap_node *_root;
};

class hybrid {
public:
	hybrid(int k, int M);
	~hybrid();
	void append(int value);
	void slide(int value);
	int get_kth();
	void show();
	void heapSummary();
	void checkquickselect();
	int check();

private:
	void quickselect(int k, heap_node* start, int num);
	void make_heap();
	void mov2max();
	void mov2min();

	int 			_k;
	int				_M;
	unsigned int	_size;
	queue 			_queue;
	heap_node* 		_head;
	maxHeap*		_p_maxHeap;
	minHeap*		_p_minHeap;
};

