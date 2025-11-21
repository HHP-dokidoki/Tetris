#include "Common.h"
#include "MapControl.h"
#include "DrawItems.h"

static const int line_bonus[5] = { 0, 1, 3, 6, 10 };
void GameStateInit(void)
{
    TRACE_ENTER();

    MapInit();

    State.Score = 0;
    State.BaseScore = 10;
    State.Difficulty = 1;
    State.LineCount = 0;

    State.Speed_interval = 50;
    State.Current_interval = 800;
    State.Base_interval = 800;

    State.Last_tick = 0;
    State.Current_tick = 0;

    State.Next_shape = -1;
    State.Current_shape = -1;
    State.Shape_pos_x = ST_X;
    State.Shape_pos_y = ST_Y;
}

int CalScore(int row_cnt)
{
    TRACE_ENTER();

    // 一般用不到这个判断
    if (row_cnt < 0 || row_cnt > 4)
    {
        return 0;
    }

    return line_bonus[row_cnt] * State.BaseScore;
}

void LevelUp(void)
{
    TRACE_ENTER();
    Uint32 new_interval = 0;

    State.Difficulty += 1;

    State.BaseScore = 10 + (State.Difficulty - 1) * 5;

    new_interval = 800 - (State.Difficulty - 1) * 100;
    State.Base_interval = new_interval > State.Base_interval ? new_interval : State.Base_interval;
    State.Current_interval = State.Base_interval;
}

void SaveGame(void)
{
    TRACE_ENTER();
    FILE* file = fopen("..\\saves\\savedata.save", "w");
    if (file == NULL)
    {
        TRACE_MSG("Failed to create save file\n");
        return;
    }
    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fprintf(file, "%d ", State.map[i][j].Value);
        }
        fputc('\n', file);
    }

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

    fprintf(file, "SCORE:%d\n", State.Score);
    fprintf(file, "DIFFICULTY:%d\n", State.Difficulty);
    fprintf(file, "LINECOUNT:%d\n", State.LineCount);
    fprintf(file, "CURRENT:%d\n", State.Current_shape);
    fprintf(file, "NEXT:%d\n", State.Next_shape);
    fprintf(file, "CURRENTX:%d\n", State.Shape_pos_x);
    fprintf(file, "CURRENTY:%d\n", State.Shape_pos_y);

    SDL_Log("Successfully saved~\n");
    fclose(file);
}

void LoadGame(void)
{
    TRACE_ENTER();
    FILE* file = fopen("..\\saves\\savedata.save", "r");
    char line[520];

    if (file == NULL)
    {
        TRACE_MSG("Save file not found\n");
        
        return;
    }
    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            fscanf_s(file, "%d", &State.map[i][j].Value);
        }
    }

    for (int i = 0; i < NY; i++)
    {
        for (int j = 0; j < NX; j++)
        {
            // Color.r 的类型为Uint8，查看宏定义发现其根本上是unsigned char，用 %hhu读取
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

    while (fgetc(file) != '\n' && !feof(file)) { ; }

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
    SDL_Log("Successfully loaded~\n");
}

void ResetGameState(void)
{
    TRACE_ENTER();
    // 这样写逻辑性更好一些,就不会在重置状态的时候调用初始化
    GameStateInit();
}