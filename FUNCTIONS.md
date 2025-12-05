# 重要函数与游戏控制逻辑说明


重点介绍**游戏主循环、输入事件处理、方块运动与旋转、地图消行与难度提升、游戏界面与状态切换**等函数。

---

## 一、整体游戏结构与入口

### `int main()`

**所在文件**：`Tetris.c`  
**职责**：程序入口，串起整个游戏初始化与主事件循环。

**主要流程**：

1. `LogInit()`：初始化日志系统，打开 `..\\log\\game_Trace.log`。
2. `WindowRdrInit()`：初始化 SDL，创建窗口和渲染器。
3. `FontInit()`：初始化字体系统与游戏所用字体。
4. `RectInit()`：根据窗口尺寸计算 UI 布局（游戏区域、侧边栏、菜单窗口等）。
5. `GameStateInit()`：初始化游戏状态（地图、分数、难度、当前方块等）。
6. `RandInit()`：初始化随机数种子（用于随机出下一块）。
7. `EnterMainEventLoop()`：进入主事件循环，驱动欢迎界面与游戏界面的切换。
8. `DeInit()`：统一释放资源并关闭 SDL。

**实现要点**：  
`main` 本身不直接处理游戏逻辑，而是负责“生命周期管理”：初始化 → 事件循环 → 清理退出。核心控制逻辑主要在 `Event.c` 与各模块函数中实现。

---

## 二、核心全局数据与基本类型

### `Shape` 与全局数组 `shapes[19]`

**所在文件**：`Common.c / Common.h`  
`Shape` 结构定义：

```c
typedef struct {
	int D[8];        // 4 格相对坐标 (x1,y1,x2,y2,x3,y3,x4,y4)
	SDL_Color Color; // 方块颜色
	int Next;        // 下一个旋转形态的索引
} Shape;
```

**`shapes[19]` 的作用**：

- 保存所有可出现的形状及其**旋转形态**：
  - 7 种俄罗斯方块形（I、J、L、O、S、Z、T 等），总共 19 个形态；
  - 每个形态通过 4 个格子的相对坐标定义。
- `Next` 字段形成一个“旋转链表”，例如：
  - 旋转一次就是将 `shape_id` 更新为 `shapes[shape_id].Next`。
- `Color` 指定绘制时使用的颜色。

**设计要点**：

- 将几何信息（小块相对中心的位置）与旋转关系编码在数据中，旋转逻辑只需修改 `shape_id`。
- 具体的移动与碰撞检测由 `ShapeControl.c` 中的函数使用这些数据实现。

---

### 全局状态对象 `GameState State`

**所在文件**：`Common.h / GameState.c`  

`GameState` 结构包含：

- `MAP map[NY][NX]`：地图网格，`Value` 表示是否被占用，`Color` 表示方块颜色。
- 分数与难度：
  - `int Score;`
  - `int BaseScore;`
  - `int LineCount;`
  - `int Difficulty;`
- 当前下落方块相关：
  - `int Current_shape;`
  - `int Next_shape;`
  - `int Shape_pos_x;`
  - `int Shape_pos_y;`
- 下落速度与计时：
  - `Uint64 Speed_interval;`   // 快速下落模式的间隔
  - `Uint64 Current_interval;` // 当前生效的下落间隔
  - `Uint64 Base_interval;`    // 基础下落间隔（随难度变化）
  - `Uint64 Current_tick;`
  - `Uint64 Last_tick;`

`State` 是驱动整个游戏逻辑的**中心数据结构**，所有模块（事件、地图、方块控制、绘制）都围绕它进行读写。

---

## 三、初始化与资源管理相关函数（Init 模块）

### `int LogInit(void)` / `void LogDeintit(void)`

**作用**：打开 / 关闭日志文件；`TRACE_ENTER()` 与 `TRACE_MSG()` 宏依赖 `log_file` 用于调试追踪。

- `LogInit`：使用 `fopen_s` 打开 `..\\log\\game_Trace.log`，写入启动标记。
- `LogDeintit`：写入结束标记并关闭文件。

