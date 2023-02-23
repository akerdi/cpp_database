#pragma once

#include <iostream>
#include "row.h"

typedef enum {
    SELECT,
    INSERT
} STATEMENT_TYPE;

typedef enum {
    FAIL,
    SUCCESS,
    UNRECOGNIZED,
    STRING_TOO_LONG,
    TABLE_FULL
} EXECUTION_RESULT;

class Statement {
public:
    Row row;
    STATEMENT_TYPE type;
    EXECUTION_RESULT input2Statement(std::string& input);
private:
    EXECUTION_RESULT parseInput2Statement(std::string& input);
};