#include "StringHelper.h"

#include <boost/uuid/uuid_io.hpp>
namespace TMF
{
	std::string StringHelper::ChangeLabel(const char* labelName, boost::uuids::uuid uuID)
	{
		auto label = std::string(labelName);
		label += "## " + boost::uuids::to_string(uuID);
		return label;
	}
}