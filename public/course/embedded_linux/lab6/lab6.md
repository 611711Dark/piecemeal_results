--- 
# 实验室6 - Linux调度器

**文件结构**  
```
lab6/
├── repeat.c    # 练习1：CPU占用测试程序
├── lab6.2.c    # 练习2：实时线程调度
└── Makefile    # 构建脚本（支持远程部署）
```

## 实验目标
1. 理解Linux的进程优先级（nice值）
2. 掌握实时调度策略（SCHED_FIFO/SCHED_RR）
3. 观察不同优先级线程的调度行为

## 操作步骤

### 练习1：进程优先级（nice值）
1. 编译程序：  
   ```bash
   make repeat
   ```
2. 运行测试：  
   ```bash
   ./repeat 0 & ./repeat 1  # 启动两个竞争进程
   ```
3. 在htop中：  
   - 按`F4`过滤进程  
   - 按`F8`调整nice值（-20~19）

### 练习2：实时线程调度
1. 编译并部署到Beaglebone：  
   ```bash
   make lab6.2 && make copy
   ```
2. 在Beaglebone上运行：  
   ```bash
   sudo ./lab6.2  # 需要root权限
   ```
3. 查看调度信息：  
   ```bash
   ps -L --format cmd,cls,ni,pri,rtprio,sched -C "lab6.2"
   ```

## 关键配置
```c
// 设置实时线程属性
pthread_attr_setschedpolicy(&attr, SCHED_FIFO);  // 或SCHED_RR
param.sched_priority = 2;  // 优先级设置（1-99）
pthread_attr_setschedparam(&attr, &param);
```

## 调度策略对比
| 策略 | 特点 |
|-------|-------|
| SCHED_FIFO | 高优先级线程独占CPU |
| SCHED_RR   | 同优先级线程轮转执行 |
| 默认策略   | 受nice值影响 |

---

