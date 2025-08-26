#include <stdio.h>
#include <stdlib.h>

// 类型前置声明
typedef struct LogicGate LogicGate;

// 定义逻辑门接口
struct LogicGate {
    // 运算方法（现在直接接受输入参数）
    int (*evaluate)(int a, int b);
};

/*----------- 具体运算实现 -----------*/
static int and_impl(int a, int b) { return a && b; }
static int or_impl(int a, int b)  { return a || b; }
static int xor_impl(int a, int b) { return a ^ b; }

/*----------- 构造函数 -----------*/
LogicGate* create_and_gate() {
    LogicGate* gate = malloc(sizeof(LogicGate));
    gate->evaluate = and_impl;
    return gate;
}

LogicGate* create_or_gate() {
    LogicGate* gate = malloc(sizeof(LogicGate));
    gate->evaluate = or_impl;
    return gate;
}

LogicGate* create_xor_gate() {
    LogicGate* gate = malloc(sizeof(LogicGate));
    gate->evaluate = xor_impl;
    return gate;
}

/*----------- 析构函数 -----------*/
void destroy_gate(LogicGate* gate) {
    free(gate);
}

int main() {
    // 创建逻辑门实例
    LogicGate* and_gate = create_and_gate();
    LogicGate* or_gate = create_or_gate();
    LogicGate* xor_gate = create_xor_gate();

    // 直接运算并输出结果
    printf("AND(1,0)  => %d\n", and_gate->evaluate(1, 0));  // 0
    printf("OR(1,0)   => %d\n", or_gate->evaluate(1, 0));   // 1
    printf("XOR(1,1)  => %d\n", xor_gate->evaluate(1, 1));  // 0

    // 内存清理
    destroy_gate(and_gate);
    destroy_gate(or_gate);
    destroy_gate(xor_gate);

    return 0;
}
