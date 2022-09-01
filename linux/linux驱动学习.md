# linux驱动学习

[TOC]



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


## 4.pinctrl和gpio子系统

### 4.1pinctrl子系统

借助pinctrl来设置一个PIN的复用和电气属性。

pinctrl_hog_1: hoggrp-1 {

​        fsl,pins = < **MX6UL_PAD_UART1_RTS_B__GPIO1_IO19    0x17059 /\* SD1 CD */**

​                   	>;

​              };

**MX6UL_PAD_UART1_RTS_B__GPIO1_IO19    0x17059 /\* SD1 CD** 

**<mux_reg     conf_reg    input_reg      mux_mode   input_val>**

   0x0090          0x031C        0x0000               0x5              0x0

**mux_reg：偏移地址**IOMUXC父节点首地址0x020e0000，因此UART1_RTS_B这个PIN的mux寄存器地址 就是：0x020e0000+0x0090=0x020e 0090。

**conf_reg**：0x020e0000+0x031C=0x020e 031C，这个寄存器就是UART1_RTS_B的电气属性配置寄存器。

**input_reg，偏移地址为0，表示UART1_RTS_B这个PIN没有input功能。**

**mux_mode：5表示复用为GPIO1_IO19，将其写入0x020e 0090**

**input_val：就是写入input_reg寄存器的值。**

**0x17059：为PIN的电气属性配置寄存器值。**

## 5.并发与竞争

### 5.1 原子操作

​     **原子整形操作与原子位操作**

 Linux 内核提供了一组原子操作 API 函数来完成此功能， Linux 内核提供了两组原子操作 API 函数，一组是对整形变量进行操作的，一组是对位进行操作的 

### 5.2 自旋锁

​       1、用于多核SMP。

​       2，使用自旋锁，要注意死锁现象的发生。

​       线程与线程，

 **线程与中断自旋锁的使用注意事项：**
①、自旋锁保护的临界区要尽可能的短，因此在 open 函数中申请自旋锁，然后在 release 函数中释放自旋锁的方法就不可取。我们可以使用一个变量来表示设备的使用情况，如果设备被使用了那么变量就加一，设备被释放以后变量就减 1，我们只需要使用自旋锁保护这个变量即可。
②、考虑驱动的兼容性，合理的选择 API 函数。 。

## 6. Linux内核定时器原理

### 6.1、内核时间管理

​       1、Cortex-M内核使用systick作为系统定时器。

​       2、硬件定时器、软件定时器，原理是依靠系统定时器来驱动。

​       3、linux内核频率可以配置，图形化界面配置。

​       4、重点，HZ表示系统节拍率，    jiffies，

 

### 6.2、节拍率高低的缺陷

 

### 6.3、jiffies

 内核使用全局变量 jiffies 来记录系统从启动以来的系统节拍数，系统启动的时候会
将 jiffies 初始化为 0

前面说了 HZ 表示每秒的节拍数，jiffies 表示系统运行的 jiffies 节拍数，所以 jiffies/HZ 就
是系统运行时间，单位为秒。

### 6.4、内核定时器

​       1、软件定时器不像硬件定时器一样，直接给周期值。设置期满以后的时间点。

​       2、定时处理函数。

​       3、内核定时器不是周期性的，一次定时时间到了以后就会关闭，除非重新打开

### 6.5、编写试验驱动      

​       1、定义一个定时器，结构体timer_list

​       2、应用ioctl函数-》unlocked_ioctl和compat_ioctl。

​       long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long)

​       long (*compat_ioctl) (struct file *, unsigned int, unsigned long);

​       ioctl的命令是自己定义的，但是要符合linux规则。构建命令：

\#define _IO(type,nr)                   /没有参数的命令

\#define _IOR(type,nr,size)           //该命令是从驱动读取数据

\#define _IOW(type,nr,size)          //该命令是向驱动写入数据

\#define _IOWR(type,nr,size)        //双向数据传输

  **type是幻数，nr是序号，size是大小。**



## 7.Linux内 核中断处理简介

### 7.1 linux中断

​       1、先知道你要使用的中断对应的中断号。

​       2、先申请request_irq，此函数会激活中断。

​       3、如果不用中断了，那就释放掉，使用free_irq。

​       4、中断处理函数irqreturn_t (*irq_handler_t) (int, void *)。

 

​       5、使能和禁止中断，

​       

### 7.2 上半部和下半部

​       中断一定要处理的越快越好，

​       **7.1、软中断**

​	static struct softirq_action softirq_vec[NR_SOFTIRQS]   10个

​       要使用软中断，要先注册，使用函数open_softir。注册以后使用raise_softirq触发。

​       软中断我们不要去用！！

​       软中断我们不要去用！！

 

​       **2、tasklet**

​       也需要用到上半部，只是上半部的中断处理函数重点是调用tasklet_schedule。

​       1、定义一个tasklet函数。

​       2、初始化、重点是设置对应的处理函数

​       **3、工作队列**



## 8.Linux阻塞和非阻塞IO

### 8.1 阻塞与非阻塞简介

​       阻塞:当资源不可用的时候，应用程序就会挂起。当资源可用的时候，唤醒任务。应用程序使用open打开驱动文件，默认是阻塞方式打开。

​       非阻塞：当资源不可用的时候，应用程序轮询查看，或放弃。会有超时处理机制。应用程序在使用open打开驱动文件的时候，使用O_NONBLOCK。

 

### 8.2 等待队列

​       1、等待队列头

​       wait_queue_head_t 需要定义一个。定义以后使用 init_waitqueue_head函数初始化。或者使用宏DECLARE_WAIT_QUEUE_HEAD。

​       

​       2、等待队列项

​       wait_queue_t表四等待队列项，或者使用宏DECLARE_WAITQUEUE(name, tsk)。

​       

​       3、添加队列项到等待队列头

​       add_wait_queue函数

​       4、移除等待队列项

​       资源可用的时候使用remove_wait_queue函数移除。

 

​       5、唤醒

​       wake_up唤醒

 

### 8.3、轮询

 

### 8.4、驱动里面的poll函数

​       unsigned int (*poll) (struct file *, struct poll_table_struct *)，将wait传递给poll_wait



## 9、异步通知简介

###    9.1、硬件中断

​       

###     9.2、信号

​       软件层次上的“中断”，也叫做软中断信号，软件层次上对中断机制的一种模拟

​       

kill -9 xxx //关闭某个应用

​       

​       

###     9.3、信号处理函数

​       应用使用signal函数，设置信号处理函数，原型为：

sighandler_t signal(int signum, sighandler_t handler);

​       信号处理函数为：

typedef void (*sighandler_t)(int);

 

CTRL+C关闭应用，发送的SIGINT信号。

谁向应用发送信号。

 

###     9.4、驱动中对异步通知的处理

​       需要实现file_operations中的fasync函数，函数原型如下：

int (*fsync) (struct file *, loff_t, loff_t, int datasync);

 

​       1、要使用fasync_struct定义一个指针结构体变量。

​       2、实现file_operations里面的fasync函数，函数原型：

int (*fasync) (int, struct file *, int);

​       fasync还需要借助fasync_helper函数。

 

​       3、驱动里面调用fasync向应用发送信号，函数原型

void kill_fasync(struct fasync_struct **fp, int sig, int band)

​       4、关闭驱动的时候要删除信号：



## 10.Linux驱动分离与分层

​       1、以前的驱动实验都很简单，就是对IO的操作。

 

​       目的：方便驱动的编写，提高软件的重用以及跨平台性能。

### 10.1、驱动的分隔与分离

​       主机驱动，设备驱动

​       1、单片机驱动开发，IIC设备MPU6050，

​       2、将驱动分离：主机控制器驱动和设备驱动，主机控制器驱动一般是半导体厂商写的。在linux驱动框架下编写具体的设备驱动。

​       3、中间的联系就是核心层。

## 10.2、总线-驱动-设备

驱动-总线-设备。

根据驱动的分离与分层衍生出了总线(bus)-驱动(driver)-设备(device)驱动框架。

​       总线代码我们不需要编写，linux内核提供给我们使用的。我们需要编写驱动和设备，当向总线注册驱动的时候，总线会从现有的所有设备中查找，看看哪个设备和此驱动匹配。同理，当向总线注册设备的时候总线也会在现有的驱动中查看与之匹配的驱动。

 

​       驱动：是具体的设备驱动

​       设备：设备属性，包括地址范围、如果是IIC的话还有IIC器件地址、速度

### 10.3、总线

​       总线数据类型为：bus_type。向内核注册总线使用bus_register。

struct bus_type {

​       const char             *name;

​       const char             *dev_name;

​       struct device         *dev_root;

​       struct device_attribute  *dev_attrs;     /* use dev_groups instead */

​       const struct attribute_group **bus_groups;

​       const struct attribute_group **dev_groups;

​       const struct attribute_group **drv_groups;

​       **int (\*match)(struct device *dev, struct device_driver *drv);**

​       int (*uevent)(struct device *dev, struct kobj_uevent_env *env);

​       int (*probe)(struct device *dev);

​       int (*remove)(struct device *dev);

​       void (*shutdown)(struct device *dev);

​       int (*online)(struct device *dev);

​       int (*offline)(struct device *dev);

​       int (*suspend)(struct device *dev, pm_message_t state);

​       int (*resume)(struct device *dev);

​       const struct dev_pm_ops *pm;

​       const struct iommu_ops *iommu_ops;

​       struct subsys_private *p;

​       struct lock_class_key lock_key;

};

