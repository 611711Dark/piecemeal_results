#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ctype.h>

#define DEVICE_NAME "caesar"  
static int major = 61;        

static int caesar_key = 0;
static char caesar_message = '\0';


static ssize_t caesarWrite(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    char input;
    
    
    if (copy_from_user(&input, buf, 1))
        return -EFAULT;
    
    
    if (isdigit(input)) {
        caesar_key = input - '0';
        printk(KERN_INFO "Caesar cipher: Key set to %d\n", caesar_key);
    } 
    
    else if (isalpha(input)) {
        char base = isupper(input) ? 'A' : 'a';
        caesar_message = ((input - base + caesar_key) % 26) + base;
        printk(KERN_INFO "Caesar cipher: Encrypted '%c' to '%c' with key %d\n", 
               input, caesar_message, caesar_key);
    } else {
        printk(KERN_WARNING "Caesar cipher: Invalid input '%c'\n", input);
    }
    
    return count;
}


static ssize_t caesarRead(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    if (*ppos > 0 || caesar_message == '\0')
        return 0;
    
    if (copy_to_user(buf, &caesar_message, 1))
        return -EFAULT;
    
    (*ppos)++;
    return 1;
}


static struct file_operations caesar_ops = {
    .owner = THIS_MODULE,
    .read = caesarRead,
    .write = caesarWrite,
};


static int __init caesar_init(void) {
    printk(KERN_INFO "Caesar cipher module loaded. Major=%d\n", major);
    return register_chrdev(major, DEVICE_NAME, &caesar_ops);
}

// Module cleanup
static void __exit caesar_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Caesar cipher module unloaded\n");
}

module_init(caesar_init);
module_exit(caesar_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Caesar Cipher Kernel Module");
MODULE_AUTHOR("SUN HUAN");
