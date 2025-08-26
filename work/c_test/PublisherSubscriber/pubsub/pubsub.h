#ifndef PUBSUB_H
#define PUBSUB_H

#include <stdbool.h>
#include <stdint.h>

// 主题数据结构
typedef struct {
  char name[32];      // 主题名称
  void *data;         // 主题数据
  uint32_t data_size; // 数据大小
  uint32_t timestamp; // 时间戳
} Topic;

// 订阅者回调函数类型
typedef void (*SubscriberCallback)(const Topic *topic, void *user_data);

// 订阅者结构
typedef struct {
  char name[32];               // 订阅者名称
  SubscriberCallback callback; // 回调函数
  void *user_data;             // 用户数据
} Subscriber;

// 发布者结构
typedef struct {
  char name[32];               // 发布者名称
  Subscriber *subscribers[16]; // 订阅者列表
  int subscriber_count;        // 订阅者数量
} Publisher;

// 创建发布者
Publisher *create_publisher(const char *name);

// 销毁发布者
void destroy_publisher(Publisher *publisher);

// 订阅主题
bool subscribe(Publisher *publisher, const char *subscriber_name,
               SubscriberCallback callback, void *user_data);

// 取消订阅
bool unsubscribe(Publisher *publisher, const char *subscriber_name);

// 发布主题
void publish(Publisher *publisher, const Topic *topic);

#endif // PUBSUB_H
