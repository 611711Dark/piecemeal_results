#include "pubsub.h"
#include "time.h"
#include <stdio.h>
// 温度传感器订阅者回调
void temperature_callback(const Topic *topic, void *user_data) {
  float *threshold = (float *)user_data;
  float temperature = *(float *)topic->data;

  printf("温度传感器收到数据: %.1f°C\n", temperature);
  if (temperature > *threshold) {
    printf("警告：温度超过阈值 %.1f°C!\n", *threshold);
  }
}

// 日志记录订阅者回调
void logger_callback(const Topic *topic, void *user_data) {
  printf("日志记录器：主题 %s, 数据大小 %d, 时间戳 %d\n", topic->name,
         topic->data_size, topic->timestamp);
}

int main() {
  // 创建发布者
  Publisher *sensor_publisher = create_publisher("传感器发布者");

  // 创建温度阈值
  float temp_threshold = 30.0f;

  // 订阅主题
  subscribe(sensor_publisher, "温度监控器", temperature_callback,
            &temp_threshold);
  subscribe(sensor_publisher, "系统日志", logger_callback, NULL);

  // 创建并发布温度数据
  float temp_data[] = {25.5f, 28.3f, 32.7f};
  for (int i = 0; i < 3; i++) {
    Topic topic = {.name = "temperature",
                   .data = &temp_data[i],
                   .data_size = sizeof(float),
                   .timestamp = (uint32_t)time(NULL)};
    publish(sensor_publisher, &topic);
  }

  // 取消订阅
  unsubscribe(sensor_publisher, "系统日志");

  // 再次发布数据
  float final_temp = 35.2f;
  Topic topic = {.name = "temperature",
                 .data = &final_temp,
                 .data_size = sizeof(float),
                 .timestamp = (uint32_t)time(NULL)};
  publish(sensor_publisher, &topic);

  // 清理资源
  destroy_publisher(sensor_publisher);

  return 0;
}
