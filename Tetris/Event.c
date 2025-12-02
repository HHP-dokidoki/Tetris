#include "Common.h"
#include "ShapeControl.h"
#include "DrawItems.h"
#include "MapControl.h"
#include "GameState.h"
#include "Init.h"


int EnterGameOverEvent()
{
	TRACE_ENTER();
	SDL_Event event;
	int pos = 0, enter_flag = 0;
	DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
		State.Current_shape, State.Next_shape);
	DrawGVLayout(pos);
	while (1)
	{
		enter_flag = 0;
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
				RectInit();
				DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
					State.Current_shape, State.Next_shape);
				DrawGVLayout(pos);
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					pos++;
					pos = (pos + 2) % 2;
					break;
				case SDLK_RIGHT:
					pos--;
					pos = (pos + 2) % 2;
					break;
				case SDLK_RETURN:
					enter_flag = 1;
					break;
				}
				DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
					State.Current_shape, State.Next_shape);
				DrawGVLayout(pos);
				break;
			}
			if (enter_flag)
			{
				switch (pos)
				{
				case 0:
					// Load;
					LoadGame();
					// 将interval更新为存档时的状态（未加速）
					State.Difficulty--;
					LevelUp();

					TRACE_MSG("Restart game.\n");
					return Restart;
					break;
				case 1:
					// Leave
					ResetGameState();
					TRACE_MSG("Reset game and back to welcome.\n");
					return EnterWelcomePage;
					break;
				}
			}
		}
	}
}

int EnterTickEvent()
{
	TRACE_ENTER();

	int state_code = 0;
	if (!Moveable(ST_X, ST_Y, State.Current_shape, DOWN))
	{	// 方块刚刚生成就无法下落
		state_code = EnterGameOverEvent();
		switch (state_code) {
			// 状态码：
			// 0、1 进入游戏
			// 2 进入欢迎界面
			// 4 退出游戏
			// 5 重开
			// 6 忽略此状态码
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
	else if (Moveable(State.Shape_pos_x, State.Shape_pos_y, State.Current_shape, DOWN))
	{// 方块下落
		MoveShape(&State.Shape_pos_x, &State.Shape_pos_y, State.Current_shape, DOWN);
	}
	else
	{

		FixShape(State.Shape_pos_x, State.Shape_pos_y, State.Current_shape);

		State.Score += DeleteRows(State.Shape_pos_y);

		State.Shape_pos_x = ST_X;
		State.Shape_pos_y = ST_Y;
		State.Current_shape = State.Next_shape;
		State.Next_shape = GetNextShape();
	}

	DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
		State.Current_shape, State.Next_shape);

	TRACE_MSG("Return IgnoreMe!\n");
	return IgnoreMe;
}

int EnterPauseEvent()
{
	// 画出Pause界面：continue、save、load、leave
	TRACE_ENTER();

	int pos = 0, back_flag = 0, enter_flag = 0;
	SDL_Event event;
	DrawPauseLayout(pos);
	while (1)
	{
		back_flag = enter_flag = 0;
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_WINDOWEVENT:
					RectInit();
					DrawPauseLayout(pos);
				break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_DOWN:
							pos++;
							pos = (pos + 4) % 4;
							break;
						case SDLK_UP:
							pos--;
							pos = (pos + 4) % 4;
							break;
						case SDLK_ESCAPE:
							back_flag = 1;
							break;
						case SDLK_RETURN:
							enter_flag = 1;
							break;
					}
			}
			DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y, 
				State.Current_shape, State.Next_shape);
			DrawPauseLayout(pos);
		}

		if (enter_flag)
		{
			// 0为continue，1为save，2为load，3为leave
			switch (pos)
			{
			case 0:
				return 0;
				break;
			case 1:
				SaveGame();
				break;
			case 2:
				LoadGame();
				
				// 将interval更新为存档时的状态（未加速）
				State.Difficulty--;
				LevelUp();

				DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
					State.Current_shape, State.Next_shape);
				DrawPauseLayout(pos);

				break;
			case 3:
				// 状态码 返回welcome界面
				TRACE_MSG("Back to welcome.\n");
				return EnterWelcomePage;
				break;

			}
		}
		else if (back_flag)
		{
			// 按esc返回游戏
			return 0;
		}
	}

}

