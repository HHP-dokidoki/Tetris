#ifndef GAMESTATE_H

#define GAMESTATE_H

void GameStateInit(void);

int CalScore(int row_cnt);

void LevelUp();

void SaveGame(void);

void LoadGame(void);

void ResetGameState(void);

#endif
