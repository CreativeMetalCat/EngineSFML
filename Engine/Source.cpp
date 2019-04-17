#include "Game.h"


int SCREENWIDTH = 1024;
int SCREENHEIGHT = 720;

using namespace luabridge;


std::string PATH = "C:/Users/catgu/source/repos/Engine/x64/Debug/";

//a = &(*CActor);
int main()
{
	



	lua_State* L = luaL_newstate();

	
	std::string d = (PATH + "scripts/actor.lua");
	try
	{

		int status = luaL_dofile(L, d.c_str());
		if (status != 0)
		{
			fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		}
		luaL_openlibs(L);

		lua_pcall(L, 0, 0, 0);

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