---

### `int WindowRdrInit(void)`

**作用**：初始化 SDL、窗口与渲染器。

**关键步骤**：

1. `SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)`：初始化视频与音频子系统。
2. 获取屏幕可用区域 `SDL_GetDisplayUsableBounds`，根据标准分辨率 `STD_W` × `STD_H` 计算合适 `ratio`。
3. 按比例创建窗口 `SDL_CreateWindow("Tetris", ...)`，允许拉伸（`SDL_WINDOW_RESIZABLE`），并设置固定纵横比 3:4。
4. 创建渲染器 `SDL_CreateRenderer(win, NULL)`，设置混合模式和初始清屏颜色。
5. `memset(State.map, 0, sizeof(State.map))`：确保地图初始为空。

**设计要点**：  
将“与逻辑无关的图形环境配置”全部封装在一个函数中，逻辑层只需要操作 `rdr` 渲染即可。

---

### `int FontInit(void)` / `void FontDeInit(void)`

**作用**：

- `FontInit`：初始化 SDL_ttf，加载 UI 字体与数字字体，设置统一字体颜色为白色。
- `FontDeInit`：关闭字体资源与 TTF 子系统。

**逻辑要点**：

- 加载字体文件 `..\\Font\\SourceCodePro-ExtraBoldItalic.ttf` 与 `..\\Font\\VT323-Regular.ttf`；
- 字体资源存于全局 `fonts` 中，供 `DrawItems` 模块统一使用。

---

### `void RectInit(void)`

**作用**：根据当前窗口大小**动态计算所有 UI 布局矩形**，包括：

- 游戏区域 (`layout.GameAreaRect`)
- 左右边框、上边界、底部边框
- 侧边栏中的 NEXT / SCORE / LINES / DIFFICULTY 区域
- 暂停菜单（`PauseMenuRect` 与各按钮）
- 欢迎界面与 GameOver 界面中的面板矩形和按钮矩形

**实现原理**：

1. 获取窗口尺寸 `SDL_GetWindowSize`。
2. 以标准宽高 `STD_W, STD_H` 为参考计算 `ratio`：
   - `ratio = min(window.w / STD_W, window.h / STD_H)`；
3. 整体 UI 尺寸 = 标准尺寸 × `ratio`；
4. 所有 `SDL_FRect` 的 `x/y/w/h` 按比例计算，保证以下几点：
   - 游戏区域在左侧固定宽度的 12 列方块。
   - 右侧留出信息栏和菜单区域。
   - 欢迎 / 暂停 / GameOver 等弹出面板居中或按指定位置出现。

**设计要点**：  
通过统一的 `RectInit`，实现**窗口缩放自适应**，不改变逻辑坐标与网格（`NX/NY`）的情况下，让显示布局随窗口大小变化。

---

### `void RandInit(void)`

- 使用 `SDL_GetTicks()` 作为随机数种子 `srand`，保证每次游戏随机方块序列不同。

---

### `void DeInit(void)`

- 顺序调用：`FontDeInit()` → `LogDeintit()` → 销毁渲染器/窗口 → `SDL_Quit()`；
- 集中处理所有资源释放，避免内存泄漏。

---

## 四、地图控制与消行逻辑（MapControl 模块）

### `void MapInit(void)`

**作用**：初始化地图 `State.map`，设置边界为“始终占用”的墙。

**实现细节**：

- `memset(State.map, 0, sizeof(State.map));` 清空地图。
- 左右边界：`State.map[i][0]` 与 `State.map[i][NX-1]` 的 `Value` 设为 1。
- 底部边界：`State.map[NY-1][i]` 的 `Value` 设为 1。

**逻辑意义**：  
将地图**可用区域**限制在 `[1, NX-2] × [0, NY-2]`，碰撞检测时只需要保证方块不覆盖 `Value=1` 的格子即可。

---

### `int LineFull(int y)`

**作用**：判断第 `y` 行是否已经除边界外全部为方块。

