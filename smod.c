#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#define PROCFS_NAME "smodparam"
static int parg = 0;
module_param(parg, int, 0660);
MODULE_PARM_DESC(parg, "Integer parameter passed to module");
static char pproc[9] = "default";
static long int psys = 100;
static struct proc_dir_entry *proc_file;
static struct kobject *smod_kobj;
ssize_t proc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    char buffer[16];
    int len = snprintf(buffer, sizeof(buffer), "%s\n", pproc);
    return simple_read_from_buffer(buf, count, ppos, buffer, len);
}
ssize_t proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    if (count >= sizeof(pproc))
        return -EINVAL;
    if (copy_from_user(pproc, buf, count))
        return -EFAULT;
    pproc[count] = '\0';
    return count;
}
static struct proc_ops proc_file_ops = {
    .proc_read = proc_read,
    .proc_write = proc_write,
};
static ssize_t psys_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%ld\n", psys);
}
static ssize_t psys_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    kstrtol(buf, 10, &psys);
    return count;
}
static struct kobj_attribute psys_attr = __ATTR(psys, 0644, psys_show, psys_store);
static int __init smod_init(void)
{
    pr_info("smod loaded with parg = %d\n", parg);
    proc_file = proc_create(PROCFS_NAME, 0644, NULL, &proc_file_ops);
    if (!proc_file)
    {
        30 pr_err("Failed to create /proc/%s\n", PROCFS_NAME);
        return -ENOMEM;
    }
    smod_kobj = kobject_create_and_add("smod", kernel_kobj);
    if (!smod_kobj)
    {
        pr_err("Failed to create kobject\n");
        remove_proc_entry(PROCFS_NAME, NULL);
        return -ENOMEM;
    }
    if (sysfs_create_file(smod_kobj, &psys_attr.attr))
    {
        pr_err("Failed to create sysfs entry\n");
        kobject_put(smod_kobj);
        remove_proc_entry(PROCFS_NAME, NULL);
        return -ENOMEM;
    }
    return 0;
}
static void __exit smod_exit(void)
{
    kobject_put(smod_kobj);
    remove_proc_entry(PROCFS_NAME, NULL);
    pr_info("smod unloaded\n");
}
module_init(smod_init);
module_exit(smod_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("KISP");
MODULE_DESCRIPTION("Test kernel module smod with parg (int), pproc (string(8)), psys (long int)");