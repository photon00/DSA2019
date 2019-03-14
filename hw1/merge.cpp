#include <iostream>
#include <cstdio>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cctype>
#include <cstring>

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

private:
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

private:

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

private:
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

class myInput{
public:
	myInput(int buf_max_size);
	~myInput();
	myInput & operator>>(int &num);
private:
	void read_from_input();

	char 	*_buf;
	char	*_tmp;
	int 	_tmp_size;
	size_t 	_current_idx;
	size_t 	_buf_max_size;
	ssize_t _buf_size;
};

void swap(heap_node &a, heap_node &b){
	int tmp = a.value;
	a.value = b.value;
	b.value = tmp;

	heap_node *tmp_hp = a.p2node->p2heap_node;
	a.p2node->p2heap_node = b.p2node->p2heap_node;
	b.p2node->p2heap_node = tmp_hp;

	node *tmp_p = a.p2node;
	a.p2node = b.p2node;
	b.p2node = tmp_p;
}


/***************************************
			Class node
***************************************/
node::node(){
	value = 0;
	nxt_node = NULL;
	p2heap_node = NULL;
}

node::node(int v){
	value = v;
	nxt_node = NULL;
	p2heap_node = NULL;
}


/***************************************
			Class queue
***************************************/
queue::queue(){
	_size = 0;
	_head = NULL;
	_tail = NULL;
}

queue::~queue(){
	node *tmp = _head;
	node *tmp_nxt;
	for (int i=0; i<_size; ++i){
		tmp_nxt = tmp->nxt_node;
		delete tmp;
		tmp = tmp_nxt;
	}
}

node *queue::enqueue(int value){
	node *p_new = new node(value);
	if (_head == NULL || _tail == NULL){
		_head = p_new;
		_tail = p_new;
	}
	else {
		node *p_tail = _tail;
		p_tail->nxt_node = p_new;
		_tail = p_new;
	}
	_size++;
	return p_new;
}

void queue::dequeue(){
	if (_size > 1){
		node *new_head = _head->nxt_node;
		delete _head;
		_head = new_head;
		_size--;
	}
	else if (_size == 1){
		delete _head;
		_head = _tail = NULL;
		_size--;
	}
	else {
		std::cerr << "The queue is already empty!" << std::endl;
		exit(-1);
	}
}

void queue::show(){
	node *tmp = _head;
	printf("queue contain values:");
	for (int i=0; i<_size; ++i){
		printf(" %d", tmp->p2heap_node->value);
		tmp = tmp->nxt_node;
	}puts("");
}

int queue::check_heap(){
	printf("Start checking queue node mapping to heap node ... ");
	node *current_node = _head;
	int flag, wrong = 0, i = 0;
	for (; i<_size; ++i){
		flag = 0;
		if (current_node->p2heap_node->p2node != current_node){
			flag = 1;
		}
		if (flag > 0){
			std::cerr << "\n\tERROR queue doesn't map to heap at " << i << std::endl;
			wrong = 1;
		}
		current_node = current_node->nxt_node;
	}
	if(!wrong) printf("PASS!\n");
	else return -1;
	return 0;
}


/***************************************
			Class heap_node
***************************************/
heap_node::heap_node(){
	value = 0;
	p2node = NULL;
}


/***************************************
			Class maxHeap
***************************************/
maxHeap::maxHeap(int size, heap_node *root){
	_root = root;
	_size = size;
	for (int i=size-1; i>=0; --i)
		maxHeapify(i);
}

maxHeap::~maxHeap(){
	// delete [] _root;	
}

void maxHeap::maxHeapify(int idx){
///////////////////////////////////
//	For index starts from 0:     //
//		left child => 2*idx+1    //
//		right child => 2*idx+2   //
//		parent => (idx-1)>>1     //
///////////////////////////////////
	int tmp = idx;
	while (true){
		if (2*tmp+2 < _size){
			if (_root[tmp].value < _root[2*tmp+1].value && 
					_root[2*tmp+1].value >= _root[2*tmp+2].value){
				swap(_root[2*tmp+1], _root[tmp]);
				tmp = 2*tmp+1;
			}
			else if (_root[tmp].value < _root[2*tmp+2].value && 
					_root[2*tmp+1].value <= _root[2*tmp+2].value){
				swap(_root[2*tmp+2], _root[tmp]);
				tmp = 2*tmp+2;
			}
			else break;
		}
		else if (2*tmp+1 < _size){
			if (_root[tmp].value < _root[2*tmp+1].value){
				swap(_root[2*tmp+1], _root[tmp]);
				tmp = 2*tmp+1;
			}
			else break;
		}
		else break;
	}
}

