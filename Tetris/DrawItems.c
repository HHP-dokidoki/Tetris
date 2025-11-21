#include "Common.h"


void TetrisDrawText(char content[], TTF_Font* font, SDL_Rect* rect, int Magnif)
{
	TRACE_ENTER();
	//SDL_Log("DrawText: '%s', font=%p, rect=(%d,%d,%d,%d), mag=%d",
	//	content, (void*)font, rect->x, rect->y, rect->w, rect->h, Magnif);

	int w, h;
	SDL_Texture* texture;
	SDL_Surface* text_surf;
	// ** 一个检查了半个小时的bug	    ：(
	// 原本是想用这个语句获取字符串的长度以动态跳转字体rect的宽度
	//w = rect->w = (int)(sizeof(content) - 1) * 20;
	// 

	// 字体基本尺寸 * 放大倍率 Magnif
	 w = rect->w = (int)strlen(content) * 15 * Magnif;
	 h = rect->h = TextHeight * Magnif;

	 //SDL_Log("Before TTF_RenderText_Blended:");
	 //SDL_Log("  Content: '%s'", content);
	 //SDL_Log("  Font: %p", (void*)font);
	 //SDL_Log("  Color: r=%d, g=%d, b=%d, a=%d",
	 //	   fonts.Color.r, fonts.Color.g, fonts.Color.b, fonts.Color.a);

	text_surf = TTF_RenderText_Blended(font, content, fonts.Color);
	if (text_surf == NULL) {
		SDL_Log("TTF_RenderText_Solid Failed: %s\n", TTF_GetError());
		return;
	}

	// 这里有个问题：按理来说texture不应该随用随创建，因为 texture 是 GPU 显存中的优化纹理
	// 频繁创建销毁会降低程序的性能，但考虑到俄罗斯方块规模较小，所以暂时忽略 XD
	texture = SDL_CreateTextureFromSurface(rdr, text_surf);
	if (texture == NULL)
	{
		SDL_Log("SDL_CreateTextureFromSurface Failed:%s\n", SDL_GetError());
		SDL_FreeSurface(text_surf);
		return;
	}
	if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) < 0)
	{
		SDL_Log("SDL_QueryTexture Failed:%s\n", SDL_GetError());
		return;
	}
	if (SDL_RenderCopy(rdr, texture, NULL, rect) < 0)
	{
		SDL_Log("SDL_RenderCopy Failed:%s\n", SDL_GetError());
		return;
	}

	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
	}
	if (text_surf != NULL)
	{
		SDL_FreeSurface(text_surf);
	}
}

void DrawRect(SDL_Rect rect,SDL_Color color)
{
	TRACE_ENTER();

	SDL_SetRenderDrawColor(rdr, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(rdr, &rect);
}

void DrawBlock(int x, int y, SDL_Color color, int w, int h)
{
	TRACE_ENTER();

	SDL_Rect block;
	block.w = w;
	block.h = h;
	block.x = x;
	block.y = y;
	SDL_SetRenderDrawColor(rdr, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(rdr, &block);
	SDL_SetRenderDrawColor(rdr, color.r + 10, color.g + 10, color.b + 10, 127);
	SDL_RenderDrawRect(rdr, &block);
}

void DrawSingleShape(int shape_id, int shape_pos_x, int shape_pos_y,int size)
{// 由于组成形状的基础元素为正方形，所以只需要传一个 size 参数即可
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
		// 为方便 map 的处理 shape_pos_x 和 y 都是从 1 开始
		// 所以转换为绝对坐标时要减一
		DrawBlock((shape_pos_x + dx - 1) * size + DX,
				  (shape_pos_y + dy - 1) * size + DY,
				  shapes[shape_id].Color,
			      size, size);
	}
}

void DrawPreviewShape(int shape_id, int shape_pos_x, int shape_pos_y, int size)
{
	TRACE_ENTER();

	if (shape_id < 0 || shape_id >= 19)
	{
		SDL_Log("Invalid shape id :%d\n", shape_id);
		return;
	}
	int dx, dy;
	for (int i = 0; i < 8; i += 2)
	{
		dx = shapes[shape_id].D[i] * size;
		dy = shapes[shape_id].D[i + 1] * size;

		DrawBlock(shape_pos_x + dx,
			shape_pos_y + dy,
			shapes[shape_id].Color,
			size, size);
	}
}

void ToString(int value, char* buffer)
{
	TRACE_ENTER();

	// 一般情况下各类数值不会达到 10^9 
	sprintf_s(buffer,20, "%d", value);
}


void DrawShapes(int shape_pos_x, int shape_pos_y, int shape_id)
{
	TRACE_ENTER();

	// 绘制已经固定在map中的方块
	for (int i = 1; i <= NY - 2; i++)
	{
		for (int j = 1; j <= NX - 2; j++)
		{
			if (State.map[i][j].Value)
			{
				DrawBlock((j - 1) * 45 + DX,
					(i - 1) * 45 + DY,
					State.map[i][j].Color,
					BLOCKWIDTH,
					BLOCKHEIGHT);
			}
		}
	}

	// 绘制未固定在map的方块
	DrawSingleShape(shape_id, shape_pos_x, shape_pos_y, BLOCKWIDTH);
}

void DrawDynamicItems(int nxt_shape_id)
{
	TRACE_ENTER();

	char str[20];
	ToString(State.Score, str);
	TetrisDrawText(str, fonts.NumberFont, &layout.ScoreValueRect,1);

	ToString(State.Difficulty, str);
	TetrisDrawText(str, fonts.NumberFont, &layout.DifficultyValueRect,1);

	ToString(State.LineCount, str);
	TetrisDrawText(str, fonts.NumberFont, &layout.LineCountValueRect,1);

	// 一个排查了半小时的 bug 
	// DrawSingleShape 接受的是map中的网格坐标
	// 而我传入的是整个游戏界面的绝对坐标
	// DrawSingleShape(nxt_shape_id,
	//	layout.NextPreviewShapeRect.x,
	//	layout.NextPreviewShapeRect.y,
	//	PREVIEW_BLOCKHEIGHT);

	// 所以为preview区域专门写了个函数......
	DrawPreviewShape(nxt_shape_id,
		layout.NextPreviewShapeRect.x,
		layout.NextPreviewShapeRect.y,
		PREVIEW_BLOCKHEIGHT);
}

void DrawGameLayout(void)
{
	TRACE_ENTER();

	// 游戏区范围：
	// 540----45 * 12
	// 990----45 * 20

	SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);

	// 边框及背景
	DrawRect((SDL_Rect){ 0, 0, 550, 990 }, BACKGROUND_COLOR);
	DrawRect((SDL_Rect){ 0, 0, 10, 990 }, BORDER_COLOR);
	DrawRect((SDL_Rect){ 550, 0, 10, 990 }, BORDER_COLOR);
	DrawRect((SDL_Rect){ 0, 990, 560, 10 }, BORDER_COLOR);
	DrawRect((SDL_Rect){ 10, 90, 540, 10 }, UPPERBOUND_COLOR);


	// 侧边栏文字
	TetrisDrawText("NEXT SHAPE", fonts.UIFont, &layout.NextPreviewRect,1);
	TetrisDrawText("DIFFICULTY", fonts.UIFont, &layout.DifficultyRect,1);
	TetrisDrawText("SCORE", fonts.UIFont, &layout.ScoreLabelRect,1);
	TetrisDrawText("LINE COUNT", fonts.UIFont, &layout.LineCountLabelRect,1);

}

