#pragma once

#include <iostream>
using namespace std;

#include "row.h"
#include "table.h"

void print_indent(uint32_t indentation) {
    for (uint32_t i = 0; i < indentation; i++) {
        cout << " ";
    }
}

void print_btree(Table& table, uint32_t page_num, uint32_t indentation) {
    void* node = table.pager->get_page(page_num);
    uint32_t child;
    switch (get_node_type(node)) {
        case NODE_TYPE_INTERNAL: {
            uint32_t num_childs = *internal_node_num_child(node);
            print_indent(indentation);
            cout << "internal size - " << num_childs << endl;
            indentation++;
            for (uint32_t i = 0; i < num_childs; i++) {
                child = *internal_node_child_value(node, i);
                print_btree(table, child, indentation);
            }
            print_indent(indentation);
            child = *internal_node_right_child(node);
            print_btree(table, child, indentation);
        }
            break;
        case NODE_TYPE_LEAF: {
            uint32_t num_cells = *leaf_node_num_cells(node);
            print_indent(indentation);
            cout << "leaf size - " << num_cells << endl;
            indentation++;
            for (uint32_t i = 0; i < num_cells; i++) {
                print_indent(indentation);
                uint32_t key = *leaf_node_cell_key(node, i);
                cout << " - " << key << endl;
            }
        }
    }
}