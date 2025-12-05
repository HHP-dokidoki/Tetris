  
---
  
# 环境配置教程
  
  
- [说在前面](#说在前面 )
- [前置条件](#前置条件 )
- [Dev-C++](#dev-c )
  - [1. 创建项目、粘贴文件](#1-创建项目-粘贴文件 )
  - [2. 设置链接选项、设置包含文件/库、设置输出目录](#2-设置链接选项-设置包含文件库-设置输出目录 )
  - [3. 设置包含文件目录与库目录](#3-设置包含文件目录与库目录 )
  - [4. 设置输出目录](#4-设置输出目录 )
  - [5. 添加源代码文件](#5-添加源代码文件 )
  - [6. 编译运行](#6-编译运行 )
- [VS Code 配置指南](#vs-code-配置指南 )
  - [1. 基础环境准备](#1-基础环境准备 )
  - [2. 整理项目文件](#2-整理项目文件 )
  - [3. 配置 VS Code 文件](#3-配置-vs-code-文件 )
  - [4. 运行程序](#4-运行程序 )
- [Visual Studio 配置指南](#visual-studio-配置指南 )
  - [1. 创建项目、粘贴文件](#1-创建项目-粘贴文件-1 )
  - [2. 配置项目属性](#2-配置项目属性 )
    - [设置输出目录](#设置输出目录 )
    - [设置包含目录](#设置包含目录 )
    - [设置库目录](#设置库目录 )
    - [设置附加依赖项](#设置附加依赖项 )
  - [3. 添加源代码文件](#3-添加源代码文件 )
  - [4. 编译运行](#4-编译运行 )
  
## 说在前面
  
配置开发环境可能会遇到各种问题，可能需要花费数小时甚至更长时间来调整。希望本教程能起到一个清晰的引导作用。
  
## 前置条件
  
如果你在课堂上已经拿到了资源包，可以跳过此步骤。否则，请下载以下开发库：
  
- **[SDL3_devel-3.2.26](https://github.com/libsdl-org/SDL/releases/tag/release-2.32.10 )**
  - **Visual Studio 用户**：下载 `SDL3-devel-3.2.26-VC.zip`
  - **其他用户（如 Dev-C++、VS Code）**：下载 `SDL3-devel-3.2.26-mingw.zip`
- **[SDL3_ttf-3.2.2](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.24.0 )**
  - **Visual Studio 用户**：下载 `SDL3_ttf-devel-3.2.2-VC.zip` 
  - **其他用户（如 Dev-C++、VS Code）**：下载 `SDL3_ttf-devel-3.2.2-mingw.zip` 
  
下载完成后，将两个压缩包解压，并按照以下结构放入你的项目根目录下的 `ExternalIncludeFile/SDL2/` 文件夹中：
  
```
YourProjectRoot/
└── ExternalIncludeFile/
    └── SDL3/
        ├── SDL3_ttf-devel-3.2.2-mingw/ (或 -VC)
        └── SDL3-devel-3.2.26-mingw/  (或 -VC)
```
  
---
  
  
## Dev-C++
### 1. 创建项目、粘贴文件
  
1. 打开 **Dev-C++**。
2. 依次点击 **文件 → 新建 → 项目**。
3. 在 **Basic** 窗口中选择 **Empty Project**。
4. 在下方勾选 **C 项目**。
5. 将项目名称改为 `Tetris`（或其他你喜欢的名称）。
6. 将项目保存到一个**新建的、独立的文件夹**中（例如 `D:\MyTetrisProject`）。
7. 在文件资源管理器中，找到你刚刚保存项目的位置，将**资源包中的所有子文件夹**复制到该目录下。
  
### 2. 设置链接选项、设置包含文件/库、设置输出目录
  
1. 回到 Dev-C++ 项目界面。
2. 依次点击 **项目 → 项目属性**。
3. 切换到 **参数** 选项卡。
   - 在 **C 编译器** 框中填入：
     ```
     -std=c99
     ```
   - 在 **链接器** 框中填入（**顺序很重要**）：
     ```
     -lSDL3_ttf
     -lSDL3
     -lmingw32
     ```
  
### 3. 设置包含文件目录与库目录
  
仍在 **项目选项** 窗口中，切换到 **文件/目录** 选项卡。
  
- **设置库目录 (`Library Directories`)**:
  1. 在 **库目录** 界面下点击右侧的文件夹图标。
  2. 找到你的项目根目录，依次进入路径：
     `ExternalIncludeFile -> SDL3 -> SDL3-devel-3.2.26-mingw -> SDL3-3.2.26 -> x86_64-w64-mingw32 -> lib`
  3. 点击 **确定**，然后点击 **添加**。
  4. 重复上述操作，添加 SDL2_ttf 的库路径：
     `ExternalIncludeFile -> SDL3 -> SDL3_ttf-devel-3.2.2-mingw -> SDL3_ttf-3.2.2 -> x86_64-w64-mingw32 -> lib`
  
- **设置包含文件目录 (`Include Directories`)**:
  1. 在 **包含文件目录** 选项下点击右侧的文件夹图标。
  2. 找到你的项目根目录，依次进入路径：
    `ExternalIncludeFile -> SDL3 -> SDL3-devel-3.2.26-mingw -> SDL3-3.2.26 -> x86_64-w64-mingw32 -> include`
  3. 点击 **确定**，然后点击 **添加**。
  4. 重复上述操作，添加 SDL2_ttf 的包含路径：
     `ExternalIncludeFile -> SDL3 -> SDL3_ttf-devel-3.2.2-mingw -> SDL3_ttf-3.2.2 -> x86_64-w64-mingw32 -> include`
  
  
### 4. 设置输出目录
  
1. 在 **项目选项** 窗口中，切换到 **生成选项** 选项卡。
2. 在 **可执行文件输出目录** 中填入：
   ```
   exe
   ```
3. 点击 **确定** 保存所有设置。
  
### 5. 添加源代码文件
  
1. 在左侧 **项目管理器** 中，右键点击你的项目名称（如 `Tetris`）。
2. 选择 **添加 → 添加文件...**。
3. 进入项目根目录下的 `Tetris` 文件夹，选中**所有 `.c` 和 `.h` 文件**，点击 **打开**。
  
### 6. 编译运行
  
现在，你可以点击 **F11** 编译并运行你的俄罗斯方块程序了。确保 `SDL3.dll` 和 `SDL3_ttf.dll` 文件已复制到 `exe` 目录中。
  
***
## VS Code 配置指南
  
### 1. 基础环境准备
  
请确保你已经安装了以下软件：
- **VS Code**
- **MinGW-w64**（包含 `gcc` 和 `gdb`），并已将其 `bin` 目录添加到系统 `PATH` 环境变量中。
  
### 2. 整理项目文件
  
1. 新建一个项目文件夹（例如 `D:\MyTetrisProject`）。
2. 将你的 **资源包中所有文件夹** 放入其中。
3. 在项目根目录下创建一个名为 `.vscode` 的文件夹。
  
### 3. 配置 VS Code 文件
  
将以下内容分别保存到 `.vscode` 文件夹中对应的文件里，注意将 compiler 和 debugger 路径更改为你自己的路径。
  
```c_cpp_properties.json```
```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**",
                // Your original MinGW system include paths.
                "D:/MinGW/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/14.2.0/include/",
                "D:/MinGW/mingw64/bin/../lib/gcc/x86_64-w64-mingw32/14.2.0/../../../../x86_64-w64-mingw32/include",
                // SDL2 include path
                "${workspaceFolder}/ExternalIncludeFile/SDL3/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/include",
                "${workspaceFolder}/ExternalIncludeFile/SDL3/SDL3_ttf-devel-3.2.2-mingw/SDL3_ttf-3.2.2/x86_64-w64-mingw32/include",
                // Project's own header directory.
                "${workspaceFolder}/Tetris"
            ],
            "defines": ["_DEBUG", "UNICODE", "_UNICODE"],
            "compilerPath": "D:/MinGW/mingw64/bin/gcc.exe",  // Path to your GCC compiler.
            "cStandard": "c99",
            "cppStandard": "c++17",
            "intelliSenseMode": "windows-gcc-x64"
        }
    ],
    "version": 4
}
```
```launch.json```
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/exe/Tetris.exe", // The executable to debug.
            "args": [],                                     // Command-line arguments passed to the program.
            "stopAtEntry": false,       
            "cwd": "${workspaceFolder}/exe",                // Working directory (must contain SDL2.dll and SDL2_ttf.dll).
            "environment": [],
            "externalConsole": true,                        // Use external console window for output.
            "MIMode": "gdb",
            "miDebuggerPath": "D:/MinGW/mingw64/bin/gdb.exe", // Path to your GDB debugger.
            "preLaunchTask": "C/C++: gcc.exe build active file", // Build task to run before launching.
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                },
                {
                    "description": "Set Disassembly Flavor to Intel",
                    "text": "-gdb-set disassembly-flavor intel",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```
```settings.json```
```json
{
    "terminal.explorerKind": "external", // Use external terminal for running/debugging.
    "files.associations": {
        "*.h": "c"                      // Treat all header files as C language.
    },
    "C_Cpp.default.cStandard": "c99",   // Default C standard for IntelliSense.
    "C_Cpp.dimInactiveRegions": false,  // Do not dim inactive preprocessor blocks.
    "files.encoding": "gbk"            // Source file encoding.
}
```
```tasks.json```
```json
{
    "tasks": [
        {
            // Build task: compiles all .c files in Tetris folder.
            "type": "shell", // Changed from "cppbuild" to support wildcard *.c reliably on Windows.
            "label": "C/C++: gcc.exe build active file",
            "command": "D:/MinGW/mingw64/bin/gcc.exe", // Your GCC compiler path.
            "args": [
                "-fdiagnostics-color=always",
                "-g",
                // Compile all C source files in the Tetris directory.
                "${workspaceFolder}/Tetris/*.c",                          
                // Output executable file path.
                "-o",
                "${workspaceFolder}/exe/Tetris.exe",            
                // Include paths for headers.
                "-I${workspaceFolder}/Tetris", 
                "-I${workspaceFolder}/ExternalIncludeFile/SDL3/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/include",
                "-I${workspaceFolder}/ExternalIncludeFile/SDL3/SDL3_ttf-devel-3.2.2-mingw/SDL3_ttf-3.2.2/x86_64-w64-mingw32/include",
                // Library search paths.
                "-L${workspaceFolder}/ExternalIncludeFile/SDL3/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/lib",
                "-L${workspaceFolder}/ExternalIncludeFile/SDL3/SDL3_ttf-devel-3.2.2-mingw/SDL3_ttf-3.2.2/x86_64-w64-mingw32/lib",
                // Link libraries. Do not change the order.
                "-lSDL3_ttf",
                "-lSDL3",
                "-lmingw32",
                // Compilation options.
                "-fexec-charset=utf-8",
                "-Wall",
                "-std=c99",
  
            ],
            "options": {
                "cwd": "${workspaceFolder}" // Working directory for the build command.
            },
            "problemMatcher": [
                "$gcc"  // Parse GCC output for errors/warnings.
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "presentation": {
                "echo": true,
                "reveal": "always",// Always show the terminal panel.
                "focus": true,
                "panel": "new"     // Use a new terminal panel for each build.
            },
            "detail": "Task generated by Debugger."
        },
  
        {
            // Run task: executes the compiled program (corrected to run .exe, not .c).
            "label": "run",
            "type": "shell",
            "dependsOn": "C/C++: gcc.exe build active file", // Build first, then run.
            "command": "${workspaceFolder}/exe/Tetris.exe",
  
            "group": {
                // This task is in the "test" group; you can bind a shortcut to "Run Test Task".
                "kind": "test",
                "isDefault": true
            },
            "presentation": {
                "echo": true,
                "reveal": "always",
                "focus": true,
                "panel": "new"
            }
        }
    ],
    "version": "2.0.0"
}
```
  
如果你的环境配置没有问题，参考以上配置文件后就能正常编译运行了。
***
  
### 4. 运行程序
  
1. 在 VS Code 中，通过 **文件 → 打开文件夹** 打开你的项目根目录。
2. 打开Tetris中任意一个 `.c` 文件。
3. 按下 **`Ctrl+F5`** 即可编译并运行程序。
  
## Visual Studio 配置指南
~~都用上VS了为何不尝试自己配置环境呢~~
### 1. 创建项目、粘贴文件
  
1. 打开 **Visual Studio**。
2. 创建一个 **空项目 (Empty Project)**。
3. 选择一个文件夹来保存项目（例如 `D:\MyTetrisProject`）。
4. 项目创建完成后，将**资源包中所有文件夹**复制到此项目文件夹中。
  
### 2. 配置项目属性
  
右键项目，选择 properties，以下任务均在此窗口中完成
1. 在 **解决方案资源管理器** 中，右键点击你的项目名称，选择 **属性 (Properties)**。
2. 在属性窗口顶部，将 **配置 (Configuration)** 设置为 `All Configurations`，**平台 (Platform)** 设置为 `x64`。（本指引只提供这两个选项的配置）；
  
#### 设置输出目录
  
1. 导航到 **配置属性 (Configuration Properties) → 常规 (General)**。
2. 找到 **输出目录 (Output Directory)**，点击右侧的下拉箭头，选择 **浏览 (Browse...)**。
3. 选择项目文件夹下的 `exe` 文件夹，点击 **选择文件夹**。
  
#### 设置包含目录
  
1. 导航到 **配置属性 (Configuration Properties) → VC++ 目录 (VC++ Directories)**。
2. 找到 **包含目录 (Include Directories)**，点击右侧的下拉箭头，选择 **编辑 (Edit...)**。
3. 在弹出的窗口中，点击上方的**新建行**图标（文件夹图标）。
4. 在项目文件夹中，依次进入路径：
   `ExternalIncludeFile -> SDL3 -> SDL3-devel-3.2.26-VC -> SDL3-3.2.26 -> include`
5. 重复步骤 3-4，添加 SDL2_ttf 的包含路径：
   `ExternalIncludeFile -> SDL3 -> SDL3_ttf-devel-3.2.2-VC -> SDL3_ttf-3.2.2 -> include`
6. 点击 **确定**。
  
#### 设置库目录
  
1. 仍在 **VC++ 目录** 页面，找到 **库目录 (Library Directories)**，点击 **编辑**。
2. 添加 SDL2 的库路径：
   `ExternalIncludeFile -> SDL3 -> SDL3-devel-3.2.26-VC -> SDL3-3.2.26 -> lib -> x64`
3. 添加 SDL2_ttf 的库路径：
   `ExternalIncludeFile -> SDL3 -> SDL3_ttf-devel-3.2.2-VC -> SDL3_ttf-3.2.2 -> lib -> x64`
4. 点击 **确定**。
  
#### 设置附加依赖项
  
1. 导航到 **配置属性 (Configuration Properties) → 链接器 (Linker) → 输入 (Input)**。
2. 找到 **附加依赖项 (Additional Dependencies)**，点击 **编辑**。
3. 在弹出的窗口中填入：
   ```
   SDL3.lib
   SDL3_ttf.lib
   ```
4. 点击 **确定**，然后点击属性窗口右下角的 **应用 (Apply)** 和 **确定 (OK)**。
  
### 3. 添加源代码文件
  
1. 在 **解决方案资源管理器** 中，右键点击 **源文件 (Source Files)**。
2. 选择 **添加 → 现有项... (Add → Existing Item...)**。
3. 进入项目文件夹下的 `Tetris` 文件夹，选中**所有 `.c` 和 `.h` 文件**，点击 **添加**。
  
### 4. 编译运行
  
现在，你可以按 **`Ctrl+F5`** 来编译并运行你的俄罗斯方块程序了。确保 `SDL3.dll` 和 `SDL3_ttf.dll` 文件已复制到 `exe` 目录中。
  
---
  