**实现要点**：

- 只检查列 `1..NX-2`（排除左右边界）；
- 若存在 `Value == 0` 则返回 0，否则返回 1。

---

### `void DeleteRow(int y)`

**作用**：清除第 `y` 行，并将其上方的内容整体下移一行。

**实现过程**：

1. 循环 `i` 从 `y` 到 `2`：
   - 将第 `i-1` 行的数据移动到第 `i` 行（只操作 `1..NX-2`）。
2. 第 1 行清零（不含边界），包括 `Value` 和 `Color`。

**设计要点**：  
采用“整体下移”的策略，实现典型的俄罗斯方块消行效果。

---

### `int DeleteRows(int y)`

**作用**：以最近落定行 `y` 为中心，最多检查 4 行，执行多行消除，并负责统计分数和提升难度。

**实现逻辑**：

1. `row_cnt = 0`；
2. 循环 4 次（对应最多一次四连消）：
   - 若当前行 `LineFull(y)` 为满，则调用 `DeleteRow(y)` 并 `row_cnt++`，继续检查相同的 `y` 行（因为上方行被压下来了）；
   - 若当前行不满，则 `y--`，向上检查下一行；
   - 若 `y < 0` 则提前终止。
3. `State.LineCount += row_cnt;` 累计消行数。
4. 调用 `CalScore(row_cnt)` 计算本次得分加成并返回。
5. 若 `State.LineCount > State.Difficulty * 10`，调用 `LevelUp()` 提升难度（见后）。

**设计要点**：

- 将**物理消行**（`DeleteRow`）与**统计和难度控制**（`CalScore` / `State.LineCount` / `LevelUp`）统一封装；
- `DeleteRows` 是“当一块落定后”的关键逻辑之一，由 `EnterTickEvent()` 调用。

---

## 五、方块控制与碰撞检测（ShapeControl 模块）

### `int GetNextShape(void)`

**作用**：按预设概率从 `shapes` 数组中随机选取一个 `shape_id`。

**实现原理**：

```c
i = rand() % 28;
if (i < 12) return i;
if (i < 24) return (i - 12) / 2 + 12;
return 18;
```

- 通过将 28 个随机数区间映射到 19 个形态索引，使各形状出现概率近似 4/28；
- 对于只有两个旋转状态的形状，通过合并两个随机区间来维持概率接近。

**设计要点**：  
专门控制各形状出现频率，使得游戏体验比较平衡，不会某一形状过于稀有或频繁。

---

### `int NotConflict(int shape_pos_x, int shape_pos_y, int shape_id)`

**作用**：检测以 `(shape_pos_x, shape_pos_y)` 为参照位置的某一形态 `shape_id` 是否与边界或已有方块发生冲突。

**检测逻辑**（对 4 个格子逐一检查）：

1. 计算绝对坐标 `x = shape_pos_x + dx`, `y = shape_pos_y + dy`。
2. 若：
   - `x < 1` 或 `x >= NX-1`（撞左右墙），
   - 或 `y >= NY-1`（撞底部墙），则冲突，返回 0。
3. 若 `y < 1`（在顶部区域）则不做地图占用检测，允许方块还在“出生区”。
4. 否则检查 `State.map[y][x].Value` 是否已被占用，若是，则冲突。

若所有格子不触发冲突条件，则返回 1。

**设计要点**：

- 把“越界”和“撞到已有方块”统一视作冲突；
- 特殊处理顶部区域，保证方块可以在看不见的上方缓冲区生成。

---

### `int Moveable(int shape_pos_x, int shape_pos_y, int shape_id, int mode)`

**作用**：判断方块是否可以朝给定方向移动一格。

- `mode` 取值：
  - `DOWN` (1)
  - `LEFT` (2)
  - `RIGHT` (3)

**实现原理**：

- 实际上是对 `NotConflict` 的简单封装：
  - 向下：`NotConflict(x, y+1, shape_id)`
  - 向左：`NotConflict(x-1, y, shape_id)`
  - 向右：`NotConflict(x+1, y, shape_id)`

