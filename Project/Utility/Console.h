#pragma once

#include <iostream>

class Console
{
public:
	void Initialize();
	void Finalize();

private:
	FILE* m_fpStdin;
	FILE* m_fpStdout;
	FILE* m_fpStderr;
};

