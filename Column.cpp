#include "Column.h"
#include "PackedColumn.h"
#include "UnpackedColumn.h"

const bool Column::IsString(Column *c) {
  return true;
//  return typeid(*c) == typeid(UnpackedColumn<string>);
}

const bool Column::IsInt(Column *c) {
  return true;
///  return typeid(*c) == typeid(PackedColumn<int>);
}


/*
const bool Column::IsPackedString(Column *c) {
  return typeid(*c) == typeid(PackedColumn<string>);
}

const bool Column::IsUnpackedString(Column *c) {
  return typeid(*c) == typeid(UnpackedColumn<string>);
}

const bool Column::IsPackedInt(Column *c) {
  return typeid(*c) == typeid(PackedColumn<int>);
}

const bool Column::IsUnpackedInt(Column *c) {
  return typeid(*c) == typeid(UnpackedColumn<int>);
}*/
