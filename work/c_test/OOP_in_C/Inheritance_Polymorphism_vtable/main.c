#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ======== 虚函数表基类声明 ======== */
typedef struct AnimalVTable {
  void (*speak)(void *self);
  void (*eat)(void *self);
} AnimalVTable;

/* ======== 基类 Animal ======== */
typedef struct Animal {
  AnimalVTable *vptr; // 指向虚函数表
  char name[20];
} Animal;

// 基类方法实现
void Animal_speak(void *self) {
  Animal *a = (Animal *)self;
  printf("%s: (silence)\n", a->name);
}
void Animal_eat(void *self) {
  Animal *a = (Animal *)self;
  printf("%s is eating...\n", a->name);
}

// 基类虚函数表
AnimalVTable animal_vtable = {Animal_speak, Animal_eat};

// 基类构造函数
void Animal_init(Animal *a, const char *name) {
  a->vptr = &animal_vtable;
  strncpy(a->name, name, sizeof(a->name) - 1);
  a->name[sizeof(a->name) - 1] = '\0';
}

/* ======== 中间类 Mammal（继承 Animal） ======== */
typedef struct Mammal {
  Animal base; // 继承自 Animal
  int age;
} Mammal;

// Mammal 方法实现
void Mammal_speak(void *self) {
  Mammal *m = (Mammal *)self;
  printf("%s: Mammal sound\n", m->base.name);
}

// Mammal 虚函数表（继承并修改 speak）
AnimalVTable mammal_vtable = {
    Mammal_speak,
    Animal_eat // 复用 Animal 的 eat
};

// Mammal 构造函数
void Mammal_init(Mammal *m, const char *name, int age) {
  Animal_init(&m->base, name);
  m->base.vptr = &mammal_vtable;
  m->age = age;
}

/* ======== 子类 Dog（继承 Mammal） ======== */
typedef struct Dog {
  Mammal base;
  char breed[20];
} Dog;

// Dog 方法实现
void Dog_speak(void *self) {
  Dog *d = (Dog *)self;
  printf("%s: Woof! I am a %s.\n", d->base.base.name, d->breed);
}

// Dog 虚函数表（重写 speak）
AnimalVTable dog_vtable = {
    Dog_speak,
    Animal_eat // 没改 eat
};

// Dog 构造函数
void Dog_init(Dog *d, const char *name, int age, const char *breed) {
  Mammal_init(&d->base, name, age);
  d->base.base.vptr = &dog_vtable;
  strncpy(d->breed, breed, sizeof(d->breed) - 1);
  d->breed[sizeof(d->breed) - 1] = '\0';
}

/* ======== 演示多态 ======== */
int main() {
  Animal a;
  Mammal m;
  Dog d;

  Animal_init(&a, "GenericAnimal");
  Mammal_init(&m, "MammalOne", 5);
  Dog_init(&d, "Buddy", 3, "Golden Retriever");

  // 基类指针数组
  Animal *animals[3];
  animals[0] = &a;
  animals[1] = (Animal *)&m;
  animals[2] = (Animal *)&d;

  // 多态调用
  for (int i = 0; i < 3; i++) {
    animals[i]->vptr->speak(animals[i]);
    animals[i]->vptr->eat(animals[i]);
  }

  return 0;
}
