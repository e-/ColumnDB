CC=g++

all: csv_parser.o ColumnTable.o
	$(CC) main.cpp -o main -std=c++11 csv_parser.o ColumnTable.o InterResult.o Column.o -O2
	./main ../data/data.csv ../data/lineitem.tbl

ColumnTable.o: ColumnTable.cpp InterResult.cpp Column.cpp TypedColumn.h
	$(CC) -c Column.cpp ColumnTable.cpp InterResult.cpp -O2 -std=c++11

csv_parser.o: lib/csv_parser.cpp
	$(CC) -c lib/csv_parser.cpp -O2 -std=c++11

clean:
	rm -f *.o
	rm -f *.gch
	rm -f main
