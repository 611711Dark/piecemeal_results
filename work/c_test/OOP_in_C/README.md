# C语言面向对象编程项目

## 项目简介

本项目展示了如何在C语言中实现面向对象编程的核心概念，包括继承、多态和虚函数表等高级特性。

## 项目结构

### Inheritance_Polymorphism/
演示基础的继承和多态实现：
- 定义了Animal基类，包含虚函数指针
- 实现了Dog和Cat子类，重写虚函数
- 展示了向上转型和多态调用
- 使用函数指针模拟虚函数机制

### Inheritance_Polymorphism_vtable/
演示更高级的虚函数表实现：
- 实现了完整的虚函数表(vtable)机制
- 支持多层继承：Animal -> Mammal -> Dog/Cat
- 每个类都有独立的虚函数表
- 更接近C++的底层实现原理

## 技术特点

- **继承**: 通过结构体嵌套实现类继承关系
- **多态**: 使用函数指针实现运行时方法绑定
- **虚函数表**: 模拟C++的vtable机制
- **向上转型**: 安全的类型转换
- **方法重写**: 子类重新定义父类方法

## 编译运行

```bash
# 基础继承多态示例
cd Inheritance_Polymorphism
gcc main.c -o inheritance_demo
./inheritance_demo

# 虚函数表示例  
cd Inheritance_Polymorphism_vtable
gcc main.c -o vtable_demo
./vtable_demo
```

## 学习价值

这个项目帮助理解：
- C++对象模型的底层实现
- 虚函数表的工作原理
- 如何在C语言中实现OOP设计模式
