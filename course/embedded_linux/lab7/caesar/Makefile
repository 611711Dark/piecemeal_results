kdir=/lib/modules/$(shell uname -r)/build  # 动态获取当前内核路径
# 模块名称
modname=caesar_255
obj-m:=$(modname).o

# 编译模块
$(modname).ko: $(modname).c
	make -C $(kdir) M=$(PWD) modules

# 清理
clean:
	make -C $(kdir) M=$(PWD) clean

.PHONY: clean
