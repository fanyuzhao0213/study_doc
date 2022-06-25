# linux驱动学习

## 1.字符设备驱动

### 1.1、字符设备驱动框架

​       字符设备驱动的编写主要就是驱动对应的open、close、read。。。其实就是

file_operations结构体的成员变量的实现。

### 1.2、驱动模块的加载与卸载

​       Linux驱动程序可以编译到kernel里面，也就是zImage，也可以编译为模块，.ko。测试的时候只需要加载.ko模块就可以。

 

编写驱动的时候注意事项！

1、编译驱动的时候需要用到linux内核源码！因此要解压缩linux内核源码，编译linux内核源码。得到zImage和.dtb。需要使用编译后的到的zImage和dtb启动系统。

2、从SD卡启动，SD卡烧写了uboot。uboot通过tftp从ubuntu里面获取zimage和dtb，rootfs也是通过nfs挂在。

3、设置bootcmd和bootargs

bootargs=console=ttymxc0,115200 rw root=/dev/nfs nfsroot=192.168.1.66:/home/zzk/linux/nfs/rootfs ip=192.168.1.50:192.168.1.66:192.168.1.1:255.255.255.0::eth0:off

 

bootcmd=tftp 80800000 zImage;tftp 83000000 imx6ull-alientek-emmc.dtb;bootz 80800000 - 83000000;

 

4、将编译出来的.ko文件放到根文件系统里面。加载驱动会用到加载命令：insmod，modprobe。移除驱动使用命令rmmod。对于一个新的模块使用modprobe加载的时候需要先调用一下depmod命令。

5，驱动模块加载成功以后可以使用lsmod查看一下。

6，卸载模块使用rmmod命令

### 1.3、字符设备的注册与注销

​       1、我们需要向系统注册一个字符设备，使用函数register_chrdev。

​       2、卸载驱动的时候需要注销掉前面注册的字符设备，使用函数unregister_chrdev，注销字符设备。

1.4、设备号

​       1，Linux内核使用dev_t。

typedef __kernel_dev_t        dev_t;

typedef __u32 __kernel_dev_t;

typedef unsigned int __u32;

2、Linux内核将设备号分为两部分：主设备号和次设备号。主设备号占用前12位，次设备号占用低20位。

3、设备号的操作函数，或宏

从dev_t获取主设备号和次设备号，MAJOR(dev_t)，MINOR(dev_t)。也可以使用主设备号和次设备号构成dev_t，通过MKDEV(major，minor)

 

 

### 1.5、file_operations的具体实现

​       struct file_operations {

​       struct module *owner;

​       loff_t (*llseek) (struct file *, loff_t, int);

​       ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);

​       ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);

​       ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);

​       ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);

​       int (*iterate) (struct file *, struct dir_context *);

​       unsigned int (*poll) (struct file *, struct poll_table_struct *);

​       long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);

​       long (*compat_ioctl) (struct file *, unsigned int, unsigned long);

​       int (*mmap) (struct file *, struct vm_area_struct *);

​       int (*mremap)(struct file *, struct vm_area_struct *);

​       int (*open) (struct inode *, struct file *);

​       int (*flush) (struct file *, fl_owner_t id);

​       int (*release) (struct inode *, struct file *);

​       int (*fsync) (struct file *, loff_t, loff_t, int datasync);

​       int (*aio_fsync) (struct kiocb *, int datasync);

​       int (*fasync) (int, struct file *, int);

​       int (*lock) (struct file *, int, struct file_lock *);

​       ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);

​       unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);

​       int (*check_flags)(int);

​       int (*flock) (struct file *, int, struct file_lock *);

​       ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);

​       ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);

​       int (*setlease)(struct file *, long, struct file_lock **, void **);

​       long (*fallocate)(struct file *file, int mode, loff_t offset,

​                       loff_t len);

​       void (*show_fdinfo)(struct seq_file *m, struct file *f);

\#ifndef CONFIG_MMU

​       unsigned (*mmap_capabilities)(struct file *);

\#endif

};

- copy_to_user 函数来完成内核空间的数据到用户空间的复制

- copy_from_user 将用户空间的数据复制到 内核空间中。

### 1.6、地址映射

​       1、裸机LED灯实验就是操作6ULL的寄存器。

​       2，Linux驱动开发也可以操作寄存器，Linux不能直接对寄存器物理地址进行读写操作，比如寄存器A物理地址为0X01010101。裸机的时候可以直接对0X01010101这个物理地址进行操作，但是linux下不行。因为linux会使能MMU。

