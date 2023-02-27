#include "cursor.h"
#include "row.h"

Cursor::Cursor(Table& t, uint32_t page_num, uint32_t row_num) {
    this->table = &t;
    this->end_of_table = t.num_rows == 0;
    this->page_num = page_num;
    this->row_num = row_num;
}

Cursor& Cursor::start_table(Table& table) {
    uint32_t page_num = 0;
    uint32_t row_num = 0;
    Cursor* cursor = new Cursor(table, page_num, row_num);
    return *cursor;
}
Cursor& Cursor::end_table(Table& table) {
    uint32_t page_num = table.num_rows / ROW_COUNT_PER_PAGE;
    uint32_t row_num = table.num_rows % ROW_COUNT_PER_PAGE;
    Cursor* cursor = new Cursor(table, page_num, row_num);
    return *cursor;
}
void Cursor::cursor_advance() {
    row_num++;
    if (row_num >= table->num_rows) {
        end_of_table = true;
    }
}

void* Cursor::cursor_value() {
    void* page = table->pager->get_page(page_num);
    return (char*)page + row_num * ROW_SIZE;
}