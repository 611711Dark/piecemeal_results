#include "pubsub.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

Publisher* create_publisher(const char* name) {
    Publisher* publisher = (Publisher*)malloc(sizeof(Publisher));
    strncpy(publisher->name, name, sizeof(publisher->name) - 1);
    publisher->subscriber_count = 0;
    memset(publisher->subscribers, 0, sizeof(publisher->subscribers));
    return publisher;
}

void destroy_publisher(Publisher* publisher) {
    if (!publisher) return;

    // 释放所有订阅者
    for (int i = 0; i < publisher->subscriber_count; i++) {
        free(publisher->subscribers[i]);
    }
    free(publisher);
}

bool subscribe(Publisher* publisher,
               const char* subscriber_name,
               SubscriberCallback callback,
               void* user_data) {
    if (!publisher || !subscriber_name || !callback) return false;

    // 检查是否已达到最大订阅者数量
    if (publisher->subscriber_count >= 16) {
        printf("订阅者数量已达上限\n");
        return false;
    }

    // 检查是否已订阅
    for (int i = 0; i < publisher->subscriber_count; i++) {
        if (strcmp(publisher->subscribers[i]->name, subscriber_name) == 0) {
            printf("订阅者 %s 已存在\n", subscriber_name);
            return false;
        }
    }

    // 创建新订阅者
    Subscriber* subscriber = (Subscriber*)malloc(sizeof(Subscriber));
    strncpy(subscriber->name, subscriber_name, sizeof(subscriber->name) - 1);
    subscriber->callback = callback;
    subscriber->user_data = user_data;

    // 添加到订阅者列表
    publisher->subscribers[publisher->subscriber_count++] = subscriber;
    printf("订阅者 %s 已添加\n", subscriber_name);
    return true;
}

bool unsubscribe(Publisher* publisher, const char* subscriber_name) {
    if (!publisher || !subscriber_name) return false;

    for (int i = 0; i < publisher->subscriber_count; i++) {
        if (strcmp(publisher->subscribers[i]->name, subscriber_name) == 0) {
            // 释放订阅者
            free(publisher->subscribers[i]);
            // 移动后续订阅者
            for (int j = i; j < publisher->subscriber_count - 1; j++) {
                publisher->subscribers[j] = publisher->subscribers[j + 1];
            }
            publisher->subscriber_count--;
            printf("订阅者 %s 已移除\n", subscriber_name);
            return true;
        }
    }

    printf("未找到订阅者 %s\n", subscriber_name);
    return false;
}

void publish(Publisher* publisher, const Topic* topic) {
    if (!publisher || !topic) return;

    printf("\n发布者 %s 发布主题 %s\n", publisher->name, topic->name);

    // 通知所有订阅者
    for (int i = 0; i < publisher->subscriber_count; i++) {
        Subscriber* subscriber = publisher->subscribers[i];
        printf("通知订阅者 %s\n", subscriber->name);
        subscriber->callback(topic, subscriber->user_data);
    }
}
