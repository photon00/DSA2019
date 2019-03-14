/* ******************************************************
 *  This program copy the original value to another array
 *  and perform quickselect at each slide.
 * ******************************************************/

#include <cstdio>
#include <algorithm>
#include <unistd.h>
#include <cstdlib>
#include <cctype>
#include <cstring>

int quickselect(int k, int* start, int num, int p); 
void copy(int*, int*, int);
void qcheck(int* arr, int num, int pivot_idx){
	int tmp = arr[pivot_idx];
	for (int i=0; i<pivot_idx; ++i){
		if (arr[i] > tmp)
			printf("Fuck left side goes wrong at %d, piv at %d\n", i, pivot_idx);
	}
	for (int i=pivot_idx+1; i<num; ++i){
		if (arr[i] < tmp)
			printf("Fuck right side goes wrong at %d, piv at %d\n", i, pivot_idx);
	}
}
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

int n, m, k, *array;
myInput myI(1024);
int main(){
	myI >> n >> m >> k;
	//scanf("%d%d%d", &n, &m, &k);
	array = new int[n];

	for (int i=0; i<n; ++i){
		//scanf("%d", &array[i]);
		myI >> array[i];
	}
	int *clone = new int[m];
	for (int i=0; i<n-m; ++i){
		copy(array+i, clone, m);
		printf("%d\n", quickselect(k, clone, m, 0));
	}
	copy(array+n-m, clone, m);
	printf("%d\n", quickselect(k, clone, m, 1));
	delete [] clone;
	delete [] array;
}

int quickselect(int k, int* start, int num, int p){ 
	int i=0, r=num-1;
	int pivot = start[r];
	for (int j=0; j<r; j++){
		if (start[j] < pivot)
			std::swap(start[i++], start[j]);
	}
	std::swap(start[i], start[r]);
	//qcheck(start, num, i);

	if (i == k-1) return start[k-1];
	if (i > k-1) return quickselect(k, start, i, p);
	return quickselect(k-i-1, &start[i+1], num-i-1, p);
}

void copy(int* array, int* clone, int num){
	for (int i=0; i<num; ++i)
		clone[i] = array[i];
}

