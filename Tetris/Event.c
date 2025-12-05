#include "Common.h"
#include "ShapeControl.h"
#include "DrawItems.h"
#include "MapControl.h"
#include "GameState.h"
#include "Init.h"


static int EnterGameOverEvent()
{
	TRACE_ENTER();
	// 游戏结束事件处理：显示选择菜单（重新开始或返回欢迎界面）
	// 使用pos变量记录当前菜单光标位置
	SDL_Event event;
	int pos = 0, enter_flag = 0;

	while (1)
	{
		// 绘制游戏结束界面和菜单
		DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
			State.Current_shape, State.Next_shape);
		DrawGVLayout(pos);
		SDL_RenderPresent(rdr);

		enter_flag = 0;
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_WINDOW_RESIZED:
				// 窗口大小改变时重新初始化布局及重新渲染
				RectInit();
				DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
					State.Current_shape, State.Next_shape);
				DrawGVLayout(pos);
				SDL_RenderPresent(rdr);

				break;

			case SDL_EVENT_KEY_DOWN:
				switch (event.key.key)
				{
				case SDLK_LEFT:
					// 左方向键：移动菜单选项（利用模运算实现循环）
					pos++;
					pos = (pos + 2) % 2;
					break;
				case SDLK_RIGHT:
					// 右方向键：移动菜单选项
					pos--;
					pos = (pos + 2) % 2;
					break;
				case SDLK_RETURN:
					// 按下回车键：标记选择完成
					enter_flag = 1;
					break;
				}

				DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
					State.Current_shape, State.Next_shape);
				DrawGVLayout(pos);
				SDL_RenderPresent(rdr);

				break;
			}
			if (enter_flag)
			{
				// 根据菜单选择返回对应状态码
				switch (pos)
				{
				case 0:
					// 选择0：重新开始游戏
					LoadGame();
					// 恢复到未加速状态
					State.Difficulty--;
					LevelUp();

					TRACE_MSG("Restart game.\n");
					return Restart;
					break;
				case 1:
					// 选择1：返回欢迎界面
					ResetGameState();
					TRACE_MSG("Reset game and back to welcome.\n");
					return EnterWelcomePage;
					break;
				}
			}
		}
		// 没有事件时延迟16毫秒，防止循环空转浪费CPU
		SDL_Delay(16);
	}
}

static int EnterTickEvent()
{
	TRACE_ENTER();
	// 定时器事件处理：每隔固定时间下落一次方块或处理碰撞固定
	int state_code = 0;
	
	// 如果方块刚生成就无法下落，说明游戏结束
	if (!Moveable(ST_X, ST_Y, State.Current_shape, DOWN))
	{	
		state_code = EnterGameOverEvent();
		switch (state_code) {
		case Restart:
			TRACE_MSG("Return Restart!\n");
			return Restart;
			break;
		case EnterWelcomePage:
			TRACE_MSG("Return EnterWelcomePage!\n");
			return EnterWelcomePage;
			break;
		}
	}
	// 检查方块是否可以继续下落
	else if (Moveable(State.Shape_pos_x, State.Shape_pos_y, State.Current_shape, DOWN))
	{
		// 方块下落一行
		MoveShape(&State.Shape_pos_x, &State.Shape_pos_y, State.Current_shape, DOWN);
	}
	else
	{
		// 方块无法下落，固定方块到地图，生成新方块
		FixShape(State.Shape_pos_x, State.Shape_pos_y, State.Current_shape);

		// 检查并删除完整的行，并累加分数
		State.Score += DeleteRows(State.Shape_pos_y);

		// 重置新方块位置，使用预生成的下一个方块，再生成新的预览方块
		State.Shape_pos_x = ST_X;
		State.Shape_pos_y = ST_Y;
		State.Current_shape = State.Next_shape;
		State.Next_shape = GetNextShape();
	}

	DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
		State.Current_shape, State.Next_shape);
	SDL_RenderPresent(rdr);

	TRACE_MSG("Return IgnoreMe!\n");
	return IgnoreMe;
}

