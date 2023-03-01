#pragma once

#include <stdlib.h>
#include "constance.hpp"
#include "util.h"

class Row {
public:
    uint32_t id;
    char name[NAME_LENGTH+1];
    char email[EMAIL_LENGTH+1];

    void deSerializedData(char* source);
    void serializeData(char* target);

    void print();
};

const uint32_t ROW_ID_OFFSET = 0;
const uint32_t ROW_ID_SIZE = sizeofAttribute(Row, id);
const uint32_t ROW_NAME_OFFSET = ROW_ID_SIZE;
const uint32_t ROW_NAME_SIZE = sizeofAttribute(Row, name);
const uint32_t ROW_EMAIL_OFFSET = ROW_NAME_OFFSET + ROW_NAME_SIZE;
const uint32_t ROW_EMAIL_SIZE = sizeofAttribute(Row, email);
const uint32_t ROW_SIZE = ROW_ID_SIZE + ROW_NAME_SIZE + ROW_EMAIL_SIZE;
const uint32_t ROW_COUNT_PER_PAGE = PAGE_SIZE / ROW_SIZE;

typedef enum {
    NODE_TYPE_LEAF,
    NODE_TYPE_INTERNAL
} NODE_TYPE;

const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t NODE_IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const uint32_t NODE_IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t NODE_PARENMT_POINTER_OFFSET = NODE_TYPE_SIZE + NODE_IS_ROOT_SIZE;
const uint32_t NODE_PARENT_POINTER_SIZE = sizeof(uint32_t);
const uint32_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + NODE_IS_ROOT_SIZE + NODE_PARENT_POINTER_SIZE;

NODE_TYPE get_node_type(void* node);
void set_node_type(void* node, NODE_TYPE type);
bool is_node_root(void* node);
void set_node_root(void* node, bool is_root);

const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NEXT_PAGE_OFFSETT = LEAF_NODE_NUM_CELLS_OFFSET + LEAF_NODE_NUM_CELLS_SIZE;
const uint32_t LEAF_NODE_NEXT_PAGE_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_HEADER_SIZE = LEAF_NODE_NUM_CELLS_OFFSET + LEAF_NODE_NUM_CELLS_SIZE + LEAF_NODE_NEXT_PAGE_SIZE;

const uint32_t LEAF_NODE_CELL_KEY_OFFSET = 0;
const uint32_t LEAF_NODE_CELL_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_CELL_VALUE_OFFSET = LEAF_NODE_CELL_KEY_SIZE;
const uint32_t LEAF_NODE_CELL_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_CELL_KEY_SIZE + LEAF_NODE_CELL_VALUE_SIZE;
const uint32_t LEAF_NODE_DATA_SPACE_SIZE = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_MAX_CELL_COUNT = LEAF_NODE_DATA_SPACE_SIZE / LEAF_NODE_CELL_SIZE;
const uint32_t LEAF_NODE_SPLIT_RIGHT_COUNT = (LEAF_NODE_MAX_CELL_COUNT + 1) / 2;
const uint32_t LEAF_NODE_SPLIT_LEFT_COUNT = (LEAF_NODE_MAX_CELL_COUNT + 1) - LEAF_NODE_SPLIT_RIGHT_COUNT;

uint32_t* leaf_node_num_cells(void* node);
uint32_t* leaf_node_next_page(void* node);
void* leaf_node_cell(void* node, uint32_t index);
uint32_t* leaf_node_cell_key(void* node, uint32_t index);
void* leaf_node_cell_value(void* node, uint32_t index);
void initialize_leaf(void* node);

const uint32_t INTERNAL_NODE_NUM_CHILD_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t INTERNAL_NODE_NUM_CHILD_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_RIGHT_CHILD_OFFSET = INTERNAL_NODE_NUM_CHILD_OFFSET + INTERNAL_NODE_NUM_CHILD_SIZE;
const uint32_t INTERNAL_NODE_RIGHT_CHILD_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_HEADER_SIZE = INTERNAL_NODE_RIGHT_CHILD_OFFSET + INTERNAL_NODE_RIGHT_CHILD_SIZE;

const uint32_t INTERNAL_NODE_CHILD_KEY_OFFSET = 0;
const uint32_t INTERNAL_NODE_CHILD_KEY_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_CHILD_VALUE_OFFSET = INTERNAL_NODE_CHILD_KEY_OFFSET + INTERNAL_NODE_CHILD_KEY_SIZE;
const uint32_t INTERNAL_NODE_CHILD_VALUE_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_CHILD_SIZE = INTERNAL_NODE_CHILD_KEY_SIZE + INTERNAL_NODE_CHILD_VALUE_SIZE;

uint32_t* internal_node_num_child(void* node);
uint32_t* internal_node_right_child(void* node);
void* internal_node_child(void* node, uint32_t index);
uint32_t* internal_node_child_key(void* node, uint32_t index);
uint32_t* internal_node_child_value(void* node, uint32_t index);
void initialize_internal_node(void* node);

uint32_t get_node_max_key(void* node);
