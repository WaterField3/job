#pragma once

#include <iostream>

class Console
{
public:
	Console();
	virtual ~Console();

private:
	FILE* m_fpStdin;
	FILE* m_fpStdout;
	FILE* m_fpStderr;
};
