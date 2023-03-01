#include <iostream>
using namespace std;

#include "row.h"

void Row::deSerializedData(char* source) {
    memcpy(&id, source + ROW_ID_OFFSET, ROW_ID_SIZE);
    memcpy(&name, source + ROW_NAME_OFFSET, ROW_NAME_SIZE);
    memcpy(&email, source + ROW_EMAIL_OFFSET, ROW_EMAIL_SIZE);
}
void Row::serializeData(char* target) {
    memcpy(target + ROW_ID_OFFSET, &id, ROW_ID_SIZE);
    memcpy(target + ROW_NAME_OFFSET, &name, ROW_NAME_SIZE);
    memcpy(target + ROW_EMAIL_OFFSET, &email, ROW_EMAIL_SIZE);
}

void Row::print() {
    cout << "id: " << id << " name: " << name << " email: " << email << endl;
}

NODE_TYPE get_node_type(void* node) {
    void* node_offset = (char*)node + NODE_TYPE_OFFSET;
    uint8_t value = *(uint8_t*)node_offset;
    return (NODE_TYPE)value;
}
void set_node_type(void* node, NODE_TYPE type) {
    uint8_t value = type;
    void* node_offset = (char*)node + NODE_TYPE_OFFSET;
    *(uint8_t*)node_offset = value;
}
bool is_node_root(void* node) {
    void* node_offset = (char*)node + NODE_IS_ROOT_OFFSET;
    uint8_t value = *(uint8_t*)node_offset;
    return (bool)value;
}
void set_node_root(void* node, bool is_root) {
    uint8_t value = is_root;
    void* node_offset = (char*)node + NODE_IS_ROOT_OFFSET;
    *(uint8_t*)node_offset = value;
}

#pragma mark leaf-------------------

uint32_t* leaf_node_num_cells(void* node) {
    return (uint32_t*)((char*)node + LEAF_NODE_NUM_CELLS_OFFSET);
}
void* leaf_node_cell(void* node, uint32_t index) {
    return (char*)node + LEAF_NODE_HEADER_SIZE + index*LEAF_NODE_CELL_SIZE;
}
uint32_t* leaf_node_cell_key(void* node, uint32_t index) {
    char* node_cell_offset = (char*)leaf_node_cell(node, index);
    return (uint32_t*)(node_cell_offset + LEAF_NODE_CELL_KEY_OFFSET);
}
void* leaf_node_cell_value(void* node, uint32_t index) {
    return ((char*)leaf_node_cell(node, index) + LEAF_NODE_CELL_VALUE_OFFSET);
}
void initialize_leaf(void* node) {
    set_node_type(node, NODE_TYPE_LEAF);
    set_node_root(node, false);
}
#pragma mark internal-------------------

uint32_t* internal_node_num_child(void* node) {
    return (uint32_t*)(char*)node + INTERNAL_NODE_NUM_CHILD_OFFSET;
}
uint32_t* internal_node_right_child(void* node) {
    return (uint32_t*)((char*)node + INTERNAL_NODE_RIGHT_CHILD_OFFSET);
}
void* internal_node_child(void* node, uint32_t index) {
    return (char*)node + INTERNAL_NODE_HEADER_SIZE + index * INTERNAL_NODE_CHILD_SIZE;
}
uint32_t* internal_node_child_key(void* node, uint32_t index) {
    return (uint32_t*)((char*)internal_node_child(node, index) + INTERNAL_NODE_CHILD_KEY_OFFSET);
}
uint32_t* internal_node_child_value(void* node, uint32_t index) {
    uint32_t num_childs = *internal_node_num_child(node);
    if (index > num_childs) {
        printf("Access internal child at %d > %d\n", index, num_childs);
        exit(1);
    }
    if (index == num_childs) {
        return internal_node_right_child(node);
    } else {
        return (uint32_t*)((char*)internal_node_child(node, index) + INTERNAL_NODE_CHILD_VALUE_OFFSET);
    }
}
void initialize_internal_node(void* node) {
    *internal_node_num_child(node) = 0;
    set_node_type(node, NODE_TYPE_INTERNAL);
    set_node_root(node, false);
}

uint32_t get_node_max_key(void* node) {
    NODE_TYPE type = get_node_type(node);
    switch (type) {
        case NODE_TYPE_LEAF:
            return *leaf_node_cell_key(node, *(leaf_node_num_cells(node))-1);
        case NODE_TYPE_INTERNAL:
            return *internal_node_child_key(node, *(internal_node_num_child(node))-1);
        default:
            cout << __func__ << ": unknown type: " << type << endl;
            exit(1);
    }
}
