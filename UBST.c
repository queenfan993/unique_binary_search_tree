#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>

MODULE_LICENSE("Dual MIT/GPL");
MODULE_DESCRIPTION("Unique binary search tree module");
MODULE_VERSION("0.1");

#define DEV_UBST_NAME "unique_binary_search_tree"

#define MAX_LENGTH 20

static dev_t ubst_dev = 0;
static struct class *ubst_class;
static DEFINE_MUTEX( ubst_mutex);
static int major = 0, minor = 0;

static long long ubst(long long k) {
	long long* dp = kmalloc(sizeof(long long) * (k + 1), GFP_KERNEL);
	dp[0] = 1;
	dp[1] = 1;
	for (int i = 2; i < k + 1; i++) {
		for (int j = 0; j < i; j++) {
			dp[i] += dp[j] * dp[i - j - 1];
		}
	}
	long long result = dp[k];
	kfree(dp);
	return result;
}

static int ubst_open(struct inode *inode, struct file *file) {
	if (!mutex_trylock(&ubst_mutex)) {
		printk(KERN_ALERT "ubst is in use\n");
		return -EBUSY;
	}
	return 0;
}

static int ubst_release(struct inode *inode, struct file *file) {
	mutex_unlock(&ubst_mutex);
	return 0;
}


static ssize_t ubst_read(struct file *file, char *buf, size_t size,
		loff_t *offset) {
	return (ssize_t) ubst(*offset);
}


static ssize_t ubst_write(struct file *file, const char *buf, size_t size,
		loff_t *offset) {
	return 1;
}

static loff_t ubst_device_llseek(struct file *file, loff_t offset, int orig) {
	loff_t new_pos = 0;
	switch (orig) {
	case 0: /* SEEK_SET: */
		new_pos = offset;
		break;
	case 1: /* SEEK_CUR: */
		new_pos = file -> f_pos + offset;
		break;
	case 2: /* SEEK_END: */
		new_pos = MAX_LENGTH - offset;
		break;
	}

	if (new_pos > MAX_LENGTH)
		new_pos = MAX_LENGTH;
	if (new_pos < 0)
		new_pos = 0;
	file -> f_pos = new_pos;
	return new_pos;
}

const struct file_operations ubst_fops = {
	.owner = THIS_MODULE,
	.read = ubst_read,
	.write = ubst_write,
	.open = ubst_open,
	.release = ubst_release,
	.llseek = ubst_device_llseek,
};

static int __init init_ubst_dev(void)
{
    int rc = 0;
    mutex_init(&ubst_mutex);

    rc = major = register_chrdev(major, DEV_UBST_NAME, &ubst_fops);
    if (rc < 0) {
        printk(KERN_ALERT "Failed to add cdev\n");
        rc = -2;
        goto failed_cdev;
    }
    ubst_dev = MKDEV(major, minor);

    ubst_class = class_create(THIS_MODULE, DEV_UBST_NAME);

    if (!ubst_class) {
            printk(KERN_ALERT "Failed to create device class\n");
            rc = -3;
            goto failed_class_create;
    }

	if (!device_create(ubst_class, NULL, ubst_dev, NULL, DEV_UBST_NAME)) {
		printk(KERN_ALERT "Failed to create device\n");
		rc = -4;
		goto failed_device_create;
	}
	return rc;

	failed_device_create:
	    class_destroy(ubst_class);
	failed_class_create:
	failed_cdev:
	    unregister_chrdev(major, DEV_UBST_NAME);
	    return rc;
}

static void __exit exit_ubst_dev(void)
{
	mutex_destroy(&ubst_mutex);
	device_destroy(ubst_class, ubst_dev);
	class_destroy(ubst_class);
	unregister_chrdev(major, DEV_UBST_NAME);
}

module_init(init_ubst_dev);
module_exit(exit_ubst_dev);
