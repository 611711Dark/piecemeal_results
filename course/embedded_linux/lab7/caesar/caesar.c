/* 修改后的本地运行代码 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "hello"  // 设备名
static int major = 60;       // 主设备号

// 读操作函数
static ssize_t helloRead(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    char mesg[] = "HelloWorld\n";
    if (*ppos >= sizeof(mesg)-1) return 0;  // 检查是否读完
    
    // 每次返回一个字符
    if (copy_to_user(buf, mesg + *ppos, 1)) 
        return -EFAULT;
    
    (*ppos)++;
    return 1;
}

// 文件操作结构体
static struct file_operations hello_ops = {
    .owner = THIS_MODULE,
    .read = helloRead,
};

// 模块初始化
static int __init hello_init(void) {
    printk(KERN_INFO "Module loaded. Major=%d\n", major);
    return register_chrdev(major, DEVICE_NAME, &hello_ops);
}

// 模块卸载
static void __exit hello_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Module unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Local HelloWorld Driver");
MODULE_AUTHOR("Your Name");
