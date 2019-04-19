#include "Game.h"


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



//a = &(*CActor);
int main()
{
	



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
		
		PATH = wTable["mainPath"].cast<std::string>();
		

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

		bool f = textureTable.isTable();
		if (!textureTable.isNil())
		{
			if (textureTable.isTable())
			{
				for (int i = 0; i < textureTable.length(); i++)
				{
					LuaRef textureData = textureTable[i];

					std::string name = textureData["name"].cast<std::string>();

					std::string path = textureData["path"].cast<std::string>();

					bool smooth = textureData["smooth"].cast<bool>();

					bool repeated = textureData["repeated"].cast<bool>();

					int oi = 0;
				}
			}
		}
		game.TextureResources->AddTextureResource(std::make_shared<Engine::Resources::CTextureResource>("dev64_orange", "textures/dev/dev_orange_64x64.png", true, false, PATH));
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