**设计意义**：  
供 `EnterTickEvent()`（定时下落）和其他逻辑快速判断能否移动，而不去直接操作地图。

---

### `void MoveShape(int* shape_pos_x, int* shape_pos_y, int shape_id, int mode)`

**作用**：在地图上“擦除当前方块显示 → 更新坐标 → 等待重新绘制”。

**实现步骤**：

1. 先根据当前 `( *shape_pos_x, *shape_pos_y )` 和 `shape_id` 遍历 4 个格子：
   - 将 `State.map[y][x].Value = 0`，`Color = BACKGROUND_COLOR`，相当于**把旧位置清空**。
2. 根据 `mode` 决定移动方向，并调用 `NotConflict` 检测：
   - 若可以移动，则对 `*shape_pos_x` 或 `*shape_pos_y` 加减 1。
3. 地图上并不在这里重新写入方块（留给 `FixShape` 或绘制时使用形状数据）。

**注意**：  
这里“擦除”与“绘制”并不完全一一对应：游戏的实时显示是通过 `DrawHoleWindow()` 每帧重新绘制当前方块和已固定的地图，而 `MoveShape` 仅负责逻辑位置的变化和地图中旧占用的清理。

---

### `void RollShape(int shape_pos_x, int shape_pos_y, int* shape_id)`

**作用**：尝试将当前方块旋转到下一形态。

**实现逻辑**：

1. 取下一个形态索引：`next_id = shapes[*shape_id].Next`。
2. 调用 `NotConflict(shape_pos_x, shape_pos_y, next_id)`：
   - 若不冲突，则更新 `*shape_id = next_id`；
   - 否则保持原状，旋转失败。

**设计要点**：  
旋转逻辑完全通过 `shapes` 中的数据和 `Next` 字段驱动，无需复杂几何变换。

---

### `void FixShape(int shape_pos_x, int shape_pos_y, int shape_id)`

**作用**：将当前活跃方块“写死”到地图上，变成静态的方块。

**实现过程**：

1. 遍历 4 个格子，计算每个格子最终落点 `(x, y)`。
2. 若 `(x,y)` 未越界（额外判断 `x>=0 && y>=0`），则：
   - `State.map[y][x].Value = 1;`
   - `State.map[y][x].Color = shapes[shape_id].Color;`

**设计要点**：  
此函数在方块不能再下落的时刻被调用（见 `EnterTickEvent()`），是从“动态方块”变为“静态方块”的关键步骤，之后会紧接着进行行检测与消行。

---

## 六、游戏状态与难度控制（GameState 模块）

### `void GameStateInit(void)`

**作用**：初始化整个游戏状态（但不包括 SDL/窗口等资源）。

**主要设置**：

- `MapInit()`：清空地图并设置边界。
- 分数相关：`Score = 0`, `BaseScore = 10`, `LineCount = 0`, `Difficulty = 1`。
- 下落速度：  
  `Speed_interval = 50`（快速下落增量），  
  `Current_interval = Base_interval = 800` 毫秒。
- 计时：`Last_tick = Current_tick = 0`。
- 当前方块与下一个方块均设为 `-1` 表示未生成。
- 初始位置：`Shape_pos_x = ST_X`, `Shape_pos_y = ST_Y`（常量在 `Common.h` 中定义）。

---

### `int CalScore(int row_cnt)`

**作用**：根据本次消除的行数计算分数加成。

**内部细节**：

- 使用静态表 `line_bonus[5] = {0,1,3,6,10}`：
  - 1 行：1 倍基准
  - 2 行：3 倍基准
  - 3 行：6 倍基准
  - 4 行：10 倍基准
- 返回值为 `line_bonus[row_cnt] * State.BaseScore`。

**设计要点**：  
通过线性表定义消行奖励权重，使得多连消的得分**非线性增加**。

---

### `void LevelUp(void)`

**作用**：提升游戏难度（加快下落速度、增加每行得分）。

