FLAGS = -std=c++11 -Wall -pedantic -g
CPP = g++
objects = main.o mst.o Tour.o nearest_insertion.o nearest_neighbour.o clarke_wright.o tsptools.o

all: main testgen

main: $(objects)
	$(CPP) $(FLAGS) -o main $(objects)

testgen: 
	$(CPP) $(FLAGS) -o tests/testgen tests/testgen.cpp
	
%.o: %.cpp %.hpp
	$(CPP) $(FLAGS) -c $<

clean:
	rm -f main tests/testgen *.o *.exe

tests: main
	./main < tests/kattis
	./main < tests/stacken
	./main < tests/test1
	./main < tests/test2
	./main < tests/test3
	./main < tests/test1000
test: main
	time ./main < tests/test300

memcheck: main
	valgrind ./main < tests/kattis
