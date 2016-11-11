#include "ColumnTable.h"
#include "InterResult.h"

shared_ptr<InterResult> ColumnTable::convertToInterResult() {
  shared_ptr<InterResult> res(new InterResult(this));

  return res;
}

void ColumnTable::loadCSV(const char * path, const char field_terminator) {
  Timer timer;
  timer.start();
//	const char field_terminator = ',';
	const char line_terminator  = '\n';
	const char enclosure_char   = '"';
  int rowCount = 0;
  int size = mColumns.size();

	csv_parser file_parser;

	file_parser.init(path);

	file_parser.set_enclosed_char(enclosure_char, ENCLOSURE_OPTIONAL);
	file_parser.set_field_term_char(field_terminator);
	file_parser.set_line_term_char(line_terminator);

	while(file_parser.has_more_rows())
	{
		unsigned int i = 0;

		csv_row row = file_parser.get_row();

		for (i = 0; i < row.size() && i < size; i++)
		{
      mColumns[i]->addValue(row[i]);
		}
    rowCount++;
  }

  mRowCount = rowCount;

  for(auto c : mColumns) {
    c -> endAddingValues(rowCount);
  }
  
  cout << timer.end() << "s elapsed for building the dictionary" << endl;

  timer.start();
	
  file_parser.init(path);
	while(file_parser.has_more_rows())
	{
		unsigned int i = 0;

		csv_row row = file_parser.get_row();

		for (i = 0; i < row.size() && i < size; i++)
		{
      mColumns[i] -> insertValue(row[i]);
		}
    rowCount++;
  }

  cout << timer.end() << "s elapsed for bitpacking" << endl;
 

  for(auto &column : mColumns) {
    column -> printInfo();
  }
}
