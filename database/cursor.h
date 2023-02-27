#pragma once

#include <stdlib.h>
#include "table.h"

class Cursor {
public:
    Table* table;
    bool end_of_table;
    uint32_t page_num;
    uint32_t row_num;

    Cursor(Table& t, uint32_t page_num, uint32_t row_num);

    void cursor_advance();
    // 返回对应的页面偏移量
    void* cursor_value();

    static Cursor& start_table(Table& table);
    static Cursor& end_table(Table& table);
};
