# 自制计算机编译套装

My computer's compiler suite

## 构建
 
请使用现代构建工具[Xmake](https://github.com/xmake-io/xmake)编译本项目。

### 在 Windows 下使用 MSVC (Visual Studio) 构建

在项目根目录运行：

```
xmake config -p windows
xmake build
```

即可安装全部依赖并输出到./build目录。如果使用mingw，请将`windows`改为`mingw`。

### 其它平台

运行` xmake config -h` 查看Xmake支持的平台列表。替换上述指令中的`windows`即可。

> 本项目已停止对过时构建系统CMake的支持。

## 使用说明

### 微程序编译器 mpc

#### 命令行

在mpc.exe所在目录下运行：

```
.\mpc.exe 源文件路径
```

即可生成微程序文件和相关输出。

输出文件中manifest.txt为输出指令清单，包括每条指令的各位值（`.`为0，`X`为1）以及指令的地址。

digest.sha1为源文件的哈希值，如果源文件未更改，将跳过二进制文件的生成。

#### 程序语法

规则：

1. 每一行都是一条**微指令**（空行和注释除外）。

2. 一条微指令中，写出的微命令对应的位置1，未写出的置0。（本程序微命令又称flag）

例如：
```
PC_I 
```
表示`PC_I`位为1，其余均为0。

如果希望所有位都置0，请使用`pass`独占该行。


3. 微指令前可以有一个**标签**。标签的格式为`0x`+16进制数+`:`。标签的值表示指令的地址。没有标签的指令地址顺延。指令间的间隙用0填充。

例如：
``` 
0x00: 
    PC_I            
    PC_O AR_I       
    RAM_ROM         
    RAM_ROM DR_I     
    DR_O IR_I INT_IR 
 
0x10: 
    RD_O ALU_I0 ALU_I1         
    ALU_OP0 ALU_O RD_I ajmp=0  
```

其中：`PC_I`在地址`0x00`中，
`PC_O AR_I`在地址`0x01`中......，
`RD_O ALU_I0 ALU_I1`在地址`0x10`中，
`ALU_OP0 ALU_O RD_I ajmp=0`在地址`0x11`中......

4. `#`及其后一直到行末均为**注释**。注释和空行会被编译器忽略。

5. 存在一些**属性**，用来一次性设置多个位。例如 `rjmp=4` 将`J3-J0`分别设置为`0100`，并将`ABS_REL`设置为`1`。

#### 自定义/拓展mpc指引

**修改命令名**

修改 mpc/backend.hpp 中`InstructionIndexMapping::mapping`的值即可，例：

```

        const std::vector<std::tuple<std::string, int>> mapping =
        {
            {"R_W",0},
            {"PAM",1},
            {"ALU_OP0",2},
            {"ALU_OP1",3},
            {"ALU_OP2",4},
            {"M_DBUS",5},
            {"P1",6},
            {"P2",7},

            {"IR_O",8},
            {"PC_O",9},
            {"RS_O",10},
            {"ALU_O",11},
            {"DR_O",12},
            {"RD_O",13},
            {"ER_O",14},

            {"DR_I",15},

            {"IR_I",16},
            {"PC_I",17},
            {"RS_I",18 },
            {"ALU_I0",19 },
            {"ALU_I1", 20},
            {"AR_I",21},
            {"RD_I",22},
            {"ER_I",23},

            {"INT_IR", 24},
            {"RAM_ROM", 25},
            {"PC1_DBUS", 26},
            {"J0", 27},
            {"J1", 28},
            {"J2", 29},
            {"J3", 30},
            {"ABS_REL", 31},
        };
```

**修改微指令长度**

修改 mpc/backend.hpp 中 `MyComputerBackend::parts` 的值即可。例如，对于32位微指令：

```
    class MyComputerBackend
    {
    public:
        static constexpr int parts = 4;
    ...
```

最终生成的二进制文件数量也等于此值。

**自定义属性**

请修改mpc/backend.hpp 中 `MyComputerBackend::write_outputs`中的内容。


