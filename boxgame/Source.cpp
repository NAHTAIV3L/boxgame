#include <vector>
#include <fstream>
#include <memory>
#include <algorithm>
#include "Maps.h"
#include "Vector2D.h"

#ifdef __linux__
	#define CLEAR std::system("clear")
#else
	#define CLEAR std::system("cls")
#endif

bool win = false;
unsigned int MapNumber = 0;
char Direction;


// auto win code: ddsssawwwwwdwaaaa sssawwwwwwwawdddd sssaawwwawdddsdwwwawd sssaawwwwwwssssssddddwwwwww


auto PosCalc = [](const vi2d& ref) { return (ref.y * 10 + ref.x); };

struct Entity
{
	virtual char Symbol() const { return ';'; }
	virtual void DrawSelf() const {}
	virtual bool Pushable(const char from) const { return false; }
};

struct Player : public Entity
{
	char symbol = '@';

	char Symbol() const override
	{
		return symbol;
	}

	void DrawSelf() const override
	{
		std::cout << symbol;
	}
	
	bool Pushable(const char from) const override
	{
		return true;
	}
};

struct Box : public Entity
{
	char symbol = '+';

	char Symbol() const override
	{
		return symbol;
	} 

	void DrawSelf() const override
	{
		std::cout << symbol;
	}

	bool Pushable(const char from) const override
	{
		return true;
	}
};

struct Wall : public Entity
{
	char symbol = '#';

	char Symbol() const override
	{
		return symbol;
	}

	void DrawSelf() const override
	{
		std::cout << symbol;
	}

	bool Pushable(const char from) const override
	{
		return false;
	}
};

struct VerticalBox : public Entity
{
	char symbol = '|';

	char Symbol() const override
	{
		return symbol;
	}

	void DrawSelf() const override
	{
		std::cout << symbol;
	}

	bool Pushable(const char from) const override
	{
		if (from == 'w' || from == 's')
			return true;
		else
			return false;
	}
};

struct HorizontalBox : public Entity
{
	char symbol = '-';

	char Symbol() const override
	{
		return symbol;
	}

	void DrawSelf() const override
	{
		std::cout << symbol;
	}

	bool Pushable(const char from) const override
	{
		if (from == 'a' || from == 'd')
			return true;
		else
			return false;
	}
};

struct Last_Move
{
	bool bLastAllowPush;
	vi2d vLastBlock;
	vi2d vLastSource;
	vi2d vLastTarget;
	char LastMove;
};

int igoals = 0;
vi2d vPlayer;
vi2d vBlock;
vi2d vSource;
unsigned int LastMoveOffSet = 0;
std::vector<vi2d> Goals;
std::vector<std::string> Map;
std::vector<std::unique_ptr<Entity>> LoadedMap;
std::vector<Last_Move> LastMove;

bool ScanGoals(const int& x, const int& y)
{
	for (auto g : Goals)
	{
		if (g.x == x && g.y == y)
			return true;
		else 
		{}
	}
	return false;
}

void LoadMap(std::string map)
{
	LoadedMap.clear();
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			switch (map[PosCalc({ x, y })])
			{
			case '#':
				LoadedMap.emplace_back(std::make_unique<Wall>());
				break;
			case '.':
				LoadedMap.emplace_back(nullptr);
				break;
			case '@':
				LoadedMap.emplace_back(std::make_unique<Player>());
				vPlayer = { x, y };
				break;
			case '+':
				LoadedMap.emplace_back(std::make_unique<Box>()); 
				break;
			case '-':
				LoadedMap.emplace_back(std::make_unique<HorizontalBox>()); 
				break;
			case '|':
				LoadedMap.emplace_back(std::make_unique<VerticalBox>()); 
				break;
			case '=':
				Goals.push_back({ x, y });
				igoals++;
				LoadedMap.emplace_back(nullptr);
				break;
			}
		}
	}
}

void init()
{
	Map.emplace_back(Map1);
	Map.emplace_back(Map2);
	Map.emplace_back(Map3);
	Map.emplace_back(Map4);
	Map.emplace_back(Map5);
	Map.emplace_back(Map6);
	Map.emplace_back(Map7);
}

std::string GetFileExt(const std::string& str)
{
	
	int position = str.find_last_of(".");

	
	return str.substr(position + 1);
}

void LoadMapFromMaster(const std::string& MasterMap, const int& MapOffset)
{
	std::string map;
	map.resize(100);
	for (int i = 0; i < 100; i++)
	{
		map[i] = MasterMap[i + (MapOffset * 100)];
	}
	Map.emplace_back(map);
}

void init(const std::string& MapFile)
{
	if (GetFileExt(MapFile) == "Maps")
	{
		std::ifstream file(MapFile);

		std::string MasterMap;
		while (!file.eof())
		{
			MasterMap += file.get();
		}
		std::remove(MasterMap.begin(), MasterMap.end(), '\n');
		int Maps = (MasterMap.size() / 100); 
		for (int i = 0; i < Maps; i++)
		{
			LoadMapFromMaster(MasterMap, i);
		}
		
	}
	else
	{
		std::cout << "File was not a .Maps file";
	}
}

