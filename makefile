build:
	g++ -std=c++20 -Wall library.cpp -o program.out

run:
	./program.out

clean:
	rm -f ./program.out