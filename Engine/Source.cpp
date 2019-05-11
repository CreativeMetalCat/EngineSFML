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
		
		game.GameContext->ShaderResources->AddShaderResource(std::make_shared<Engine::Resources::Materials::CShaderResource>("test","shaders/testshader.vert", "shaders/testshader.frag",PATH));
		game.GameContext->ShaderResources->AddShaderResource(std::make_shared<Engine::Resources::Materials::CShaderResource>("normal", "shaders/normal.vert", "shaders/normal.frag", PATH));

		
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

						game.GameContext->TextureResources->AddTextureResource(std::make_shared<Engine::Resources::Materials::CTextureResource>(name, pathToFile, smooth, repeated, PATH));
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

		//-----------------------------------------------------------------

		d = (PATH + "scripts/Animations.lua");

		status = luaL_dofile(L, d.c_str());
		if (status != 0)
		{
			fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		}
		luaL_openlibs(L);

		lua_pcall(L, 0, 0, 0);

		LuaRef animationsTable = getGlobal(L, "animations");

		if (!animationsTable.isNil())
		{
			if (animationsTable.isTable())
			{
				//In LUA first array index is 1 while in c/c++ it's zero
				for (int i = 1; i <= animationsTable.length(); i++)
				{
					LuaRef animData = animationsTable[i];
					//if there is a mistake in table we skip this field and continue
					if (!animData.isNil())
					{
						std::string name = animData["name"].cast<std::string>();

						std::string texture_name = animData["texture_name"].cast<std::string>();
						sf::Vector2f size;
						if (!animData["frame_size"].isNil() && animData["frame_size"].isTable())
						{
							size = sf::Vector2f(animData["frame_size"]["x"].cast<int>(), animData["frame_size"]["y"].cast<int>());
						}

						LuaRef patternTable = animData["patterns"];

						std::vector<Engine::Animation::SpritesheetAnimationPattern>patterns;

						if (!patternTable.isNil() && patternTable.isTable())
						{
							std::cout << patternTable.length() << std::endl;
							for (int u = 1; u <= patternTable.length(); u++)
							{
								std::string pattern_name = animData["patterns"][u]["name"].cast<std::string>();

								patterns.push_back(Engine::Animation::SpritesheetAnimationPattern(pattern_name, {}));

								LuaRef indicies = animData["patterns"][u]["indicies"];
								if (!indicies.isNil() && indicies.isTable())
								{
									for (int o = 1; o <= indicies.length(); o++)
									{

										patterns.at(patterns.size() - 1).Indices.push_back(sf::Vector2i(indicies[o]["x"].cast<int>(), indicies[o]["y"].cast<int>()));
									}
								}
							}


						}

						game.GameContext->SpritesheetAnimations.push_back(std::make_shared< Engine::Animation::SpritesheetAnimation>(size, texture_name));
						game.GameContext->SpritesheetAnimations.at(game.GameContext->SpritesheetAnimations.size() - 1)->Animations = patterns;

					}
					else
					{
						std::cout << "LUA Warning: Attempted to read Nil value in table. Execution will be continued\n";
					}


				}
			}
		}

		//----------------------------------------------------------------------------------

		std::shared_ptr<Engine::Animation::SpritesheetAnimation> Anim = std::make_shared<Engine::Animation::SpritesheetAnimation>(sf::Vector2f(64.f, 64.f), "dev64_anim");
		Anim->Animations.push_back(Engine::Animation::SpritesheetAnimationPattern("anim", { {0,0},{1,0} }));
		Anim->CurrentAnimationName = "anim";
		game.GameContext->SpritesheetAnimations.push_back(Anim);
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