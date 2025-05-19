#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ctype.h>
#include <linux/slab.h>

#define DEVICE_NAME "caesar"  // Device name
static int major = 61;        // Major number
#define MAX_MSG_LEN 255       // Maximum message length

// Global variables for key and encrypted message
static int caesar_key = 0;
static char *caesar_message = NULL;
static size_t message_length = 0;

// Write operation function
static ssize_t caesarWrite(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    char *input;
    size_t i;

    // Allocate buffer for input
    input = kmalloc(count + 1, GFP_KERNEL);
    if (!input)
        return -ENOMEM;

    // Copy data from user space
    if (copy_from_user(input, buf, count)) {
        kfree(input);
        return -EFAULT;
    }
    input[count] = '\0'; // Null-terminate

    // Check if input is a key (first character is digit)
    if (count > 0 && isdigit(input[0])) {
        // Simple key setting (only first digit considered)
        caesar_key = input[0] - '0';
        printk(KERN_INFO "Caesar cipher: Key set to %d\n", caesar_key);
        kfree(input);
        return count;
    }

    // Process message
    if (caesar_message) {
        kfree(caesar_message);
        caesar_message = NULL;
        message_length = 0;
    }

    caesar_message = kmalloc(count + 1, GFP_KERNEL);
    if (!caesar_message) {
        kfree(input);
        return -ENOMEM;
    }

    for (i = 0; i < count && i < MAX_MSG_LEN; i++) {
        if (isupper(input[i])) {
            caesar_message[i] = ((input[i] - 'A' + caesar_key) % 26) + 'A';
        } else if (islower(input[i])) {
            caesar_message[i] = ((input[i] - 'a' + caesar_key) % 26) + 'a';
        } else {
            caesar_message[i] = input[i];
        }
    }

    message_length = i;
    caesar_message[message_length] = '\0';

    printk(KERN_INFO "Caesar cipher: Encrypted %zu characters with key %d\n",
           message_length, caesar_key);

    kfree(input);
    return count;
}

static ssize_t caesarRead(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
    size_t remaining;

    if (*ppos >= message_length || !caesar_message)
        return 0;

    remaining = message_length - *ppos;
    if (count > remaining)
        count = remaining;

    if (copy_to_user(buf, caesar_message + *ppos, count))
        return -EFAULT;

    *ppos += count;
    return count;
}

static struct file_operations caesar_ops = {
    .owner = THIS_MODULE,
    .read = caesarRead,
    .write = caesarWrite,
};

static int __init caesar_init(void) {
    printk(KERN_INFO "Caesar module loaded. Major=%d\n", major);
    return register_chrdev(major, DEVICE_NAME, &caesar_ops);
}

static void __exit caesar_exit(void) {
    if (caesar_message)
        kfree(caesar_message);
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Caesar module unloaded\n");
}

module_init(caesar_init);
module_exit(caesar_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Caesar Module");
MODULE_AUTHOR("SUN HUAN");
