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

/* string parameter up to 8 chars, readable/writable by owner */
module_param_string(parg, parg, sizeof(parg), 0644);
MODULE_PARM_DESC(parg, "string(8) parameter");

/* int parameter, read-only in sysfs */
module_param(pproc, int, 0444);
MODULE_PARM_DESC(pproc, "int parameter visible in /proc/smodparam");

/* short parameter, readable/writable by owner in sysfs */
module_param(psys, short, 0644);
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

/* Use struct proc_ops for modern kernels */
static const struct proc_ops proc_file_ops = {
    .proc_read = proc_read,
};

static int __init smod_init(void)
{
    proc_entry = proc_create(PROC_NAME, 0444, NULL, &proc_file_ops);
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
