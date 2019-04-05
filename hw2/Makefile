all:
	g++ -g main.cpp myclass.cpp -o main

merge:
	cat myclass.hpp > merge.cpp
	tail -n +2 myclass.cpp >> merge.cpp
	tail -n +2 main.cpp >> merge.cpp

clean:
	rm main