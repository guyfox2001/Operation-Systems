// 4 ��������, ����������� ������ ���, ����� ���������� ���� �
// ����������� ��������� ���, ����� ����, ��� ���� ����������- ������. P-2 Q-2.
// GetStdHandle/SetStdHandle; �����, ����/�����, ����
// copy, create, echo, find, sort

#include <iostream>
#include <Windows.h>
#include <string>
//����� ������� SYNCHRONIZE
int main(int argc, char* argv[])
{
    HANDLE inputHandle, outputHandle;
    HANDLE h[4];
    inputHandle = GetStdHandle(STD_INPUT_HANDLE);//���������� ��������� ����
    outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);//���������� ��������� �����
    LPSTR programm_name;
    programm_name = argv[2];
    STARTUPINFOA info = { sizeof(info) };
    PROCESS_INFORMATION pi[2], pi3;
    SECURITY_ATTRIBUTES sa{ true, NULL, sizeof(SECURITY_ATTRIBUTES) };//��������� ������������ ������������; ������ �� ���������
    h[0] = CreateFileA("output1.txt", GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, 0, NULL);
    h[1] = CreateFileA("output2.txt", GENERIC_WRITE, 0, &sa, CREATE_ALWAYS, 0, NULL);
    h[2] = CreateFileA("input2.txt", GENERIC_READ, 0, &sa, OPEN_EXISTING, 0, NULL);
    h[3] = CreateFileA("input3.txt", GENERIC_READ, 0, &sa, OPEN_EXISTING, 0, NULL);
    //�����
    SetStdHandle(STD_OUTPUT_HANDLE, h[0]);
    if (CreateProcessA(NULL, programm_name, NULL, &sa, TRUE, 0, NULL, NULL, &info, &pi[0])) {
        std::cout << "\tfirst" << std::endl;
    }
    SetStdHandle(STD_OUTPUT_HANDLE, h[1]);
    SetStdHandle(STD_INPUT_HANDLE, h[2]);
    //����/�����
    if (CreateProcessA(NULL, programm_name, NULL, &sa, TRUE, 0, NULL, NULL, &info, &pi[1]))
    {
        std::cout << "\tsecond" << std::endl;
    }
    WaitForMultipleObjects(2, &pi->hProcess, FALSE, INFINITE);
    //����
    SetStdHandle(STD_INPUT_HANDLE, h[3]);
    SetStdHandle(STD_OUTPUT_HANDLE, outputHandle);
    if (CreateProcessA(NULL, programm_name, NULL, &sa, TRUE, 0, NULL, NULL, &info, &pi3)) {
        CloseHandle(pi3.hProcess);
        CloseHandle(pi3.hThread);
        std::cout << "\tthird" << std::endl;
        WaitForSingleObject(&pi3.hProcess, INFINITE);
    }
}
