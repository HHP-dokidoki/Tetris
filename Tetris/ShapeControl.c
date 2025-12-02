#include "Common.h"


int GetNextShape(void)
{
	TRACE_ENTER();

	int i;

	i = rand() % 28;
	if (i < 12) return i;
	if (i < 24) return (i - 12) / 2 + 12;
	return 18;
}

int NotConflict(int shape_pos_x, int shape_pos_y, int shape_id)
{
	TRACE_ENTER();

	for (int i = 0; i < 8; i += 2)
	{
		if (shape_pos_x + shapes[shape_id].D[i] < 1
			|| shape_pos_x + shapes[shape_id].D[i] >= NX - 1
			|| shape_pos_y + shapes[shape_id].D[i + 1] >= NY - 1)
		{
			return 0;
		}
		if (shape_pos_y + shapes[shape_id].D[i + 1] < 1)
		{
			continue;
		}
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

	//DOWN  1
	//LEFT  2
	//RIGHT 3
	switch (mode)
	{
	case 1:
		return NotConflict(shape_pos_x, shape_pos_y + 1, shape_id);
	case 2:
		return NotConflict(shape_pos_x - 1, shape_pos_y, shape_id);
	case 3:
		return NotConflict(shape_pos_x + 1, shape_pos_y, shape_id);
	}
	return -1;
}

void MoveShape(int* shape_pos_x, int* shape_pos_y, int shape_id, int mode)
{
	TRACE_ENTER();

	int dx, dy;
	for (int i = 0; i < 8; i += 2)
	{
		dx = shapes[shape_id].D[i];
		dy = shapes[shape_id].D[i + 1];

		// 2025.11.21 1:50  查找到此bug
		// 提到字体指针会莫名被恢复成 0x7f7f7f7f00000000
		// 由于 fonts 为全局变量，所以怀疑是否有另外一个全局指针操作时误将区域上的数据抹除
		// 经过 ***一番排查*** 之后找到这里，当方块再屏幕最上方时 y 偏移后为负值
		// 由于 map 的内存区域极其靠近 fonts 的内存区域，所以就把字体文件擦了 XD
		// 唉唉修好了这个bug又出现了一个新bug
		if (*shape_pos_y + dy < 0 || *shape_pos_x + dx)
		{
			continue;
		}

		State.map[*shape_pos_y + dy][*shape_pos_x + dx].Value = 0;
		State.map[*shape_pos_y + dy][*shape_pos_x + dx].Color = BACKGROUND_COLOR;

		//SDL_Log("======Now In Shape Control======\n");
		//SDL_Log("shape_pos_y:%d --> %d    shape_pos_x:%d --> %d\n",
		//		*shape_pos_y, *shape_pos_y + dy,
		//		*shape_pos_x, *shape_pos_x + dx);
		//SDL_Log("content:%p", fonts.UIFont);
		
	}
	//DOWN  1
	//LEFT  2
	//RIGHT 3
	switch (mode)
	{
	case 1:
		if (NotConflict(*shape_pos_x, *shape_pos_y + 1, shape_id))
		{
			*shape_pos_y += 1;
		}
		break;
	case 2:
		if (NotConflict(*shape_pos_x - 1, *shape_pos_y, shape_id))
		{
			*shape_pos_x -= 1;
		}
		break;
	case 3:
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

	if (NotConflict(shape_pos_x, shape_pos_y, shapes[*shape_id].Next))
	{
		*shape_id = shapes[*shape_id].Next;
	}
}

void FixShape(int shape_pos_x, int shape_pos_y, int shape_id)
{
	TRACE_ENTER();

	int dx = 0, dy = 0;
	for (int i = 0; i < 8; i += 2)
	{
		dx = shapes[shape_id].D[i];
		dy = shapes[shape_id].D[i + 1];
		// 防止越界
		if (shape_pos_y + dy < 0 || shape_pos_x + dx < 0)
		{
			continue;
		}
		State.map[shape_pos_y + dy][shape_pos_x + dx].Value = 1;
		State.map[shape_pos_y + dy][shape_pos_x + dx].Color = shapes[shape_id].Color;
	}
}