
#include <iostream>

#include <Windows.h>

using namespace std;

#define BUFSIZE 4096

HANDLE hChildStdinRd, hChildStdinWr, hChildStdinWrDup,

hChildStdoutRd, hChildStdoutWr, hChildStdoutRdDup,

hInputFile, hSaveStdin, hSaveStdout;

BOOL CreateChildProcess(VOID);

VOID WriteToPipe(VOID);

VOID ReadFromPipe(VOID);

VOID ErrorExit(LPTSTR);

VOID ErrMsg(LPTSTR, BOOL);

DWORD main(int argc, LPSTR argv[])

{

	SECURITY_ATTRIBUTES saAttr;

	BOOL fSuccess;

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	hSaveStdout = GetStdHandle(STD_OUTPUT_HANDLE);


	if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0))
		ErrorExit("Stdout pipe creation failed\n");
	
	if (!SetStdHandle(STD_OUTPUT_HANDLE, hChildStdoutWr))
		ErrorExit("Redirecting STDOUT failed");

	fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdoutRd,
		GetCurrentProcess(), &hChildStdoutRdDup, 0,
		FALSE,
		DUPLICATE_SAME_ACCESS);

	if (!fSuccess)
		ErrorExit("DuplicateHandle failed");

	CloseHandle(hChildStdoutRd);


	hSaveStdin = GetStdHandle(STD_INPUT_HANDLE);

	// Создадим канал для STDIN дочернего процесса.

	if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0))

		ErrorExit("Stdin pipe creation failed\n");

	// Установим дескриптор чтения для канала, который будет STDIN.

	if (!SetStdHandle(STD_INPUT_HANDLE, hChildStdinRd))

		ErrorExit("Redirecting Stdin failed");

	// Продублируем дескриптор записи для канала, так что он не был унаследован.

	fSuccess = DuplicateHandle(GetCurrentProcess(), hChildStdinWr,

		GetCurrentProcess(), &hChildStdinWrDup, 0,

		FALSE, // не наследуется

		DUPLICATE_SAME_ACCESS);

	if (!fSuccess)

		ErrorExit((LP)"DuplicateHandle failed");

	CloseHandle(hChildStdinWr);

	// Теперь создадим дочерний процесс.

	if (!CreateChildProcess())

		ErrorExit((LP)"Create process failed");

	// После создания процесса, восстановим сохраненные STDIN и STDOUT.

	if (!SetStdHandle(STD_INPUT_HANDLE, hSaveStdin))

		ErrorExit((LP)"Re-redirecting Stdin failed\n");

	if (!SetStdHandle(STD_OUTPUT_HANDLE, hSaveStdout))

		ErrorExit((LP)"Re-redirecting Stdout failed\n");

	// Получим дескриптор для входного файла родителя.

	if (argc > 1)

		hInputFile = CreateFileA(argv[1], GENERIC_READ, 0, NULL,

			OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);

	else

		hInputFile = hSaveStdin;

	if (hInputFile == INVALID_HANDLE_VALUE)

		ErrorExit((LP)"no input file\n");

	// Запишем в канал, который является стандартным вводом данных для дочернего

	// процесса.

	WriteToPipe();

	// Прочитаем из канала, который является стандартным выводом для дочернего

	// процесса.

	ReadFromPipe();

	return 0;

}

BOOL CreateChildProcess()

{

	PROCESS_INFORMATION piProcInfo;

	LPSTARTUPINFOA siStartInfo;

	// Установим член структуры PROCESS_INFORMATION.

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
	// Установим член структуры STARTUPINFO.

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

	siStartInfo->cb = sizeof(STARTUPINFO);

	// Создадим дочерний процесс.

	return CreateProcessA(
		"W:\\Operation-Systems\\swh\\swh\\x64\\Release\\swh.exe",
		NULL, // командная строка
		NULL, // атрибуты защиты процесса
		NULL, // атрибуты защиты первичного потока
		TRUE, // дескрипторы наследуемы
		0, // флажки создания
		NULL, // использование конфигурации родителя
		NULL, // использование текущего каталога родителя
		siStartInfo, // указатель на STARTUPINFO
		&piProcInfo); // принимаем PROCESS_INFORMATION

}

VOID WriteToPipe(VOID)

{

	DWORD dwRead, dwWritten;

	CHAR chBuf[BUFSIZE];

	// Чтение из файла и запись его контекста в канал.

	for (;;)

	{

		if (!ReadFile(hInputFile, chBuf, BUFSIZE, &dwRead, NULL) ||

			dwRead == 0) break;

		if (!WriteFile(hChildStdinWrDup, chBuf, dwRead,

			&dwWritten, NULL)) break;

	}

	// Закрываем дескриптор канала, так чтобы дочерний процесс остановил чтение.

	if (!CloseHandle(hChildStdinWrDup))

		ErrorExit((LP)"Close pipe failed\n");

}

VOID ReadFromPipe(VOID)

{

	DWORD dwRead, dwWritten;

	CHAR chBuf[BUFSIZE];

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// Закрываем конец записи канала перед чтением из конца чтения канала.

	if (!CloseHandle(hChildStdoutWr))

		ErrorExit((LP)"Closing handle failed");

	// Читаем выводимую информацию дочерним процессом, и записываем в STDOUT

	// родителя.

	for (;;)

	{

		if (!ReadFile(hChildStdoutRdDup, chBuf, BUFSIZE, &dwRead,

			NULL) || dwRead == 0) break;

		if (!WriteFile(hSaveStdout, chBuf, dwRead, &dwWritten, NULL))

			break;

	}

}

VOID ErrorExit(char * lpszMessage)

{

	fprintf(stderr, "%s\n", lpszMessage);

	ExitProcess(0);

}