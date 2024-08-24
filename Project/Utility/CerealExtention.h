#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/cereal.hpp>
#include <SimpleMath.h>
#include <string>

namespace cereal
{
	template <class Archive>
	void save(Archive& ar, boost::uuids::uuid const& at)
	{ 
		ar(cereal::make_nvp("uuid", boost::uuids::to_string(at)));
	}

	template <class Archive>
	void load(Archive& ar, boost::uuids::uuid& at)
	{
		std::string uuidStr;
		ar(uuidStr);
		at = boost::uuids::string_generator()(uuidStr);
	}

	template<class Archive>
	void serialize(Archive& archive, DirectX::SimpleMath::Vector3& v)
	{
		archive(v.x, v.y, v.z);
	}

	template<class Archive>
	void serialize(Archive& archive, DirectX::SimpleMath::Quaternion& q)
	{
		archive(q.x, q.y, q.z, q.w);
	}
}