**实现逻辑**：

1. `State.Difficulty++`。
2. `State.BaseScore = 10 + (State.Difficulty - 1) * 5;`
   - 难度每升一级，基础分数 +5。
3. 计算新的基础下落间隔：
   - `new_interval = 800 - (State.Difficulty - 1) * 100;`
4. 更新下落速度：
   - `State.Base_interval = new_interval > 50 ? new_interval : State.Base_interval;`
   - `State.Current_interval = State.Base_interval;`

**说明**：  
代码中更新 `Base_interval` 时先对`new_interval` 进行判断**确保下落间隔不会变得比最大值更快**。整体设计仍然体现了“难度升高 → 基础间隔逐步减小”的趋势。

---

### `void SaveGame(void)` / `void LoadGame(void)`

**作用**：完成**完整存档 / 读档**，包括：

- 地图网格中每个格子的 `Value` 与 `Color`（RGBA 四个通道）；
- 当前分数、难度、已消行数；
- 当前方块和下一个方块编号；
- 当前方块位置 `(Shape_pos_x, Shape_pos_y)`。

**存档实现**（`SaveGame`）：

1. `fopen_s("..\\saves\\savedata.save", "w")`；
2. 逐行将 `map[i][j].Value` 输到文件；
3. 分别写入 R/G/B/A 四张“图层”的值；
4. 空行分隔；
5. 写入状态信息，格式如：
   - `SCORE:<value>`
   - `DIFFICULTY:<value>`
   - `LINECOUNT:<value>`
   - `CURRENT:<value>` 等。

**读档实现**（`LoadGame`）：

1. 打开相同路径文件；
2. 依次读取 `Value` 网格和 RGBA 四个通道；
3. 跳过空行；
4. 按行读取字符串，用 `strncmp` 判断前缀并使用 `sscanf_s` 解析各字段；
5. 更新 `State` 中相应字段。

---

### `void ResetGameState(void)`

- 封装为一个简单入口：内部直接调用 `GameStateInit()`，用于“重开游戏”时快速恢复初始状态（被 `EnterGameOverEvent()` 用到）。

---

## 七、绘制与界面布局（DrawItems 模块）

此模块虽然偏“显示层”，但与整体控制及状态显示紧密相关。

### `void DrawHoleWindow(int shape_pos_x, int shape_pos_y, int current_shape, int next_shape)`

**核心作用**：每一帧重绘**整个游戏画面**：

1. 清屏（设为黑色）。
2. `DrawGameLayout()`：绘制背景、游戏区域边框、信息栏标签。
3. `DrawShapes(shape_pos_x, shape_pos_y, current_shape)`：
   - 绘制已经固定的方块（从 `State.map` 读取）；
   - 绘制当前活动方块（使用 `shapes` 数据）。
4. `DrawDynamicItems(next_shape)`：
   - 绘制分数、难度、已消行数数值；
   - 绘制预览方块（`next_shape`）。

这一函数是逻辑层与渲染层的**关键桥接点**——所有事件逻辑更新数据后，都调用该函数再 `SDL_RenderPresent(rdr)` 进行刷新。

---

### `void DrawPauseLayout(int pos)`

- 在当前游戏画面上叠加一个暂停菜单，`pos` 表示当前选中的菜单项索引（0~3）。
- 依次绘制：
  - 半透明背景框；
  - 文本按钮：`"Paused"`, `"Continue"`, `"Save"`, `"Load"`, `"Leave"`；
  - 高亮当前选项矩形（利用 `PauseRect_p[pos]`，指向对应 `layout` 中的按钮矩形）。

### `void DrawWelcomeLayout(int pos)`

- 绘制欢迎界面：背景清空、欢迎面板、`"TETRIS"` 标题、`"Start"`, `"Load"`, `"Leave"` 三个选项，高亮当前选择 `pos`。

### `void DrawGVLayout(int pos)`

- Game Over 界面：绘制 `"Game Over!"` 文字和 `"Load"` / `"Leave"` 两个选项，并根据 `pos` 高亮对应按钮。

