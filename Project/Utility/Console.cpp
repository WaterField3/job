#include "Console.h"

#include <iostream>
#include <windows.h>

Console::Console()
{
	AllocConsole();
	freopen_s(&m_fpStdin, "CONIN$", "r", stdin);
	freopen_s(&m_fpStdout, "CONOUT$", "w", stdout);
	freopen_s(&m_fpStderr, "CONOUT$", "w", stderr);
}

Console::~Console()
{
	fclose(m_fpStdin);
	fclose(m_fpStdout);
	fclose(m_fpStderr);
	FreeConsole();
}
