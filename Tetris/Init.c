#include "common.h"

void RandInit(void)
{
	TRACE_ENTER();

	srand((unsigned int)SDL_GetTicks());
}

void RectInit(void)
{
	TRACE_ENTER();

	layout.DifficultyRect.x = DifficultyPosx;
	layout.DifficultyRect.y = DifficultyPosy;
	layout.DifficultyRect.w = TextHeight;

	layout.NextPreviewRect.x = NextPreviewPosx;
	layout.NextPreviewRect.y = NextPreviewPosy;
	layout.NextPreviewRect.h = TextHeight;

	layout.ScoreLabelRect.x = ScoreLabelPosx;
	layout.ScoreLabelRect.y = ScoreLabelPosy;
	layout.ScoreLabelRect.h = TextHeight;

	layout.LineCountLabelRect.x = LineCountLabelPosx;
	layout.LineCountLabelRect.y = LineCountLabelPosy;
	layout.LineCountLabelRect.h = TextHeight;

	layout.ScoreValueRect.x = ScoreLabelPosx + 15; //让数字不要太靠近变框，不然不美观
	layout.ScoreValueRect.y = ScoreLabelPosy + TextHeight + 20;
	layout.ScoreValueRect.h = TextHeight;

	layout.DifficultyValueRect.x = DifficultyPosx + 15;
	layout.DifficultyValueRect.y = DifficultyPosy + TextHeight + 20;
	layout.DifficultyValueRect.h = TextHeight;

	layout.LineCountValueRect.x = LineCountLabelPosx + 15;
	layout.LineCountValueRect.y = LineCountLabelPosy + TextHeight + 20;
	layout.LineCountValueRect.h = TextHeight;

	layout.NextPreviewShapeRect.x = NextPreviewPosx + 55;
	layout.NextPreviewShapeRect.y = NextPreviewPosy + +TextHeight + 90;

	layout.PauseMenuRect.x = PauseMenuPosx;
	layout.PauseMenuRect.y = PauseMenuPosy;
	layout.PauseMenuRect.w = PauseMenuWidth;
	layout.PauseMenuRect.h = PauseMenuHeight;

	layout.PauseMessageRect.x = PauseMenuPosx + 30; // + n 是为了居中对齐
	layout.PauseMessageRect.y = PauseMenuPosy;
	layout.PauseMessageRect.h = TextHeight;

	layout.ContinueRect.x = PauseMenuPosx;
	layout.ContinueRect.y = PauseMenuPosy + TextHeight * 2 + 30;
	layout.ContinueRect.h = TextHeight;

	layout.SaveRect.x = PauseMenuPosx + 55;
	layout.SaveRect.y = PauseMenuPosy + TextHeight * 3 + 60;
	layout.SaveRect.h = TextHeight;

	layout.LoadRect.x = PauseMenuPosx + 55;
	layout.LoadRect.y = PauseMenuPosy + TextHeight * 4 + 90;
	layout.LoadRect.h = TextHeight;

	layout.LeaveRect.x = PauseMenuPosx + 47;
	layout.LeaveRect.y = PauseMenuPosy + TextHeight * 5 + 120;
	layout.LeaveRect.h = TextHeight;

	layout.WelPageRect.x = WelPagePosx;
	layout.WelPageRect.y = WelPagePosy;
	layout.WelPageRect.h = WelPageHeight;
	layout.WelPageRect.w = WelPageWidth;

	layout.WelcomeRect.x = WelcomePosx;
	layout.WelcomeRect.y = WelcomePosy;
	layout.WelcomeRect.h = WelcomeHeight;
	layout.WelcomeRect.w = WelcomeWidth;

	layout.StartRect.x = StartPosx;
	layout.StartRect.y = StartPosy;
	layout.StartRect.h = StartHeight;
	layout.StartRect.w = StartWidth;

	layout.WelLoadRect.x = WelLoadPosx;
	layout.WelLoadRect.y = WelLoadPosy;
	layout.WelLoadRect.h = WelLoadHeight;
	layout.WelLoadRect.w = WelLoadWidth;

	layout.WelLeaveRect.x = WelLeavePosx;
	layout.WelLeaveRect.y = WelLeavePosy;
	layout.WelLeaveRect.h = WelLeaveHeight;
	layout.WelLeaveRect.w = WelLeaveWidth;

	layout.GVPageRect.x = GVPageRectPosx;
	layout.GVPageRect.y = GVPageRectPosy;
	layout.GVPageRect.w = GVPageRectWidth;
	layout.GVPageRect.h = GVPageRectHeight;

	layout.GameOverRect.x = GameOverRectPosx;
	layout.GameOverRect.y = GameOverRectPosy;
	layout.GameOverRect.w = GameOverRectWidth;
	layout.GameOverRect.h = GameOverRectHeight;

	layout.GVLoadRect.x = GVLoadRectPosx;
	layout.GVLoadRect.y = GVLoadRectPosy;
	layout.GVLoadRect.w = GVLoadRectWidth;
	layout.GVLoadRect.h = GVLoadRectHeight;

	layout.GVLeaveRect.x = GVLeaveRectPosx;
	layout.GVLeaveRect.y = GVLeaveRectPosy;
	layout.GVLeaveRect.w = GVLeaveRectWidth;
	layout.GVLeaveRect.h = GVLeaveRectHeight;
	
}