这些函数配合 `Event.c` 中的状态机，共同完成**状态切换界面**的展示。

---

## 八、事件与主循环逻辑（Event 模块）

`Event.c` 是整个游戏逻辑的核心：将 **SDL 事件**、**定时下落**、**状态机切换** 统一管理。

### `int EnterMainEventLoop(void)`

**作用**：顶层事件循环，管理大状态切换：

- 欢迎界面 → 游戏中 → Paused / GameOver → 返回欢迎界面 → 退出。

**主逻辑**：

1. 先进入 `EnterWelcomeEventLoop()`，获得初始 `state_code`。
2. 在无限循环中根据 `state_code` 决定后续动作：
   - `StartGame` / `Restart`：进入 `EnterGameEventLoop()`。
   - `EnterWelcomePage`：回到 `EnterWelcomeEventLoop()`。
   - `LeaveGame`：跳出循环结束游戏。
3. 函数最终返回 0，仅表示主循环正常结束。

**设计思想**：  
构建一个简化的“有限状态机”，各内部循环负责子状态内部的逻辑和 UI，`state_code` 用于状态间跳转。

---

### `static int EnterWelcomeEventLoop(void)`

**作用**：处理**欢迎界面**中的键盘输入与菜单选择。

**行为**：

- 局部变量：`pos`（0~2 表示菜单项）、`enter_flag`。
- 循环中：
  - 始终绘制当前欢迎界面：`DrawWelcomeLayout(pos) + SDL_RenderPresent`。
  - 使用 `SDL_PollEvent` 读取事件：
    - `SDL_EVENT_WINDOW_RESIZED`：调用 `RectInit()` 重新布局，并重绘。
    - `SDL_EVENT_KEY_DOWN`：
      - `SDLK_DOWN` / `SDLK_UP`：通过 `(pos + 3) % 3` 形成循环选择。
      - `SDLK_RETURN`：设置 `enter_flag=1`，表示用户确认选择。
  - 当 `enter_flag` 为 1 时，根据 `pos` 返回：
    - `pos == 0`：开始新游戏 → 返回 `StartGame`。
    - `pos == 1`：先 `LoadGame()` 再返回 `StartGame`。
    - `pos == 2`：返回 `LeaveGame`。
- 没有事件时通过 `SDL_Delay(16)` 降低 CPU 占用。

**控制要点**：  
该循环不处理游戏下落逻辑，而是**只处理菜单导航与状态决定**，在 `EnterMainEventLoop()` 中被调用。

---

### `static int EnterGameEventLoop(void)`

**作用**：游戏进行过程中的主循环（包含下落计时和玩家操作）。

**内部机制**：

1. 初始化计时：
   - `State.Last_tick = State.Current_tick = SDL_GetTicks();`
   - `State.Current_tick += State.Current_interval;`（为第一帧提前设置触发点）。
2. 无限循环中执行两类工作：

#### 1. 初始方块生成

- 若 `State.Next_shape == -1`：
  - 连续调用两次 `GetNextShape()` 填充 `Current_shape` 与 `Next_shape`。
  - 绘制当前游戏界面并显示。

#### 2. 定时下落逻辑

- 更新当前时间 `State.Current_tick = SDL_GetTicks()`。
- 若 `Current_tick - Last_tick >= Current_interval`：
  - 调用 `EnterTickEvent()`，根据下落结果可能返回：
    - `EnterWelcomePage`：游戏结束后玩家在 GameOver 菜单中选择回到欢迎界面（会清空状态）。
    - `Restart`：游戏结束后玩家选择读档。
    - `IgnoreMe`：普通下落，无需状态跳转。
  - 若是 `EnterWelcomePage` 或 `Restart`，则直接返回这个状态码给上层。
  - 更新计时器 `Last_tick`。

#### 3. 用户键盘事件

