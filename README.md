
---

# 俄罗斯方块游戏

Created By Chenchang</br>
Migrated By HHP-DokiDoki</br>
Welcome to visit [my blog](https://hhp-dokidoki.cn/)(though there are no articles yet).</br>

## 项目简介
这是一个使用 C 语言和 SDL2 库开发的经典俄罗斯方块游戏。项目采用模块化设计，实现了游戏的核心玩法、图形渲染、事件处理和存档功能。

## 项目特点
- **模块化设计**：代码按功能划分为独立模块，提高可维护性和可读性。
- **图形化界面**：使用 SDL2 库实现跨平台的图形渲染和用户交互。
- **动态难度**：游戏难度随消除行数递增，提升挑战性。
- **存档功能**：支持游戏状态的保存和加载，方便玩家随时继续。

## 模块功能与实现逻辑

### 1. `Common.h`
- **功能**：定义全局常量、宏、结构体和外部变量声明。
- **关键内容**：
  - 游戏窗口尺寸、方块大小、布局坐标等常量。
  - `MAP`（地图单元格）、`Layout`（UI布局）、`GameState`（游戏状态）等结构体。
  - 颜色宏和状态码定义。

### 2. `Init.c`
- **功能**：初始化 SDL 环境、窗口、渲染器、字体和游戏布局。
- **关键函数**：
  - `WindowRdrInit`：初始化 SDL，创建窗口和渲染器。
  - `FontInit`：加载字体文件，初始化 `TTF_Font`。
  - `RectInit`：初始化 UI 元素的 `SDL_Rect` 坐标。
  - `LogInit`：打开日志文件。

### 3. `MapControl.c`
- **功能**：管理游戏地图（二维网格）的状态，包括方块固定、行消除等。
- **关键函数**：
  - `MapInit`：清空地图并设置边界。
  - `LineFull`：检查指定行是否已满。
  - `DeleteRow`：删除指定行，并将上方行下移。
  - `DeleteRows`：批量处理满行，计算得分和等级提升。

### 4. `GameState.c`
- **功能**：管理游戏核心状态（分数、等级、当前方块等）和存档逻辑。
- **关键函数**：
  - `GameStateInit`：初始化游戏状态。
  - `CalScore`：根据一次消除的行数计算得分。
  - `LevelUp`：等级提升，增加基础分值并缩短下落间隔。
  - `SaveGame`：将当前游戏状态写入存档文件。
  - `LoadGame`：从存档文件读取游戏状态。

### 5. `ShapeControl.c`
- **功能**：管理游戏中的方块形状，包括移动、旋转、碰撞检测等。
- **关键函数**：
  - `GetNextShape`：随机生成下一个方块形状。
  - `NotConflict`：检查方块在指定位置是否与地图或边界冲突。
  - `Moveable`：判断方块是否可以向指定方向移动。
  - `MoveShape`：执行方块的移动逻辑（左、右、下）。
  - `RollShape`：执行方块的旋转逻辑。
  - `FixShape`：将方块固定到地图上。

### 6. `DrawItems.c`
- **功能**：负责所有图形元素的绘制，包括方块、UI 文字、菜单等。
- **关键函数**：
  - `DrawBlock`：绘制单个方块。
  - `DrawSingleShape`：绘制活动方块。
  - `DrawPreviewShape`：绘制预览区域的方块。
  - `TetrisDrawText`：绘制文本。
  - `DrawGameLayout`：绘制游戏主界面布局。
  - `DrawShapes`：绘制地图上固定的方块和活动方块。
  - `DrawDynamicItems`：绘制分数、等级、下一个方块等动态信息。
  - `DrawPauseLayout`、`DrawWelcomeLayout`、`DrawGVLayout`：分别绘制暂停、欢迎和游戏结束界面。
  - `DrawHoleWindow`：核心绘制函数，整合并呈现整个游戏窗口。

### 7. `Event.c`
- **功能**：处理游戏主循环和各种事件（键盘输入、游戏状态转换）。
- **关键函数**：
  - `EnterMainEventLoop`：主事件循环，协调欢迎和游戏循环。
  - `EnterWelcomeEventLoop`：欢迎界面的事件处理。
  - `EnterGameEventLoop`：游戏主循环，处理方块下落和用户输入。
  - `EnterMotionEvent`：处理键盘按键事件（移动、旋转、暂停）。
  - `EnterTickEvent`：处理方块自动下落逻辑。
  - `EnterPauseEvent`：处理暂停菜单的事件。
  - `EnterGameOverEvent`：处理游戏结束菜单的事件。

### 8. `Tetris.c`
- **功能**：程序入口点，调用各模块的初始化函数并启动主事件循环。
- **关键函数**：
  - `main`：执行初始化，进入主循环，最后执行清理。

## 代码不足
- **代码规范**：部分注释和变量命名可以更清晰，提高代码可读性。

## 未来可拓展功能
- **音效与背景音乐**：集成 SDL_mixer 库，增加游戏音效。
- **更多方块类型**：引入自定义或更复杂的方块形状。
- **更丰富的视觉效果**：例如方块下落时的阴影、消除时的动画效果。
- **配置文件**：允许用户自定义键位、难度初始值等。
- **多存档位**： 增加存档槽，并提供预览存档功能。
- **自定义开始页面背景图**： 允许使用游戏截图作为背景图。

---
