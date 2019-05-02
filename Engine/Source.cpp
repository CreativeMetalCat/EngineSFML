#include "Game.h"
#include <filesystem>

int SCREENWIDTH = 1024;
int SCREENHEIGHT = 720;

using namespace luabridge;

//delete this before releasing or any check outside of this machine
#define DEBUG




#ifdef DEBUG
std::string PATH = "C:/Users/catgu/source/repos/Engine/x64/Debug/";
#else
std::string PATH = "./../";
#endif // !DEBUG


int main()
{
	std::cout << PATH << std::endl;
	
	
	lua_State* L = luaL_newstate();
	try
	{
		std::string d = (PATH + "scripts/TexturesPaths.lua");

		

		d = (PATH + "scripts/window.lua");
		luaL_dofile(L, d.c_str());
		luaL_openlibs(L);

		lua_pcall(L, 0, 0, 0);

		//Get table with window data
		LuaRef wTable = getGlobal(L, "window");
		//Get table with window size that is inside of window data table
		LuaRef wSize = wTable["size"];

		SCREENWIDTH = wSize["width"].cast<int>();
		SCREENHEIGHT = wSize["height"].cast<int>();
		
		//PATH = wTable["mainPath"].cast<std::string>();
		

		std::string title = wTable["title"].cast<std::string>();



		Game game(title, sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), PATH);
		
		//-----------------------------------------------------------------


		d = (PATH + "scripts/TexturesPaths.lua");

		int status = luaL_dofile(L, d.c_str());
		if (status != 0)
		{
			fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		}
		luaL_openlibs(L);

		lua_pcall(L, 0, 0, 0);

		LuaRef textureTable = getGlobal(L, "textures");

		if (!textureTable.isNil())
		{
			if (textureTable.isTable())
			{
				//In LUA first array index is 1 while in c/c++ it's zero
				for (int i = 1; i <= textureTable.length(); i++)
				{
					LuaRef textureData = textureTable[i];
					//if there is a mistake in table we skip this field and continue
					if (!textureData.isNil())
					{
						std::string name = textureData["name"].cast<std::string>();

						std::string pathToFile = textureData["path"].cast<std::string>();

						bool smooth = textureData["smooth"].cast<bool>();

						bool repeated = textureData["repeated"].cast<bool>();

						game.TextureResources->AddTextureResource(std::make_shared<Engine::Resources::Materials::CTextureResource>(name, pathToFile, smooth, repeated, PATH));
					}
					else 
					{
						std::cout << "LUA Warning: Attempted to read Nil value in table. Execution will be continued\n";
					}

					
				}
			}
		}
		
		//----------------------------------------------------------------------------------



		d = (PATH + "scripts/SoundPaths.lua");

		status = luaL_dofile(L, d.c_str());
		if (status != 0)
		{
			fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		}
		luaL_openlibs(L);

		lua_pcall(L, 0, 0, 0);

		LuaRef soundTable = getGlobal(L, "sounds");

		if (!soundTable.isNil())
		{
			if (soundTable.isTable())
			{
				//In LUA first array index is 1 while in c/c++ it's zero
				for (int i = 1; i <= soundTable.length(); i++)
				{
					LuaRef soundData = soundTable[i];
					//if there is a mistake in table we skip this field and continue
					if (!soundData.isNil())
					{
						std::string name = soundData["name"].cast<std::string>();

						std::string pathToFile = soundData["path"].cast<std::string>();


						game.GameContext->Sounds->AddSoundResource(std::make_shared<Engine::Resources::Sound::CSoundResource>(name, pathToFile, PATH));
					}
					else
					{
						std::cout << "LUA Warning: Attempted to read Nil value in table. Execution will be continued\n";
					}


				}
			}
		}

		//----------------------------------------------------------------------------------
		game.Init();

		game.Run();

	
	}
	catch (LuaException e)
	{
		std::cout << e.what() << std::endl;
	}

	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	
	std::cin.get();
}