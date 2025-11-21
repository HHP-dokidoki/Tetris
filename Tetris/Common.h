#ifndef COMMON_H

#define COMMON_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#define W 720
#define H 1000

#define ST_X 6
#define ST_Y 0

#define BLOCKHEIGHT 45
#define BLOCKWIDTH  45

#define PREVIEW_BLOCKHEIGHT 25
#define PREVIEW_BLOCKWIDTH 25

#define NextPreviewPosx 565
#define NextPreviewPosy 60

#define ScoreLabelPosx 565
#define ScoreLabelPosy 260

#define LineCountLabelPosx 565
#define LineCountLabelPosy 460

#define DifficultyPosx 565
#define DifficultyPosy 660

#define PauseMenuPosx 240
#define PauseMenuPosy 250
#define PauseMenuWidth 240
#define PauseMenuHeight 500


#define WelPagePosx 150
#define WelPagePosy 200
#define WelPageWidth 420
#define WelPageHeight 700

//Tetris
#define WelcomePosx 215
#define WelcomePosy 200	
#define WelcomeWidth 360	// 字宽 60， 六个字
#define WelcomeHeight 150	// 字高 150

//Start
#define StartPosx 285
#define StartPosy 400		
#define StartWidth 150		// 字宽 30， 五个字
#define StartHeight 100		// 字高 100

//Load
#define WelLoadPosx 300
#define WelLoadPosy 550
#define WelLoadWidth 120	// 字宽30，四个字
#define WelLoadHeight 100

//Leave
#define WelLeavePosx 285
#define WelLeavePosy 700
#define WelLeaveWidth 150	// 字宽30，五个字
#define WelLeaveHeight 100

#define GVPageRectPosx 125
#define GVPageRectPosy 300
#define GVPageRectWidth 470
#define GVPageRectHeight 300

#define GameOverRectPosx 135
#define GameOverRectPosy 300
#define GameOverRectWidth 450
#define GameOverRectHeight 150

#define GVLoadRectPosx 125
#define GVLoadRectPosy 500
#define GVLoadRectWidth 120
#define GVLoadRectHeight 100

#define GVLeaveRectPosx 445
#define GVLeaveRectPosy 500
#define GVLeaveRectWidth 150
#define GVLeaveRectHeight 100

// 状态码：
// 1 进入游戏
// 2 进入欢迎界面
// 3 退出游戏
// 4 重开
// 5 忽略此状态码
#define StartGame 1
#define EnterWelcomePage 2
#define LeaveGame 3
#define Restart 4
#define IgnoreMe 5


#define TextHeight 50

#define NX 14
#define NY 24

#define DX 10
#define DY 0

#define RED		{255,0  ,0  ,255}
#define ORANGE	{255,165,0  ,255}
#define YELLOW	{255,255,0  ,255}
#define GREEN	{127,255,0  ,255}
#define BLUE	{0  ,0  ,255,255}
#define PURPLE	{128,0  ,128,255}
#define PINK	{255,192,203,255}
#define BACKGROUND_COLOR (SDL_Color){127, 127, 127, 127}
#define BORDER_COLOR	 (SDL_Color){255, 255, 255, 255}
#define UPPERBOUND_COLOR (SDL_Color){127, 0, 0, 127}
#define PauseMenu_Color  (SDL_Color){90, 90, 90, 90}
#define Highlight_Color  (SDL_Color){255, 255, 255, 100}

#define DOWN  1
#define LEFT  2
#define RIGHT 3


// 简单的日志宏
#define TRACE_ENTER() do { \
    if (log_file) { \
        fprintf(log_file, "ENTER: %s()\n", __FUNCTION__); \
        fflush(log_file); \
    } \
} while(0)

#define TRACE_MSG(msg) do { \
    if (log_file) { \
        fprintf(log_file, "%s: %s\n", __FUNCTION__, msg); \
        fflush(log_file); \
    } \
} while(0)

typedef struct {
	int Value;
	SDL_Color Color;
}MAP;

// 布局（几何区域）
typedef struct {

	SDL_Rect ScoreLabelRect;		// "SCORE" 文字位置
	SDL_Rect NextPreviewRect;		// "NEXT" 
	SDL_Rect LineCountLabelRect;	// "LINES"
	SDL_Rect DifficultyRect;		// "DITFFICULTY"

	SDL_Rect ScoreValueRect;		// 分数值位置
	SDL_Rect NextPreviewShapeRect;	// 下一块预览区域
	SDL_Rect LineCountValueRect;	// 行数值
	SDL_Rect DifficultyValueRect;	// 难度数值

	SDL_Rect PauseMenuRect;
	SDL_Rect PauseMessageRect;
	SDL_Rect ContinueRect;
	SDL_Rect SaveRect;
	SDL_Rect LoadRect;
	SDL_Rect LeaveRect;

	SDL_Rect WelPageRect;
	SDL_Rect WelcomeRect;
	SDL_Rect StartRect;
	SDL_Rect WelLoadRect;
	SDL_Rect WelLeaveRect;

	// GameOver
	SDL_Rect GVPageRect;
	SDL_Rect GameOverRect;
	SDL_Rect GVLoadRect;
	SDL_Rect GVLeaveRect;


} Layout;

// 字体资源（只读，初始化加载）
typedef struct {
	SDL_Color Color;		// 字体颜色：统一为白色
	TTF_Font* UIFont;		// 用于 "SCORE", "NEXT", "Difficulty","LINES"
	TTF_Font* NumberFont;   // 用于数字
} GameFonts;


//形状
typedef struct {
	/*{x1,y1,x2,y2,x3,y3,x4,y4, color, next}*/
	int D[8];
	SDL_Color Color;
	int Next;
}Shape;

// 游戏状态
typedef struct {

	MAP map[NY][NX];

	int Score;
	int BaseScore;
	int LineCount;
	int Difficulty;

	int Current_shape;
	int Next_shape;
	int Shape_pos_x;
	int Shape_pos_y;

	Uint32 Current_tick;
	Uint32 Last_tick;

	Uint32 Speed_interval;
	Uint32 Current_interval;
	Uint32 Base_interval;

} GameState;

extern Shape shapes[19];
extern SDL_Window* win;
extern SDL_Renderer* rdr;
extern GameFonts fonts;
extern Layout layout;
extern GameState State;

extern FILE* log_file;
#endif