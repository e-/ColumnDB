all: csv_parser.o ColumnTable.o
	g++ main.cpp -o main -std=c++11 csv_parser.o ColumnTable.o -O2
	./main ../data/data2.csv ../data/lineitem2.tbl

ColumnTable.o: ColumnTable.cpp InterResult.h
	g++ -c ColumnTable.cpp -O2 -std=c++11

csv_parser.o: lib/csv_parser.cpp
	g++ -c lib/csv_parser.cpp -O2 -std=c++11

clean:
	rm main
	rm *.o
	rm *.gch