向linux内核注册总线，使用bus_register函数。bus_unregister函数卸载。

**总**线主要工作就是完成总线下的设备和驱动之间的匹配。

## 10.4、驱动                 

​       驱动数据类型为device_driver，驱动程序向内核注册驱动采用driver_register。

struct device_driver {

​       const char             *name;

​       **struct bus_type           \*bus;**             

​       struct module       *owner;

​       const char             *mod_name;  /* used for built-in modules */

​       bool suppress_bind_attrs;    /* disables bind/unbind via sysfs */

​       **const struct of_device_id   \*of_match_table;**

​       const struct acpi_device_id   *acpi_match_table;

​       **int (\*probe) (struct device *dev);**                    

​       **int (\*remove) (struct device *dev);**

​       **void (\*shutdown) (struct device *dev);**

​       **int (\*suspend) (struct device *dev, pm_message_t state);**

​       **int (\*resume) (struct device *dev);**

​       const struct attribute_group **groups;

​       const struct dev_pm_ops *pm;

​       struct driver_private *p;

};

​       驱动和设备匹配以后驱动里面的probe函数就会执行。

​       使用driver_register注册驱动。

​       driver_register

​              -> bus_add_driver

​                     -> driver_attach           //查找bus下所有设备，找预期匹配的。

​                            ->bus_for_each_dev(drv->bus, NULL, drv, __driver_attach); 

