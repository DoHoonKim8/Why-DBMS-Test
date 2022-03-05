// Copyright 2022 Wook-shin Han

#ifndef WHY_DBMS_PROJECT1_H_
#define WHY_DBMS_PROJECT1_H_
#include <string>
#include <vector>
#include <variant>
#include <utility>
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

    void InsertData(T data) {
        datas_.push_back(data);
    }
};

template<typename... Types>
class Table {
protected:
    std::string table_name_;
    std::tuple<Column<Types>...> columns_;

    Table(std::string table_name, std::tuple<Column<Types>...> columns)
        : table_name_(table_name), columns_(columns) {}

    using DataType = std::variant<Types...>;

    void InsertRecord(std::vector<DataType> record) {
        for (int i = 0; i < record.size(); i++) {
            columns_[i].InsertData(record[i]);
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
