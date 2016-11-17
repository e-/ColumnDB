#include "Column.h"
#include "PackedColumn.h"
#include "UnpackedColumn.h"
#include "TextColumn.h"

const bool Column::IsString(shared_ptr<Column> c) {
  return typeid(*c) == typeid(UnpackedColumn<string>) || typeid(*c) == typeid(PackedColumn<string>) || typeid(*c) == typeid(TextColumn);
}

const bool Column::IsInt(shared_ptr<Column> c) {
  return typeid(*c) == typeid(PackedColumn<int>) || typeid(*c) == typeid(UnpackedColumn<int>);
}