​                                   ->__driver_attach  //每个设备都调用此函数，

//查看每个设备是否与驱动匹配

​                      -> driver_match_device  //检查是否匹配。

​                                     -> driver_probe_device  

​                                          ->really_probe

​                                                 -> drv->probe(dev);  //执行driver的probe函数​                               

​       向线注册驱动的时候，会检查当前总线下的所有设备，有没有与此驱动匹配的设备，如果有的话就执行驱动里面的probe函数。

### 10.5、设备

​       设备数据类型为device，通过device_register向内核注册设备。

struct device {

​       struct device         *parent;

​       struct device_private     *p;

​       struct kobject kobj;

​       const char             *init_name; /* initial name of the device */

​       const struct device_type *type;

​       struct mutex          mutex;    /* mutex to synchronize calls to \* its driver.

​       **struct bus_type    \*bus;             /* type of bus device is on */**

​       **struct device_driver \*driver;      /* which driver has allocated this**

​                                      **device \*/**  

……

};

​       向总线注册设备的时候，使用device_register。

​       device_register

​              -> device_add

​                     -> bus_add_device

​                  -> bus_probe_device

​                            ->device_attach

​                                   ->bus_for_each_drv(dev->bus, NULL, dev, __device_attach);

​                                          -> __device_attach

​                                                 -> driver_match_device  //匹配驱动

​                                                        ->bus->match

​                                                 ->driver_probe_device  //执行此函数

​                                                        -> really_probe

​                                                               **-> drv->probe(dev);**

​       驱动与设备匹配以后驱动的probe函数就会执行，probe函数就是驱动编写人员去编写的！！！！

### 10.6、platform平台驱动模型 

​       根据总线-驱动-设备驱动模型，IIC、SPI、USB这样实实在在的总线是完全匹配的，但是要有一些外设是没法归结为具体的总线：比如定时器、RTC、LCD等。为此linux内核创造了一个虚拟的总线：platform总线。

​       1、方便开发，linux提出了驱动分离与分层。

​       2、进一步引出了驱动-总线-设备驱动模型，或者框架。

​       3、对于SOC内部的RTC，timer等等不好归结为具体的总线，为此linux内核提出了一个虚拟总线：platform总线，platform设备和platform驱动 

#### 10.6.1、platform总线注册

platform_bus_init

​       -> bus_register

注册的内容就是：

struct bus_type platform_bus_type = {

​       **.name            = "platform",**

​       .dev_groups   = platform_dev_groups,

​       .match           = platform_match,

​       .uevent          = platform_uevent,

​       .pm        = &platform_dev_pm_ops,

}

