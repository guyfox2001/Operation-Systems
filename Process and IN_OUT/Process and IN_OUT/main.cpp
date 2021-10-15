#include <iostream>
#include <Windows.h>
#include <string>
#include <iomanip>


using namespace std;

//process count - 5, 
//����������� ������ ���, ����� ���������� ���� � ����������� ��������� ���
// P - 2, Q - 1


int main(int argc, char * argv[]) {

    PROCESS_INFORMATION fProcesses[3], sProcesses[2];

    HANDLE
        outStreamHandle,
        inputStreamHandle,
        fileHandles[4];

    DuplicateHandle(GetCurrentProcess(), GetStdHandle(STD_INPUT_HANDLE), GetCurrentProcess(), &inputStreamHandle, 0, FALSE, DUPLICATE_SAME_ACCESS);

    outStreamHandle = GetStdHandle(STD_OUTPUT_HANDLE);   
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


    //��������� ������ ��� ��������� STARTUPINFO
    ZeroMemory(&fPrcInfo, sizeof(STARTUPINFO));
    ZeroMemory(&sPrcInfo, sizeof(STARTUPINFO));
    ZeroMemory(&tPrcInfo, sizeof(STARTUPINFO));

   
    SetStdHandle(STD_INPUT_HANDLE, fileHandles[0]);
    if (CreateProcessA(argv[1], (char*)"1\ ", NULL, &sa, TRUE, 0, NULL, NULL, &fPrcInfo, &fProcesses[0])) {
        
        cout << "\t first process start\n";
    }
    /*WaitForSingleObject(pi[0].hProcess, INFINITE);*/
    SetStdHandle(STD_INPUT_HANDLE, fileHandles[1]);
    SetStdHandle(STD_OUTPUT_HANDLE, fileHandles[2]);
    if (CreateProcessA(argv[1], (char*)"2 ", NULL, &sa, TRUE, 0, NULL, NULL, &sPrcInfo, &fProcesses[1])) {
        std::cout << "\t second process start" << std::endl;

    }
    SetStdHandle(STD_INPUT_HANDLE, inputStreamHandle);
    SetStdHandle(STD_OUTPUT_HANDLE, fileHandles[3]);
    if (CreateProcessA(argv[1], (char*)"3 ", NULL, &sa, FALSE, 0, NULL, NULL, &tPrcInfo, &fProcesses[2])) {
        std::cout << "\t third process start" << std::endl;
    }
    
        /*DWORD dw = WaitForMultipleObjects(3, &fProcesses[2].hProcess, FALSE, INFINITE);*/
    SetStdHandle(STD_OUTPUT_HANDLE, outStreamHandle);

    if (CreateProcessA(argv[1], (char*)"4", NULL, &sa, FALSE, 0, NULL, NULL, &tPrcInfo, &sProcesses[0])) {
        std::cout << "\t third process start" << std::endl;
    }

    if (CreateProcessA(argv[1], (char*)"5", NULL, &sa, FALSE, 0, NULL, NULL, &tPrcInfo, &sProcesses[1])) {
        std::cout << "\t third process start" << std::endl;
    }
    WaitForMultipleObjects(2, &sProcesses->hProcess, TRUE, INFINITE);
}
