#include "Common.h"


void TetrisDrawText(char content[], TTF_Font* font, SDL_FRect* rect, int mode)
{
	TRACE_ENTER();
	// 渲染文本到屏幕
	// mode=1: 固定宽度；mode=2: 根据文本长度动态计算宽度
	SDL_Texture* texture;
	SDL_Surface* text_surf;

	// 根据mode计算文本框宽度
	SDL_FRect dest_frect = *rect;
	
	if (mode == 2)
	{
		// 动态宽度：按字符个数×单位宽度计算
		dest_frect.w = strlen(content) * rect->w;
	}
	else
	{
		// 固定宽度
		dest_frect.w = rect->w;
	}

	// 渲染文本为表面
	text_surf = TTF_RenderText_Blended(font, content, strlen(content), fonts.Color);
	if (text_surf == NULL) {
		SDL_Log("TTF_RenderText_Blended Failed.");
		return;
	}

	// 将表面转为GPU纹理（频繁创建销毁会影响性能，但游戏规模小可接受）
	texture = SDL_CreateTextureFromSurface(rdr, text_surf);
	if (texture == NULL)
	{
		SDL_Log("SDL_CreateTextureFromSurface Failed.\n");
		SDL_DestroySurface(text_surf);
		return;
	}

	// 渲染纹理到屏幕
	if (SDL_RenderTexture(rdr, texture, NULL, &dest_frect) < 0)
	{
		SDL_Log("SDL_RenderCopy Failed.\n");
		return;
	}

	// 清理资源
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
	}
	if (text_surf != NULL)
	{
		SDL_DestroySurface(text_surf);
	}
}

