#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 全局计数器，用于测试副作用
static int global_counter = 0;
static int param_counter = 0;
static int method_call_counter = 0;

/**
 * 增强的计算器结构体
 */
typedef struct {
    int value;
    char name[50];
    void (*display)(void *self);
    int (*add)(void *self, int x);
    int (*multiply)(void *self, int x);
    void (*set_name)(void *self, const char *name);
    void (*destroy)(void *self);
} Calculator;

void display_impl(void *self) {
    Calculator *this = (Calculator *)self;
    printf("Calculator '%s': %d\n", this->name, this->value);
}

int add_impl(void *self, int x) {
    Calculator *this = (Calculator *)self;
    this->value += x;
    printf("Added %d to '%s', new value: %d\n", x, this->name, this->value);
    return this->value;
}

int multiply_impl(void *self, int x) {
    Calculator *this = (Calculator *)self;
    this->value *= x;
    printf("Multiplied '%s' by %d, new value: %d\n", this->name, x, this->value);
    return this->value;
}

void set_name_impl(void *self, const char *name) {
    Calculator *this = (Calculator *)self;
    strncpy(this->name, name, sizeof(this->name) - 1);
    this->name[sizeof(this->name) - 1] = '\0';
    printf("Renamed calculator to: %s\n", this->name);
}

void destroy_impl(void *self) {
    Calculator *this = (Calculator *)self;
    printf("Destroying calculator: %s\n", this->name);
    free(this);
}

Calculator *calculator_create(int initial_value, const char *name) {
    Calculator *calc = (Calculator *)malloc(sizeof(Calculator));
    if (!calc) return NULL;
    
    calc->value = initial_value;
    strncpy(calc->name, name, sizeof(calc->name) - 1);
    calc->name[sizeof(calc->name) - 1] = '\0';
    calc->display = display_impl;
    calc->add = add_impl;
    calc->multiply = multiply_impl;
    calc->set_name = set_name_impl;
    calc->destroy = destroy_impl;
    
    printf("Created calculator: %s (value: %d)\n", calc->name, calc->value);
    return calc;
}

/**
 * 有副作用的函数 - 每次调用都会增加计数器并打印
 */
Calculator *get_calculator_with_side_effect() {
    static Calculator calc = {0, "SideEffect", display_impl, add_impl, multiply_impl, set_name_impl, destroy_impl};
    global_counter++;
    printf("get_calculator_with_side_effect() called! Global counter: %d\n", global_counter);
    calc.value = global_counter * 10;
    return &calc;
}

int get_value_with_side_effect() {
    param_counter++;
    printf("get_value_with_side_effect() called! Param counter: %d\n", param_counter);
    return param_counter * 5;
}

char *get_string_with_side_effect() {
    static char buffer[50];
    param_counter++;
    snprintf(buffer, sizeof(buffer), "String%d", param_counter);
    printf("get_string_with_side_effect() called! Returning: %s\n", buffer);
    return buffer;
}

/*****************************************************************
 * 6种不同的宏定义方法
 *****************************************************************/

/**
 * 方法1: 基础宏 - 直接替换（最不安全）
 */
#define METHOD_CALL_BASIC(obj, method, ...) (obj)->method(obj, ##__VA_ARGS__)

/**
 * 方法2: do-while包装 - 语句安全但仍可能多次求值
 */
#define METHOD_CALL_DOWHILE(obj, method, ...) do { \
    (obj)->method(obj, ##__VA_ARGS__); \
} while(0)

/**
 * 方法3: 临时变量技巧 - 相对安全（纯C）
 */
#define METHOD_CALL_TEMP(obj, method, ...) \
    do { \
        Calculator *_obj = (obj); \
        _obj->method(_obj, ##__VA_ARGS__); \
    } while(0)

/**
 * 方法4: GCC语句表达式 - 最安全（GCC/Clang扩展）
 */
#if defined(__GNUC__) || defined(__clang__)
#define METHOD_CALL_GCC(obj, method, ...) ({ \
    Calculator *_obj = (obj); \
    _obj->method(_obj, ##__VA_ARGS__); \
})
#else
#define METHOD_CALL_GCC(obj, method, ...) (obj)->method(obj, ##__VA_ARGS__)
#endif

/**
 * 方法5: C11泛型 - 类型安全
 */
