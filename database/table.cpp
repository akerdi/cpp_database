#include "table.h"

Table& Table::openTable(const string& filename) {
    Pager* pager = &Pager::openPager(filename);
    Table* table = new Table;
    table->pager = pager;
    return *table;
}
