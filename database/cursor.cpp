#include "cursor.h"
#include "row.h"

Cursor::Cursor(Table& t, uint32_t page_num, uint32_t row_num) {
    this->table = &t;
    this->page_num = page_num;
    this->row_num = row_num;
}

void Cursor::cursor_advance() {
    row_num++;
    void* node = table->pager->get_page(page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    if (row_num >= num_cells) {
        end_of_table = true;
    }
}

void* Cursor::cursor_value() {
    void* page = table->pager->get_page(page_num);
    return leaf_node_cell_value(page, row_num);
}

Cursor& Cursor::start_table(Table& table) {
    uint32_t page_num = 0;
    uint32_t row_num = 0;

    Cursor* cursor = new Cursor(table, page_num, row_num);
    void* node = table.pager->get_page(page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    cursor->end_of_table = num_cells == 0;

    return *cursor;
}
Cursor& Cursor::leaf_node_find(Table& table, uint32_t page_num, uint32_t key) {
    char* node = (char*)table.pager->get_page(page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);

    uint32_t min = 0, max = num_cells;
    while (min != max) {
        uint32_t index = (min + max) / 2;
        uint32_t index_key = *leaf_node_cell_key(node, index);
        if (index_key == key) {
            min = index;
            break;
        } else if (key < index_key) {
            max = index;
        } else {
            min = index + 1;
        }
    }
    Cursor* cursor = new Cursor(table, page_num, min);
    return *cursor;
}
Cursor& Cursor::table_find(Table& table, uint32_t key) {
    uint32_t page_num = table.root_page_num;
    void* node = table.pager->get_page(page_num);

    NODE_TYPE type = get_node_type(node);
    switch (type) {
        case NODE_TYPE_LEAF: return leaf_node_find(table, page_num, key);
        case NODE_TYPE_INTERNAL: {
            printf("Need to implement search in internal!\n");
            exit(1);
        }
        default: {
            printf("Not implement type: %d\n", type);
            exit(1);
        }
    }
}