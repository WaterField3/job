#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
namespace TMF
{
	class StringHelper
	{
	public:

		static StringHelper& Instance()
		{
			static StringHelper instance;
			return instance;
		 }
		std::string ChangeLabel(const char* labelName, boost::uuids::uuid uuID);
	};
}