static int EnterPauseEvent()
{
	// 暂停事件处理：显示暂停菜单（继续、保存、加载、返回）
	TRACE_ENTER();

	// 恢复未加速状态的速度
	State.Difficulty--;
	LevelUp();

	int pos = 0, back_flag = 0, enter_flag = 0;
	SDL_Event event;

	while (1)
	{
		// 绘制暂停菜单界面
		DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
			State.Current_shape, State.Next_shape);
		DrawPauseLayout(pos);
		SDL_RenderPresent(rdr);

		back_flag = enter_flag = 0;
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_WINDOW_RESIZED:
				// 窗口大小改变时重新初始化布局及重新渲染
				RectInit();
				DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
					State.Current_shape, State.Next_shape);
				DrawPauseLayout(pos);
				SDL_RenderPresent(rdr);

				break;
			case SDL_EVENT_KEY_DOWN:
				switch (event.key.key)
				{
				case SDLK_DOWN:
					// 下方向键：向下移动菜单选项（循环）
					pos++;
					pos = (pos + 4) % 4;
					break;
				case SDLK_UP:
					// 上方向键：向上移动菜单选项（循环）
					pos--;
					pos = (pos + 4) % 4;
					break;
				case SDLK_ESCAPE:
					// ESC键：标记返回游戏
					back_flag = 1;
					break;
				case SDLK_RETURN:
					// 回车键：标记选择完成
					enter_flag = 1;
					break;
				}
			}

			if (enter_flag)
			{
				// 根据菜单位置执行对应操作
				switch (pos)
				{
				case 0:
					// 继续游戏
					return 0;
					break;
				case 1:
					// 保存游戏
					SaveGame();
					break;
				case 2:
					// 加载游戏
					LoadGame();

					DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
						State.Current_shape, State.Next_shape);
					DrawPauseLayout(pos);
					SDL_RenderPresent(rdr);

					break;
				case 3:
					// 返回欢迎界面
					TRACE_MSG("Back to welcome.\n");
					return EnterWelcomePage;
					break;
				}
			}
			else if (back_flag)
			{
				// 按ESC返回游戏
				return 0;
			}
		}
		// 没有事件时延迟16毫秒，防止循环空转浪费CPU
		SDL_Delay(16);
	}
}

static int EnterMotionEvent(SDL_Event event)
{
	TRACE_ENTER();
	// 键盘/窗口事件处理：处理方块移动、旋转、加速等动作
	switch (event.type)
	{
		case SDL_EVENT_WINDOW_RESIZED:
			// 窗口大小改变时重新初始化布局及重新渲染
			RectInit();
			DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
				State.Current_shape, State.Next_shape);
			SDL_RenderPresent(rdr);

			break;
		case SDL_EVENT_KEY_DOWN:
			switch (event.key.key)
			{
				case SDLK_DOWN:
					// 按下DOWN键：将下落速度设置为加速间隔
					State.Current_interval = State.Speed_interval;
					break;
				case SDLK_LEFT:
					// 按下LEFT键：左移方块（带碰撞检测）
					MoveShape(&State.Shape_pos_x, &State.Shape_pos_y, State.Current_shape, LEFT);
					DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
						State.Current_shape, State.Next_shape);
					SDL_RenderPresent(rdr);

					break;
				case SDLK_RIGHT:
					// 按下RIGHT键：右移方块（带碰撞检测）
					MoveShape(&State.Shape_pos_x, &State.Shape_pos_y, State.Current_shape,
						RIGHT);
					DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
						State.Current_shape, State.Next_shape);
					SDL_RenderPresent(rdr);

					break;
				case SDLK_SPACE:
					// 按下SPACE键：旋转方块（带碰撞检测）
					RollShape(State.Shape_pos_x, State.Shape_pos_y, &State.Current_shape);
					DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
						State.Current_shape, State.Next_shape);
					SDL_RenderPresent(rdr);

					break;
				case SDLK_ESCAPE:
					// 按下ESC键：进入暂停菜单
					if (EnterPauseEvent() == 2)
					{
						return 2;
					}
					// 如果从暂停菜单返回且方块处于加速状态，需要恢复原速度
					State.Current_interval = State.Base_interval;
					break;
			}
			break;

		case SDL_EVENT_KEY_UP:
			switch (event.key.key)
			{
				case SDLK_DOWN:
					// 松开DOWN键：恢复原始下落速度
					State.Current_interval = State.Base_interval;
					break;
			}
			break;
	}
	return 0;
}

