#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>  // ファイル入出力用

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
				std::cerr << "ファイルを開けませんでした。" << std::endl;
			}

			outFile << str.c_str() << std::endl;  // ファイルに書き込み

			outFile.close();  // ファイルを閉じる
		}
	};
}
