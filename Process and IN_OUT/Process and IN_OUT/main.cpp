#include <iostream>
#include <Windows.h>
#include <string>
#include <iomanip>


using namespace std;

//process count - 5, 
//Параллельно первые три, после завершения трех – параллельно последние два
// P - 2, Q - 1


int main(int argc, char * argv[]) {

    PROCESS_INFORMATION pi[4];

    HANDLE
        outStreamHandle,
        inputStreamHandle,
        childIn,
        childOut,
        h2,
        fileHandles[4];

    DuplicateHandle(GetCurrentProcess(), GetStdHandle(STD_INPUT_HANDLE), GetCurrentProcess(), &inputStreamHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);

    inputStreamHandle = GetStdHandle(STD_INPUT_HANDLE);
    outStreamHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    // разрешение наследования дескрипторов    
    SECURITY_ATTRIBUTES sa{
        sizeof(SECURITY_ATTRIBUTES),
        NULL,
        true
    };

    
    STARTUPINFOA
        fPrcInfo = { sizeof(fPrcInfo) },
        sPrcInfo = { sizeof(sPrcInfo) },
        tPrcInfo = { sizeof(tPrcInfo) };

    fileHandles[0] = CreateFileA("inputFirstProcess.txt", GENERIC_READ, 0, &sa, OPEN_ALWAYS, 0, NULL);
    fileHandles[1] = CreateFileA("inputSecondProcess.txt", GENERIC_READ, 0, &sa, OPEN_ALWAYS, 0, NULL);
    fileHandles[2] = CreateFileA("outputSecondProcess.txt", GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, 0, NULL);
    fileHandles[3] = CreateFileA("outputThirdProcess.txt", GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, 0, NULL);
    

    

    LPSTR inputString = argv[2];


    //выделение памяти для структуры STARTUPINFO
    ZeroMemory(&fPrcInfo, sizeof(STARTUPINFO));
    ZeroMemory(&sPrcInfo, sizeof(STARTUPINFO));
    ZeroMemory(&tPrcInfo, sizeof(STARTUPINFO));




    //назначение потоков относительно Q-1
    //fPrcInfo.hStdInput = fileHandles[0];

    //sPrcInfo.hStdInput = fileHandles[1];
    //sPrcInfo.hStdOutput = fileHandles[2];

    //tPrcInfo.hStdOutput = fileHandles[3];
   
    SetStdHandle(STD_INPUT_HANDLE, fileHandles[0]);
    if (CreateProcessA(argv[1], (char*)"1\ ", NULL, &sa, TRUE, 0, NULL, NULL, &fPrcInfo, &pi[0])) {
        
        cout << "\t first process start\n";
    }
    SetStdHandle(STD_INPUT_HANDLE, fileHandles[1]);
    SetStdHandle(STD_OUTPUT_HANDLE, fileHandles[2]);
    if (CreateProcessA(argv[1], (char*)"2 ", NULL, &sa, TRUE, 0, NULL, NULL, &sPrcInfo, &pi[1])) {
        std::cout << "\t second process start" << std::endl;

    }

    SetStdHandle(STD_INPUT_HANDLE, inputStreamHandle);
    SetStdHandle(STD_OUTPUT_HANDLE, fileHandles[3]);
    if (CreateProcessA(argv[1], (char*)"3 ", NULL, &sa, TRUE, 0, NULL, NULL, &tPrcInfo, &pi[2])) {
        std::cout << "\t third process start" << std::endl;
    }
    WaitForMultipleObjects(3, &pi->hProcess, TRUE, INFINITE);
    SetStdHandle(STD_OUTPUT_HANDLE, outStreamHandle);
}
