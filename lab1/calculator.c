#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>

#define BUFF_SIZE 1024

static dev_t first;
static struct cdev c_dev; 
static struct class *cl;
static char input_buff[BUFF_SIZE];
static char output_buff[BUFF_SIZE];
static int64_t results[BUFF_SIZE];
static size_t result_index;
static struct proc_dir_entry *entry;


static uint8_t is_sign(char ch) {
  return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

static uint8_t is_digit(char ch) {
  return (ch >= '0' && ch <= '9');
}

static void evaluate_expession(const char *buf, size_t len) {
    char first[21] = {0};
    char second[21] = {0};
    int64_t first_int = 0;
    int64_t second_int = 0;
    uint8_t index = 0;
    char sign = 0;
    size_t i = 0;

    for (i = 0; i < len; ++i) {
      uint8_t sign_flag = is_sign(buf[i]);
      if (!sign_flag && !is_digit(buf[i]) && buf[i] != '\0' && buf[i] != '\n') {
        printk(KERN_ERR "%s : Parsing device input failed - wrong symbol '%c'\n", THIS_MODULE->name, buf[i]);
        
        return;
      }
      if (!sign_flag) {
        if (sign == 0) {
          first[index] = buf[i];
          ++index;
        }
        else {
          second[index] = buf[i];
          ++index;
        }
      }
      else {
        index = 0;
        sign = buf[i];
      }
		  
	  }

    if (kstrtol(first, 10, (long int*)&first_int) != 0) {
      printk(KERN_ERR "%s : Parsing device input failed - can't convert string '%s' to integer\n", THIS_MODULE->name, first);
      return;
    }
    
    if (kstrtol(second, 10, (long int*)&second_int) != 0) {
      printk(KERN_ERR "%s : Parsing device input failed - can't convert string '%s' to integer\n", THIS_MODULE->name, second);
      return;
    }

    switch (sign) {
      case '+': {
        results[result_index] = first_int + second_int;
        break;
      }
      case '-': {
        results[result_index] = first_int - second_int;
        break;
      }
      case '*': {
        results[result_index] = first_int * second_int;
        break;
      }
      case '/': {
        results[result_index] = first_int / second_int;
        break;
      }
      default:
        break;
    }
    printk(KERN_DEBUG "%s : Evaluated the expession, the result is %lld\n", THIS_MODULE->name, results[result_index]);
    ++result_index;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
  size_t i = 0;
  size_t count = 0;
  if (*off > 0) {
    return 0;
  }
  printk(KERN_INFO "%s : Reading from device\n", THIS_MODULE->name);
  
  for (i = 0; i < result_index; ++i) {
    sprintf(output_buff + count, "%lld\n", results[i]);
    //printk(KERN_DEBUG "%s : %lld results to str %s\n", THIS_MODULE->name, results[i], output_buff + count);
    count += strlen(output_buff);
  }

  if (copy_to_user(buf, output_buff, count) != 0) {
      printk(KERN_ERR "%s : Reading from device failed\n", THIS_MODULE->name);
      return -EFAULT;
  }
  else {
    printk(KERN_DEBUG "%s : Read %zu chars\n", THIS_MODULE->name, strlen(output_buff));
    *off = count;
  }

  return count;
}

static ssize_t my_write(struct file *f, const char __user *buf,  size_t len, loff_t *off)
{
  printk(KERN_INFO "%s : Writing to device\n", THIS_MODULE->name);
  if (len > BUFF_SIZE) {
    len = BUFF_SIZE;
  }
  if (copy_from_user(input_buff, buf, len) != 0) {
    
    return -EFAULT;
  }
  else {
    printk(KERN_DEBUG "%s : Wrote %zu chars - %s\n", THIS_MODULE->name, len, input_buff);
    evaluate_expession(buf, len);
    printk(KERN_DEBUG "%s : Resetting input buffer\n", THIS_MODULE->name);
    memset(input_buff, 0, BUFF_SIZE);
    return len;
  }
}

static int my_dev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

static struct file_operations mychdev_fops =
{
  .owner      = THIS_MODULE,
  .read       = my_read,
  .write      = my_write
};

static const struct proc_ops proc_fops = { 
	.proc_read 	= my_read
};

static int __init ch_drv_init(void)
{
    printk(KERN_INFO "%s : Device initialising started\n", THIS_MODULE->name);

    if (alloc_chrdev_region(&first, 0, 1, "ch_dev") < 0)
	  {
		  return -1;
	  }

    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
	  {
		  unregister_chrdev_region(first, 1);
		  return -1;
	  }

    cl->dev_uevent = my_dev_uevent;

    if (device_create(cl, NULL, first, NULL, "var2") == NULL)
	  {
		  class_destroy(cl);
		  unregister_chrdev_region(first, 1);
		  return -1;
	  }

    cdev_init(&c_dev, &mychdev_fops);
    if (cdev_add(&c_dev, first, 1) == -1)
	  {
		  device_destroy(cl, first);
		  class_destroy(cl);
		  unregister_chrdev_region(first, 1);
		  return -1;
	  }

    entry = proc_create("var2", 0666, NULL, &proc_fops);
    if (entry == NULL) {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    printk(KERN_INFO "%s : Device initialising finished\n<Major, Minor>: <%d, %d>\n", THIS_MODULE->name, MAJOR(first), MINOR(first));

    return 0;
}
 
static void __exit ch_drv_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);

    proc_remove(entry);

    printk(KERN_INFO "%s : Device removed\n", THIS_MODULE->name);
}
 
module_init(ch_drv_init);
module_exit(ch_drv_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daria Kosheleva");
MODULE_DESCRIPTION("Calculator module");

