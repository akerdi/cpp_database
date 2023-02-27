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