void PrintMap()
{
	CLEAR;
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			if (LoadedMap[PosCalc({ x, y })] == nullptr)
			{		
				if (ScanGoals(x, y))
				{
					std::cout << "=";
				}
				else 
				{
					std::cout << ".";
				}
				
			}
			else 
			{
				LoadedMap[PosCalc({ x, y })]->DrawSelf();
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}



void GetInput()
{
	std::cout << "What direction do you want to go: ";
	std::cin >> Direction;
}

void Move()
{
	vBlock = vPlayer;
	bool bPushing = false;

	if (Direction == 'r')
	{
		igoals = 0;
		MapNumber--;
	}
	else if (Direction == 'u')
	{
		if (LastMove[0].bLastAllowPush)
		{
			while (LastMove[0].vLastBlock != LastMove[0].vLastTarget)
			{
				LastMove[0].vLastSource = LastMove[0].vLastBlock;
				switch (LastMove[0].LastMove)
				{
				case 'w': LastMove[0].vLastSource.y++; break;
				case 's': LastMove[0].vLastSource.y--; break;
				case 'd': LastMove[0].vLastSource.x--; break;
				case 'a': LastMove[0].vLastSource.x++; break;
				}

				std::swap(LoadedMap[PosCalc(LastMove[0].vLastSource)], LoadedMap[PosCalc(LastMove[0].vLastBlock)]);
				LastMove[0].vLastBlock = LastMove[0].vLastSource;
			}

			switch (LastMove[0].LastMove)
			{
			case 'w': vPlayer.y--; break;
			case 's': vPlayer.y++; break;
			case 'd': vPlayer.x++; break;
			case 'a': vPlayer.x--; break;
			}
		}
		LastMove.pop_back();
	}
	else if (Direction == 'w')
	{
		bPushing = true;
	}
	else if (Direction == 'a')
	{
		bPushing = true;
	}
	else if (Direction == 's')
	{
		bPushing = true;
	}
	else if (Direction == 'd')
	{
		bPushing = true;
	}
	else
	{
		bPushing = false;
	}

	if (bPushing)
	{
		bool bAllowPush = false;
		bool bTest = true;
		

		while (bTest)
		{
			if (LoadedMap[PosCalc(vBlock)] != nullptr)
			{
				if (LoadedMap[PosCalc(vBlock)]->Pushable(Direction))
				{
					switch (Direction)
					{
					case 'w': vBlock.y--; break;
					case 's': vBlock.y++; break;
					case 'd': vBlock.x++; break;
					case 'a': vBlock.x--; break;
					}
				}
				else
					bTest = false;
			}
			else
			{
				bAllowPush = true;
				bTest = false;
			}
		}

		switch (Direction)
		{
		case 'w':LastMove.push_back({bAllowPush, vPlayer, {0, 0}, vBlock, 's'}); break;
		case 's':LastMove.push_back({bAllowPush, vPlayer, {0, 0}, vBlock, 'w'}); break;
		case 'a':LastMove.push_back({bAllowPush, vPlayer, {0, 0}, vBlock, 'd'}); break;
		case 'd':LastMove.push_back({bAllowPush, vPlayer, {0, 0}, vBlock, 'a'}); break;
		}

		if (bAllowPush)
		{
			while (vBlock != vPlayer)
			{
				vSource = vBlock;
				switch (Direction)
				{
				case 'w': vSource.y++; break;
				case 's': vSource.y--; break;
				case 'd': vSource.x--; break;
				case 'a': vSource.x++; break;
				}
				std::swap(LoadedMap[PosCalc(vSource)], LoadedMap[PosCalc(vBlock)]);

				vBlock = vSource;
			}

			switch (Direction)
			{
			case 'w': vPlayer.y--; break;
			case 's': vPlayer.y++; break;
			case 'd': vPlayer.x++; break;
			case 'a': vPlayer.x--; break;
			}
		}
	}
}

void CheckWin()
{
	if (igoals == 0)
	{
		MapNumber++;
		win = true;
	}
	else 
	{}
}

void CheckGoals()
{
	for (auto& g : Goals)
	{
		if (LoadedMap[PosCalc(g)] == nullptr)
		{

		}
		else if (LoadedMap[PosCalc(g)]->Symbol() == '+')
		{
			igoals--;
			LoadedMap[PosCalc(g)] = std::make_unique<Wall>();
		}
		else
		{ }

	}
	CheckWin();
}

int main(int argc, char **argv)
{
	
	if (argc > 1)
	{
		std::string MapFile;
		for (int i = 1; i <= argc; i++)
		{
			MapFile = argv[i];
			init(MapFile);
		}
	}
	else
	{
		init();
	}
	LoadMap(Map[MapNumber]);

	while (true)
	{
		PrintMap();
		GetInput();
		Move();
		CheckGoals();
		if (MapNumber < Map.size())
		{
			if (win)
			{
				LastMove.clear();
				Goals.clear();
				LoadMap(Map[MapNumber]);
				win = false;
			}
		}
		else
		{
			break;
		}
	}
	
	PrintMap();
	std::cin.get();
	return 0;
}