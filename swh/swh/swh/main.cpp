#include <iostream>
#include <iomanip>
#include <Windows.h>
using namespace std;


int main(int argc, char* argv[]) {

	wstring strbuf;
	HANDLE stdIn, stdOut;
	DWORD dwReaded, dwWritten;

	stdIn = GetStdHandle(STD_INPUT_HANDLE);
	stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	cout << argv[0] << "\n";

	/*WriteFile(stdOut, "Please input string\n", 21, NULL, NULL);*/

	cout << "Please input string \n";
	
	ReadFile(stdIn, &strbuf , 128, &dwReaded, NULL);



	/*WriteFile(stdOut, "Inputed string: \n", 18 , &dwWritten, NULL);*/

	cout << "Input string \n";

	WriteFile(stdOut, &strbuf, dwReaded , &dwWritten, NULL);

	cout << "\n";
	return 0;
}