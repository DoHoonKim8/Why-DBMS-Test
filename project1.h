// Copyright 2022 Wook-shin Han

#ifndef WHY_DBMS_PROJECT1_H_
#define WHY_DBMS_PROJECT1_H_
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

using Record = std::unordered_map<std::string, std::string>;

inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](char c) { return !std::isspace(c); })
              .base(),
          s.end());
}

class Column {
 private:
  std::string column_name_;
  int max_length_;
  std::vector<std::string> datas_;

 public:
  Column(std::string column_name, int max_length)
      : column_name_(column_name), max_length_(max_length) {}

  void InsertData(std::string data) {
    rtrim(data);
    datas_.push_back(data);
  }

  std::string GetColumnName() { return column_name_; }

  int GetMaxLength() { return max_length_; }

  std::string GetData(int index) { return datas_[index]; }
};

class Table {
 private:
  std::string table_name_;
  std::vector<Column> columns_;
  int records_size_;

 public:
  Table(std::string table_name, std::vector<Column> columns)
      : table_name_(table_name), columns_(columns), records_size_(0) {}

  std::string GetTableName() { return table_name_; }

  void LoadFile(std::ifstream &stream) {
    std::string line;
    std::getline(stream, line);
    std::getline(stream, line);

    while (std::getline(stream, line)) {
      int offset = 0;
      for (int i = 0; i < (int)columns_.size(); i++) {
        Column &col = columns_[i];
        col.InsertData(line.substr(offset, col.GetMaxLength()));
        offset = offset + col.GetMaxLength() + 1;
      }
      records_size_++;
    }
  }

  std::vector<Record> SelectAll() {
    std::vector<Record> records;

    for (int i = 0; i < records_size_; i++) {
      Record record;
      for (int j = 0; j < (int)columns_.size(); j++) {
        record[columns_[j].GetColumnName()] = columns_[j].GetData(i);
      }
      records.push_back(record);
    }
    return records;
  }

  std::vector<Record> Select(std::function<bool(const Record &r)> pred) {
    std::vector<Record> records = SelectAll();

    std::vector<Record> result;
    for (auto &r : records) {
      if (pred(r)) result.push_back(r);
    }

    return result;
  }

  std::map<std::string, std::vector<Record>> GroupBy(std::string column_name) {
    std::vector<Record> records = SelectAll();

    std::map<std::string, std::vector<Record>> result;
    for (auto &r : records) {
      std::string key = r[column_name];
      result[key].push_back(r);
    }
    return result;
  }
};

class CustomerTable : public Table {
 public:
  CustomerTable()
      : Table("Customer",
              {Column("UNAME", 20), Column("PASSWD", 20), Column("LNAME", 20),
               Column("FNAME", 20), Column("ADDR", 50), Column("ZONE", 6),
               Column("SEX", 3), Column("AGE", 6), Column("LIMIT", 6),
               Column("BALANCE", 10), Column("CREDITCARD", 20),
               Column("EMAIL", 50), Column("ACTIVE", 6)}) {}

  using Customer = Record;
};

class ZonecostTable : public Table {
 public:
  ZonecostTable()
      : Table("Zonecost", {Column("ZONEID", 6), Column("ZONEDESC", 20),
                           Column("PRICE", 6)}) {}

  using Zonecost = Record;
};

class LineitemTable : public Table {
 public:
  LineitemTable()
      : Table("Lineitem", {Column("UNAME", 20), Column("DATE", 10),
                           Column("TIME", 8), Column("BARCODE", 20),
                           Column("QUANTITY", 8), Column("PRICE", 9)}) {}

  using Lineitem = Record;
};

class ProductTable : public Table {
 public:
  ProductTable()
      : Table("Product", {Column("BARCODE", 20), Column("PRICE", 10),
                          Column("PRODDESC", 50), Column("MFG", 20),
                          Column("SUPPLIER", 20), Column("TAXABLE", 7),
                          Column("CATEGORY", 20), Column("SALE_PERCENT", 12)}) {
  }

  using Product = Record;
};

#endif  // WHY_DBMS_PROJECT1_H_
