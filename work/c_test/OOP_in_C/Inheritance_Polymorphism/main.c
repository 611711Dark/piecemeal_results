#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ======== 基类 Animal ======== */
typedef struct Animal {
    char name[20];

    // 函数指针，模拟虚函数
    void (*speak)(struct Animal *self);
} Animal;

// Animal 构造函数
void Animal_init(Animal *animal, const char *name) {
    strncpy(animal->name, name, sizeof(animal->name) - 1);
    animal->name[sizeof(animal->name) - 1] = '\0';
    animal->speak = NULL; // 默认无实现
}

/* ======== 子类 Dog ======== */
typedef struct Dog {
    Animal base; // 继承自 Animal
    int age;
} Dog;

// Dog 的 speak 实现
void Dog_speak(Animal *self) {
    Dog *dog = (Dog *)self; // 向下转型
    printf("%s: Woof! I'm %d years old.\n", dog->base.name, dog->age);
}

// Dog 构造函数
void Dog_init(Dog *dog, const char *name, int age) {
    Animal_init(&dog->base, name);
    dog->age = age;
    dog->base.speak = Dog_speak; // 绑定虚函数
}

/* ======== 子类 Cat ======== */
typedef struct Cat {
    Animal base;
    char color[20];
} Cat;

// Cat 的 speak 实现
void Cat_speak(Animal *self) {
    Cat *cat = (Cat *)self;
    printf("%s: Meow! My color is %s.\n", cat->base.name, cat->color);
}

// Cat 构造函数
void Cat_init(Cat *cat, const char *name, const char *color) {
    Animal_init(&cat->base, name);
    strncpy(cat->color, color, sizeof(cat->color) - 1);
    cat->color[sizeof(cat->color) - 1] = '\0';
    cat->base.speak = Cat_speak;
}

/* ======== 主函数，演示多态 ======== */
int main() {
    Dog d;
    Cat c;

    Dog_init(&d, "Buddy", 3);
    Cat_init(&c, "Kitty", "white");

    // 基类指针数组，存放不同子类
    Animal *animals[2];
    animals[0] = (Animal *)&d;
    animals[1] = (Animal *)&c;

    // 多态调用
    for (int i = 0; i < 2; i++) {
        animals[i]->speak(animals[i]);
    }

    return 0;
}

