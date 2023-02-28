#pragma once

#include <iostream>
#include "row.h"

typedef enum {
    STATEMENT_SELECT,
    STATEMENT_INSERT
} STATEMENT_TYPE;

typedef enum {
    EXEC_FAIL,
    EXEC_SUCCESS,
    EXEC_UNRECOGNIZED,
    EXEC_STRING_TOO_LONG,
    EXEC_TABLE_FULL,
    EXEC_DUPLICATE
} EXECUTION_RESULT;

class Statement {
public:
    Row row;
    STATEMENT_TYPE type;
    EXECUTION_RESULT input2Statement(std::string& input);
private:
    EXECUTION_RESULT parseInput2Statement(std::string& input);
};