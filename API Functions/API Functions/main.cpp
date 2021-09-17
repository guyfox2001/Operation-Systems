#include <iostream>
#include <Windows.h>
#include <string>
#include <cctype>
#include <set>
#include <stdlib.h>

#ifndef UTF_8
#define UTF_8
#pragma execution_character_set("utf-8")
#endif

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 2024
#endif

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
            from = (string)argv[i];
           
            SET_FM = true;
            continue;
        }
        if (((string)argv[i]).find("-") == -1 && !SET_TO) {
            to = (string)argv[i];
            SET_TO = true;
            continue;
        }
    }
    if (dwFlgAndAttributes == 0) dwFlgAndAttributes = FILE_ATTRIBUTE_NORMAL;
    if (SET_FM && SET_TO && SET_SC && SET_SN) return true;
    return false;
}


void _buffer_init(char * BUFFER) {
    for (int i = 0; i < BUFFER_SIZE; i++) BUFFER[i] = '\0';
}


int main(int argc, char * argv[])
{
    char
        bufferFirst[BUFFER_SIZE],
        bufferSecond[BUFFER_SIZE],
        bufferResult[BUFFER_SIZE * 2];
    HANDLE 
        hFrom,
        hTo,
        hOutFile;
    DWORD
        brFrom,
        brTo,
        brRes;
    int 
        stringIndex = 0;

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    if (!CLI_CONFIG(argc, argv))return -1;
    _buffer_init(bufferFirst);
    _buffer_init(bufferSecond);
    _buffer_init(bufferResult);
    hFrom = CreateFileA(
        from.c_str(),
        dwDesAccess,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    hTo = CreateFileA(
        to.c_str(),
        dwDesAccess,
        FILE_SHARE_READ,
        NULL, 
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );                                                  
    hOutFile = CreateFileA(
        "result.txt",
        dwDesAccess,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        dwFlgAndAttributes,
        NULL
    );
    ReadFile(hFrom, bufferFirst, BUFFER_SIZE, &brFrom, NULL);
    ReadFile(hTo, bufferSecond, BUFFER_SIZE, &brTo, NULL);
    for (int str = 1, j =0; bufferFirst[stringIndex] != '\0'; stringIndex++) {
        if (string_numbers.find(str) != string_numbers.end()) {
            for (; bufferFirst[stringIndex] != '\n'; j++) {
                bufferResult[j] = bufferFirst[stringIndex++];
            }
            bufferResult[j++] = bufferFirst[stringIndex];
            str++;
            continue; 
        }
        if (bufferFirst[stringIndex] == '\n')str++;
    }
    for (int j = 0; bufferSecond[j] != '\0'; j++, stringIndex++) {
        bufferResult[stringIndex] = bufferSecond[j];
    }
    WriteFile(hOutFile, bufferResult, stringIndex, NULL, NULL);
    CloseHandle(hFrom); CloseHandle(hTo); CloseHandle(hOutFile);
    return 0;
}



