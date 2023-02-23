#pragma once

#include <iostream>

using namespace std;

static int read_line(string& input, string prompt = "> ") {
    cout << prompt;
    getline(cin, input);
    return input.length();
}