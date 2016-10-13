all: csv_parser.o
	g++ main.cpp -o main -std=c++11 csv_parser.o
	./main ../data/data2.csv

csv_parser.o: lib/csv_parser.cpp
	g++ -c lib/csv_parser.cpp

clean:
	rm main
	rm *.o
