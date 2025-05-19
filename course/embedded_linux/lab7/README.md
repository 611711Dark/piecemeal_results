# 实验报告：Linux内核模块与驱动开发

## 实验概述
本次实验主要涉及Linux内核模块的开发和字符设备驱动的实现，包括基础模块开发、字符设备通信、以及实现一个凯撒密码加密模块。

---

## 实验环境
- **开发环境**：Virtual Machine (VM)
- **目标设备**：BeagleBone开发板
- **工具链**：交叉编译工具

---

## 实验内容与步骤

### 练习1：hello_dev内核模块
1. **准备工作**  
   - 将`hello_dev`示例文件夹复制到主目录：
     ```bash
     cp -R /opt/beaglebone/samples/hello_dev ./
     ```
   - 测试编译：
     ```bash
     cd hello_dev
     make copy
     ```

2. **加载与卸载模块**  
   - 登录BeagleBone并通过SSH加载模块：
     ```bash
     sudo -s
     insmod hellodev.ko
     ```
   - 卸载模块：
     ```bash
     rmmod hellodev
     ```
   - 查看内核日志：
     ```bash
     dmesg
     ```

3. **功能验证**  
   - 检查`hellodev.c`文件，理解模块的初始化和退出函数，以及`helloRead`函数的功能。

---

### 练习2：字符设备驱动开发
1. **修改代码**  
   - 取消`helloRead`函数中的`printk`注释，并更新模块作者信息。
   - 重新编译并复制模块到BeagleBone：
     ```bash
     make copy
     ```

2. **创建设备节点**  
   - 在BeagleBone上执行：
     ```bash
     mknod /dev/hello c 60 0
     ```

3. **测试功能**  
   - 加载模块并检查日志：
     ```bash
     insmod hellodev.ko
     dmesg
     ```
   - 触发`read`操作：
     ```bash
     cat /dev/hello
     ```
   - 观察`read()`消息出现的次数，并解释原因。

---

### 练习3：实现`open`和`release`事件处理
1. **代码修改**  
   - 在`hellodev.c`中添加`open`和`release`函数，并分别打印日志消息。
   - 重新编译并加载模块。

2. **测试功能**  
   - 使用`cat /dev/hello`触发事件，并通过`dmesg`查看日志。

---

### 练习4：凯撒密码模块
1. **准备工作**  
   - 复制`hello_dev`模块并重命名为`caesar`：
     ```bash
     cp -R hello_dev caesar
     cd caesar
     make clean
     mv hellodev.c caesar.c
     ```

2. **代码修改**  
   - 更新`Makefile`和`caesar.c`文件，修改模块名、主设备号和函数名称。
   - 实现`caesarWrite`和`caesarRead`函数，支持凯撒密码加密功能。

3. **测试功能**  
   - 创建设备节点：
     ```bash
     mknod /dev/caesar c 61 0
     ```
   - 加载模块并测试加密功能：
     ```bash
     insmod caesar.ko
     echo 3 > /dev/caesar  # 设置密钥为3
     echo A > /dev/caesar   # 加密字母A
     cat /dev/caesar        # 读取加密结果（应为D）
     ```

---

### 练习5：支持长字符串的凯撒密码模块
1. **代码修改**  
   - 在`caesar_255.c`中扩展功能，支持最长255个字符的加密。
   - 使用动态内存分配（`kmalloc`和`kfree`）管理消息缓冲区。

2. **测试功能**  
   - 加载模块并测试长字符串加密：
     ```bash
     echo 5 > /dev/caesar
     echo "HelloWorld" > /dev/caesar
     cat /dev/caesar
     ```

---

## 实验结果与分析
1. **hello_dev模块**  
   - 成功加载和卸载模块，日志中显示`hello Loading ...`和`hello Unloading ...`。
   - `helloRead`函数每次读取一个字符，因此`cat /dev/hello`会触发多次`read()`调用。

2. **凯撒密码模块**  
   - 成功实现单字符和长字符串的加密功能。
   - 密钥设置和消息加密功能正常，加密结果符合预期。

---

## 问题与解决
1. **问题**：`read()`消息多次出现。  
   **原因**：`helloRead`函数每次只读取一个字符，`cat`命令会多次调用该函数直到读取完整个字符串。

2. **问题**：动态内存分配失败。  
   **解决**：在`caesar_255.c`中添加错误检查，确保`kmalloc`成功后再进行操作。

---

## 实验总结
通过本次实验，掌握了以下内容：
- Linux内核模块的开发流程。
- 字符设备驱动的实现与通信机制。
- 凯撒密码算法的内核模块实现。
- 动态内存管理在内核模块中的应用。

