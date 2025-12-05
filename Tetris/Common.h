#ifndef COMMON_H

#define COMMON_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define STD_W 720
#define STD_H 960

#define STD_BLOCK_SIZE 45
#define STD_PREVIEW_BLOCK_SIZE 25

#define STD_SMALL_TEXT_WIDTH 15
#define STD_SMALL_TEXT_HEIGHT 40

#define STD_BIG_TEXT_WIDTH 50
#define STD_BIG_TEXT_HEIGHT 125

#define ST_X 6
#define ST_Y 0


// 状态码：
// 1 进入游戏
// 2 进入欢迎界面
// 3 退出游戏
// 4 重开
// 5 忽略此状态码
#define StartGame 1
#define EnterWelcomePage 2
#define LeaveGame 3
#define LoadSave 4
#define IgnoreMe 5

#define NX 14
#define NY 23

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
#define PauseMenu_Color  (SDL_Color){50, 50, 50, 100}
#define Highlight_Color  (SDL_Color){255, 255, 255, 100}

#define DOWN  1
#define LEFT  2
#define RIGHT 3


// 简单的日志宏
#define TRACE_ENTER()  {									\
	if (log_file) {											\
		fprintf(log_file, "ENTER: %s()\n", __FUNCTION__);	\
		fflush(log_file);									\
	}														\
}

#define TRACE_MSG(msg) {									\
	if (log_file) {											\
		fprintf(log_file, "%s: %s\n", __FUNCTION__, msg);	\
		fflush(log_file);									\
	}														\
}


#ifndef _MSC_VER 
	// 通过宏定义提升兼容， 在非 MSVC 平台编译时将 _s 函数宏定义为一般函数
	#ifndef _ERRNO_T_DEFINED
		typedef int errno_t;
		#define _ERRNO_T_DEFINED
	#endif

	#define sscanf_s sscanf
	#define fscanf_s fscanf
	#define sprintf_s snprintf

	// 一个简单的内联 fopen_s_ 函数
	static inline errno_t fopen_s_(FILE** file, const char* name, const char* mode) {
		*file = fopen(name, mode);
		if (*file == NULL) return 1; // Fail
		return 0; // Success
	}

	#ifdef fopen_s
		#undef fopen_s 
	#endif

	#define fopen_s fopen_s_

#endif




typedef struct {
	int Value;
	SDL_Color Color;
}MAP;

// 布局（几何区域）
typedef struct {

	SDL_FRect ScoreLabelRect;		// "SCORE" 文字位置
	SDL_FRect NextPreviewRect;		// "NEXT" 
	SDL_FRect LineCountLabelRect;	// "LINES"
	SDL_FRect DifficultyRect;		// "DITFFICULTY"

	SDL_FRect ScoreValueRect;		// 分数值位置
	SDL_FRect NextPreviewShapeRect;	// 下一块预览区域
	SDL_FRect LineCountValueRect;	// 行数值
	SDL_FRect DifficultyValueRect;	// 难度数值

	SDL_FRect PauseMenuRect;
	SDL_FRect PauseMessageRect;
	SDL_FRect ContinueRect;
	SDL_FRect SaveRect;
	SDL_FRect LoadRect;
	SDL_FRect LeaveRect;

	SDL_FRect WelPageRect;
	SDL_FRect WelcomeRect;
	SDL_FRect StartRect;
	SDL_FRect WelLoadRect;
	SDL_FRect WelLeaveRect;

	// GameOver
	SDL_FRect GVPageRect;
	SDL_FRect GameOverRect;
	SDL_FRect GVLoadRect;
	SDL_FRect GVLeaveRect;

	SDL_FRect GameAreaRect;
	SDL_FRect LeftBorderRect;
	SDL_FRect RightBorderRect;
	SDL_FRect ButtonBorderRect;
	SDL_FRect UpperboundBorderRect;

	float ratio;
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

	Uint64 Current_tick;
	Uint64 Last_tick;

	Uint64 Speed_interval;
	Uint64 Current_interval;
	Uint64 Base_interval;

} GameState;

extern Shape shapes[19];
extern SDL_Window* win;
extern SDL_Renderer* rdr;
extern GameFonts fonts;
extern Layout layout;
extern GameState State;

extern FILE* log_file;

#endif