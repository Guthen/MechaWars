#pragma once

namespace utility
{
	//  We have to compare the values and not the pointers for my use of std::map keys so.. (for instance: textures of AssetsManager)
	//  https://stackoverflow.com/questions/4157687/using-char-as-a-key-in-stdmap
	struct StrComp
	{
		bool operator()( const char* a, const char* b ) const
		{
			return std::strcmp( a, b ) < 0;
		}
	};
}