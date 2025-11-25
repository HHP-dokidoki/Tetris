--- Functions.md (原始)


+++ Functions.md (修改后)
# Functions Documentation

This document details the important functions in the Tetris game and their implementation approaches.

## 1. Initialization Functions

### 1.1 `LogInit()`
**Purpose**: Initialize the logging system by opening a log file for writing.
**Implementation Approach**: Uses `fopen_s()` to open "..\\log\\game_Trace.log" in write mode and assigns it to the global `log_file` variable. If successful, writes a session start message to the log file. Returns 0 on success, -1 on failure.

### 1.2 `WindowRdrInit()`
**Purpose**: Initialize SDL window and renderer.
**Implementation Approach**:
- Initializes SDL video subsystem with `SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)`
- Creates a window with dimensions W x H (720 x 1000) using `SDL_CreateWindow`
- Creates a renderer with `SDL_CreateRenderer`
- Sets renderer draw color to black and blend mode to blend
- Clears and presents the renderer
- Initializes the game map with `memset(State.map, 0, sizeof(State.map))`
- Returns 0 on success, -1 on failure

### 1.3 `FontInit()`
**Purpose**: Initialize fonts for text rendering.
**Implementation Approach**:
- Initializes SDL_ttf library with `TTF_Init()`
- Sets font color to white (255, 255, 255, 255)
- Loads "SourceCodePro-ExtraBoldItalic.ttf" as NumberFont for numbers
- Loads "VT323-Regular.ttf" as UIFont for UI labels
- Returns 0 on success, -1 on failure

### 1.4 `RectInit()`
**Purpose**: Initialize all SDL_Rect structures for UI elements.
**Implementation Approach**: Sets up coordinates and dimensions for all UI elements by assigning values from the defined macros to the corresponding fields in the `layout` structure. This includes:
- Position and size for labels (Difficulty, Next, Score, LineCount)
- Position and size for value displays
- Position and size for pause menu elements
- Position and size for welcome screen elements
- Position and size for game over screen elements

### 1.5 `RandInit()`
**Purpose**: Initialize the random number generator.
**Implementation Approach**: Uses `srand()` with the current SDL tick count (`SDL_GetTicks()`) as the seed to ensure different random sequences on each game run.

### 1.6 `GameStateInit()`
**Purpose**: Initialize the game state variables.
**Implementation Approach**:
- Calls `MapInit()` to initialize the game map
- Sets initial score to 0, base score to 10, difficulty to 1, and line count to 0
- Sets speed interval parameters (initial 800ms with 50ms intervals)
- Initializes timing variables (Last_tick, Current_tick)
- Sets initial shape positions (ST_X=6, ST_Y=0) and shape IDs to -1

## 2. Game Logic Functions

### 2.1 `EnterMainEventLoop()`
**Purpose**: Main game loop that handles events, rendering, and game logic.
**Implementation Approach**:
- Runs continuously until game exit condition is met
- Handles different game states (welcome, playing, pause, game over)
- Processes keyboard input for game controls
- Updates game state based on timing
- Calls rendering functions to draw current state

### 2.2 `GetNextShape()`
**Purpose**: Generate a random shape ID for the next piece.
**Implementation Approach**:
- Generates a random number between 0-27 using `rand() % 28`
- Maps the result to valid shape IDs (0-18) with different probabilities
- Returns shape IDs 0-11 with higher probability (12/28 chance)
- Returns shape IDs 12-17 with lower probability (12/28 chance, mapped from 12-23)
- Returns shape ID 18 with lowest probability (4/28 chance)

### 2.3 `NotConflict()`
**Purpose**: Check if a shape placement conflicts with existing blocks or boundaries.
**Implementation Approach**:
- Takes shape position (x, y) and shape ID as parameters
- Iterates through the 8 coordinates of the shape (4 blocks, x and y for each)
- Checks if any block is outside the game boundaries (x < 1, x >= NX-1, y >= NY-1)
- Skips blocks that would be above the playfield (y < 1)
- Checks if any block position is already occupied in the game map (Value != 0)
- Returns 1 if no conflict, 0 if conflict exists

### 2.4 `Moveable()`
**Purpose**: Determine if a shape can move in a specified direction.
**Implementation Approach**:
- Takes current position (x, y), shape ID, and movement mode (DOWN=1, LEFT=2, RIGHT=3)
- Uses a switch statement to calculate new position based on movement mode
- Calls `NotConflict()` with the new position to check for valid movement
- Returns 1 if movement is possible, 0 otherwise

### 2.5 `MoveShape()`
**Purpose**: Move a shape in the specified direction if possible.
**Implementation Approach**:
- First, clears the current shape from the map by setting Value to 0 and Color to BACKGROUND_COLOR
- Then, updates the shape's x or y position based on the movement mode (DOWN, LEFT, RIGHT)
- Only updates if the move is valid (confirmed with `NotConflict()` check)
- Handles boundary checking to prevent invalid moves

