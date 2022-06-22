#pragma once

#include "../libs/rapidxml/rapidxml_utils.hpp"

namespace utility
{
	using namespace rapidxml;
	static char* get_xml_contents( const char* filename )
	{
		file<> xml_data( filename );
		return xml_data.data();
	}

	/*static xml_document<>* get_xml_document( const char* filename )
	{
		xml_document<> doc;
		doc.parse<0>( get_xml_contents( filename ) );

		return &doc;
	}*/
}