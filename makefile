install:
	mkdir -p /usr/local/include 
	cp argparser.h /usr/local/include 

uninstall:
	rm -rf /usr/local/include/argparser.h 

tests:
	cmake -S ./run-tests -B ./run-tests/build
	cmake --build ./run-tests/build 
	cd ./run-tests/build && ctest 

