#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <string>
#include <iostream>

const uint32_t BUFFER_LENGTH = 1024;
typedef struct {
    char* buffer;
    size_t buffer_length;
    uint32_t str_length;
} InputBuffer;

static bool read_line(InputBuffer& input, std::string prompt) {
    std::cout << prompt;

    ssize_t read_count = getline(&(input.buffer), &(input.buffer_length), stdin);
    if (read_count <= 0 || read_count > BUFFER_LENGTH) {
        printf("read_line error: %s\n", input.buffer);
        return false;
    }
    input.buffer[strlen(input.buffer)-1] = '\0';
    input.str_length = strlen(input.buffer);

    return true;
}