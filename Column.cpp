#include "Column.h"
#include "PackedColumn.h"
#include "UnpackedColumn.h"

const bool Column::IsString(Column *c) {
  return typeid(*c) == typeid(UnpackedColumn<string>) || typeid(*c) == typeid(PackedColumn<string>);
}

const bool Column::IsInt(Column *c) {
  return typeid(*c) == typeid(PackedColumn<int>) || typeid(*c) == typeid(UnpackedColumn<int>);
}