- 使用 `SDL_PollEvent(&event)`：
  - 交给 `EnterMotionEvent(event)` 处理玩家实时输入（左右移动、旋转、快速下落、暂停）。
  - 若其返回 `EnterWelcomePage`（代码中约定为 2），则从游戏中直接切回欢迎界面（例如在暂停菜单中选择返回）。

整个循环不直接修改状态机大状态，而是通过返回值交由 `EnterMainEventLoop()` 去管理。

---

### `static int EnterTickEvent()`

**作用**：在定时器触发时执行一次“重力下落 + 落地/结束判定”。

**逻辑流程**：

1. **顶部无法下落 → 游戏结束判定**：
   - 首先调用 `Moveable(ST_X, ST_Y, State.Current_shape, DOWN)` 检查“方块在起始位置”是否可以下落。
   - 如果**起始位置也无法下落**，说明新的方块已经一生成就与堆叠块冲突 → 调用 `EnterGameOverEvent()` 处理游戏结束逻辑，返回菜单选择：
     - `Restart`：重新开始（带继续/读档逻辑）。
     - `EnterWelcomePage`：返回欢迎界面。
2. **普通下落**：
   - 若当前形态在当前位置仍可向下移动（`Moveable(State.Shape_pos_x, State.Shape_pos_y, ...)`）：
     - 调用 `MoveShape(&State.Shape_pos_x, &State.Shape_pos_y, State.Current_shape, DOWN)` 将方块下移一行。
3. **落地处理**（当前不能下移，但起始位置可下移的情况）：
   - 调用 `FixShape(State.Shape_pos_x, State.Shape_pos_y, State.Current_shape)` 将方块固定到地图；
   - 通过 `DeleteRows(State.Shape_pos_y)` 执行消行，返回获得的加分，累加到 `State.Score`；
   - 将当前方块位置重置为 `(ST_X, ST_Y)`；
   - 将 `Current_shape` 置为原来的 `Next_shape`，并通过 `GetNextShape()` 再随机出新的 `Next_shape`。

4. 最后统一调用：
   - `DrawHoleWindow(...)` + `SDL_RenderPresent(rdr)` 刷新画面；
   - 返回 `IgnoreMe`（普通情形）。

**设计要点**：  
将“定时下落”执行的逻辑完全收敛在同一个函数内，结合 `Moveable`、`FixShape`、`DeleteRows` 等函数实现清晰的状态转换。

---

### `static int EnterMotionEvent(SDL_Event event)`

**作用**：处理游戏进行中与玩家操作直接相关的事件。

**关键行为**：

- `SDL_EVENT_WINDOW_RESIZED`：
  - `RectInit()` 重新计算布局，然后全局重绘游戏画面。
- `SDL_EVENT_KEY_DOWN`：
  - `SDLK_DOWN`：将 `State.Current_interval` 设置为 `State.Speed_interval`，实现**加速下落**（软降）。
  - `SDLK_LEFT`：调用 `MoveShape(..., LEFT)`，再重绘。
  - `SDLK_RIGHT`：调用 `MoveShape(..., RIGHT)`，再重绘。
  - `SDLK_SPACE`：调用 `RollShape(...)`，再重绘，实现方块旋转。
  - `SDLK_ESCAPE`：调用 `EnterPauseEvent()` 进入暂停菜单：
    - 若其返回 `EnterWelcomePage`，则把 `2` 返回给游戏循环以触发状态切换；
    - 不论如何，从暂停返回后都将 `State.Current_interval` 恢复为 `Base_interval`，避免停留在“快速下落”状态。
- `SDL_EVENT_KEY_UP`：
  - `SDLK_DOWN`：松开下键时，恢复 `Current_interval = Base_interval`（结束加速）。

**设计要点**：

- 将“即时响应类”事件（移动、旋转、加速、暂停）与“定时下落”逻辑分开，提升结构清晰度。
- 在每次操作后都会调用 `DrawHoleWindow()` + `SDL_RenderPresent()`，保证画面与逻辑同步。

---

### `static int EnterPauseEvent()`

**作用**：处理暂停状态下的菜单逻辑，包括继续、存档、读档、返回欢迎界面。

