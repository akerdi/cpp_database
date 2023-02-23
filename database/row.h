#pragma once

#include <stdlib.h>
#include "constance.hpp"

class Row {
public:
    uint32_t id;
    char name[NAME_LENGTH+1];
    char email[EMAIL_LENGTH+1];
};