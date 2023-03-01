#include <iostream>

using namespace std;

#include "cursor.h"

Cursor::Cursor(Table& t, uint32_t page_num, uint32_t row_num) {
    this->table = &t;
    this->page_num = page_num;
    this->row_num = row_num;
}

void Cursor::leaf_node_split_and_insert(uint32_t key, Row& value) {
    Pager& pager = *(table->pager);
    void* old_node = pager.get_page(page_num);
    uint32_t old_node_max = get_node_max_key(old_node);

    uint32_t new_child_num = pager.get_unused_page_num();
    void* new_child = pager.get_page(new_child_num);
    initialize_leaf(new_child);

    for (int32_t i = LEAF_NODE_MAX_CELL_COUNT; i >= 0; i--) {
        void* destination_node;
        if (i >= LEAF_NODE_SPLIT_LEFT_COUNT) {
            destination_node = new_child;
        } else {
            destination_node = old_node;
        }
        uint32_t index = i % LEAF_NODE_SPLIT_LEFT_COUNT;
        void* destination = leaf_node_cell(destination_node, index);
        if (i == row_num) {
            *leaf_node_cell_key(destination_node, index) = key;
            value.serializeData((char*)leaf_node_cell_value(destination_node, index));
        } else if (i > row_num) {
            memcpy(destination, leaf_node_cell(old_node, i-1), LEAF_NODE_CELL_SIZE);
        } else {
            memcpy(destination, leaf_node_cell(old_node, i), LEAF_NODE_CELL_SIZE);
        }
    }
    *leaf_node_num_cells(old_node) = LEAF_NODE_SPLIT_LEFT_COUNT;
    *leaf_node_num_cells(new_child) = LEAF_NODE_SPLIT_RIGHT_COUNT;

    if (is_node_root(old_node)) {
        create_new_root(page_num, new_child_num);
        return;
    } else {
        cout << "Need implement updating parent node!" << endl;
        exit(1);
    }
}
void Cursor::create_new_root(uint32_t root_page_num, uint32_t right_page_num) {
    void* root = table->pager->get_page(root_page_num);

    uint32_t left_child_page_num = table->pager->get_unused_page_num();
    void* left_child = table->pager->get_page(left_child_page_num);
    memcpy(left_child, root, PAGE_SIZE);
    initialize_internal_node(root);
    set_node_root(root, true);

    *internal_node_num_child(root) = 1;
    *internal_node_right_child(root) = right_page_num;
    *internal_node_child_key(root, 0) = get_node_max_key(left_child);
    *internal_node_child_value(root, 0) = left_child_page_num;
}
void Cursor::leaf_node_insert(uint32_t key, Row& value) {
    void* node = table->pager->get_page(page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    if (num_cells >= LEAF_NODE_MAX_CELL_COUNT) {
        leaf_node_split_and_insert(key, value);
        return;
    }
    if (row_num < num_cells) {
        for (int32_t i = num_cells; i > row_num; i--) {
            memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i-1), LEAF_NODE_CELL_SIZE);
        }
    }

    *leaf_node_num_cells(node) += 1;
    *leaf_node_cell_key(node, row_num) = key;
    value.serializeData((char*)leaf_node_cell_value(node, row_num));
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