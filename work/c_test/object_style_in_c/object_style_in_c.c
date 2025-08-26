/**
 * object_style_in_c.c
 * 
 * 演示如何使用C语言的结构体、函数指针和宏来模拟面向对象编程风格
 * 特别是模拟Python中对象调用方法的风格：obj.method(arg)
 */

#include <stdio.h>
#include <stdlib.h>

/******************************* 第一部分：基础结构体和函数定义 *******************************/

/**
 * 定义一个简单的"对象"类型 - 计算器
 * 包含数据成员和一个函数指针成员
 */
typedef struct {
    int value;  // 数据成员，存储当前值
    
    // 方法成员 - 函数指针
    int (*add)(void *self, int x);          // 加法
    int (*multiply)(void *self, int x);     // 乘法
    void (*display)(void *self);            // 显示当前值
    void (*destroy)(void *self);            // 清理资源
} Calculator;

/**
 * add方法的实现
 * 注意：第一个参数是void*，在实际使用中需要转换为具体类型
 */
int add_impl(void *self, int x) {
    Calculator *this = (Calculator *)self;  // 将void*转换为Calculator*
    this->value += x;
    return this->value;
}

/**
 * multiply方法的实现
 */
int multiply_impl(void *self, int x) {
    Calculator *this = (Calculator *)self;
    this->value *= x;
    return this->value;
}

/**
 * display方法的实现
 */
void display_impl(void *self) {
    Calculator *this = (Calculator *)self;
    printf("Current value: %d\n", this->value);
}

/**
 * destroy方法的实现
 * 在这个简单示例中，我们只是释放对象内存
 */
void destroy_impl(void *self) {
    free(self);
}

/******************************* 第二部分：创建对象的"构造函数" *******************************/

/**
 * 创建Calculator对象的"构造函数"
 * 分配内存并初始化函数指针
 */
Calculator *calculator_create(int initial_value) {
    Calculator *obj = (Calculator *)malloc(sizeof(Calculator));
    if (!obj) return NULL;
    
    obj->value = initial_value;
    obj->add = add_impl;
    obj->multiply = multiply_impl;
    obj->display = display_impl;
    obj->destroy = destroy_impl;
    
    return obj;
}

/******************************* 第三部分：使用宏模拟Python风格调用 *******************************/

/**
 * 方法1: 为每个方法单独定义调用宏
 * 优点: 调用时最简洁，最接近Python风格
 * 缺点: 需要为每个方法定义一个宏
 */
#define CALL_ADD(obj, x) (obj)->add(obj, x)
#define CALL_MULTIPLY(obj, x) (obj)->multiply(obj, x)
#define CALL_DISPLAY(obj) (obj)->display(obj)
#define CALL_DESTROY(obj) (obj)->destroy(obj)

/**
 * 方法2: 通用方法调用宏
 * 优点: 一个宏适用于所有方法
 * 缺点: 调用时需要指定方法名作为字符串
 */
#define METHOD_CALL(obj, method, ...) (obj)->method(obj, ##__VA_ARGS__)

/**
 * 方法3: 使用GCC的语句表达式扩展，创建更安全的宏
 * 这个宏可以确保参数只被计算一次，避免副作用问题
 * 注意: 这是GCC扩展，不是标准C
 */
#if defined(__GNUC__)
#define SAFE_METHOD_CALL(obj, method, ...) ({ \
    typeof(obj) _obj = (obj); \
    _obj->method(_obj, ##__VA_ARGS__); \
})
#else
// 非GCC编译器的回退方案
#define SAFE_METHOD_CALL(obj, method, ...) (obj)->method(obj, ##__VA_ARGS__)
#endif

/******************************* 第四部分：演示代码 *******************************/

int main() {
    printf("=== C语言模拟面向对象编程风格示例 ===\n\n");
    
    // 创建Calculator对象
    Calculator *calc = calculator_create(10);
    if (!calc) {
        fprintf(stderr, "Failed to create calculator object\n");
        return 1;
    }
    
    printf("1. 传统C风格调用:\n");
    calc->display(calc);                    // 显示初始值
    calc->add(calc, 5);                     // 加5
    calc->display(calc);                    // 显示结果
    calc->multiply(calc, 2);                // 乘2
    calc->display(calc);                    // 显示结果
    
    printf("\n2. 使用单独的方法调用宏 (类似Python风格):\n");
    CALL_DISPLAY(calc);                     // 显示当前值
    CALL_ADD(calc, 10);                     // 加10
    CALL_DISPLAY(calc);                     // 显示结果
    CALL_MULTIPLY(calc, 3);                 // 乘3
    CALL_DISPLAY(calc);                     // 显示结果
    
    printf("\n3. 使用通用方法调用宏:\n");
    METHOD_CALL(calc, display);             // 显示当前值
    METHOD_CALL(calc, add, 100);            // 加100
    METHOD_CALL(calc, display);             // 显示结果
    
    printf("\n4. 使用安全的方法调用宏 (GCC扩展):\n");
    SAFE_METHOD_CALL(calc, display);        // 显示当前值
    SAFE_METHOD_CALL(calc, add, 50);        // 加50
    SAFE_METHOD_CALL(calc, display);        // 显示结果
    
    // 清理对象
    printf("\n5. 销毁对象:\n");
    CALL_DESTROY(calc);
    printf("Object destroyed.\n");
    
    return 0;
}

/******************************* 第五部分：编译和运行说明 *******************************/

/**
 * 编译指令:
 *   gcc -o object_style object_style_in_c.c
 * 
 * 如果使用安全宏(SAFE_METHOD_CALL)，需要GCC编译器
 * 对于其他编译器，可以注释掉GCC特定部分或使用回退方案
 * 
 * 运行:
 *   ./object_style
 * 
 * 预期输出:
 *   演示了四种不同的方法调用风格，展示如何从传统C风格过渡到更接近Python的风格
 */
