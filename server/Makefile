CC=g++

all: csv_parser.o ColumnTable.o ServerSocket.o Socket.o
	$(CC) main.cpp -o main -std=c++11 csv_parser.o ColumnTable.o InterResult.o Column.o ServerSocket.o Socket.o -O2 -pthread
	./main 

ServerSocket.o:
	$(CC) -c ServerSocket.cc -O2 -std=c++11

Socket.o:
	$(CC) -c Socket.cc -O2 -std=c++11

ColumnTable.o: ColumnTable.cpp InterResult.cpp Column.cpp TypedColumn.h
	$(CC) -c Column.cpp ColumnTable.cpp InterResult.cpp -O2 -std=c++11

csv_parser.o: lib/csv_parser.cpp
	$(CC) -c lib/csv_parser.cpp -O2 -std=c++11

clean:
	rm -f *.o
	rm -f *.gch
	rm -f main