​       对于platform平台而言，platform_match函数就是月老，负责驱动和设备的匹配。

#### 10.6.2 platform驱动

​       结构体为platform_driver，结构体内容为：

struct platform_driver {

​       **int (\*probe)(struct platform_device *);**

​       int (*remove)(struct platform_device *);

​       void (*shutdown)(struct platform_device *);

​       int (*suspend)(struct platform_device *, pm_message_t state);

​       int (*resume)(struct platform_device *);

​       **struct device_driver driver;** 

**->** **const struct of_device_id     \*of_match_table;**

**-> const char              \*name;**

​       **const struct platform_device_id \*id_table;**

​       bool prevent_deferred_probe;

};

​       使用platform_driver_register向内核注册platform驱动。

​       platform_driver_register

​              -> __platform_driver_register   (platform_driver)

​                     -> 设置driver的probe为platform_drv_probe， //如果platform_driver的

​                                                                                                  // probe函数有效的话。

​                     -> driver_register

​                            ->执行device_drive->probe，对于platform总线，也就是platform_drv_probe函数。而platform_drv_probe函数会执行platform_driver下的probe函数。

​       结论：向内核注册platform驱动的时候，如果驱动和设备匹配成功，最终会执行platform_driver的probe函数。

#### 10.6.3 platform设备

结构体platform_device：

struct platform_device {

​       const char      *name;

​       int          id;

​       bool       id_auto;

​       struct device  dev;

​       u32         num_resources;

​       struct resource      *resource;

​       const struct platform_device_id   *id_entry;

​       char *driver_override; /* Driver name to force a match *

​       /* MFD cell pointer */

​       struct mfd_cell *mfd_cell;

​       /* arch specific additions */

​       struct pdev_archdata    archdata;

};

​       1、无设备树的时候，此时需要驱动开发人员编写设备注册文件，使用platform_device_register函数注册设备。

​       2，有设备树，修改设备树的设备节点即可。

​       当设备与platform的驱动匹配以后，就会执行platform_driver->probe函数。

### 10.7 platform匹配过程

​       根据前面的分析，驱动和设备匹配是通过bus->match函数，platform总线下的match函数就是：platform_match。

platform_match     

​       -> of_driver_match_device，设备树

​       -> acpi_driver_match_device ACPI类型的

​       -> platform_match_id 根据platform_driver-> id_table 

​       -> strcmp(pdev->name, drv->name)  //最终的就是比较字符串，就是platform_device->name，和platform_driver->driver->name。无设备树情况下使用。

 

​       1、有设备树的时候：

​       of_driver_match_device

​              -> of_match_device(drv->of_match_table, dev)  //of_match_table非常重要，

类型为of_device_id。

//compatible属性

### 10.8 platform编写注意事项

##### 10.8.1、无设备树

​       两部分：platform_driver、platform_device。以点灯为例。

​       1、编写向platform总线注册设备。编写驱动需要寄存器地址信息，地址信息使用设备信息，定义在platform_device里面，因此需要在驱动里面获取设备中的信息，或者叫资源。使用函数platform_get_resource()

##### 10.8.2、有设备树

​       有设备树的时候设备是由设备树描述的，因此不需要向总线注册设备，而是直接修改设备树。只需要修改设备树，然后编写驱动

​       驱动和设备匹配成功以后，设备信息就会从设备树节点转为platform_device结构体。

​       platform提供了很多API函数去获取设备相关信息的。

## 11.内核自带MISC驱动简介 

​       **1、MISC设备的主设备号为10。**

​       **2、MISC设备 会自动创建cdev，不需要我们手动创建****

​       **3、MISC驱动是基于platform。****

 

​	**MISC驱动编写的核心就是初始化miscdevice结构体变量。然后使用misc_register向内核注册，卸载驱动的时候使用misc_deregister来卸载miscdevice。**

  	 **如果设置miscdevice里面minor为255的话，表示由内核自动分配也给次设备号。**

## 12.INPUT子系统简介 

​       input子系统也是字符设备，input核心层会帮我们注册input字符设备驱动。既然内核已经帮我们写好了input驱动，那我们要干啥呢？需要我们去完善具体的输入设备，完善输入设备的时候就要按照inpu子系统驱动框架的要求来。

###     12.1、input_dev

​       申请并初始化并注册input_dev。使用input_allocate_device申请，evbit表示输入事件，比如按键对应的事件就是EV_KEY，如果要连按，那么还要加EV_REP。

