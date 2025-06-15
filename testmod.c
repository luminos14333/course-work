/* testmod.c - The simple test kernel module. */

#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yakymiv R.R.");
MODULE_DESCRIPTION("The simple test kernel module");

int init_module(void)
{
    pr_info("The kernel module testmod has been loaded.\n");

    /* A non 0 return means init_module failed; module can't be loaded. */
    return 0;
}

void cleanup_module(void)
{
    pr_info("The kernel module testmod has been removed.\n");
}
