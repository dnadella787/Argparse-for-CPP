
default: arg.cpp arg.h parser.cpp parser.h main.cpp
	g++ -std=c++1z arg.cpp parser.cpp main.cpp -o ./arg 