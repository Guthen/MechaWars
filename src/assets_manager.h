#pragma once

#include <map>
#include <vector>

#include "raylib.h"

#include "utility/str_comp.h"

class AssetsManager
{
private:
	static std::map<const char*, Texture, utility::StrComp> textures;
	static std::map<const char*, Shader, utility::StrComp> shaders;
public:
	static Texture get_or_load_texture( const char* filename );
	static Shader get_or_load_shader( const char* filename );
	static std::vector<Rectangle> slice_texture( const Texture texture, const int width, const int height);

	static void free();
};

