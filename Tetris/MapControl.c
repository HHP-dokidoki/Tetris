#include "Common.h"
#include "GameState.h"


void MapInit(void)
{
	//SDL_Log("MapInit: GameMap size = %zu bytes", sizeof(State.map));
	//SDL_Log("MapInit: GameMap address = %p", (void*)State.map);
	//SDL_Log("MapInit: fonts address = %p", (void*)&fonts);
	//SDL_Log("MapInit: Distance = %td bytes",
	//	(char*)&fonts - (char*)State.map);
	
	// 游戏区范围：
	// 540----45 * 12
	// 990----45 * 22
	//NX = 14		NY = 24
	TRACE_ENTER();

	memset(State.map, 0, sizeof(State.map));
	for (int i = 0; i < NY; i++)
	{
		State.map[i][NX - 1].Value = State.map[i][0].Value = 1;
	}
	for (int i = 0; i < NX; i++)
	{
		State.map[NY - 1][i].Value = 1;
	}
}

int LineFull(int y)
{
	TRACE_ENTER();

	for (int i = 1; i < NX - 1; i++)
	{
		if (State.map[y][i].Value == 0)
		{
			return 0;
		}
	}
	return 1;
}

void DeleteRow(int y)
{
	TRACE_ENTER();

	for (int i = y; i >= 2; i--)
	{
		for (int j = 1; j < NX - 1; j++)
		{
			State.map[i][j] = State.map[i - 1][j];
		}
	}
	// 清除第一行的数据（如果删除了一行，那么第一行在删除后肯定为空）
	for (int j = 1; j < NX - 1; j++)
	{
		State.map[1][j].Value = 0;
		State.map[1][j].Color.r = 0;
		State.map[1][j].Color.g = 0;
		State.map[1][j].Color.b = 0;
		State.map[1][j].Color.a = 0;
	}
}

int DeleteRows(int y)
{
	TRACE_ENTER();

	int row_cnt = 0,bonus = 0;
	for (int i = 0; i < 4; i++)
	{
		while (LineFull(y))
		{
			DeleteRow(y);
			row_cnt++;
			i++;
		}
		y--;
		// 超出地图则不计
		if (y < 0)
		{
			break;
		}
	}

	State.LineCount += row_cnt;
	bonus = CalScore(row_cnt);
	if (State.LineCount > State.Difficulty * 10)
	{
		LevelUp();
	}
	return bonus;
}
