#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <string>

namespace cereal
{
	template <class Archive>
	void save(Archive& ar, boost::uuids::uuid const& at)
	{ 
		ar(boost::uuids::to_string(at));
	}

	template <class Archive>
	void load(Archive& ar, boost::uuids::uuid& at)
	{
		std::string uuidStr;
		ar(uuidStr);
		at = boost::uuids::string_generator()(uuidStr);
	}
}
