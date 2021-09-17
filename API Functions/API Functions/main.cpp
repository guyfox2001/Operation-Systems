#include <iostream>
#include <Windows.h>
#include <string>
#include <cctype>
#include <set>
#include <stdlib.h>

using namespace std;

int string_count;
set<int> string_numbers;
DWORD dwDesAccess = GENERIC_READ | GENERIC_WRITE, dwFlgAndAttributes = 0;
string from, to;



bool CLI_CONFIG(int argc, char* argv[]) {
    bool SET_FM = false, SET_TO = false, SET_SC = false, SET_SN= false;
    if (argc < 5) {
        cerr << "Error! You're input lether 4 args!\n";
        return false;
    }
    for (int i = 1; i < argc; i++) {
        cout << argv[i] << ' ';
        if (((string)argv[i]) == "--sc") {
            if (atoi(argv[++i]) == NULL) return false;
            string_count = atoi(argv[i++]);
            SET_SC = true;
        }
        if (((string)argv[i]) == "--sn") {\
            if (atoi(argv[++i]) == NULL) return false;
            while (atoi(argv[i]) != NULL)
                string_numbers.insert(atoi(argv[i++]));
            SET_SN = true;
        }
        if (((string)argv[i]) == "--gr"){
            dwDesAccess = GENERIC_READ;
            continue;
        }
        if (((string)argv[i]) == "--gw") {
            dwDesAccess = GENERIC_WRITE;
            continue;
        }
        if (((string)argv[i]) == "--gwr") {
            dwDesAccess = GENERIC_READ | GENERIC_WRITE;
        }
        if (((string)argv[i]) == "-R") {
            dwFlgAndAttributes |= FILE_ATTRIBUTE_READONLY;
            continue;
        }
        if (((string)argv[i]) == "-A") {
            dwFlgAndAttributes |= FILE_ATTRIBUTE_ARCHIVE;
            continue;
        }
        if (((string)argv[i]) == "-H") {
            dwFlgAndAttributes |= FILE_ATTRIBUTE_HIDDEN;
            continue;
        }
        if (((string)argv[i]).find("-") == -1 && !SET_FM) {
            from = argv[i];
            SET_FM = true;
            continue;
        }
        if (((string)argv[i]).find("-") == -1 && !SET_TO) {
            to = argv[i];
            SET_TO = true;
            continue;
        }
    }
    if (dwFlgAndAttributes == 0) dwFlgAndAttributes = FILE_ATTRIBUTE_NORMAL;
    if (SET_FM && SET_TO && SET_SC && SET_SN) return true;
    return false;
}


int main(int argc, char * argv[])
{
    char buffer[1024];
    HANDLE hInput, hOutput;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    if (!CLI_CONFIG(argc, argv))return -1;
    cout << "\n";

    return 0;
}



