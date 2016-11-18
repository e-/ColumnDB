CC=g++
FLAGS= -O2

all: csv_parser.o ColumnTable.o ServerSocket.o Socket.o
	$(CC) main.cpp -o main -std=c++11 csv_parser.o ColumnTable.o InterResult.o Column.o ServerSocket.o Socket.o $(FLAGS) -pthread
	./main 

ServerSocket.o:
	$(CC) -c ServerSocket.cc $(FLAGS) -std=c++11

Socket.o:
	$(CC) -c Socket.cc $(FLAGS) -std=c++11

ColumnTable.o: ColumnTable.cpp InterResult.cpp Column.cpp TypedColumn.h ColumnTable.h BitPacker.h
	$(CC) -c Column.cpp ColumnTable.cpp InterResult.cpp $(FLAGS) -std=c++11

csv_parser.o: lib/csv_parser.cpp
	$(CC) -c lib/csv_parser.cpp $(FLAGS) -std=c++11

clean:
	rm -f *.o
	rm -f *.gch
	rm -f main
