// Copyright 2022 Wook-shin Han

#ifndef WHY_DBMS_PROJECT1_H_
#define WHY_DBMS_PROJECT1_H_
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <variant>
#include <optional>
#include <tuple>
#include <unordered_map>

enum Status {
    PASSIVE,
    ACTIVE
};

template<typename T>
class Column {
private:
    std::string name_;
    int max_length_;
    std::vector<std::optional<T>> datas_; // QUESTION : Is this a nice choice?

public:
    using Type = T; // denotes Column's type

    Column(std::string name, int max_length)
        : name_(name), max_length_(max_length) {}

    // parses string into its own data type
    void InsertData(std::string raw_data) {
        std::istringstream is(raw_data);
        Type data;
        is >> data;
        datas_.push_back(data);
    }

    int GetMaxLength() { return max_length_; }
};

template<typename... Types>
class Table {
protected:
    std::string table_name_;
    std::tuple<Column<Types>...> columns_;

    Table(std::string table_name, std::tuple<Column<Types>...> columns)
        : table_name_(table_name), columns_(columns) {}

    std::vector<std::string> Parse(std::string raw_data) {
        std::vector<std::string> record_data;
        int offset = 0;
        std::apply([raw_data, &record_data, offset](auto&& column) mutable {
            int col_max_len = column.GetMaxLength();
            record_data.push_back(raw_data.substr(offset, col_max_len));
            offset = offset + col_max_len + 1;
        }, &columns_);
    }

    void InsertRecord(std::vector<std::string> data) {
        int idx = 0;
        std::apply([data, idx](auto&& column) mutable {
            column.InsertData(data[idx]);
            idx++;
        }, &columns_);
    }

public:
    void Load(std::ifstream& input_stream) {
        std::string line;
        getline(input_stream, line);
        getline(input_stream, line);
 
        while(getline(input_stream, line)) {
            int offset = 0;
            InsertRecord(Parse(line));
        }
    }
};

class CustomerTable 
    : public Table<
        std::string,
        std::string,
        std::string,
        std::string,
        std::string,
        short,
        char,
        short,
        short,
        double,
        int,
        std::string,
        Status> {
public:
    CustomerTable()
        : Table("Customer",
                std::make_tuple(
                    Column<std::string>("UNAME", 20),
                    Column<std::string>("PASSWD", 20),
                    Column<std::string>("LNAME", 20),
                    Column<std::string>("FNAME", 20),
                    Column<std::string>("ADDR", 50),
                    Column<short>("ZONE", 6),
                    Column<char>("SEX", 3),
                    Column<short>("AGE", 6),
                    Column<short>("LIMIT", 6),
                    Column<double>("BALANCE", 10),
                    Column<int>("CREDITCARD", 20),
                    Column<std::string>("EMAIL", 50),
                    Column<Status>("ACTIVE", 6)
                )) {}
};

#endif  // WHY_DBMS_PROJECT1_H_
