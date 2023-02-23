#include <iostream>

using namespace std;

static int get_user_input(string& input, string prompt = "> ") {
    cout << prompt;
    getline(cin, input);
    return input.length();
}

void start_database() {
    cout << "AKDatabase" << endl;

    while (true) {
        string userInput;
        if (!get_user_input(userInput)) {
            cout << "请输入指令:select / insert" << endl;
            return ;
        }
        cout << "str: " << userInput << " len: " << userInput.length() << endl;
    }
}