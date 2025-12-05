#include "Common.h"
#include "GameState.h"


void MapInit(void)
{
	// 初始化游戏地图：清空所有方块，设置边界为已占据状态
	// 地图范围：NX=14 (宽度), NY=23 (高度)
	// 0和NX-1列为左右边界，NY-1行为底部边界
	TRACE_ENTER();

	// 清空整个地图
	memset(State.map, 0, sizeof(State.map));
	
	// 设置左右边界（第0列和第13列）为已占据
	for (int i = 0; i < NY; i++)
	{
		State.map[i][NX - 1].Value = State.map[i][0].Value = 1;
	}
	
	// 设置底部边界（第22行）为已占据
	for (int i = 0; i < NX; i++)
	{
		State.map[NY - 1][i].Value = 1;
	}
}

int LineFull(int y)
{
	// 检查指定行是否完全填满（不计左右边界）
	// 检查范围：1到NX-2列（排除左右边界）
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
	// 删除指定行的方块：上方所有行下移一行，顶部空出空行
	TRACE_ENTER();

	// 从y行开始，将上方所有行向下移动一行
	for (int i = y; i >= 2; i--)
	{
		for (int j = 1; j < NX - 1; j++)
		{
			State.map[i][j] = State.map[i - 1][j];
		}
	}
	
	// 清除第1行的数据（顶部新产生的空行）
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
	// 删除完整的行，最多检查y点周围4行
	// 返回删除的行数*对应分数
	TRACE_ENTER();

	int row_cnt = 0, bonus = 0;
	
	// 从y点附近向下检查最多4行
	for (int i = 0; i < 4; i++)
	{
		// 如果当前行填满，则删除并继续检查
		while (LineFull(y))
		{
			DeleteRow(y);
			row_cnt++;
			i++;
		}
		y--;
		// 超出地图上边界则停止检查
		if (y < 0)
		{
			break;
		}
	}

	// 更新消除行数统计
	State.LineCount += row_cnt;
	
	// 根据删除行数计算得分
	bonus = CalScore(row_cnt);
	
	// 检查是否升级（每升10行升一级难度）
	if (State.LineCount > State.Difficulty * 10)
	{
		LevelUp();
	}
	return bonus;
}
