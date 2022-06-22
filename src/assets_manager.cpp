#include "assets_manager.h"

std::map<const char*, Texture, utility::StrComp> AssetsManager::textures;
std::map<const char*, Shader, utility::StrComp> AssetsManager::shaders;

Texture AssetsManager::get_or_load_texture( const char* filename )
{
	//  retrieve our registered texture
	if ( textures.find( filename ) != textures.end() )
	{
		//printf( "\nAssets: get \"%s\" (ID: %d) from cache\n", filename, textures[filename].id );
		return textures[filename];
	}

	//  load texture
	Texture texture = LoadTexture( filename );
	textures.insert( std::pair<const char*, Texture>( filename, texture ) );
	//printf( "\nAssets: texture \"%s\" (ID: %d) has been loaded\n", filename, texture.id );
	return texture;
}

Shader AssetsManager::get_or_load_shader( const char* filename )
{
	//  retrieve our registered shader
	if ( textures.find( filename ) != textures.end() )
	{
		//printf( "\nAssets: get \"%s\" (ID: %d) from cache\n", filename, textures[filename].id );
		return shaders[filename];
	}

	//  load texture
	Shader shader = LoadShader( 0, filename );
	shaders.insert( std::pair<const char*, Shader>( filename, shader ) );
	//printf( "\nAssets: texture \"%s\" (ID: %d) has been loaded\n", filename, texture.id );
	return shader;
}

std::vector<Rectangle> AssetsManager::slice_texture( const Texture texture, const int width, const int height )
{
	std::vector<Rectangle> quads;

	for ( int y = 0; y < texture.height; y += height )
		for ( int x = 0; x < texture.width; x += width )
		{
			Rectangle quad { x, y, width, height };
			quads.push_back( quad );
		}

	return quads;
}

void AssetsManager::free()
{
	//  textures
	for ( const std::pair<const char*, Texture>& pair : textures )
	{
		UnloadTexture( pair.second );
		//printf( "Assets: texture \"%s\" has been freed\n", pair.first );
	}
	textures.clear();

	//  shaders
	for ( const std::pair<const char*, Shader>& pair : shaders )
	{
		UnloadShader( pair.second );
		//printf( "Assets: texture \"%s\" has been freed\n", pair.first );
	}
	shaders.clear();
}
