#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cctype>
#include <cstring>

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

