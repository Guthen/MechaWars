#include "assets_manager.h"

std::map<std::string, Texture> AssetsManager::textures;
std::map<std::string, Shader> AssetsManager::shaders;

Texture AssetsManager::get_or_load_texture( std::string filename )
{
	//  retrieve our registered texture
	if ( textures.find( filename ) != textures.end() )
	{
		//printf( "\nAssets: get \"%s\" (ID: %d) from cache\n", filename, textures[filename].id );
		return textures[filename];
	}

	//  load texture
	Texture texture = LoadTexture( filename.c_str() );
	textures.insert( std::pair<std::string, Texture>( filename, texture ) );
	printf( "\nAssets: texture \"%s\" (ID: %d) has been loaded\n", filename.c_str(), texture.id);
	return texture;
}

Shader AssetsManager::get_or_load_shader( std::string filename )
{
	//  retrieve our registered shader
	if ( shaders.find( filename ) != shaders.end() )
	{
		//printf( "\nAssets: get \"%s\" (ID: %d) from cache\n", filename, textures[filename].id );
		return shaders[filename];
	}

	//  load texture
	Shader shader = LoadShader( 0, filename.c_str() );
	shaders.insert( std::pair<std::string, Shader>( filename, shader ) );
	printf( "\nAssets: shader \"%s\" (ID: %d) has been loaded\n", filename.c_str(), shader.id);
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
	for ( const auto& pair : textures )
	{
		UnloadTexture( pair.second );
		printf( "Assets: texture \"%s\" has been freed\n", pair.first.c_str() );
	}
	textures.clear();

	//  shaders
	for ( const auto& pair : shaders )
	{
		UnloadShader( pair.second );
		printf( "Assets: shader \"%s\" has been freed\n", pair.first.c_str() );
	}
	shaders.clear();
}
