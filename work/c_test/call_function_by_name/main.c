#include <stdio.h>
#include <string.h>

// 定义一些示例函数
void func1(void) {
    printf("Calling func1\n");
}

void func2(void) {
    printf("Calling func2\n");
}

void func3(void) {
    printf("Calling func3\n");
}

// 定义函数指针类型
typedef void (*func_ptr)();

// 定义一个结构体来存储函数名和函数指针
struct FunctionMap {
    const char* name;
    func_ptr function;
};

// 创建一个函数映射表
struct FunctionMap function_map[] = {
    {"func1", func1},
    {"func2", func2},
    {"func3", func3},
    {NULL, NULL} // 哨兵值，用于标记数组结束
};

// 通过字符串查找并调用函数
void call_function_by_name(const char* name) {
    for (int i = 0; function_map[i].name != NULL; i++) {
        if (strcmp(function_map[i].name, name) == 0) {
            function_map[i].function(); // 调用找到的函数
            return;
        }
    }
    printf("Error: Function '%s' not found.\n", name);
}

int main() {
    call_function_by_name("func1");
    call_function_by_name("func3");
    call_function_by_name("func_non_existent");

    return 0;
}
