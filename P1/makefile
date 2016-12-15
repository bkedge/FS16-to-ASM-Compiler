all: testScanner

testScanner: main.o testScanner.o scanner.o
	g++ -o testScanner main.o testScanner.o scanner.o

main.o:
	g++ -c main.cpp

testScanner.o:
	g++ -c testScanner.cpp

scanner.o:
	g++ -c scanner.cpp

clean:
	rm *.o testScanner project.fs16
