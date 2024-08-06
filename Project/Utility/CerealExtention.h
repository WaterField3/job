#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <SimpleMath.h>
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

	template<class Archive>
	void serialize(Archive& archive, DirectX::SimpleMath::Matrix& m)
	{
		archive(m._11, m._12, m._13, m._14,
			m._21, m._22, m._23, m._24,
			m._31, m._32, m._33, m._34,
			m._41, m._42, m._43, m._44);
	}
}
