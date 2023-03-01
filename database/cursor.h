#pragma once

#include <stdlib.h>
#include "table.h"
#include "row.h"

class Cursor {
public:
    Table* table;
    bool end_of_table;
    uint32_t page_num;
    uint32_t row_num;

    Cursor(Table& t, uint32_t page_num, uint32_t row_num);

    void create_new_root(uint32_t root_page_num, uint32_t right_page_num);
    void leaf_node_insert(uint32_t key, Row& value);
    void leaf_node_split_and_insert(uint32_t key, Row& value);
    void cursor_advance();
    // 返回对应的页面偏移量
    void* cursor_value();
    static Cursor& start_table(Table& table);
    static Cursor& table_find(Table& table, uint32_t key);

private:
    static Cursor& internal_node_find(Table& table, uint32_t page_num, uint32_t key);
    static Cursor& leaf_node_find(Table& table, uint32_t page_num, uint32_t key);
};
