#pragma once

#include <string>

using namespace std;

#include "pager.h"

class Table {
public:
    Pager* pager;
    uint32_t num_rows;

    static Table& openTable(const string& filename);
};
