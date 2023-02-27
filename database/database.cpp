#include "readline.hpp"

#include "statement.h"
#include "table.h"
#include "row.h"

EXECUTION_RESULT doHelpCommand(Table& table, string& userInput);

// -------------------

char* row_slot(Pager& pager, const uint32_t row_index) {
    uint32_t page_index = row_index / ROW_COUNT_PER_PAGE;
    void* page = pager.get_page(page_index);
    uint32_t row_inline_index = row_index % ROW_COUNT_PER_PAGE;
    return (char*)page + row_inline_index * ROW_SIZE;
}

EXECUTION_RESULT execSelect(Table& table, Statement& statement) {
    Row row;
    for (uint32_t i = 0; i < table.num_rows; i++) {
        char* slot = row_slot(*table.pager, i);
        row.deSerializedData(slot);
        row.print();
    }
    return EXEC_SUCCESS;
}
EXECUTION_RESULT execInsert(Table& table, Statement& statement) {
    Row& row = statement.row;
    uint32_t insert_index = table.num_rows;

    char* slot = row_slot(*table.pager, insert_index);
    row.serializeData(slot);

    table.num_rows++;
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

void start_database() {
    cout << "AKDatabase" << endl;
    Table& table = Table::openTable("row.db");
    table.num_rows = table.pager->file_length / ROW_SIZE;
    InputBuffer inputBuffer;
    while (true) {
        if (!read_line(inputBuffer, "> ")) {
            cout << "请输入指令:select / insert" << endl;
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
                    cout << "未识别的指令: " << inputStr << endl;
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
                cout << "执行失败" << endl;
            }
                continue;
        }
    }
}

void writeDB(Table& table) {
    uint32_t full_page_num = table.num_rows / ROW_COUNT_PER_PAGE;
    for (uint32_t i = 0; i < full_page_num; i++) {
        table.pager->flushPage(i, PAGE_SIZE);
        free(table.pager->pages[i]);
        table.pager->pages[i] = NULL;
    }
    uint32_t additional_row_num =table.num_rows % ROW_COUNT_PER_PAGE;
    if (additional_row_num) {
        table.pager->flushPage(full_page_num, additional_row_num*ROW_SIZE);
        free(table.pager->pages[full_page_num]);
        table.pager->pages[full_page_num] = NULL;
    }
    table.pager->closeDB();
    table.pager->cleanDB();
}

EXECUTION_RESULT doHelpCommand(Table& table, string& userInput) {
    if (strncmp(userInput.c_str(), ".exit", 5) == 0) {
        cout << "欢迎下次使用" << endl;
        writeDB(table);
        exit(0);
    } else {
        return EXEC_UNRECOGNIZED;
    }
}