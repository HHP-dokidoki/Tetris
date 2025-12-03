#include "common.h"

void RandInit(void)
{
	TRACE_ENTER();
	// 初始化随机数生成器，使用当前时间作为种子
	srand((unsigned int)SDL_GetTicks());
}

void RectInit(void)
{
	TRACE_ENTER();
	// 根据窗口大小计算所有UI元素的位置和尺寸（支持窗口缩放）
	// 计算缩放比例，确保UI按比例调整
	SDL_Rect window;
	SDL_GetWindowSize(win, &window.w, &window.h);

	float ratio = fmin((float)window.w / STD_W, (float)window.h / STD_H);

	layout.ratio = ratio;
	
	int now_w = STD_W * ratio;
	int now_h = STD_H * ratio;

	// 计算各类元素的缩放尺寸
	float s_text_h = STD_SMALL_TEXT_HEIGHT * ratio;
	float s_text_w = STD_SMALL_TEXT_WIDTH * ratio;
	float b_text_h = STD_BIG_TEXT_HEIGHT * ratio;
	float b_text_w = STD_BIG_TEXT_WIDTH * ratio;
	float preview_block_size = STD_PREVIEW_BLOCK_SIZE * ratio;
	float block_size = STD_BLOCK_SIZE * ratio;

	/* ---- 游戏区域：边框和游戏面板 ---- */
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
	layout.GameAreaRect.y = now_h - layout.GameAreaRect.h - layout.ButtonBorderRect.h;

	layout.UpperboundBorderRect.w = layout.ButtonBorderRect.w;
	layout.UpperboundBorderRect.h = now_h - layout.GameAreaRect.h - layout.ButtonBorderRect.h;
	layout.UpperboundBorderRect.x = 0;
	layout.UpperboundBorderRect.y = 0;

	layout.RightBorderRect.w = layout.LeftBorderRect.w;
	layout.RightBorderRect.h = layout.LeftBorderRect.h;
	layout.RightBorderRect.x = layout.GameAreaRect.w + block_size / 5;
	layout.RightBorderRect.y = 0;

	/* ---- 侧边栏：分数、行数、难度、预览 ---- */
	float msg_spacing = s_text_h + (20 * ratio);
	float base_y = now_h / 16;
	float base_x = now_w * 0.78;
	
	// 下一个方块预览区
	layout.NextPreviewRect.x = base_x;
	layout.NextPreviewRect.y = base_y;
	layout.NextPreviewRect.h = s_text_h;
	layout.NextPreviewRect.w = s_text_w * 10;

	layout.NextPreviewShapeRect.x = base_x + 2 * preview_block_size;
	layout.NextPreviewShapeRect.y = base_y + 2 * msg_spacing;

	// 分数显示区
	layout.ScoreLabelRect.x = base_x;
	layout.ScoreLabelRect.y = base_y + 3 * msg_spacing;
	layout.ScoreLabelRect.h = s_text_h;
	layout.ScoreLabelRect.w = s_text_w * 5;

	layout.ScoreValueRect.x = base_x;
	layout.ScoreValueRect.y = base_y + 4 * msg_spacing;
	layout.ScoreValueRect.h = s_text_h;
	layout.ScoreValueRect.w = s_text_w;

	// 消除行数显示区
	layout.LineCountLabelRect.x = base_x;
	layout.LineCountLabelRect.y = base_y + 5 * msg_spacing;
	layout.LineCountLabelRect.h = s_text_h;
	layout.LineCountLabelRect.w = s_text_w * 9;

	layout.LineCountValueRect.x = base_x;
	layout.LineCountValueRect.y = base_y + 6 * msg_spacing;
	layout.LineCountValueRect.h = s_text_h;
	layout.LineCountValueRect.w = s_text_w;

	// 难度显示区
	layout.DifficultyRect.x = base_x;
	layout.DifficultyRect.y = base_y + 7 * msg_spacing;
	layout.DifficultyRect.h = s_text_h;
	layout.DifficultyRect.w = s_text_w * 10;

	layout.DifficultyValueRect.x = base_x;
	layout.DifficultyValueRect.y = base_y + 8 * msg_spacing;
	layout.DifficultyValueRect.h = s_text_h;
	layout.DifficultyValueRect.w = s_text_w;

	/* ---- 暂停菜单布局 ---- */
	float pm_x = 0.16 * now_w;
	float pm_y = 0.1 * now_h;
	float pm_w = 10 * b_text_w;
	float pm_h = 6.5 * b_text_h;
	float p_midline = pm_x + pm_w / 2;

	layout.PauseMenuRect.x = pm_x;
	layout.PauseMenuRect.y = pm_y;
	layout.PauseMenuRect.h = pm_h;
	layout.PauseMenuRect.w = pm_w;

	// 菜单标题
	layout.PauseMessageRect.x = p_midline - 2.5 * 1.5 * b_text_w;
	layout.PauseMessageRect.y = pm_y + (10 * ratio);
	layout.PauseMessageRect.h = b_text_h * 1.5;
	layout.PauseMessageRect.w = b_text_w * 1.5 * 5;

	// 菜单选项按钮
	float pm_btn_y = pm_y + 1.5 * b_text_h + (30 * ratio);
	float pm_spacing = b_text_h + (20 * ratio);

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

	/* ---- 欢迎菜单布局 ---- */
	float wel_x = 0.175 * now_w;
	float wel_y = 0.1 * now_h;
	float wel_w = 10 * b_text_w;
	float wel_h = 6 * b_text_h;
	float wel_midline = wel_x + wel_w / 2;

	layout.WelPageRect.x = wel_x;
	layout.WelPageRect.y = wel_y;
	layout.WelPageRect.h = wel_h;
	layout.WelPageRect.w = wel_w;

	// 菜单标题
	layout.WelcomeRect.x = wel_midline - 3.5 * 1.5 * b_text_w;
	layout.WelcomeRect.y = wel_y + (20 * ratio);
	layout.WelcomeRect.h = 1.5 * b_text_h;
	layout.WelcomeRect.w = 1.5 * b_text_w * 7;

	// 菜单选项按钮
	float wel_btn_y = wel_y + 1.5 * b_text_h + (30 * ratio);
	float wel_spacing = b_text_h + (20 * ratio);

	layout.StartRect.x = wel_midline - 2.5 * b_text_w;
	layout.StartRect.y = wel_btn_y;
	layout.StartRect.h = b_text_h;
	layout.StartRect.w = b_text_w * 5;

	layout.WelLoadRect.x = wel_midline - 2 * b_text_w;
	layout.WelLoadRect.y = wel_btn_y + wel_spacing;
	layout.WelLoadRect.h = b_text_h;
	layout.WelLoadRect.w = b_text_w * 4;

	layout.WelLeaveRect.x = wel_midline - 2.5 * b_text_w;
	layout.WelLeaveRect.y = wel_btn_y + wel_spacing * 2;
	layout.WelLeaveRect.h = b_text_h;
	layout.WelLeaveRect.w = b_text_w * 5;

	/* ---- 游戏结束菜单布局 ---- */
	float gv_x = 0.06 * now_w;
	float gv_y = 0.35 * now_h;
	float gv_w = 10 * b_text_w;
	float gv_h = 2.5 * b_text_h;
	float gv_midline = gv_x + gv_w / 2;

	layout.GVPageRect.x = gv_x;
	layout.GVPageRect.y = gv_y;
	layout.GVPageRect.h = gv_h;
	layout.GVPageRect.w = gv_w;

	// 菜单标题
	layout.GameOverRect.x = gv_midline - 4.5 * b_text_w;
	layout.GameOverRect.y = gv_y + 20 * ratio;
	layout.GameOverRect.h = b_text_h;
	layout.GameOverRect.w = b_text_w * 9;

	// 菜单选项按钮
	float gv_btn_y = gv_y + b_text_h + (20 * ratio);

	layout.GVLoadRect.x = gv_midline - 5 * b_text_w;
	layout.GVLoadRect.y = gv_btn_y;
	layout.GVLoadRect.h = b_text_h;
	layout.GVLoadRect.w = b_text_w * 4;

	layout.GVLeaveRect.x = gv_midline + 2 * b_text_w;
	layout.GVLeaveRect.y = gv_btn_y;
	layout.GVLeaveRect.h = b_text_h;
	layout.GVLeaveRect.w = b_text_w * 5;
}

