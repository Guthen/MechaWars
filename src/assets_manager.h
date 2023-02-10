#pragma once

#include <map>
#include <vector>

#include "raylib.h"

#include "utility/str_comp.h"
#include <string>

class AssetsManager
{
private:
	static std::map<std::string, Texture> textures;
	static std::map<std::string, Shader> shaders;
public:
	static Texture get_or_load_texture( std::string filename );
	static Shader get_or_load_shader( std::string filename );
	static std::vector<Rectangle> slice_texture( const Texture texture, const int width, const int height);

	static void free();
};

