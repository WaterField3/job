#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>
namespace TMF
{
	// ���OLabelUtilty��
	class StringHelper
	{
	public:
		static std::string CreateLabel(const char* labelName, boost::uuids::uuid uuID);
	};
}

