#include "Common.h"

int GetNextShape(void)
{
	TRACE_ENTER();

	// 保证每种方块的概率都为 4/28
	// i <= 12 ：前三种方块，每种方块都有四个不同旋转形态
	// 13 <= i && i <= 23 : 三种只有两个旋转形态的方块，
	//			每个形态出现概率均为 2/28， 加和后方块出现概率为 4/28
// i >= 24 : 正方形

	int i;

	i = rand() % 28;
	if (i < 12) return i;
	if (i < 24) return (i - 12) / 2 + 12;
	return 18;
}

int NotConflict(int shape_pos_x, int shape_pos_y, int shape_id)
{
	TRACE_ENTER();
	// 检查方块是否与边界或已固定方块发生碰撞
	// 遍历方块的4个块（8个坐标值）检查每个块位置
	for (int i = 0; i < 8; i += 2)
	{
		// 检查左右边界和下边界（坐标值在有效范围内）
		if (shape_pos_x + shapes[shape_id].D[i] < 1
			|| shape_pos_x + shapes[shape_id].D[i] >= NX - 1
			|| shape_pos_y + shapes[shape_id].D[i + 1] >= NY - 1)
		{
			return 0;
		}
		// 跳过超出上边界的块（游戏起始时方块可能部分超出屏幕）
		if (shape_pos_y + shapes[shape_id].D[i + 1] < 1)
		{
			continue;
		}
		// 检查该位置是否已被占据
		if (State.map[shape_pos_y + shapes[shape_id].D[i + 1]][shape_pos_x + shapes[shape_id].D[i]].Value)
		{
			return 0;
		}
	}
	return 1;
}

int Moveable(int shape_pos_x, int shape_pos_y, int shape_id, int mode)
{
	TRACE_ENTER();
	// 判断方块在指定方向是否可移动
	// mode: DOWN(1)-下移 LEFT(2)-左移 RIGHT(3)-右移
	switch (mode)
	{
	case 1:
		// 向下移动：检查y+1位置是否无碰撞
		return NotConflict(shape_pos_x, shape_pos_y + 1, shape_id);
	case 2:
		// 向左移动：检查x-1位置是否无碰撞
		return NotConflict(shape_pos_x - 1, shape_pos_y, shape_id);
	case 3:
		// 向右移动：检查x+1位置是否无碰撞
		return NotConflict(shape_pos_x + 1, shape_pos_y, shape_id);
	}
	return -1;
}

void MoveShape(int* shape_pos_x, int* shape_pos_y, int shape_id, int mode)
{
	TRACE_ENTER();
	// 移动方块并更新地图显示
	// 先清除方块原位置的显示，再移动到新位置
	int dx, dy;
	for (int i = 0; i < 8; i += 2)
	{
		dx = shapes[shape_id].D[i];
		dy = shapes[shape_id].D[i + 1];

		// 跳过超出地图范围的块（防止数组越界 这里越界了经常会导致字体文件读取失败）
		if (*shape_pos_y + dy < 0 || *shape_pos_x + dx)
		{
			continue;
		}

		// 清除地图上该位置的方块显示
		State.map[*shape_pos_y + dy][*shape_pos_x + dx].Value = 0;
		State.map[*shape_pos_y + dy][*shape_pos_x + dx].Color = BACKGROUND_COLOR;
	}
	
	// 根据移动方向更新方块位置（已验证无碰撞）
	switch (mode)
	{
	case 1:
		// 下移
		if (NotConflict(*shape_pos_x, *shape_pos_y + 1, shape_id))
		{
			*shape_pos_y += 1;
		}
		break;
	case 2:
		// 左移
		if (NotConflict(*shape_pos_x - 1, *shape_pos_y, shape_id))
		{
			*shape_pos_x -= 1;
		}
		break;
	case 3:
		// 右移
		if (NotConflict(*shape_pos_x + 1, *shape_pos_y, shape_id))
		{
			*shape_pos_x += 1;
		}
		break;
	}
}

void RollShape(int shape_pos_x, int shape_pos_y, int* shape_id)
{
	TRACE_ENTER();
	// 旋转方块：若旋转后无碰撞，则更新方块形态为下一个旋转态
	if (NotConflict(shape_pos_x, shape_pos_y, shapes[*shape_id].Next))
	{
		*shape_id = shapes[*shape_id].Next;
	}
}

void FixShape(int shape_pos_x, int shape_pos_y, int shape_id)
{
	TRACE_ENTER();
	// 方块固定到地图：将方块的4个块标记为已占据，保存对应颜色
	int dx = 0, dy = 0;
	for (int i = 0; i < 8; i += 2)
	{
		dx = shapes[shape_id].D[i];
		dy = shapes[shape_id].D[i + 1];
		// 防止越界：跳过超出地图范围的块
		if (shape_pos_y + dy < 0 || shape_pos_x + dx < 0)
		{
			continue;
		}
		// 标记该位置已被占据，并保存方块颜色
		State.map[shape_pos_y + dy][shape_pos_x + dx].Value = 1;
		State.map[shape_pos_y + dy][shape_pos_x + dx].Color = shapes[shape_id].Color;
	}
}