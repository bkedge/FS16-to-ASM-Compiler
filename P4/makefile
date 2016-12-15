all: comp

comp: main.o parser.o scanner.o treePrint.o semantic.o
	g++ -g -o comp main.o parser.o scanner.o treePrint.o semantic.o

main.o:
	g++ -g -c main.cpp

parser.o:
	g++ -g -c parser.cpp

scanner.o:
	g++ -g -c scanner.cpp

treePrint.o:
	g++ -g -c treePrint.cpp

semantic.o:
	g++ -g -c semantic.cpp

clean:
	rm *.o comp project.fs16