### 2.6 `RollShape()`
**Purpose**: Rotate a shape in-place if the rotation is valid.
**Implementation Approach**:
- Takes current position and pointer to shape ID
- Checks if the next shape in the rotation sequence (stored in shapes[shape_id].Next) conflicts with existing blocks or boundaries
- If no conflict, updates the shape ID to the next shape in the rotation sequence
- Uses the `Next` field in the Shape structure to determine rotation sequence

### 2.7 `FixShape()`
**Purpose**: Fix a shape to the game map when it can no longer move.
**Implementation Approach**:
- Takes the current position and shape ID
- Iterates through the 4 blocks of the shape (8 coordinates, processed as 4 pairs)
- Updates the game map with Value=1 and the shape's color
- Includes boundary checks to prevent negative array access
- After fixing the shape, calls line clearing and scoring logic

## 3. Map Control Functions

### 3.1 `MapInit()`
**Purpose**: Initialize the game map with border values.
**Implementation Approach**:
- Uses `memset()` to clear the entire map to zero values
- Sets the left and right borders (first and last column) to Value=1
- Sets the bottom border (last row) to Value=1
- Creates a playable area surrounded by border blocks

### 3.2 `LineFull()`
**Purpose**: Check if a row is completely filled with blocks.
**Implementation Approach**:
- Takes the row index (y) as parameter
- Checks each column in the specified row from index 1 to NX-2 (excluding borders)
- Returns 0 if any position in the row has Value=0 (empty)
- Returns 1 if all non-border positions in the row are filled (Value != 0)

### 3.3 `DeleteRow()`
**Purpose**: Remove a completed row and shift all rows above it down.
**Implementation Approach**:
- Takes the row index (y) to be deleted
- Iterates from the specified row down to row 2 (preserving top border)
- Copies each row down one position in the map (row i becomes row i+1)
- Clears the topmost row (row 1) after shifting by setting Value to 0 and Color to zero values

### 3.4 `DeleteRows()`
**Purpose**: Find and delete all completed rows in the game map.
**Implementation Approach**:
- Takes the starting row index (typically where the shape was fixed)
- Checks up to 4 rows starting from the given row (since max 4 rows can be completed by one shape)
- Uses a while loop to handle multiple rows being completed at the same level
- Counts the number of rows deleted and updates State.LineCount
- Calls `CalScore()` to calculate score based on rows cleared
- Checks if enough lines have been cleared to trigger a level up
- Returns the bonus score earned

## 4. Game State Management Functions

### 4.1 `CalScore()`
**Purpose**: Calculate score based on number of lines cleared.
**Implementation Approach**:
- Takes the number of rows cleared (0-4) as input
- Uses a static array `line_bonus` with values {0, 1, 3, 6, 10} for different line counts
- Multiplies the bonus by the BaseScore which increases with difficulty
- Returns the calculated score increment

### 4.2 `LevelUp()`
**Purpose**: Increase game difficulty and speed.
**Implementation Approach**:
- Increases the difficulty level by 1
- Updates BaseScore to 10 + (Difficulty - 1) * 5
- Decreases the base interval by 100ms per level (new_interval = 800 - (Difficulty - 1) * 100)
- Sets the current interval to the new base interval
- Ensures the interval doesn't become too fast (doesn't decrease below current interval)

### 4.3 `SaveGame()`
**Purpose**: Save the current game state to a file.
**Implementation Approach**:
- Opens "..\\saves\\savedata.save" for writing
- Saves the entire game map in a specific format:
  - First saves all Value fields for each position
  - Then saves all Color.r, Color.g, Color.b, and Color.a values separately
- Saves game state variables (Score, Difficulty, LineCount, Current_shape, Next_shape, Shape_pos_x, Shape_pos_y) with labels
- Closes the file after saving

### 4.4 `LoadGame()`
**Purpose**: Load a saved game state from a file.
**Implementation Approach**:
- Opens "..\\saves\\savedata.save" for reading
- Reads the game map in the same format as saved:
  - First reads all Value fields
  - Then reads all Color.r, Color.g, Color.b, and Color.a values
- Reads game state variables by parsing lines with specific labels
- Prints a success message after loading
- Closes the file after loading

### 4.5 `ResetGameState()`
**Purpose**: Reset all game state variables to initial values.
**Implementation Approach**:
- Simply calls `GameStateInit()` to reset all state variables to their initial values
- This includes resetting the map, score, difficulty, lines, timing, and shape positions

## 5. Drawing Functions

### 5.1 `DrawHoleWindow()`
**Purpose**: Render the main game window with current state.
**Implementation Approach**:
- Draws the game map with colored blocks
- Renders the current falling shape
- Draws the next shape preview
- Displays score, line count, and difficulty values
- Updates the renderer

### 5.2 `DrawWelcomeLayout()` and `DrawGVLayout()`
**Purpose**: Render welcome screen and game over screen layouts.
**Implementation Approach**:
- Draws UI elements for menu screens
- Handles highlighting of selected menu items
- Renders text elements using loaded fonts

### 5.3 `DrawPauseLayout()`
**Purpose**: Render the pause menu.
**Implementation Approach**:
- Draws the semi-transparent pause overlay
- Renders pause menu options (Continue, Save, Load, Leave)
- Handles visual feedback for selected options