static int EnterGameEventLoop(void)
{
	TRACE_ENTER();
	// 游戏主循环：处理定时器事件和用户输入事件
	// 根据时间间隔触发方块下落，同时处理用户操作
	int state_code = 0;
	SDL_Event event;

	// 初始化计时器
	State.Last_tick = State.Current_tick = SDL_GetTicks();
	// 第一次进入循环时预设时间，使得第一帧不会立即下落
	State.Current_tick += State.Current_interval;

	while (1)
	{
		// 如果是首次进入循环，初始化当前方块和预览方块
		if (State.Next_shape == -1)
		{
			State.Current_shape = GetNextShape();
			State.Next_shape = GetNextShape();

			DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
				State.Current_shape, State.Next_shape);
			SDL_RenderPresent(rdr);

		}
		// 获取当前时间
		State.Current_tick = SDL_GetTicks();

		// 检查是否达到下落时间间隔
		if (State.Current_tick - State.Last_tick >= State.Current_interval)
		{
			// 触发定时器事件：方块下落或固定
			state_code = EnterTickEvent();
			switch (state_code)
			{
			case EnterWelcomePage:
				// 游戏结束，返回欢迎界面
				return 2;
				break;
			case Restart:
				// 游戏结束，重新开始
				return Restart;
				break;
			}
			// 重置计时器
			State.Current_tick = State.Last_tick = SDL_GetTicks();
		}

		// 检查并处理用户输入事件
		if (SDL_PollEvent(&event))
		{
			state_code = EnterMotionEvent(event);
			if (state_code == 2)
			{
				// 在暂停菜单选择返回欢迎界面
				return EnterWelcomePage;
			}
		}
	}
	return IgnoreMe;
}

static int EnterWelcomeEventLoop(void)
{
	TRACE_ENTER();
	// 欢迎界面主循环：显示菜单并处理用户选择（开始游戏、加载游戏、退出）
	SDL_Event event;
	int pos = 0, enter_flag = 0;

	while (1)
	{
		// 绘制欢迎界面菜单
		DrawWelcomeLayout(pos);
		SDL_RenderPresent(rdr);
		enter_flag = 0;
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_EVENT_WINDOW_RESIZED:
					// 窗口大小改变时重新初始化布局及重新渲染
					RectInit();
					DrawWelcomeLayout(pos);
					SDL_RenderPresent(rdr);

					break;
				case SDL_EVENT_KEY_DOWN:
					switch (event.key.key)
					{
						case SDLK_DOWN:
							// 下方向键：向下移动菜单选项（循环）
							pos++;
							pos = (pos + 3) % 3;

							break;
						case SDLK_UP:
							// 上方向键：向上移动菜单选项（循环）
							pos--;
							pos = (pos + 3) % 3;

							break;
						case SDLK_RETURN:
							// 回车键：标记选择完成
							enter_flag = 1;
							break;
					}
					DrawWelcomeLayout(pos);
					SDL_RenderPresent(rdr);
					break;
			}
			if (enter_flag)
			{
				// 根据菜单选择返回对应状态码
				switch (pos)
				{
				case 0:
					// 选择：开始新游戏
					return StartGame;
					break;
				case 1:
					// 选择：加载已保存游戏
					LoadGame();
					return StartGame;
					break;
				case 2:
					// 选择：退出游戏
					return LeaveGame;
					break;
				}
			}
		}
		// 没有事件时延迟16毫秒，防止循环空转浪费CPU
		SDL_Delay(16);
	}
}

int EnterMainEventLoop(void)
{
	TRACE_ENTER();
	// 应用主事件循环：协调欢迎界面和游戏主循环的状态转移
	// 循环处理三个主要界面：欢迎界面、游戏界面、游戏结束界面
	int state_code = 0;
	state_code = EnterWelcomeEventLoop();
	while (1)
	{
		// 根据状态码进行状态转移
		if (state_code == StartGame || state_code == Restart)
		{
			// 进入游戏主循环
			state_code = EnterGameEventLoop();
			continue;
		}
		else if (state_code == EnterWelcomePage)
		{
			// 返回欢迎界面
			state_code = EnterWelcomeEventLoop();
			continue;
		}
		else if (state_code == LeaveGame)
		{
			// 退出游戏
			break;
		}
	}

	return 0;
}