void maxHeap::insert(node *new_node){
	_root[_size].value = new_node->value;  // assign value to heap node
	_root[_size].p2node = new_node;
	new_node->p2heap_node = &_root[_size++];
	int tmp = _size - 1;
	while (tmp > 0){
		if (_root[(tmp-1)>>1].value >= _root[tmp].value) break;
		else {
			swap(_root[(tmp-1)>>1], _root[tmp]);
			tmp = (tmp-1) >> 1;
		}
	}
}

heap_node *maxHeap::erase(int idx){
	increase(idx, INT_MAX);
	return extract_max();
}

void maxHeap::exchange(int idx, node *new_node){
	_root[idx].value = new_node->value;
	_root[idx].p2node = new_node;
	new_node->p2heap_node = &_root[idx];
	if (idx > 0 && _root[idx].value > _root[(idx-1)>>1].value){
		int tmp = idx;
		while (tmp > 0){
			if (_root[(tmp-1)>>1].value >= _root[tmp].value) break;
			else {
				swap(_root[(tmp-1)>>1], _root[tmp]);
				tmp = (tmp-1) >> 1;
			}
		}
	}
	else maxHeapify(idx);
}

void maxHeap::show(){
	printf("maxHeap contains values:");
	for (int i=0; i<_size; ++i){
		printf(" %d", _root[i].value);
	}puts("");
}

int maxHeap::check(){
	printf("Start checking heap ordering ... ");
	int flag = 0;
	for (int i=0; i<_size; ++i){
		if ((2*i+1) < _size && _root[2*i+1].value > _root[i].value){
			std::cerr << "WRONG at " << i << " " << 2*i+1 << std::endl;
			flag = 1;
			break;
		}
		else if ((2*i+2) < _size && _root[2*i+2].value > _root[i].value){
			std::cerr << "WRONG at " << i << " " << 2*i+2 << std::endl;
			flag = 1;
			break;
		}
	}
	if (!flag) std::cout << "PASS!!" << std::endl;
	else return -1;
	return 0;
}

void maxHeap::increase(int idx, int new_value){
	if (new_value < _root[idx].value){
		std::cerr << "You need to increase the value!!" << std::endl;
		return;
	}
	_root[idx].value = new_value;  // assign value to heap node
	int tmp = idx;
	while (tmp > 0){
		if (_root[(tmp-1)>>1].value >= _root[tmp].value) break;
		else {
			swap(_root[(tmp-1)>>1], _root[tmp]);
			tmp = (tmp-1) >> 1;
		}
	}
}

heap_node *maxHeap::extract_max(){
	swap(_root[0], _root[--_size]);
	heap_node *return_value = &_root[_size];  // use default constructor
	maxHeapify(0);
	return return_value;
}


/***************************************
			Class minHeap
***************************************/
minHeap::minHeap(int size, heap_node *root){
	_root = root;
	_size = size;
	for (int i=size-1; i>=0; --i)
		minHeapify(i);
}

minHeap::~minHeap(){
	//delete [] _root;	
}

void minHeap::minHeapify(int idx){
///////////////////////////////////
//	For index starts from 0:     //
//		left child => 2*idx+1    //
//		right child => 2*idx+2   //
//		parent => (idx-1)>>1     //
///////////////////////////////////
	int tmp = idx;
	while (true){
		if (2*tmp+2 < _size){
			if (_root[tmp].value > _root[2*tmp+1].value && 
					_root[2*tmp+1].value <= _root[2*tmp+2].value){
				swap(_root[2*tmp+1], _root[tmp]);
				tmp = 2*tmp+1;
			}
			else if (_root[tmp].value > _root[2*tmp+2].value && 
					_root[2*tmp+1].value >= _root[2*tmp+2].value){
				swap(_root[2*tmp+2], _root[tmp]);
				tmp = 2*tmp+2;
			}
			else break;
		}
		else if (2*tmp+1 < _size){
			if (_root[tmp].value > _root[2*tmp+1].value){
				swap(_root[2*tmp+1], _root[tmp]);
				tmp = 2*tmp+1;
			}
			else break;
		}
		else break;
	}
}

void minHeap::insert(node *new_node){
	_root[_size].value = new_node->value;  // assign value to heap node
	_root[_size].p2node = new_node;
	new_node->p2heap_node = &_root[_size++];
	int tmp = _size - 1;
	while (tmp > 0){
		if (_root[(tmp-1)>>1].value <= _root[tmp].value) break;
		else {
			swap(_root[(tmp-1)>>1], _root[tmp]);
			tmp = (tmp-1) >> 1;
		}
	}
}

heap_node *minHeap::erase(int idx){
	decrease(idx, -1);
	return extract_min();
}