​       设置按键对应的键值，也就是keybit。

​       初始化完成input_dev以后，需要向内核注册。使用input_register_device

####     12.2、事件上报

​       按键按下以后上报事件，比如对于按键而言就是在按键中断服务函数，或者消抖定时器函数里面获取按键按下情况，并且上报，可以使用：

input_event(struct input_dev *dev, unsigned int type, unsigned int code, int value);

​       对于按键而言，也可以使用：

input_report_key

​       使用上面两个函数上报完成输入事件以后，还需要使用input_sync做同步

####     12.3、input_event

​       应用程序可以通过input_event来获取输入事件数据，比如按键值，input_event是一个结构体：

struct input_event {

​       struct timeval time;

​       __u16 type;

​       __u16 code;

​       __s32 value;

};

struct timeval {

​       __kernel_time_t            tv_sec;           /* seconds */

​       __kernel_suseconds_t   tv_usec;  /* microseconds */

};

展开：

struct timeval {

​       long       tv_sec;           /* seconds */

​       long       tv_usec;  /* microseconds */

};

最终将input_event展开以后：

struct input_event {

​       struct timeval {

​              long       tv_sec;           /* seconds */               			32位表示秒

​              long       tv_usec;  /* microseconds */             			 32位表示微秒

};

​       __u16 type;                                                           				 16位的事件类型

​       __u16 code;                                                     				16位的事件码，对于按键而言就是键码

​       __s32 value;                                                    				32位的值，对于按键就是按下或抬起

};

**/* 编号 */ 	/* tv_sec */	 /* tv_usec */	 /* type */ 		/* code */ 		/* value */**
**0000000 	0c41 0000 	d7cd 000c 	0001 		000b 		0001 0000**
**0000010 	0c41 0000 	d7cd 000c 	0000 		0000 		0000 0000**
**0000020 	0c42 0000 	54bb 0000 	0001 		000b 		0000 0000**
**0000030 	0c42 0000 	54bb 0000	 0000	 	0000 		0000 0000** 

**type 为事件类型，查看示例代码 58.1.2.3 可知， EV_KEY 事件值为 1， EV_SYN 事件值为**
**0。因此第 1 行表示 EV_KEY 事件，第 2 行表示 EV_SYN 事件。 code 为事件编码，也就是按键**
**号，查看示例代码 58.1.2.4 可以， KEY_0 这个按键编号为 11，对应的十六进制为 0xb，因此第**
**1 行表示 KEY_0 这个按键事件，最后的 value 就是按键值，为 1 表示按下，为 0 的话表示松开。**
**综上所述，示例代码 58.4.2.1 中的原始事件值含义如下：**
**第 1 行，按键(KEY_0)按下事件。**
**第 2 行， EV_SYN 同步事件，因为每次上报按键事件以后都要同步的上报一个 EV_SYN 事**
**件。**
**第 3 行，按键(KEY_0)松开事件。**
**第 4 行， EV_SYN 同步事件，和第 2 行一样。**



## 13.LCD

# 13.1、Framebuffer设备 

​       RGB LCD屏幕，framebuffer是一种机制，应用程序操作驱动里面LCD显存的一种机制，因为应用程序需要通过操作显存来在LCD上显示字符，图片等信息。

​       通过framebuffer机制将底层的LCD抽象为/dev/fbX，X=0、1、2…，应用程序可以通过操作/dev/fbX来操作屏幕。

​       

​       framebuffer在内核中的表现就是fb_info结构体，屏幕驱动重点就是初始化fb_info里面的各个成员变量。初始化完成fb_info以后，通过register_framebuffer函数向内核注册刚刚初始化以后的fb_info。

​       卸载驱动的时候调用unregister_framebuffer来卸载前面注册的fb_info

 

 

# 13.2、LCD驱动简析

 

​       驱动文件为mxsfb.c，为platform驱动框架，驱动和设备匹配以后，mxsfb_probe函数就会执行。

​       结构体mxsfb_info，    

给mxsfb_info申请内存，申请fb_info，然后将这两个联系起来。

host.base就是内存映射以后的LCDIF外设基地址。

mxsfb_probe函数会调用mxsfb_init_fbinfo来初始化fb_info。

 

 

fb_ops

 

mxsfb_probe函数重点工作：

1、初始化fb_info并且向内核注册

2、初始化LCDIF控制器。

 

 

mxsfb_init_fbinfo_dt函数会从设备树中读取相关属性信息：