#include "Console.h"

#include <iostream>
#include <windows.h>

void Console::Initialize()
{
	AllocConsole();
	freopen_s(&m_fpStdin, "CONIN$", "r", stdin);
	freopen_s(&m_fpStdout, "CONOUT$", "w", stdout);
	freopen_s(&m_fpStderr, "CONOUT$", "w", stderr);
}

void Console::Finalize()
{
	fclose(m_fpStdin);
	fclose(m_fpStdout);
	fclose(m_fpStderr);
	FreeConsole();
}
