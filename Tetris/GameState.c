#include "Common.h"
#include "MapControl.h"
#include "DrawItems.h"

// 消除1~4行的得分倍数表
static const int line_bonus[5] = { 0, 1, 3, 6, 10 };

void GameStateInit(void)
{
    TRACE_ENTER();
    // 初始化游戏状态：地图、分数、难度、计时器、方块
    MapInit();

    State.Score = 0;
    State.BaseScore = 10;
    State.Difficulty = 1;
    State.LineCount = 0;

    // 加速下落间隔和当前间隔
    State.Speed_interval = 50;
    State.Current_interval = 800;
    State.Base_interval = 800;

    State.Last_tick = 0;
    State.Current_tick = 0;

    // 初始化方块（-1表示未设置）
    State.Next_shape = -1;
    State.Current_shape = -1;
    State.Shape_pos_x = ST_X;
    State.Shape_pos_y = ST_Y;
}

int CalScore(int row_cnt)
{
    TRACE_ENTER();
    // 根据消除行数计算获得的分数
    if (row_cnt < 0 || row_cnt > 4)
    {
        return 0;
    }

    return line_bonus[row_cnt] * State.BaseScore;
}

void LevelUp(void)
{
    TRACE_ENTER();
    // 升级处理：难度+1，增加基础分数，加快下落速度
    Uint32 new_interval = 0;

    State.Difficulty += 1;

    // 难度越高，每行基础分数越高（每级+5）
    State.BaseScore = 10 + (State.Difficulty - 1) * 5;

    // 计算新的下落速度（每级快100ms，最快50ms）
    new_interval = 800 - (State.Difficulty - 1) * 100;
    State.Base_interval = new_interval > State.Base_interval ? new_interval : State.Base_interval;
    State.Current_interval = State.Base_interval;
}

void SaveGame(void)
{
    TRACE_ENTER();
    // 保存游戏状态到文件：地图+分数+难度+当前方块信息
    FILE* file = NULL;
    errno_t err = 0;

    err = fopen_s(&file, "..\\saves\\savedata.save", "w");
    if (!(log_file && file && err == 0))
    {
        fprintf(log_file, "Load failed when trying to open save file.\n");
        fflush(log_file);
        return;
    }

    // 保存地图占用状态（Value）
    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fprintf(file, "%d ", State.map[i][j].Value);
        }
        fputc('\n', file);
    }

    // 分别保存颜色的R、G、B、A分量
    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fprintf(file, "%d ", State.map[i][j].Color.r);
        }
        fputc('\n', file);
    }

    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fprintf(file, "%d ", State.map[i][j].Color.g);
        }
        fputc('\n', file);
    }

    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fprintf(file, "%d ", State.map[i][j].Color.b);
        }
        fputc('\n', file);
    }

    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fprintf(file, "%d ", State.map[i][j].Color.a);
        }
        fputc('\n', file);
    }

    fputc('\n', file);

    // 保存游戏状态数据
    fprintf(file, "SCORE:%d\n", State.Score);
    fprintf(file, "DIFFICULTY:%d\n", State.Difficulty);
    fprintf(file, "LINECOUNT:%d\n", State.LineCount);
    fprintf(file, "CURRENT:%d\n", State.Current_shape);
    fprintf(file, "NEXT:%d\n", State.Next_shape);
    fprintf(file, "CURRENTX:%d\n", State.Shape_pos_x);
    fprintf(file, "CURRENTY:%d\n", State.Shape_pos_y);

    fclose(file);
}

void LoadGame(void)
{
    TRACE_ENTER();
    // 从文件加载游戏状态：地图+分数+难度+当前方块信息
    char line[520];
    errno_t err;
    FILE* file = NULL;

    err = fopen_s(&file, "..\\saves\\savedata.save", "r");
    if (!(log_file && file && err == 0))
    {
        fprintf(log_file, "Save failed when trying to open save file.\n");
        fflush(log_file);
        return;
    }

    // 加载地图占用状态
    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fscanf_s(file, "%d", &State.map[i][j].Value);
        }
    }

    // 加载颜色分量（R、G、B、A）
    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fscanf_s(file, "%hhu", &State.map[i][j].Color.r);
        }
    }

    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fscanf_s(file, "%hhu", &State.map[i][j].Color.g);
        }
    }

    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fscanf_s(file, "%hhu", &State.map[i][j].Color.b);
        }
    }

    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fscanf_s(file, "%hhu", &State.map[i][j].Color.a);
        }
    }

    // 跳过空行
    while (fgetc(file) != '\n' && !feof(file)) { ; }

    // 加载游戏状态数据
    while (fgets(line, (int)sizeof(line), file))
    {
        if (strncmp(line, "SCORE:", 6) == 0)
        {
            sscanf_s(line, "SCORE:%d", &State.Score);
        }
        else if (strncmp(line, "DIFFICULTY:", 11) == 0)
        {
            sscanf_s(line, "DIFFICULTY:%d", &State.Difficulty);
        }
        else if (strncmp(line, "LINECOUNT:", 10) == 0)
        {
            sscanf_s(line, "LINECOUNT:%d", &State.LineCount);
        }
        else if (strncmp(line, "CURRENT:", 8) == 0)
        {
            sscanf_s(line, "CURRENT:%d", &State.Current_shape);
        }
        else if (strncmp(line, "NEXT:", 5) == 0)
        {
            sscanf_s(line, "NEXT:%d", &State.Next_shape);
        }
        else if (strncmp(line, "CURRENTX:", 9) == 0)
        {
            sscanf_s(line, "CURRENTX:%d", &State.Shape_pos_x);
        }
        else if (strncmp(line, "CURRENTY:", 9) == 0)
        {
            sscanf_s(line, "CURRENTY:%d", &State.Shape_pos_y);
        }
    }
   
    fclose(file);
}

void ResetGameState(void)
{
    TRACE_ENTER();
    // 重置游戏状态（调用初始化而非复制逻辑）
    GameStateInit();
}