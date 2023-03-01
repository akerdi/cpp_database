#include "pager.h"

#include <sys/stat.h>
#include <iostream>
using namespace std;


void* Pager::get_page(uint32_t index) {
    if (index > TABLE_MAX_PAGES) {
        printf("get page larger than maxmum: %d > %d\n", index, TABLE_MAX_PAGES);
        exit(1);
    }
    void* page = pages[index];
    if (!page) {
        page = malloc(PAGE_SIZE);
        uint32_t existPage = file_length / PAGE_SIZE;
        if (file_length % PAGE_SIZE) {
            existPage += 1;
        }
        fseek(file, index*PAGE_SIZE, SEEK_SET);
        fread(page, PAGE_SIZE, 1, file);
        pages[index] = page;
        if (index >= num_pages) {
            num_pages = index+1;
        }
    }
    return pages[index];
}

void Pager::flushPage(uint32_t index) {
    if (pages[index] == NULL) {
        std::cout << "Access empty data at index: " << index << "!" << endl;
        exit(1);
    }
    fseek(file, PAGE_SIZE * index, SEEK_SET);
    fwrite(pages[index], PAGE_SIZE, 1, file);
}
void Pager::closeDB() {
    fclose(file);
}
void Pager::cleanDB() {
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        pages[i] = NULL;
    }
}
uint32_t Pager::get_unused_page_num() {
    return num_pages;
}

Pager& Pager::openPager(const string& filename) {
    FILE* file;
    struct stat st;
    if (stat(filename.c_str(), &st) == 0) {
        file = fopen(filename.c_str(), "rb+");
    } else {
        file = fopen(filename.c_str(), "wb+");
    }
    if (!file) {
        perror("openPager");
        exit(1);
    }
    Pager* pager = new Pager;
    pager->file = file;
    fseek(file, 0L, SEEK_END);
    long file_length = ftell(file);
    pager->file_length = file_length;
    pager->num_pages = file_length / PAGE_SIZE;
    if (file_length % PAGE_SIZE) {
        cout << "Interupt by opening a not full db! Incorrupt db file!" << endl;
        exit(1);
    }
    for (int i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }
    return *pager;
}
