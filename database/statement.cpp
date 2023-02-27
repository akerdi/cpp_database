#include "statement.h"
#include <vector>
#include <stdlib.h>

using namespace std;

EXECUTION_RESULT Statement::parseInput2Statement(std::string& input) {
    static const char* const token = " ";

    char* str = (char*)malloc(sizeof(char)*(input.length()+1));
    strcpy(str, input.c_str());
    strtok(str, token);

    string id_Str, name_Str, email_Str;
    vector<string*> v;
    v.push_back(&id_Str); v.push_back(&name_Str); v.push_back(&email_Str);
    for (vector<string*>::iterator it = v.begin(); it != v.end(); it++) {
        char* str = strtok(NULL, token);
        if (!str) return EXEC_FAIL;
        *(*it) = string(str);
    }
    const uint id = atoi(id_Str.c_str());
    row.id = id;
    if (name_Str.length() > NAME_LENGTH) return EXEC_STRING_TOO_LONG;
    strcpy(row.name, name_Str.c_str());

    if (email_Str.length() > EMAIL_LENGTH) return EXEC_STRING_TOO_LONG;
    strcpy(row.email, email_Str.c_str());
    return EXEC_SUCCESS;
}

EXECUTION_RESULT Statement::input2Statement(std::string& input) {
    if (strncmp(input.c_str(), "select", 6) == 0) {
        type = STATEMENT_SELECT;
        return EXEC_SUCCESS;
    } else if (strncmp(input.c_str(), "insert", 6) == 0) {
        type = STATEMENT_INSERT;
        return parseInput2Statement(input);
    } else {
        return EXEC_UNRECOGNIZED;
    }
}
