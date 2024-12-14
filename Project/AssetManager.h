#pragma once

#include <memory>

namespace TMF
{
	class AssetManager
	{
	public:
		void Initialize();
		void Finalize();

		static AssetManager& Instance()
		{
			static AssetManager instance;
			return instance;
		}

	private:
	};
}