​       在linux里面操作的都是虚拟地址，所以需要先得到0X01010101这个物理地址对应的虚拟地址。

​       获得物理物理地址对应的虚拟地址使用ioremap函数。

​       第一个参数就是物理地址其实大小，第二个参数就是要转化的字节数量。0X01010101，开始10个地址进行转换，

va=ioremap(0X01010101, 10).

​       卸载驱动的时候：

​       iounmap(va);

### 1.7、创建设备节点

mknod /dev/led c 200 0



## 2.新字符设备驱动

### 2.1 新字符设备设置过程

/* newchrled设备结构体 */*



#define NEWCHRLED_CNT			1		  	/* 设备号个数 */
#define NEWCHRLED_NAME			"newchrled"	/* 名字 */

struct newchrled_dev{
​	dev_t devid;			/* 设备号 	 */
​	struct cdev cdev;		/* cdev 	*/
​	struct class *class;		/* 类 		*/
​	struct device *device;	/* 设备 	 */
​	int major;				/* 主设备号	  */
​	int minor;				/* 次设备号   */
};

struct newchrled_dev newchrled;	/* led设备 */



/* 1、创建设备号 */
if (newchrled.major) {		/*  定义了设备号 */
​	newchrled.devid = MKDEV(newchrled.major, 0);
​	register_chrdev_region(newchrled.devid, NEWCHRLED_CNT, NEWCHRLED_NAME);
} else {						/* 没有定义设备号 */
​	alloc_chrdev_region(&newchrled.devid, 0, NEWCHRLED_CNT, NEWCHRLED_NAME);	/* 申请设备号 */
​	newchrled.major = MAJOR(newchrled.devid);	/* 获取分配号的主设备号 */
​	newchrled.minor = MINOR(newchrled.devid);	/* 获取分配号的次设备号 */
}
printk("newcheled major=%d,minor=%d\r\n",newchrled.major, newchrled.minor);

/* 2、初始化cdev */
newchrled.cdev.owner = THIS_MODULE;
cdev_init(&newchrled.cdev, &newchrled_fops);

/* 3、添加一个cdev */
cdev_add(&newchrled.cdev, newchrled.devid, NEWCHRLED_CNT);

/* 4、创建类 */
newchrled.class = class_create(THIS_MODULE, NEWCHRLED_NAME);
if (IS_ERR(newchrled.class)) {
​	return PTR_ERR(newchrled.class);
}

/* 5、创建设备 */
newchrled.device = device_create(newchrled.class, NULL, newchrled.devid, NULL, NEWCHRLED_NAME);
if (IS_ERR(newchrled.device)) {
​	return PTR_ERR(newchrled.device);
}

return 0;

## 3.设备树

### 3.1 什么是设备树

- ​       1、uboot启动内核用到zImage，imx6ull-alientek-emmc.dtb。bootz 80800000 – 83000000.

- ​       2、设备树：设备和树。

- ​       3，在单片机驱动里面比如W25QXX，SPI，速度都是在.c文件里面写死。板级信息都写到.c里面，导致linux内核臃肿。因此 将板子信息做成独立的格式，文件扩展名为.dts。一个平台或者机器对应一个.dts。

### 3.2 DTS、DTB、DTC

​	.dts相当于.c，就是DTS源码文件。

​      DTC工具相当于gcc编译器，将.dts编译成.dtb。

​      .dtb相当于bin文件，或可执行文件。

​     通过make dtbs编译所有的dts文件。如果要编译指定的dtbs。eg:make imx6ull-alientek-emmc.dtb

### 3.3 DTS的基本用法

- **node-name@unit-address**

  其中“node-name”是节点名字，为 ASCII 字符串，节点名字应该能够清晰的描述出节点的功能，比如“uart1”就表示这个节点是 UART1 外设。“unit-address”一般表示设备的地址或寄存器首地址，如果某个节点没有地址或者寄存器的话“unit-address”可以不要，比如“cpu@0”、“interrupt-controller@00a01000”。 

- **label: node-name@unit-address**

  引入 label 的目的就是为了方便访问节点，可以直接通过&label 来访问这个节点，比如通过&cpu0 就可以访问“cpu@0”这个节点，而不需要输入完整的节点名字。再比如节点 “intc:interrupt-controller@00a01000”，节点 label 是 intc，而节点名字就很长了，为“interruptcontroller@00a01000”。很明显通过&intc 来访问“interrupt-controller@00a01000”这个节点要方便很多！ 