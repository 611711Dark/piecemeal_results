# 逻辑门类实现项目

## 项目简介

本项目演示了如何在C语言中使用结构体和函数指针来模拟面向对象编程，实现逻辑门类的功能。

## 项目内容

### LogicGate.c
- 实现了完整的逻辑门类，包含输入状态存储
- 支持AND和OR逻辑运算
- 使用函数指针实现多态性
- 包含构造函数和析构函数

### LogicGate_easy.c  
- 简化版本的逻辑门实现
- 直接传递参数进行运算，不存储状态
- 支持AND、OR、XOR逻辑运算
- 更轻量级的设计

## 技术特点

- 使用结构体模拟类
- 函数指针实现方法绑定
- 动态内存分配管理对象生命周期
- 展示了C语言中面向对象编程的基本思想

## 编译运行

```bash
gcc LogicGate.c -o logicgate
./logicgate

gcc LogicGate_easy.c -o logicgate_easy  
./logicgate_easy
```
