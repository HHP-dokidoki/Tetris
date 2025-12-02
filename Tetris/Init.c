#include "common.h"

void RandInit(void)
{
	TRACE_ENTER();

	srand((unsigned int)SDL_GetTicks());
}

void RectInit(void)
{
	TRACE_ENTER();

	SDL_Rect window;
	SDL_GetRendererOutputSize(rdr, &window.w, &window.h);

	long double ratio = fmin((long double)window.w / STD_W, (long double)window.h / STD_H);

	layout.ratio = ratio;
	
	int now_w = STD_W * ratio;
	int now_h = STD_H * ratio;

	long double s_text_h = STD_SMALL_TEXT_HEIGHT * ratio;
	long double s_text_w = STD_SMALL_TEXT_WIDTH * ratio;
	long double b_text_h = STD_BIG_TEXT_HEIGHT * ratio;
	long double b_text_w = STD_BIG_TEXT_WIDTH * ratio;
	long double preview_block_size = STD_PREVIEW_BLOCK_SIZE * ratio;
	long double block_size = STD_BLOCK_SIZE * ratio;

	/* ---------------- 游戏区域 ----------------*/

	layout.LeftBorderRect.w = block_size / 5;
	layout.LeftBorderRect.h = now_h;
	layout.LeftBorderRect.x = 0;
	layout.LeftBorderRect.y = 0;

	layout.ButtonBorderRect.w = 12 * block_size + 2 * block_size / 5;
	layout.ButtonBorderRect.h = block_size / 5;
	layout.ButtonBorderRect.x = 0;
	layout.ButtonBorderRect.y = now_h - block_size / 5;

	layout.GameAreaRect.w = 12 * block_size;
	layout.GameAreaRect.h = 21 * block_size;
	layout.GameAreaRect.x = layout.LeftBorderRect.w;
	layout.GameAreaRect.y = now_h - layout.GameAreaRect.h;

	layout.UpperboundBorderRect.w = layout.ButtonBorderRect.w;
	layout.UpperboundBorderRect.h = now_h - layout.GameAreaRect.h;
	layout.UpperboundBorderRect.x = 0;
	layout.UpperboundBorderRect.y = 0;

	layout.RightBorderRect.w = layout.LeftBorderRect.w;
	layout.RightBorderRect.h = layout.LeftBorderRect.h;
	layout.RightBorderRect.x = layout.GameAreaRect.w + block_size / 5;
	layout.RightBorderRect.y = 0;



	/* ---------------- 侧边栏 ---------------- */

	long double msg_spacing = s_text_h + (20 * ratio);
	long double base_y = now_h / 16;
	long double base_x = now_w * 0.78;
	// Next Preview
	layout.NextPreviewRect.x = base_x;
	layout.NextPreviewRect.y = base_y;
	layout.NextPreviewRect.h = s_text_h;
	layout.NextPreviewRect.w = s_text_w * 10;

	layout.NextPreviewShapeRect.x = base_x + 2 * preview_block_size;
	layout.NextPreviewShapeRect.y = base_y + 2 * msg_spacing;

	// Score
	layout.ScoreLabelRect.x = base_x;
	layout.ScoreLabelRect.y = base_y + 3 * msg_spacing;
	layout.ScoreLabelRect.h = s_text_h;
	layout.ScoreLabelRect.w = s_text_w * 5;

	layout.ScoreValueRect.x = base_x;
	layout.ScoreValueRect.y = base_y + 4 * msg_spacing;
	layout.ScoreValueRect.h = s_text_h;
	layout.ScoreValueRect.w = s_text_w;

	// Lines
	layout.LineCountLabelRect.x = base_x;
	layout.LineCountLabelRect.y = base_y + 5 * msg_spacing;
	layout.LineCountLabelRect.h = s_text_h;
	layout.LineCountLabelRect.w = s_text_w * 9;

	layout.LineCountValueRect.x = base_x;
	layout.LineCountValueRect.y = base_y + 6 * msg_spacing;
	layout.LineCountValueRect.h = s_text_h;
	layout.LineCountValueRect.w = s_text_w;

	// Difficulty
	layout.DifficultyRect.x = base_x;
	layout.DifficultyRect.y = base_y + 7 * msg_spacing;
	layout.DifficultyRect.h = s_text_h;
	layout.DifficultyRect.w = s_text_w * 10;

	layout.DifficultyValueRect.x = base_x;
	layout.DifficultyValueRect.y = base_y + 8 * msg_spacing;
	layout.DifficultyValueRect.h = s_text_h;
	layout.DifficultyValueRect.w = s_text_w;


	/* ---------------- 暂停界面 ---------------- */
	// pm --> pause
	long double pm_x = 0.16 * now_w;
	long double pm_y = 0.1 * now_h;
	long double pm_w = 10 * b_text_w;
	long double pm_h = 6.5 * b_text_h;
	long double p_midline = pm_x + pm_w / 2;

	layout.PauseMenuRect.x = pm_x;
	layout.PauseMenuRect.y = pm_y;
	layout.PauseMenuRect.h = pm_h;
	layout.PauseMenuRect.w = pm_w;

	// Title (Big Text * 1.5)
	layout.PauseMessageRect.x = p_midline - 2.5 * 1.5 * b_text_w;
	layout.PauseMessageRect.y = pm_y + (10 * ratio); // 略微向下偏移
	layout.PauseMessageRect.h = b_text_h * 1.5;
	layout.PauseMessageRect.w = b_text_w * 1.5 * 5;

	// Buttons
	// btn --> button
	long double pm_btn_y = pm_y + 1.5 * b_text_h + (30 * ratio);
	long double pm_spacing = b_text_h + (20 * ratio);

	layout.ContinueRect.x = p_midline - 4 * b_text_w;
	layout.ContinueRect.y = pm_btn_y;
	layout.ContinueRect.h = b_text_h;
	layout.ContinueRect.w = b_text_w * 8;

	layout.SaveRect.x = p_midline - 2 * b_text_w;
	layout.SaveRect.y = pm_btn_y + pm_spacing;
	layout.SaveRect.h = b_text_h;
	layout.SaveRect.w = b_text_w * 4;

	layout.LoadRect.x = p_midline - 2 * b_text_w;
	layout.LoadRect.y = pm_btn_y + pm_spacing * 2;
	layout.LoadRect.h = b_text_h;
	layout.LoadRect.w = b_text_w * 4;

	layout.LeaveRect.x = p_midline - 2.5 * b_text_w;
	layout.LeaveRect.y = pm_btn_y + pm_spacing * 3;
	layout.LeaveRect.h = b_text_h;
	layout.LeaveRect.w = b_text_w * 5;


	/* ---------------- 开始界面 ---------------- */
	long double wel_x = 0.175 * now_w;
	long double wel_y = 0.1 * now_h;
	long double wel_w = 10 * b_text_w;
	long double wel_h = 6 * b_text_h;
	long double wel_midline = wel_x + wel_w / 2;

	layout.WelPageRect.x = wel_x;
	layout.WelPageRect.y = wel_y;
	layout.WelPageRect.h = wel_h;
	layout.WelPageRect.w = wel_w;

	// Title (Big Text)
	layout.WelcomeRect.x = wel_midline - 3.5 * 1.5 * b_text_w;
	layout.WelcomeRect.y = wel_y + (20 * ratio);
	layout.WelcomeRect.h = 1.5 * b_text_h;
	layout.WelcomeRect.w = 1.5 * b_text_w * 7;

	// Buttons
	long double wel_btn_y = wel_y + 1.5 * b_text_h + (30 * ratio);
	long double wel_spacing = b_text_h + (20 * ratio);

	layout.StartRect.x = wel_midline - 2.5 * (long double)b_text_w;
	layout.StartRect.y = wel_btn_y;
	layout.StartRect.h = b_text_h * ratio;
	layout.StartRect.w = b_text_w * ratio * 5;


	layout.WelLoadRect.x = wel_midline - 2 * (long double)b_text_w;
	layout.WelLoadRect.y = wel_btn_y + wel_spacing;
	layout.WelLoadRect.h = b_text_h * ratio;
	layout.WelLoadRect.w = b_text_w * ratio * 4;

	layout.WelLeaveRect.x = wel_midline - 2.5 * (long double)b_text_w;
	layout.WelLeaveRect.y = wel_btn_y + wel_spacing * 2;
	layout.WelLeaveRect.h = b_text_h * ratio;
	layout.WelLeaveRect.w = b_text_w * ratio * 5;


	/* ---------------- Game Over Page ---------------- */
	long double gv_x = 0.06 * now_w;
	long double gv_y = 0.35 * now_h;
	long double gv_w = 10 * b_text_w;
	long double gv_h = 3 * b_text_h;
	long double gv_midline = gv_x + gv_w / 2;

	layout.GVPageRect.x = gv_x;
	layout.GVPageRect.y = gv_y;
	layout.GVPageRect.h = gv_h;
	layout.GVPageRect.w = gv_w;


	// Title (Big Text for consistency)
	layout.GameOverRect.x = gv_midline - 4 * b_text_w ;
	layout.GameOverRect.y = gv_y + 20 * ratio;
	layout.GameOverRect.h = b_text_h * ratio;
	layout.GameOverRect.w = b_text_w * ratio * 9;

	// Buttons
	long double gv_btn_y = gv_y + b_text_h + (20 * ratio);

	layout.GVLoadRect.x = gv_midline - 5 * b_text_w;
	layout.GVLoadRect.y = gv_btn_y;
	layout.GVLoadRect.h = b_text_h * ratio;
	layout.GVLoadRect.w = b_text_w * ratio * 4;

	layout.GVLeaveRect.x = gv_midline + 2 * b_text_w;
	layout.GVLeaveRect.y = gv_btn_y;
	layout.GVLeaveRect.h = b_text_h * ratio;
	layout.GVLeaveRect.w = b_text_w * ratio * 5;
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

	int win_w, win_h;
	SDL_Rect screen;
	SDL_GetDisplayUsableBounds(0, &screen);

	// 防止超出屏幕
	screen.h -= 100;

	// 计算相对标注尺寸的缩放比例
	long double ratio = fmin((long double)screen.w / STD_W, (long double)screen.h / STD_H);
	win_w = STD_W * ratio;
	win_h = STD_H * ratio;
	// 创建窗口
	win = SDL_CreateWindow("Tetris", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_w, win_h,
		 SDL_WINDOW_RESIZABLE);
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
