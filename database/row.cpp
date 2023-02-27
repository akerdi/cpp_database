#include <iostream>
using namespace std;

#include "row.h"

void Row::deSerializedData(char* source) {
    memcpy(&id, source + ROW_ID_OFFSET, ROW_ID_SIZE);
    memcpy(&name, source + ROW_NAME_OFFSET, ROW_NAME_SIZE);
    memcpy(&email, source + ROW_EMAIL_OFFSET, ROW_EMAIL_SIZE);
}
void Row::serializeData(char* target) {
    memcpy(target + ROW_ID_OFFSET, &id, ROW_ID_SIZE);
    memcpy(target + ROW_NAME_OFFSET, &name, ROW_NAME_SIZE);
    memcpy(target + ROW_EMAIL_OFFSET, &email, ROW_EMAIL_SIZE);
}

void Row::print() {
    cout << "id: " << id << " name: " << name << " email: " << email << endl;
}