int FontInit(void)
{
	TRACE_ENTER();

	if (TTF_Init())
	{
		TRACE_MSG(TTF_GetError());
		return -1;
	}

	fonts.Color.r = 255; fonts.Color.g = 255;
	fonts.Color.b = 255; fonts.Color.a = 255;

	fonts.NumberFont = TTF_OpenFont("..\\Font\\SourceCodePro-ExtraBoldItalic.ttf", 50);
	fonts.UIFont = TTF_OpenFont("..\\Font\\VT323-Regular.ttf", 50);



	if (fonts.NumberFont == NULL || fonts.UIFont == NULL)
	{
		TRACE_MSG(TTF_GetError());
		return -1;
	}


	return 0;
}

int WindowRdrInit(void)
{
	TRACE_ENTER();

	// 初始化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
	{
		TRACE_MSG(SDL_GetError());
		return -1;
	}

	// 创建窗口
	win = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_INPUT_GRABBED);
	if (win == NULL)
	{
		TRACE_MSG(SDL_GetError());
		return -1;
	}

	// 创建渲染器、初始化渲染颜色与渲染模式
	rdr = SDL_CreateRenderer(win, -1, 0);
	if (rdr == NULL)
	{
		TRACE_MSG(SDL_GetError());
		return -1;
	}
	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
	SDL_SetRenderDrawBlendMode(rdr, SDL_BLENDMODE_BLEND);
	SDL_RenderClear(rdr);
	SDL_RenderPresent(rdr);

	// 地图状态设为 0
	memset(State.map, 0, sizeof(State.map));
	return 0;
}

void FontDeInit(void)
{
	TRACE_ENTER();

	TTF_CloseFont(fonts.NumberFont);
	TTF_CloseFont(fonts.UIFont);
	TTF_Quit();
}

int LogInit(void)
{
	errno_t err;
	err = fopen_s(&log_file, "..\\log\\game_Trace.log", "w");
	if (log_file && err == 0)
	{
		fprintf(log_file, "=== Game Session Started ===\n");
		fflush(log_file);
	}
	else
	{
		log_file = NULL;
		return -1;
	}
	return 0;
}

void LogDeintit(void)
{
	TRACE_ENTER();

	if (log_file)
	{
		fprintf(log_file, "=== Game Session Ended ===\n");
		fflush(log_file);
		fclose(log_file);
		log_file = NULL;
	}
}

void DeInit(void)
{
	TRACE_ENTER();

	FontDeInit();

	LogDeintit();

	SDL_DestroyRenderer(rdr);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