void minHeap::exchange(int idx, node *new_node){
	_root[idx].value = new_node->value;
	_root[idx].p2node = new_node;
	new_node->p2heap_node = &_root[idx];
	if (idx > 0 && _root[idx].value < _root[(idx-1)>>1].value){
		int tmp = idx;
		while (tmp > 0){
			if (_root[(tmp-1)>>1].value <= _root[tmp].value) break;
			else {
				swap(_root[(tmp-1)>>1], _root[tmp]);
				tmp = (tmp-1) >> 1;
			}
		}
	}
	else minHeapify(idx);
}

void minHeap::show(){
	printf("minHeap contains values:");
	for (int i=0; i<_size; ++i){
		printf(" %d", _root[i].value);
	}puts("");
}

int minHeap::check(){
	printf("Start checking heap ordering ... ");
	int flag = 0;
	for (int i=0; i<_size; ++i){
		if ((2*i+1) < _size && _root[2*i+1].value < _root[i].value){
			std::cerr << "WRONG at " << i << " " << 2*i+1 << std::endl;
			flag = 1;
			break;
		}
		else if ((2*i+2) < _size && _root[2*i+2].value < _root[i].value){
			std::cerr << "WRONG at " << i << " " << 2*i+2 << std::endl;
			flag = 1;
			break;
		}
	}
	if (!flag) std::cout << "PASS!!" << std::endl;
	else return -1;
	return 0;
}

void minHeap::decrease(int idx, int new_value){
	if (new_value > _root[idx].value){
		std::cerr << "You need to decrease the value!!" << std::endl;
		return;
	}
	_root[idx].value = new_value;  // assign value to heap node
	int tmp = idx;
	while (tmp > 0){
		if (_root[(tmp-1)>>1].value <= _root[tmp].value) break;
		else {
			swap(_root[(tmp-1)>>1], _root[tmp]);
			tmp = (tmp-1) >> 1;
		}
	}
}

heap_node *minHeap::extract_min(){
	heap_node *return_value = &_root[--_size];  // use default constructor
	swap(_root[0], _root[_size]);
	minHeapify(0);
	return return_value;
}


/***************************************
			Class hybrid
***************************************/
hybrid::hybrid(int k, int M){
	_head = new heap_node[M];
	_k = k;
	_M = M;
	_size = 0;
	_p_maxHeap = NULL;
	_p_minHeap = NULL;
}

hybrid::~hybrid(){
	delete [] _head;
	if (_p_maxHeap != NULL) delete _p_maxHeap;
	if (_p_minHeap != NULL) delete _p_minHeap;
	//_queue.~queue();
}

void hybrid::append(int value){
	if (_size >= _M){
		std::cerr << "The container is already full, you should call slide()" << std::endl;
		exit(-1);
	}
	node *new_node = _queue.enqueue(value);
	_head[_size].value = value;
	_head[_size].p2node = new_node;
	new_node->p2heap_node = &_head[_size++];
}

void hybrid::slide(int value){
	heap_node *pop_hp = _queue.getHead()->p2heap_node;
	int idx = pop_hp - _head;
	node *new_node = _queue.enqueue(value);
	if (idx < _k) {
		_p_maxHeap->exchange(idx, new_node);
		if (_p_minHeap->get_size() && _p_maxHeap->getMax() > _p_minHeap->getMin()){
			swap(_head[0], _head[_k]);
			_p_minHeap->minHeapify(0);
		}
	}
	else {
		_p_minHeap->exchange(idx-_k, new_node);
		if (_p_maxHeap->getMax() > _p_minHeap->getMin()){
			swap(_head[0], _head[_k]);
			_p_maxHeap->maxHeapify(0);
		}	
	}
	_queue.dequeue();
}

int hybrid::get_kth(){
	if (_p_maxHeap == NULL || _p_minHeap == NULL)
		make_heap();
	//heapSummary();
	return _head[0].value;
}

void hybrid::show(){
	_queue.show();
	printf("Hybrid contains %d elements\n", _size);
	printf("overall heap contains values:\t");
	for (int i=0; i<_k; ++i)
		printf(" %d", _head[i].value);
	printf("\t|\t");
	for (int i=_k; i<_M; ++i)
		printf(" %d", _head[i].value);
	puts("");
}

void hybrid::heapSummary(){
	if (_p_maxHeap == NULL || _p_minHeap == NULL){
		std::cerr << "Heap is not yet built!!" << std::endl;
	}
	else {
		printf("=================================================\n");
		show();
		printf("Max Heap contains %d elements\n", _p_maxHeap->get_size());
		_p_maxHeap->show();
		printf("Min Heap contains %d elements\n", _p_minHeap->get_size());
		_p_minHeap->show();
		printf("=================================================\n");
	}
}