void DrawRect(SDL_FRect *rect,SDL_Color color)
{
	TRACE_ENTER();
	// 绘制填充矩形，用于背景、菜单框等UI元素
	SDL_SetRenderDrawColor(rdr, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(rdr, rect);
}

void DrawBlock(int x, int y, SDL_Color color, float size)
{
	TRACE_ENTER();
	// 绘制方块：填充正方形+边框（边框颜色略深）
	SDL_FRect block;
	block.w = size;
	block.h = size;
	block.x = x;
	block.y = y;
	// 填充
	SDL_SetRenderDrawColor(rdr, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(rdr, &block);
	// 边框：颜色加深10以增加视觉效果
	SDL_SetRenderDrawColor(rdr, color.r + 10, color.g + 10, color.b + 10, 127);
	SDL_RenderRect(rdr, &block);
}

void DrawSingleShape(int shape_id, int shape_pos_x, int shape_pos_y,float size)
{
	// 绘制单个方块形状（4个小正方形组成）
	// 坐标系：shape_pos_x/y以map格子为单位（从1开始），需转换为像素坐标
	TRACE_ENTER();

	if (shape_id < 0 || shape_id >= 19)
	{
		SDL_Log("Invalid shape id :%d\n", shape_id);
		return;
	}
	int dx, dy;
	for (int i = 0; i < 8; i += 2)
	{
		dx = shapes[shape_id].D[i];
		dy = shapes[shape_id].D[i + 1];
		// 坐标转换：map坐标(减1)→像素坐标(乘size)+偏移值
		DrawBlock((shape_pos_x + dx - 1) * size + layout.LeftBorderRect.w,
				(shape_pos_y + dy - 1) * size + layout.UpperboundBorderRect.h,
				  shapes[shape_id].Color,
			      size);
	}
}

void DrawShapes(int shape_pos_x, int shape_pos_y, int shape_id)
{
	TRACE_ENTER();
	// 绘制所有方块：已固定在地图上的+当前下落的方块
	float block_size = (layout.ratio * STD_BLOCK_SIZE);
	
	// 绘制已固定的方块（遍历map）
	for (int i = 1; i <= NY - 2; i++)
	{
		for (int j = 1; j <= NX - 2; j++)
		{
			if (State.map[i][j].Value)
			{
				DrawBlock(
					block_size * (j - 1) + layout.LeftBorderRect.w,
					block_size * (i - 1) + layout.UpperboundBorderRect.h,
					State.map[i][j].Color,
					block_size
						);
			}
		}
	}

	// 绘制当前下落的方块
	DrawSingleShape(shape_id, shape_pos_x, shape_pos_y, block_size);
}

void DrawPreviewShape(int shape_id, int shape_pos_x, int shape_pos_y, float size)
{
	// 在预览区绘制下一个方块（大小和位置独立于游戏区）
	TRACE_ENTER();

	if (shape_id < 0 || shape_id >= 19)
	{
		SDL_Log("Invalid shape id :%d\n", shape_id);
		return;
	}
	int dx, dy;
	for (int i = 0; i < 8; i += 2)
	{
		// 预览区使用绝对像素坐标（不需要地图坐标转换）
		dx = shapes[shape_id].D[i] * size;
		dy = shapes[shape_id].D[i + 1] * size;
		DrawBlock(shape_pos_x + dx,
			shape_pos_y + dy,
			shapes[shape_id].Color,
			size);
	}
}

void ToString(int value, char* buffer)
{
	TRACE_ENTER();
	// 将整数转换为字符串（用于显示分数、行数、难度等）
	sprintf_s(buffer, 20, "%d", value);
}

void DrawDynamicItems(int nxt_shape_id)
{
	TRACE_ENTER();
	// 绘制动态变化的UI元素：分数、难度、行数、预览方块
	char str[20];
	
	// 绘制分数
	ToString(State.Score, str);
	TetrisDrawText(str, fonts.NumberFont, &layout.ScoreValueRect, 2);

	// 绘制难度
	ToString(State.Difficulty, str);
	TetrisDrawText(str, fonts.NumberFont, &layout.DifficultyValueRect, 2);

	// 绘制消除行数
	ToString(State.LineCount, str);
	TetrisDrawText(str, fonts.NumberFont, &layout.LineCountValueRect, 2);

	// 绘制预览方块
	DrawPreviewShape(nxt_shape_id,
		layout.NextPreviewShapeRect.x,
		layout.NextPreviewShapeRect.y,
		(STD_PREVIEW_BLOCK_SIZE * layout.ratio)
	);
}

void DrawGameLayout(void)
{
	TRACE_ENTER();
	// 绘制游戏界面的静态UI：背景、边框、标签文字

	SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);

	// 游戏区背景和边框
	DrawRect(&layout.GameAreaRect, BACKGROUND_COLOR);
	DrawRect(&layout.UpperboundBorderRect, UPPERBOUND_COLOR);
	DrawRect(&layout.LeftBorderRect, BORDER_COLOR);
	DrawRect(&layout.RightBorderRect, BORDER_COLOR);
	DrawRect(&layout.ButtonBorderRect, BORDER_COLOR);

	// 侧边栏标签文字
	TetrisDrawText("NEXT SHAPE", fonts.UIFont, &layout.NextPreviewRect, 1);
	TetrisDrawText("DIFFICULTY", fonts.UIFont, &layout.DifficultyRect, 1);
	TetrisDrawText("SCORE", fonts.UIFont, &layout.ScoreLabelRect, 1);
	TetrisDrawText("LINE COUNT", fonts.UIFont, &layout.LineCountLabelRect, 1);
}

static SDL_FRect* PauseRect_p[5] =
{ 
	&layout.ContinueRect, &layout.SaveRect, &layout.LoadRect, 
	&layout.LeaveRect, &layout.PauseMessageRect 
};
void DrawPauseLayout(int pos)
{
	TRACE_ENTER();
	// 绘制暂停菜单：半透明背景+菜单选项+高亮显示当前选项
	if (pos < 0 || pos >= 5) {
		SDL_Log("Invalid position index: %d\n", pos);
		return;
	}

	// 菜单背景
	DrawRect(&layout.PauseMenuRect, PauseMenu_Color);

	// 菜单选项文字
	TetrisDrawText("Paused", fonts.UIFont, &layout.PauseMessageRect, 1);
	TetrisDrawText("Continue", fonts.UIFont, &layout.ContinueRect, 1);
	TetrisDrawText("Save", fonts.UIFont, &layout.SaveRect, 1);
	TetrisDrawText("Load", fonts.UIFont, &layout.LoadRect, 1);
	TetrisDrawText("Leave", fonts.UIFont, &layout.LeaveRect, 1);

	// 高亮当前选中的菜单项
	DrawRect(PauseRect_p[pos], Highlight_Color);
}

static SDL_FRect* WelRect_p[3] =
{
	&layout.StartRect, &layout.WelLoadRect, &layout.WelLeaveRect
};
void DrawWelcomeLayout(int pos)
{
	TRACE_ENTER();
	// 绘制欢迎菜单：清空屏幕+标题+选项+高亮显示当前选项
	if (pos < 0 || pos > 2) {
		SDL_Log("Invalid position index: %d\n", pos);
		return;
	}

	// 清空屏幕（黑色背景）
	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
	SDL_RenderClear(rdr);

	// 菜单背景
	DrawRect(&layout.WelPageRect, PauseMenu_Color);

	// 标题和选项
	TetrisDrawText("TETRIS", fonts.UIFont, &layout.WelcomeRect, 1);
	TetrisDrawText("Start", fonts.UIFont, &layout.StartRect, 1);
	TetrisDrawText("Load", fonts.UIFont, &layout.WelLoadRect, 1);
	TetrisDrawText("Leave", fonts.UIFont, &layout.WelLeaveRect, 1);

	// 高亮当前选中的菜单项
	DrawRect(WelRect_p[pos], Highlight_Color);
}

void DrawHoleWindow(int shape_pos_x, int shape_pos_y, int current_shape, int next_shape)
{
	TRACE_ENTER();
	// 绘制游戏窗口全部内容：清空→背景→方块→动态数值
	
	// 清空屏幕
	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
	SDL_RenderClear(rdr);

	// 绘制静态UI（边框、标签）
	DrawGameLayout();
	
	// 绘制所有方块
	DrawShapes(shape_pos_x, shape_pos_y, current_shape);

	// 绘制动态UI（分数、难度、预览）
	DrawDynamicItems(next_shape);
}

static SDL_FRect* GVRect_p[2] =
{
	&layout.GVLoadRect, &layout.GVLeaveRect
};
void DrawGVLayout(int pos) {
	// 绘制游戏结束菜单：背景+标题+选项+高亮显示当前选项
	TRACE_ENTER();

	if (pos < 0 || pos > 1) {
		SDL_Log("Invalid position index: %d\n", pos);
		return;
	}

	// 菜单背景
	DrawRect(&layout.GVPageRect, PauseMenu_Color);

	// 标题和选项
	TetrisDrawText("Game Over!", fonts.UIFont, &layout.GameOverRect, 1);
	TetrisDrawText("Load", fonts.UIFont, &layout.GVLoadRect, 1);
	TetrisDrawText("Leave", fonts.UIFont, &layout.GVLeaveRect, 1);

	// 高亮当前选中的菜单项
	DrawRect(GVRect_p[pos], Highlight_Color);
}