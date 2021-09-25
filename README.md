# 自制计算机编译套装

My computer's compiler suite

## 构建
 
请使用现代构建工具[Xmake](https://github.com/xmake-io/xmake)编译本项目。

### 在 Windows 下使用 MSVC (Visual Studio) 编译

在项目根目录运行：

```
xmake config -p windows
xmake build
```

即可安装全部依赖并编译到./build目录。如果使用mingw，请将`windows`改为`mingw`。

### 其它平台

运行` xmake config -h` 查看Xmake支持的平台列表。替换上述指令中的`windows`即可。

> 本项目已停止对过时构建系统CMake的支持。


