#pragma once

#include <string>
#include <fstream>
#include <format>

namespace utility
{
	static std::string get_file_contents( const char* filename )
	{
		std::ifstream file( filename );
		if ( !file ) throw std::exception( filename );

		std::string contents;

		std::string line;
		while ( std::getline( file, line ) )
			contents += line;

		return contents;
	}
}