int EnterMotionEvent(SDL_Event event)
{
	TRACE_ENTER();

	switch (event.type)
	{
		case SDL_QUIT:
			return 0;
		case SDL_WINDOWEVENT:
			RectInit();
			DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
				State.Current_shape, State.Next_shape);
			break;
		case SDL_MOUSEMOTION:
			SDL_Log("x:%d  y:%d\n", event.motion.x, event.motion.y);
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				case SDLK_DOWN:
					//accelerate
					State.Current_interval = State.Speed_interval;
					break;
				case SDLK_LEFT:
					MoveShape(&State.Shape_pos_x, &State.Shape_pos_y, State.Current_shape, LEFT);
					DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
						State.Current_shape, State.Next_shape);
					break;
				case SDLK_RIGHT:
					MoveShape(&State.Shape_pos_x, &State.Shape_pos_y, State.Current_shape,
						RIGHT);
					DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
						State.Current_shape, State.Next_shape);
					break;
				case SDLK_SPACE:
					RollShape(State.Shape_pos_x, State.Shape_pos_y, &State.Current_shape);
					DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
						State.Current_shape, State.Next_shape);
					break;
				case SDLK_ESCAPE:
					if (EnterPauseEvent() == 2)
					{
						return 2;
					}
					// 如果暂停时处于加速状态
					// 没有下面的代码则回复时会一直加速
					State.Current_interval = State.Base_interval;
					break;
			}
			//SDL_Log("(sym)%d (sacncode)%d wan pressed", event.key.keysym.sym, event.key.keysym.scancode);
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
				case SDLK_DOWN:
					// 恢复原速度
					State.Current_interval = State.Base_interval;
					break;
			}
			//SDL_Log("(sym)%d (sacncode)%d wan released", event.key.keysym.sym, event.key.keysym.scancode);
			break;
			//.......
	}
	return 0;
}

int EnterGameEventLoop(void)
{
	TRACE_ENTER();
	int state_code = 0;
	SDL_Event event;

	State.Last_tick = State.Current_tick = SDL_GetTicks();
	//第一次进入循环时渲染游戏界面
	State.Current_tick += State.Current_interval;

	while (1)
	{

		if (State.Next_shape == -1)
		{
			State.Current_shape = GetNextShape();
			State.Next_shape = GetNextShape();
			DrawHoleWindow(State.Shape_pos_x, State.Shape_pos_y,
				State.Current_shape, State.Next_shape);
		}
		State.Current_tick = SDL_GetTicks();


		/* 计时器时间间隔达到上限*/
		if (State.Current_tick - State.Last_tick >= State.Current_interval)
		{
			state_code = EnterTickEvent();
			switch (state_code)
			{
			case EnterWelcomePage:
				return 2;
				break;
			case Restart:
				return Restart;
				break;
			}
			// 清零计时器，开始下一次下落
			State.Current_tick = State.Last_tick = SDL_GetTicks();
		}

		if (SDL_PollEvent(&event))
		{
			state_code = EnterMotionEvent(event);
			if (state_code == 2)
			{// 在pause界面选择了QUIT
				return EnterWelcomePage;
			}
		}
	}
	return IgnoreMe;
}

int EnterWelcomeEventLoop(void)
{
	TRACE_ENTER();

	SDL_Event event;
	int pos = 0, enter_flag = 0;
	DrawWelcomeLayout(pos);
	while (1)
	{
		enter_flag = 0;
		if (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_WINDOWEVENT:
					RectInit();
					DrawWelcomeLayout(pos);
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_DOWN:
							pos++;
							pos = (pos + 3) % 3;
							DrawWelcomeLayout(pos);
							//SDL_Log("pos:%d\n", pos);
							break;
						case SDLK_UP:
							pos--;
							pos = (pos + 3) % 3;
							DrawWelcomeLayout(pos);
							//SDL_Log("pos:%d\n", pos);
							break;
						case SDLK_RETURN:
							enter_flag = 1;
							break;
					}
					break;
			}
			if (enter_flag)
			{
				switch (pos)
				{
				case 0:
					// Start;
					return StartGame;
					break;
				case 1:
					//Load
					LoadGame();
					return StartGame;
					break;
				case 2:
					//Quit
					return LeaveGame;
					break;
				}
			}

		}
	}
}

int EnterMainEventLoop(void)
{
	TRACE_ENTER();

	int state_code = 0;
	state_code = EnterWelcomeEventLoop();
	while (1)
	{
		// 状态码：
		// 0、1 进入游戏
		// 2 进入欢迎界面
		// 4 退出游戏
		// 5 重开
		// 6 忽略此状态码

		if (state_code == StartGame || state_code == Restart)
		{
			state_code = EnterGameEventLoop();
			continue;
		}
		else if (state_code == EnterWelcomePage)
		{
			state_code = EnterWelcomeEventLoop();
			continue;
		}
		else if (state_code == LeaveGame)
		{
			break;
		}
	}

	return 0;
}