**核心流程**：

1. 函数入口先执行：
   - `State.Difficulty--; LevelUp();`
   - 一种“在暂停时补偿/修正难度”的小技巧，用以恢复暂停前的速度设置。
2. 建立局部状态：
   - `pos` 为当前选项（0~3 分别对应 Continue/Save/Load/Leave）。
   - `back_flag` 与 `enter_flag` 分别表示按下 ESC / Enter。
3. 在循环中：
   - 先绘制现有游戏画面 `DrawHoleWindow`，再叠加 `DrawPauseLayout(pos)`，最后 `SDL_RenderPresent`。
   - 通过 `SDL_PollEvent` 处理：
     - 窗口缩放：`RectInit()` + 重绘。
     - `SDLK_DOWN/UP`：更改 `pos`（循环菜单）。
     - `SDLK_ESCAPE`：设置 `back_flag=1`，准备直接返回游戏。
     - `SDLK_RETURN`：设置 `enter_flag=1`，执行当前选项。
   - 当 `enter_flag` 为 1 时：
     - `pos == 0`：返回 0（继续游戏）。
     - `pos == 1`：`SaveGame()`。
     - `pos == 2`：`LoadGame()`，并立即重绘画面与暂停菜单。
     - `pos == 3`：返回 `EnterWelcomePage`（回到欢迎界面）。
   - 若 `back_flag` 为 1，则返回 0（只是关闭暂停界面）。
4. 没事件时通过 `SDL_Delay(16)` 避免 CPU 空转。

**设计要点**：  
暂停菜单本质上是一个**嵌套状态机**，专门处理在游戏中暂时脱离主循环的情形，并允许玩家完成存/读档和返回主菜单等操作。

---

### `static int EnterGameOverEvent()`

**作用**：在游戏结束时显示 GameOver 界面，并根据玩家选择返回状态码。

**关键逻辑**：

- 与暂停 / 欢迎界面的模式类似，区别在于菜单项和行为：
  - `pos == 0`：调用 `LoadGame()`，然后执行：
    - `State.Difficulty--;`
    - `LevelUp();`  
    用以调整难度/速度，类似 `EnterPauseEvent` 中的处理；
    - 返回 `Restart`。
  - `pos == 1`：调用 `ResetGameState()`，清空游戏状态并返回欢迎界面状态码 `EnterWelcomePage`。
- 在每一轮循环中都会 `DrawHoleWindow(...)` + `DrawGVLayout(pos)`，实时显示选中状态。

---

## 九、小结：控制逻辑的整体串联

将各模块函数串联起来，整个游戏的核心控制流程可以概括为：

1. `main()`：完成日志 / SDL / 字体 / 布局 / 状态的初始化，进入 `EnterMainEventLoop()`。
2. `EnterMainEventLoop()`：构建三大状态：
   - 欢迎界面 → `EnterWelcomeEventLoop()`；
   - 游戏进行 → `EnterGameEventLoop()`；
   - 退出 → 结束循环。
3. `EnterWelcomeEventLoop()` 根据玩家选项返回状态码，决定是开始新游戏、从存档开始还是退出。
4. `EnterGameEventLoop()` 内部：
   - 周期性调度 `EnterTickEvent()` 完成方块自动下落、落地、消行、游戏结束判定。
   - 使用 `EnterMotionEvent()` 响应玩家输入，实现左右移动、旋转、加速下落与暂停。
5. `EnterPauseEvent()` / `EnterGameOverEvent()` 提供游戏中断和结束时的菜单交互，允许存/读档、难度重置或回到欢迎页面。
6. `ShapeControl` 与 `MapControl` 分别封装“局部物理逻辑”和“全局网格逻辑”，`GameState` 模块负责整体状态与难度、分数关系，`DrawItems` 负责将这些状态在屏幕上直观地表示出来。

通过这种模块/函数划分，本项目实现了一个结构清晰、可维护性较好的俄罗斯方块游戏控制框架。
