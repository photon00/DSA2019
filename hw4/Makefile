all:
	g++ -g -std=c++11 main.cpp myclass.cpp -o main

judge:
	g++ -std=c++11 -O2 main.cpp myclass.cpp -o main

merge:
	cp myclass.hpp merge.cpp
	tail -n +2 myclass.cpp >> merge.cpp
	tail -n +2 main.cpp >> merge.cpp

clean:
	rm main