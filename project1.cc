// Copyright 2022 Wook-shin Han

#include <fstream>
#include <functional>
#include <iostream>

#include "project1.h"

bool IsActiveCustomer(const CustomerTable::Customer &c) {
  return !c.at("ACTIVE").compare("1");
}

bool IsZoneDescToronto(ZonecostTable &zonecost_table, std::string zone_id) {
  bool result = false;
  std::map<std::string, std::vector<ZonecostTable::Zonecost>> zones =
      zonecost_table.GroupBy("ZONEID");

  for (auto &z : zones[zone_id]) {
    if (!z["ZONEDESC"].compare("Toronto")) result = true;
  }
  return result;
}

// Customers who are active and live in Toronto
std::vector<std::string> Query1(CustomerTable &customer_table,
                                ZonecostTable &zonecost_table) {
  std::vector<CustomerTable::Customer> active_customers =
      customer_table.Select(IsActiveCustomer);
  std::vector<std::string> lnames;
  for (auto &ac : active_customers) {
    if (IsZoneDescToronto(zonecost_table, ac["ZONE"])) {
      lnames.push_back(ac["LNAME"]);
    }
  }
  return lnames;
}

std::string GetProdDesc(ProductTable &product_table, std::string barcode) {
  std::function<bool(const ProductTable::Product &)> hasSameBarcode =
      [barcode](const ProductTable::Product &p) {
        return !p.at("BARCODE").compare(barcode);
      };
  return product_table.Select(hasSameBarcode).front().at("PRODDESC");
}

// BARCODE, PRODDESC for each product that has been purchased by at least two
// customers
std::vector<std::pair<std::string, std::string>> Query2(
    LineitemTable &lineitem_table, ProductTable &produect_table) {
  std::map<std::string, std::vector<LineitemTable::Lineitem>> lineitems =
      lineitem_table.GroupBy("BARCODE");

  std::vector<std::pair<std::string, std::string>> barcode_proddescs;
  for (const auto &[barcode, items] : lineitems) {
    if (items.size() >= 2) {
      std::string proddesc = GetProdDesc(produect_table, barcode);
      barcode_proddescs.push_back(make_pair(barcode, proddesc));
    }
  }

  return barcode_proddescs;
}

int main(int argc, char **argv) {
  if (std::string(argv[1]) == "q1") {
    std::ifstream customer_file(argv[2]);
    CustomerTable customer_table;
    customer_table.LoadFile(customer_file);
    customer_file.close();

    std::ifstream zonecost_file(argv[3]);
    ZonecostTable zonecost_table;
    zonecost_table.LoadFile(zonecost_file);
    zonecost_file.close();

    std::vector<std::string> lnames = Query1(customer_table, zonecost_table);
    for (auto &ln : lnames) std::cout << ln << std::endl;
  } else if (std::string(argv[1]) == "q2") {
    std::ifstream lineitem_file(argv[2]);
    LineitemTable lineitem_table;
    lineitem_table.LoadFile(lineitem_file);
    lineitem_file.close();

    std::ifstream product_file(argv[3]);
    ProductTable product_table;
    product_table.LoadFile(product_file);
    product_file.close();

    std::vector<std::pair<std::string, std::string>> barcode_proddescs =
        Query2(lineitem_table, product_table);

    for (auto &bp : barcode_proddescs)
      std::cout << std::get<0>(bp) << ' ' << std::get<1>(bp) << std::endl;
  }
  return 0;
}
