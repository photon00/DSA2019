#include "myclass.hpp"

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
	_value = 0;
	pre_node = NULL;
	nxt_node = NULL;
	p2heap_node = NULL;
}

node::node(int value){
	_value = value;
	pre_node = NULL;
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
		p_new->pre_node = p_tail;
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
		printf(" %d", tmp->get_value());
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
	if (2*idx+2 < _size){
		if (_root[idx].value < _root[2*idx+1].value && 
				_root[2*idx+1].value >= _root[2*idx+2].value){
			swap(_root[2*idx+1], _root[idx]);
			maxHeapify(2*idx+1);
		}
		else if (_root[idx].value < _root[2*idx+2].value && 
				_root[2*idx+1].value <= _root[2*idx+2].value){
			swap(_root[2*idx+2], _root[idx]);
			maxHeapify(2*idx+2);
		}
	}
	else if (2*idx+1 < _size){
		if (_root[idx].value < _root[2*idx+1].value){
			swap(_root[2*idx+1], _root[idx]);
			maxHeapify(2*idx+1);
		}
	}
	else return;
}

void maxHeap::insert(node *new_node){
	_root[_size].value = new_node->get_value();  // assign value to heap node
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
	if (2*idx+2 < _size){
		if (_root[idx].value > _root[2*idx+1].value && 
				_root[2*idx+1].value <= _root[2*idx+2].value){
			swap(_root[2*idx+1], _root[idx]);
			minHeapify(2*idx+1);
		}
		else if (_root[idx].value > _root[2*idx+2].value && 
				_root[2*idx+1].value >= _root[2*idx+2].value){
			swap(_root[2*idx+2], _root[idx]);
			minHeapify(2*idx+2);
		}
	}
	else if (2*idx+1 < _size){
		if (_root[idx].value > _root[2*idx+1].value){
			swap(_root[2*idx+1], _root[idx]);
			minHeapify(2*idx+1);
		}
	}
	else return;
}

void minHeap::insert(node *new_node){
	_root[_size].value = new_node->get_value();  // assign value to heap node
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
	_head = new heap_node[M+1];
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
	if (idx < _k) _p_maxHeap->erase(idx);
	else _p_minHeap->erase(idx-_k);
	_queue.dequeue();

	node *new_node = _queue.enqueue(value);
	int size_maxHeap = _p_maxHeap->get_size();
	int size_minHeap = _p_minHeap->get_size();
	if (size_maxHeap == 0 || size_minHeap == 0){
		if (!size_maxHeap && !size_minHeap){
			_p_maxHeap->insert(new_node);
		}
		else if (!size_maxHeap){
			if (value > _p_minHeap->getMin()){
				if (size_minHeap == _M-_k+1) mov2max();
				_p_minHeap->insert(new_node);
			}
			else _p_maxHeap->insert(new_node);
		}
		else {
			if (value < _p_maxHeap->getMax()){
				if (size_maxHeap == _k) mov2min();
				_p_maxHeap->insert(new_node);
			}
			else _p_minHeap->insert(new_node);
		}
	}
	else if (value >= _p_maxHeap->getMax() && value <= _p_minHeap->getMin()){
		if (size_maxHeap < _k)
			_p_maxHeap->insert(new_node);
		else _p_minHeap->insert(new_node);
	}
	else if (value < _p_maxHeap->getMax()){
		if (size_maxHeap == _k) mov2min();
		_p_maxHeap->insert(new_node);
	}
	else {
		if (size_minHeap == _M-_k+1) mov2max();
		_p_minHeap->insert(new_node);
	}
}

int hybrid::get_kth(){
	if (_p_maxHeap == NULL || _p_minHeap == NULL)
		make_heap();
	//heapSummary();
	if (_p_maxHeap->get_size() == _k) return _head[0].value;
	else return _head[_k].value;
}

void hybrid::show(){
	_queue.show();
	printf("Hybrid contains %d elements\n", _size);
	printf("overall heap contains values:\t");
	for (int i=0; i<_k; ++i)
		printf(" %d", _head[i].value);
	printf("\t|\t");
	for (int i=_k; i<_M+1; ++i)
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
