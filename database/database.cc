#include "readline.hpp"

#include "statement.h"

void start_database() {
    cout << "AKDatabase" << endl;

    while (true) {
        string userInput;
        if (!read_line(userInput)) {
            cout << "请输入指令:select / insert" << endl;
            continue;
        }
        Statement statement;
        switch (statement.input2Statement(userInput)) {
            case SUCCESS: break;
            case FAIL: {
                cout << "fail input2Statement: " << userInput << endl;
                continue;
            }
            case UNRECOGNIZED:
            default: {
                cout << "unrecognized command: " << userInput << endl;
                continue;
            }
        }
        cout << "command type: " << statement.type << " rowId: " << statement.row.id << endl;
    }
}