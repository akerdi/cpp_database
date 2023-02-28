#include "readline.hpp"

#include "statement.h"
#include "table.h"
#include "row.h"
#include "cursor.h"

EXECUTION_RESULT doHelpCommand(Table& table, string& userInput);

// -------------------

EXECUTION_RESULT execSelect(Table& table, Statement& statement) {
    Row row;
    Cursor& cursor = Cursor::start_table(table);
    while (!cursor.end_of_table) {
        char* slot = (char*)cursor.cursor_value();
        row.deSerializedData(slot);
        row.print();
        cursor.cursor_advance();
    }

    delete &cursor;
    return EXEC_SUCCESS;
}
void leaf_node_insert(Cursor& cursor, uint32_t key, Row& value) {
    void* node = cursor.table->pager->get_page(cursor.page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    if (cursor.row_num >= LEAF_NODE_MAX_CELL_COUNT) {
        cout << "Need to implement leaf node splitting!" << endl;
        exit(1);
    }
    if (cursor.row_num < num_cells) {
        for (int32_t i = num_cells; i > cursor.row_num; i--) {
            memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i-1), LEAF_NODE_CELL_SIZE);
        }
    }

    *leaf_node_num_cells(node) += 1;
    *leaf_node_cell_key(node, cursor.row_num) = key;
    value.serializeData((char*)leaf_node_cell_value(node, cursor.row_num));
}
EXECUTION_RESULT execInsert(Table& table, Statement& statement) {
    Row& row = statement.row;

    Cursor& cursor = Cursor::end_table(table);
    void* node = table.pager->get_page(cursor.page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    if (num_cells >= LEAF_NODE_MAX_CELL_COUNT) {
        return EXEC_TABLE_FULL;
    }

    leaf_node_insert(cursor, row.id, row);

    delete &cursor;
    return EXEC_SUCCESS;
}

EXECUTION_RESULT execCommand(Table& table, Statement& statement) {
    if (statement.type == STATEMENT_SELECT) {
        return execSelect(table, statement);
    } else if (statement.type == STATEMENT_INSERT) {
        return execInsert(table, statement);
    }
    return EXEC_UNRECOGNIZED;
}

void configTable(Table& table) {
    if (table.pager->num_pages == 0) {
        table.root_page_num = 0;
        void* node = table.pager->get_page(table.root_page_num);
        initialize_leaf(node);
        set_node_root(node, true);
    }
}

void start_database() {
    cout << "AKDatabase" << endl;
    Table& table = Table::openTable("row.db");
    configTable(table);

    InputBuffer inputBuffer;
    while (true) {
        if (!read_line(inputBuffer, "> ")) {
            cout << "please type command:select / insert" << endl;
            continue;
        }
        string inputStr = string(inputBuffer.buffer);
        if ('.' == inputBuffer.buffer[0]) {
            switch (doHelpCommand(table, inputStr)) {
                case EXEC_SUCCESS: continue;
                case EXEC_FAIL: {
                    cout << "doHelpCommand fail: " << inputStr << endl;
                    continue;
                }
                default: {
                    cout << "unrecognized command: " << inputStr << endl;
                }
                    continue;
            }
        }
        Statement statement;
        switch (statement.input2Statement(inputStr)) {
            case EXEC_SUCCESS: break;
            case EXEC_FAIL: {
                cout << "fail input2Statement: " << inputStr << endl;
                continue;
            }
            case EXEC_UNRECOGNIZED:
            default: {
                cout << "unrecognized command: " << inputStr << endl;
                continue;
            }
        }
        switch (execCommand(table, statement)) {
            case EXEC_SUCCESS: break;
            case EXEC_FAIL:
            default: {
                cout << "execution fail!" << endl;
            }
                continue;
        }
    }
}

void writeDB(Table& table) {
    uint32_t full_page_num = table.pager->num_pages;
    for (uint32_t i = 0; i < full_page_num; i++) {
        table.pager->flushPage(i);
        free(table.pager->pages[i]);
        table.pager->pages[i] = NULL;
    }
    table.pager->closeDB();
    table.pager->cleanDB();
}

EXECUTION_RESULT doHelpCommand(Table& table, string& userInput) {
    if (strncmp(userInput.c_str(), ".exit", 5) == 0) {
        cout << "AKDB good bye!" << endl;
        writeDB(table);
        exit(0);
    } else {
        return EXEC_UNRECOGNIZED;
    }
}