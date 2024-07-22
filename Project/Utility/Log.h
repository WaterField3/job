#pragma once

#include <stdio.h>

namespace TMF
{
	class Log
	{
	public:
		static void Info(char const* const _Format, ...)
		{
			printf(_Format);
		}
	};
}

