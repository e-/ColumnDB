CC=g++
FLAGS= -O2 -std=c++11 

all: csv_parser.o ColumnTable.o ServerSocket.o Socket.o LogManager.o Column.o
	$(CC) main.cpp -o main csv_parser.o ColumnTable.o InterResult.o Column.o ServerSocket.o Socket.o LogManager.o $(FLAGS) -pthread
	./main 

ServerSocket.o: ServerSocket.cc
	$(CC) -c ServerSocket.cc $(FLAGS)

Socket.o: Socket.cc
	$(CC) -c Socket.cc $(FLAGS)

ColumnTable.o: ColumnTable.cpp InterResult.cpp
	$(CC) -c ColumnTable.cpp InterResult.cpp $(FLAGS)

Column.o: Column.cpp 
	$(CC) -c Column.cpp $(FLAGS)

csv_parser.o: lib/csv_parser.cpp
	$(CC) -c lib/csv_parser.cpp $(FLAGS)

LogManager.o: LogManager.cpp
	$(CC) -c LogManager.cpp $(FLAGS)

clean:
	rm -f *.o
	rm -f *.gch
	rm -f main