static SDL_Rect* PauseRect_p[5] =
{ 
	&layout.ContinueRect, &layout.SaveRect, &layout.LoadRect, 
	&layout.LeaveRect, &layout.PauseMessageRect 
};
void DrawPauseLayout(int pos)
{
	TRACE_ENTER();

	// 添加边界检查
	if (pos < 0 || pos >= 5) {
		SDL_Log("Invalid position index: %d\n", pos);
		return;
	}

	DrawRect(layout.PauseMenuRect, PauseMenu_Color);

	TetrisDrawText("Paused", fonts.UIFont, &layout.PauseMessageRect, 2);
	TetrisDrawText("Continue", fonts.UIFont, &layout.ContinueRect, 2);
	TetrisDrawText("Save", fonts.UIFont, &layout.SaveRect, 2);
	TetrisDrawText("Load", fonts.UIFont, &layout.LoadRect, 2);
	TetrisDrawText("Leave", fonts.UIFont, &layout.LeaveRect, 2);

	DrawRect(*PauseRect_p[pos], Highlight_Color);

	SDL_RenderPresent(rdr);
}

static SDL_Rect* WelRect_p[3] =
{
	&layout.StartRect, &layout.WelLoadRect, &layout.WelLeaveRect
};
void DrawWelcomeLayout(int pos)
{
	TRACE_ENTER();

	// 边界检查
	if (pos < 0 || pos > 2) {
		SDL_Log("Invalid position index: %d\n", pos);
		return;
	}

	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
	SDL_RenderClear(rdr);

	DrawRect(layout.WelPageRect, PauseMenu_Color);

	TetrisDrawText("TETRIS", fonts.UIFont, &layout.WelcomeRect, 3);
	TetrisDrawText("Start", fonts.UIFont, &layout.StartRect, 2);
	TetrisDrawText("Load", fonts.UIFont, &layout.WelLoadRect, 2);
	TetrisDrawText("Leave", fonts.UIFont, &layout.WelLeaveRect, 2);

	DrawRect(*WelRect_p[pos], Highlight_Color);

	SDL_RenderPresent(rdr);
}

void DrawHoleWindow(int shape_pos_x, int shape_pos_y, int current_shape, int next_shape)
{
	TRACE_ENTER();


	SDL_SetRenderDrawColor(rdr, 0, 0, 0, 255);
	SDL_RenderClear(rdr);

	DrawGameLayout();
	DrawShapes(shape_pos_x, shape_pos_y, current_shape);

	DrawDynamicItems(next_shape);

	SDL_RenderPresent(rdr);
}

SDL_Rect* GVRect_p[2] =
{
	&layout.GVLoadRect, &layout.GVLeaveRect
};
void DrawGVLayout(int pos) {
	// DrawGameOverLayout
	TRACE_ENTER();

	// 边界检查
	if (pos < 0 || pos > 1) {
		SDL_Log("Invalid position index: %d\n", pos);
		return;
	}

	DrawRect(layout.GVPageRect, PauseMenu_Color);

	TetrisDrawText("Game Over!", fonts.UIFont, &layout.GameOverRect, 2);
	TetrisDrawText("Load", fonts.UIFont, &layout.GVLoadRect, 2);
	TetrisDrawText("Leave", fonts.UIFont, &layout.GVLeaveRect, 2);

	DrawRect(*GVRect_p[pos], Highlight_Color);

	SDL_RenderPresent(rdr);
}