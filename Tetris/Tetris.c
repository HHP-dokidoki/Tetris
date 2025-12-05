#include "Common.h"
#include "Init.h"
#include "MapControl.h"
#include "GameState.h"
#include "Event.h"

#undef main


int main()
{
	// 日志初始化
	if (LogInit())
	{
		SDL_Log("Open log file failed\n");
		return -1;
	}

	// 初始化SDL窗口和渲染器
	if (WindowRdrInit())
	{
		return -1;
	}
	
	// 初始化字体系统
	if (FontInit())
	{
		return -1;
	}
	
	// 计算UI布局（根据窗口大小缩放）
	RectInit();
	
	// 初始化游戏状态和地图
	GameStateInit();
	
	// 初始化随机数生成器
	RandInit();
	
	// 进入主事件循环（游戏主程序）
	EnterMainEventLoop();

	// 清理所有资源
	DeInit();
	

	return 0;
}