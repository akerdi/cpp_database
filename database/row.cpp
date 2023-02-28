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