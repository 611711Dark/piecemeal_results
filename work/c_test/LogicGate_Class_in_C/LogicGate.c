#include <stdio.h>
#include <stdlib.h>

// 前置声明结构体
typedef struct LogicGate LogicGate;

// 定义逻辑门结构体（类）
struct LogicGate {
    // 成员变量
    int input1;
    int input2;
    
    // 方法指针
    int (*evaluate)(LogicGate* self);
    void (*set_inputs)(LogicGate* self, int a, int b);
};

/*----------- 方法实现 -----------*/
// 逻辑与运算
int and_evaluate(LogicGate* self) {
    return self->input1 && self->input2;
}

// 逻辑或运算
int or_evaluate(LogicGate* self) {
    return self->input1 || self->input2;
}

// 设置输入参数的通用方法
void set_inputs(LogicGate* self, int a, int b) {
    self->input1 = a;
    self->input2 = b;
}

/*----------- 构造函数 -----------*/
// 创建AND逻辑门
LogicGate* create_and_gate() {
    LogicGate* gate = malloc(sizeof(LogicGate));
    gate->evaluate = and_evaluate;
    gate->set_inputs = set_inputs;
    return gate;
}

// 创建OR逻辑门
LogicGate* create_or_gate() {
    LogicGate* gate = malloc(sizeof(LogicGate));
    gate->evaluate = or_evaluate;
    gate->set_inputs = set_inputs;
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

    // 设置输入参数
    and_gate->set_inputs(and_gate, 1, 0);
    or_gate->set_inputs(or_gate, 1, 0);

    // 执行运算并输出结果
    printf("AND Gate(1,0): %d\n", and_gate->evaluate(and_gate));  // 输出0
    printf("OR Gate(1,0): %d\n", or_gate->evaluate(or_gate));    // 输出1

    // 清理内存
    destroy_gate(and_gate);
    destroy_gate(or_gate);

    return 0;
}
