all: testScanner

testScanner: main.o parser.o scanner.o
	g++ -g -o testScanner main.o parser.o scanner.o

main.o:
	g++ -g -c main.cpp

parser.o:
	g++ -g -c parser.cpp

scanner.o:
	g++ -g -c scanner.cpp

clean:
	rm *.o testScanner project.fs16
