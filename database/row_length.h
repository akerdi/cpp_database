#pragma once

#include "row.h"

#define sizeofAttribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

const uint32_t ROW_ID_OFFSET = 0;
const uint32_t ROW_ID_SIZE = sizeofAttribute(Row, id);
const uint32_t ROW_NAME_OFFSET = ROW_ID_SIZE;
const uint32_t ROW_NAME_SIZE = sizeofAttribute(Row, name);
const uint32_t ROW_EMAIL_OFFSET = ROW_NAME_OFFSET + ROW_NAME_SIZE;
const uint32_t ROW_EMAIL_SIZE = sizeofAttribute(Row, email);
const uint32_t ROW_SIZE = ROW_ID_SIZE + ROW_NAME_SIZE + ROW_EMAIL_SIZE;