#if __STDC_VERSION__ >= 201112L
#define METHOD_CALL_GENERIC(obj, method, ...) \
    _Generic((obj), \
        Calculator*: ({ \
            Calculator *_obj = (obj); \
            _obj->method(_obj, ##__VA_ARGS__); \
        }) \
    )
#else
#define METHOD_CALL_GENERIC(obj, method, ...) \
    do { \
        Calculator *_obj = (obj); \
        _obj->method(_obj, ##__VA_ARGS__); \
    } while(0)
#endif

/**
 * 方法6: 最安全的纯C方案
 */
#define METHOD_CALL_SAFEST(obj, method, ...) \
    do { \
        Calculator *_obj = (obj); \
        (*(_obj)).method(_obj, ##__VA_ARGS__); \
    } while(0)

/*****************************************************************
 * 增强的安全宏（带NULL检查）
 *****************************************************************/

/**
 * 带NULL检查的安全宏
 */
#define METHOD_CALL_SAFE(obj, method, ...) \
    do { \
        if ((obj) != NULL) { \
            Calculator *_obj = (obj); \
            _obj->method(_obj, ##__VA_ARGS__); \
        } else { \
            printf("Warning: Attempted to call method on NULL object\n"); \
        } \
    } while(0)

/*****************************************************************
 * 测试函数
 *****************************************************************/

void test_macro_safety() {
    printf("=== 测试1: 宏安全性（对象参数求值次数）===\n");
    printf("初始全局计数器: %d\n\n", global_counter);
    
    #define TEST_MACRO_SAFETY(macro_name) \
        do { \
            printf(#macro_name ":\n"); \
            global_counter = 0; \
            macro_name(get_calculator_with_side_effect(), display); \
            printf("最终全局计数器: %d\n\n", global_counter); \
        } while(0)
    
    TEST_MACRO_SAFETY(METHOD_CALL_BASIC);
    TEST_MACRO_SAFETY(METHOD_CALL_DOWHILE);
    TEST_MACRO_SAFETY(METHOD_CALL_TEMP);
    TEST_MACRO_SAFETY(METHOD_CALL_GCC);
    TEST_MACRO_SAFETY(METHOD_CALL_GENERIC);
    TEST_MACRO_SAFETY(METHOD_CALL_SAFEST);
}

void test_parameter_safety() {
    printf("=== 测试2: 参数安全性（函数参数求值次数）===\n");
    
    Calculator *calc = calculator_create(50, "ParamTest");
    
    printf("初始值: ");
    calc->display(calc);
    
    #define TEST_PARAM_SAFETY(macro_name) \
        do { \
            printf("\n" #macro_name " 参数测试:\n"); \
            param_counter = 0; \
            macro_name(calc, add, get_value_with_side_effect()); \
            printf("最终参数计数器: %d\n", param_counter); \
        } while(0)
    
    TEST_PARAM_SAFETY(METHOD_CALL_BASIC);
    TEST_PARAM_SAFETY(METHOD_CALL_DOWHILE);
    TEST_PARAM_SAFETY(METHOD_CALL_TEMP);
    TEST_PARAM_SAFETY(METHOD_CALL_GCC);
    TEST_PARAM_SAFETY(METHOD_CALL_GENERIC);
    TEST_PARAM_SAFETY(METHOD_CALL_SAFEST);
    
    calc->destroy(calc);
}

void test_return_values() {
    printf("\n=== 测试3: 返回值测试 ===\n");
    
    Calculator *calc = calculator_create(10, "ReturnTest");
    
    #define TEST_RETURN_VALUE(macro_name) \
        do { \
            printf("\n" #macro_name " 返回值测试:\n"); \
            int result = macro_name(calc, add, 5); \
            printf("返回值: %d\n", result); \
        } while(0)
    
    // 注意：只有GCC语句表达式和C11泛型可以直接返回值
    #if defined(__GNUC__) || defined(__clang__)
    TEST_RETURN_VALUE(METHOD_CALL_GCC);
    #endif
    
    #if __STDC_VERSION__ >= 201112L
    TEST_RETURN_VALUE(METHOD_CALL_GENERIC);
    #endif
    
    // 对于其他宏，我们需要使用不同的方式获取返回值
    printf("\nMETHOD_CALL_TEMP 返回值测试:\n");
    int result;
    METHOD_CALL_TEMP(calc, add, 5) /* 没有返回值 */;
    // 手动获取结果
    result = calc->value;
    printf("计算结果: %d\n", result);
    
    calc->destroy(calc);
}

void test_multiple_parameters() {
    printf("\n=== 测试4: 多参数测试 ===\n");
    
    Calculator *calc = calculator_create(100, "MultiParamTest");
    
    #define TEST_MULTIPLE_PARAMS(macro_name) \
        do { \
            printf("\n" #macro_name " 多参数测试:\n"); \
            param_counter = 0; \
            macro_name(calc, set_name, get_string_with_side_effect()); \
            macro_name(calc, display); \
        } while(0)
    
    TEST_MULTIPLE_PARAMS(METHOD_CALL_BASIC);
    TEST_MULTIPLE_PARAMS(METHOD_CALL_DOWHILE);
    TEST_MULTIPLE_PARAMS(METHOD_CALL_TEMP);
    TEST_MULTIPLE_PARAMS(METHOD_CALL_GCC);
    TEST_MULTIPLE_PARAMS(METHOD_CALL_GENERIC);
    TEST_MULTIPLE_PARAMS(METHOD_CALL_SAFEST);
    
    calc->destroy(calc);
}

void test_complex_operations() {
    printf("\n=== 测试5: 复杂操作链测试 ===\n");
    
    // 为每个测试创建独立的计算器
    #define TEST_COMPLEX_OPERATIONS(macro_name, id) \
        do { \
            Calculator *calc = calculator_create(2, "ComplexTest" #id); \
            printf("\n" #macro_name " 复杂操作链:\n"); \
            macro_name(calc, add, 3); \
            macro_name(calc, multiply, 4); \
            macro_name(calc, add, 10); \
            macro_name(calc, display); \
            calc->destroy(calc); \
        } while(0)
    
    TEST_COMPLEX_OPERATIONS(METHOD_CALL_BASIC, 1);
    TEST_COMPLEX_OPERATIONS(METHOD_CALL_DOWHILE, 2);
    TEST_COMPLEX_OPERATIONS(METHOD_CALL_TEMP, 3);
    TEST_COMPLEX_OPERATIONS(METHOD_CALL_GCC, 4);
    TEST_COMPLEX_OPERATIONS(METHOD_CALL_GENERIC, 5);
    TEST_COMPLEX_OPERATIONS(METHOD_CALL_SAFEST, 6);
}

void test_edge_cases() {
    printf("\n=== 测试6: 边界情况测试 ===\n");
    
    // 测试空参数
    Calculator *calc = calculator_create(42, "EdgeCaseTest");
    
    #define TEST_NO_PARAMS(macro_name) \
        do { \
            printf("\n" #macro_name " 无参数测试:\n"); \
            macro_name(calc, display); \
        } while(0)
    
    TEST_NO_PARAMS(METHOD_CALL_BASIC);
    TEST_NO_PARAMS(METHOD_CALL_DOWHILE);
    TEST_NO_PARAMS(METHOD_CALL_TEMP);
    TEST_NO_PARAMS(METHOD_CALL_GCC);
    TEST_NO_PARAMS(METHOD_CALL_GENERIC);
    TEST_NO_PARAMS(METHOD_CALL_SAFEST);
    
    // 测试NULL对象 - 使用带NULL检查的安全宏
    printf("\n测试NULL对象 (使用带NULL检查的宏):\n");
    Calculator *null_calc = NULL;
    
    METHOD_CALL_SAFE(null_calc, display);
    
    calc->destroy(calc);
}

void test_performance() {
    printf("\n=== 测试7: 性能测试（方法调用次数）===\n");
    
    const int iterations = 100000;
    
    #define TEST_PERFORMANCE(macro_name, id) \
        do { \
            Calculator *calc = calculator_create(0, "PerfTest" #id); \
            printf("\n" #macro_name " 性能测试 (%d 次迭代):\n", iterations); \
            method_call_counter = 0; \
            for (int i = 0; i < iterations; i++) { \
                macro_name(calc, add, 1); \
            } \
            printf("最终值: %d, 方法调用次数: %d\n", calc->value, method_call_counter); \
            calc->destroy(calc); \
        } while(0)
    
    // 修改add实现以计数
    int add_count_impl(void *self, int x) {
        Calculator *this = (Calculator *)self;
        this->value += x;
        method_call_counter++;
        return this->value;
    }
    
    // 为每个测试创建独立的计算器并设置计数方法
    #define TEST_WITH_COUNT(macro_name, id) \
        do { \
            Calculator *calc = calculator_create(0, "PerfTest" #id); \
            calc->add = add_count_impl; \
            printf("\n" #macro_name " 性能测试 (%d 次迭代):\n", iterations); \
            method_call_counter = 0; \
            for (int i = 0; i < iterations; i++) { \
                macro_name(calc, add, 1); \
            } \
            printf("最终值: %d, 方法调用次数: %d\n", calc->value, method_call_counter); \
            calc->destroy(calc); \
        } while(0)
    
    TEST_WITH_COUNT(METHOD_CALL_BASIC, 1);
    TEST_WITH_COUNT(METHOD_CALL_DOWHILE, 2);
    TEST_WITH_COUNT(METHOD_CALL_TEMP, 3);
    TEST_WITH_COUNT(METHOD_CALL_GCC, 4);
    TEST_WITH_COUNT(METHOD_CALL_GENERIC, 5);
    TEST_WITH_COUNT(METHOD_CALL_SAFEST, 6);
}

void test_real_world_scenario() {
    printf("\n=== 测试8: 真实场景模拟 ===\n");
    
    // 模拟一个真实的使用场景
    Calculator *calc1 = calculator_create(100, "Calculator1");
    Calculator *calc2 = calculator_create(200, "Calculator2");
    
    printf("\n模拟计算场景:\n");
    
    // 使用最安全的宏进行复杂操作
    METHOD_CALL_TEMP(calc1, add, 50);
    METHOD_CALL_TEMP(calc2, multiply, 2);
    METHOD_CALL_TEMP(calc1, add, calc2->value);
    METHOD_CALL_TEMP(calc1, display);
    METHOD_CALL_TEMP(calc2, display);
    
    // 使用带返回值的宏（如果可用）
    #if defined(__GNUC__) || defined(__clang__)
    int final_result = METHOD_CALL_GCC(calc1, multiply, 2);
    printf("最终结果: %d\n", final_result);
    #endif
    
    calc1->destroy(calc1);
    calc2->destroy(calc2);
}

/*****************************************************************
 * 主函数
 *****************************************************************/

int main() {
    printf("全面的C语言宏安全调用测试套件\n");
    printf("===============================\n\n");
    
    // 运行所有测试
    test_macro_safety();
    test_parameter_safety();
    test_return_values();
    test_multiple_parameters();
    test_complex_operations();
    test_edge_cases();
    test_performance();
    test_real_world_scenario();
    
    printf("\n=== 最终总结 ===\n");
    printf("✓ 安全的方法:\n");
    printf("  - 临时变量技巧 (METHOD_CALL_TEMP)\n");
    printf("  - GCC语句表达式 (METHOD_CALL_GCC)\n");
    printf("  - C11泛型 (METHOD_CALL_GENERIC)\n");
    printf("  - 最安全的纯C方案 (METHOD_CALL_SAFEST)\n");
    printf("\n✗ 不安全的方法:\n");
    printf("  - 基础宏 (METHOD_CALL_BASIC)\n");
    printf("  - do-while包装 (METHOD_CALL_DOWHILE)\n");
    printf("\n💡 推荐方案:\n");
    printf("  - 可移植代码: 使用临时变量技巧或最安全的纯C方案\n");
    printf("  - GCC/Clang项目: 使用GCC语句表达式（支持返回值）\n");
    printf("  - C11项目: 使用C11泛型（类型安全）\n");
    printf("  - 绝对避免: 基础宏和do-while包装\n");
    printf("\n🛡️  增强安全: 可以使用带NULL检查的宏 (METHOD_CALL_SAFE)\n");
    printf("\n📊 测试覆盖:\n");
    printf("  - 对象参数安全性 ✓\n");
    printf("  - 函数参数安全性 ✓\n");
    printf("  - 返回值支持 ✓\n");
    printf("  - 多参数支持 ✓\n");
    printf("  - 复杂操作链 ✓\n");
    printf("  - 边界情况 ✓\n");
    printf("  - 性能测试 ✓\n");
    printf("  - 真实场景模拟 ✓\n");
    
    return 0;
}
