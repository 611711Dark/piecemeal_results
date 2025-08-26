# GENANN神经网络库测试项目

## 项目简介

本项目使用GENANN库进行人工神经网络的学习和测试。GENANN是一个轻量级的C语言神经网络库，适合学习神经网络的基本概念和实现简单的机器学习任务。

## GENANN库介绍

GENANN (Minimal C Artificial Neural Network) 是由Lewis Van Winkle开发的开源神经网络库，具有以下特点：

- **轻量级**: 代码简洁，易于理解和集成
- **纯C实现**: 无外部依赖，可移植性强
- **支持反向传播**: 实现标准的BP算法
- **灵活配置**: 支持任意层数和神经元数量

## 项目文件

### 核心库文件
- `genann.h`: 库的头文件，定义API接口
- `genann.c`: 库的实现文件
- `libgenann.a`: 编译好的静态库文件

### 示例程序
- `example1.c`: XOR函数学习示例
- `example2.c`: 更复杂的学习任务
- `example3.c`: 神经网络保存和加载
- `example4.c`: 高级应用示例

### 测试文件
- `test.c`: 单元测试
- `minctest.h`: 轻量级测试框架
- `persist.txt`: 保存的神经网络数据

## 主要功能

1. **网络创建**: 创建指定结构的神经网络
2. **训练学习**: 使用反向传播算法训练网络
3. **预测推理**: 使用训练好的网络进行预测
4. **模型保存**: 将训练好的模型保存到文件
5. **模型加载**: 从文件加载预训练模型

## 编译运行

```bash
# 编译示例1 (XOR学习)
gcc example1.c genann.c -o example1 -lm
./example1

# 编译其他示例
gcc example2.c genann.c -o example2 -lm
gcc example3.c genann.c -o example3 -lm
gcc example4.c genann.c -o example4 -lm

# 运行测试
gcc test.c genann.c -o test -lm
./test
```

## 学习价值

- 理解神经网络的基本结构
- 学习反向传播算法的实现
- 掌握C语言机器学习库的使用
- 了解神经网络的训练和预测过程

## 应用场景

- 模式识别
- 函数逼近
- 分类问题
- 简单的机器学习任务
