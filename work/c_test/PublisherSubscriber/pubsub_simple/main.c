#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SUBSCRIBERS 10
#define MSG_SIZE 128

typedef void (*SubscriberCallback)(const char *msg);

typedef struct {
    SubscriberCallback subscribers[MAX_SUBSCRIBERS];
    int count;
} MessageBroker;

// 初始化
void initBroker(MessageBroker *broker) {
    broker->count = 0;
}

// 订阅
void subscribe(MessageBroker *broker, SubscriberCallback cb) {
    if (broker->count < MAX_SUBSCRIBERS) {
        broker->subscribers[broker->count++] = cb;
    }
}

// 发布消息
void publish(MessageBroker *broker, const char *msg) {
    for (int i = 0; i < broker->count; i++) {
        broker->subscribers[i](msg);
    }
}

// 模拟订阅者
void subscriberA(const char *msg) {
    printf("[Subscriber A] 收到消息: %s\n", msg);
}
void subscriberB(const char *msg) {
    printf("[Subscriber B] 收到消息: %s\n", msg);
}

int main() {
    MessageBroker broker;
    initBroker(&broker);

    // 订阅者注册
    subscribe(&broker, subscriberA);
    subscribe(&broker, subscriberB);

    // 发布消息
    publish(&broker, "Hello, World!");
    publish(&broker, "这是一条新消息");

    return 0;
}

