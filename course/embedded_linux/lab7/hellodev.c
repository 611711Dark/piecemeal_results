/* Sample Kernel module with /dev interface
*	NOTE : the /dev entry must be created before loading this module with the command
*		mknod /dev/hello c 60 0
*	executed as root.
* This program is GPL.
*/

#define HELLO_DEV_NAME "hello"

/** Includes **/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h> // printk
#include <linux/fs.h>
#include <linux/uaccess.h> /* copy_from/to_user */

/** Informations du driver **/
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("An HelloWorld module with /dev interface");
MODULE_AUTHOR("Romain Rossi <romain.rossi@esigelec.fr>");
MODULE_SUPPORTED_DEVICE(HELLO_DEV_NAME);

static int major = 60;

/* Prototypes */
static ssize_t helloRead(struct file *file, char *buf, size_t count, loff_t *ppos);

static struct file_operations hello_ops=
{
	.read = helloRead
};



/*
 * Fonctions lancées au chargement / déchargement du module
 */
static int __init hello_init(void)
{
	printk(KERN_ALERT "hello Loading ...\n");

	register_chrdev(major,HELLO_DEV_NAME,&hello_ops);

	printk(KERN_ALERT "MAJOR = %d\n" , major);

	return 0;
};

static void __exit hello_exit(void)
{
	printk(KERN_ALERT "hello Unloading ...\n");
	unregister_chrdev(major,HELLO_DEV_NAME);
};

// Fait connaitre ces fonctions au noyau
module_init(hello_init);
module_exit(hello_exit);



/*
 * Fonctions lancées lors des accès au fichier /dev/hello
 */
// Lecture du fichier
static ssize_t helloRead(struct file *file, char *buf, size_t count, loff_t *ppos)
{
    char mesg[] = "HelloWorld\n";

    // Debug message
    //    printk(KERN_ALERT "read()\n");

    if ( mesg[(*ppos)] != '\0' ){

      copy_to_user( buf , (mesg+(*ppos)) , 1 );
      *ppos += 1;
      return 1;
    }else{
      return 0;
    }

}
