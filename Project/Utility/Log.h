#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>  // �t�@�C�����o�͗p

namespace TMF
{
	class Log
	{
	public:
		static void Info(char const* const _Format, ...)
		{
			printf(_Format);
		}
		static void DebugInfo(std::string str)
		{
			std::ifstream checkFile("Debug.txt");
			bool fileExists = checkFile.good();
			checkFile.close();

			std::ofstream outFile("Debug.txt", fileExists ? std::ios::app : std::ios::trunc);
			if (!outFile) {
				std::cerr << "�t�@�C�����J���܂���ł����B" << std::endl;
			}

			outFile << str.c_str() << std::endl;  // �t�@�C���ɏ�������

			outFile.close();  // �t�@�C�������
		}
	};
}
