#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace std;

#include "constance.hpp"

class Pager {
public:
    FILE* file;
    uint32_t file_length;
    void* pages[TABLE_MAX_PAGES];

    void* get_page(uint32_t index);
    void flushPage(uint32_t index, uint32_t size);
    void closeDB();
    void cleanDB();
    static Pager& openPager(const string& filename);
};
