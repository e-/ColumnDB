CC=g++
FLAGS= -O2 -std=c++11

all: csv_parser.o ColumnTable.o ServerSocket.o Socket.o LogManager.o
	$(CC) main.cpp -o main -std=c++11 csv_parser.o ColumnTable.o InterResult.o Column.o ServerSocket.o Socket.o LogManager.o $(FLAGS) -pthread
	./main 

ServerSocket.o:
	$(CC) -c ServerSocket.cc $(FLAGS)

Socket.o:
	$(CC) -c Socket.cc $(FLAGS)

ColumnTable.o: ColumnTable.cpp InterResult.cpp Column.cpp TypedColumn.h ColumnTable.h BitPacker.h
	$(CC) -c Column.cpp ColumnTable.cpp InterResult.cpp $(FLAGS)

csv_parser.o: lib/csv_parser.cpp
	$(CC) -c lib/csv_parser.cpp $(FLAGS)

LogManager.o: LogManager.cpp
	$(CC) -c LogManager.cpp $(FLAGS)

clean:
	rm -f *.o
	rm -f *.gch
	rm -f main
