#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yakymiv R.R.");
MODULE_DESCRIPTION("Simple smod kernel module with proc and sysfs parameters");
MODULE_VERSION("1.0");

/* Module parameters */
static char parg[9] = "";
static int pproc = 0;
static short psys = 0;

module_param_string(parg, parg, sizeof(parg), S_IRUGO | S_IWUGO);
MODULE_PARM_DESC(parg, "string(8) parameter");

module_param(pproc, int, S_IRUGO);
MODULE_PARM_DESC(pproc, "int parameter visible in /proc/smodparam");

module_param(psys, short, S_IRUGO | S_IWUGO);
MODULE_PARM_DESC(psys, "short parameter available in sysfs");

/* Proc interface */
#define PROC_NAME "smodparam"
static struct proc_dir_entry *proc_entry;

static ssize_t proc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    char tmp[32];
    int len = snprintf(tmp, sizeof(tmp), "%d\n", pproc);
    return simple_read_from_buffer(buf, count, ppos, tmp, len);
}

static const struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read  = proc_read,
};

static int __init smod_init(void)
{
    /* Create /proc entry */
    proc_entry = proc_create(PROC_NAME, 0444, NULL, &proc_ops);
    if (!proc_entry) {
        pr_err("Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    pr_info("smod module loaded: parg=%s, pproc=%d, psys=%hd\n", parg, pproc, psys);
    return 0;
}

static void __exit smod_exit(void)
{
    proc_remove(proc_entry);
    pr_info("smod module unloaded\n");
}

module_init(smod_init);
module_exit(smod_exit);