void hybrid::quickselect(int k, heap_node* start, int num){ 
	int i=0, r=num-1;
	swap(start[rand()%num], start[r]);
	int pivot = start[r].value;
	for (int j=0; j<r; j++){
		if (start[j].value < pivot)
			swap(start[i++], start[j]);
	}
	swap(start[i], start[r]);

	if (i == k-1) return;
	if (i > k-1) return quickselect(k, start, i);
	return quickselect(k-i-1, &start[i+1], num-i-1);
}

void hybrid::checkquickselect(){
	int flag = 0;
	for (int i=0; i<_k-1; ++i){
		if (_head[i].value > _head[_k-1].value){
			std::cerr << "Left with ERROR at " << i << std::endl;
			flag = 1;
		}
	}
	for (int i=_k; i<_M; ++i){
		if (_head[i].value < _head[_k-1].value){
			std::cerr << "Right with ERROR at " << i << std::endl;
			flag = 1;
		}
	}
	if (!flag) printf("quick select PASS!!\n");
	else exit(-1);
}

int hybrid::check(){
	int flag = 0;
	flag += _queue.check_heap();
	flag += _p_maxHeap->check();
	flag += _p_minHeap->check();
	if (_p_maxHeap->get_size() && _p_minHeap->get_size()){
		if (_p_maxHeap->getMax() > _p_minHeap->getMin())
			std::cerr << "The hybrid heap's order go wrong" << std::endl;
			flag += -1;
	}
	if (flag) exit(-1);
	return 0;
}

void hybrid::make_heap(){
	if (_size < _M){
		std::cerr << "You should append more elements(" << _size << '/' << _M << ')' << std::endl;
		exit(-1);
	}
	//show();
	quickselect(_k, _head, _size);
	//checkquickselect();
	//show();
	_p_maxHeap = new maxHeap(_k, _head);
	_p_minHeap = new minHeap(_size-_k, &_head[_k]);
	//heapSummary();
	//check();
}

void hybrid::mov2max(){
	heap_node *tmp = _p_minHeap->extract_min();
	_p_maxHeap->insert(tmp->p2node);
}

void hybrid::mov2min(){
	heap_node *tmp = _p_maxHeap->extract_max();
	_p_minHeap->insert(tmp->p2node);
}


/***********************************
 			Class MyInput
************************************/
myInput::myInput(int buf_max_size){
	_buf_size = 0;	
	_current_idx = 0;
	_buf_max_size = (size_t)buf_max_size;
	_buf = new char[buf_max_size];
	_tmp = new char[11];
	_tmp_size = 0;
}

myInput::~myInput(){
	delete [] _buf;
	delete [] _tmp;
}

void myInput::read_from_input(){
	if (_current_idx != 0 && isdigit(_buf[_buf_size-1])){
		int count = 0, idx = _buf_size - 1;
		while(isdigit(_buf[idx])){
			++count;
			--idx;
		}
		strncpy(_tmp, _buf+idx+1, count);
		_tmp_size = count;
	}
	_buf_size = read(0, _buf, _buf_max_size);
	_current_idx = 0;
	if (_buf_size <= 0){
		fprintf(stderr, "input ERROR!\n");
		exit(-1);
	}
}

myInput & myInput::operator>>(int &num){
	// TODO continuing from input
	while (isspace(_buf[_current_idx]) && _current_idx < _buf_size) ++_current_idx;
	if (_current_idx >= (size_t)_buf_size) read_from_input();
	size_t start_idx = _current_idx;
	while (isdigit(_buf[_current_idx])){ ++_current_idx; }
	if (_current_idx == _buf_size) {
		read_from_input();
	}
	if (_tmp_size != 0){
		while (!isspace(_buf[_current_idx])){ ++_current_idx; }
		strncpy(_tmp+_tmp_size, _buf, _current_idx);
		_tmp[_tmp_size+(_current_idx++)] = '\0';
		num = std::atoi(_tmp);
		_tmp_size = 0;
	}
	else {
		_buf[_current_idx++] = '\0';
		num = std::atoi(_buf+start_idx);
	}
	return *this;
}


int main(){
	using namespace std;
	ios_base::sync_with_stdio(false);
	cin.tie(0);

	int n, m, k;
	myInput myI(1024);
	cin >> n >> m >> k;
	hybrid myHybrid(k, m);
	int num;
	for (int i=0; i<m; ++i){
		cin >> num;
		myHybrid.append(num);
	}
	cout << myHybrid.get_kth() << '\n';
	for (int i=0; i<n-m; ++i){
		cin >> num;
		myHybrid.slide(num);
		cout << myHybrid.get_kth() << '\n';
	}
	return 0;	
}
