#pragma once

#include <string>

using namespace std;

#include "pager.h"

class Table {
public:
    Pager* pager;
    uint32_t root_page_num;

    static Table& openTable(const string& filename);
};