int FontInit(void)
{
	TRACE_ENTER();
	// 初始化字体系统：加载UI字体和数字字体

	if (!TTF_Init())
	{
		TRACE_MSG("Init TTF Failed.");
		return -1;
	}

	// 设置字体颜色为白色
	fonts.Color.r = 255; fonts.Color.g = 255;
	fonts.Color.b = 255; fonts.Color.a = 255;

	// 加载字体文件
	fonts.NumberFont = TTF_OpenFont("..\\Font\\SourceCodePro-ExtraBoldItalic.ttf", 50);
	fonts.UIFont = TTF_OpenFont("..\\Font\\VT323-Regular.ttf", 50);

	if (fonts.NumberFont == NULL || fonts.UIFont == NULL)
	{
		TRACE_MSG("Open Font Failed.");
		return -1;
	}

	return 0;
}

int WindowRdrInit(void)
{
	TRACE_ENTER();
	// 初始化SDL窗口和渲染器

	// 初始化SDL视频和音频模块
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
	{
		TRACE_MSG(SDL_GetError());
		return -1;
	}

	// 获取屏幕尺寸，防止窗口超过屏幕
	int win_w, win_h;
	SDL_Rect screen;
	if (!SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &screen))
	{
		screen.w = STD_W / 2;
		screen.h = STD_H / 2;
	}

	if (screen.h > 100)
	{
		screen.h -= 100;
	}

	// 计算缩放比例，按标准分辨率等比缩放
	float ratio = fmin((float)screen.w / STD_W, (float)screen.h / STD_H);
	win_w = (int)(STD_W * ratio + 0.5L);
	win_h = (int)(STD_H * ratio + 0.5L);

	// 创建可调整大小的窗口
	win = SDL_CreateWindow("Tetris", 
		 win_w, win_h,
		 SDL_WINDOW_RESIZABLE);
	if (win == NULL)
	{
		TRACE_MSG(SDL_GetError());
		return -1;
	}
	// 设置窗口宽高比为3:4
	SDL_SetWindowAspectRatio(win, 0.75f, 0.75f);

	// 创建渲染器
	rdr = SDL_CreateRenderer(win, NULL);
	if (rdr == NULL)
	{
		TRACE_MSG(SDL_GetError());
		SDL_DestroyWindow(win);
		return -1;
	}
	
	// 初始化渲染器：黑色背景，启用混合模式（支持透明度）
	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
	SDL_SetRenderDrawBlendMode(rdr, SDL_BLENDMODE_BLEND);
	SDL_RenderClear(rdr);
	SDL_RenderPresent(rdr);

	// 清空游戏地图
	memset(State.map, 0, sizeof(State.map));
	return 0;
}

void FontDeInit(void)
{
	TRACE_ENTER();
	// 释放字体资源
	TTF_CloseFont(fonts.NumberFont);
	TTF_CloseFont(fonts.UIFont);
	TTF_Quit();
}

int LogInit(void)
{
	// 初始化日志文件用于debug输出
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
	// 关闭日志文件
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
	// 清理所有资源：字体、日志、渲染器、窗口、SDL库
	FontDeInit();
	LogDeintit();
	SDL_DestroyRenderer(rdr);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
