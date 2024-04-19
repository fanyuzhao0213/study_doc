# 			RTT 归纳总结

## 1、RTT启动流程

在这里 `$Sub$$main` 函数调用了 rtthread_startup() 函数

```

int rtthread_startup(void)
{
    rt_hw_interrupt_disable();

    /* 板级初始化：需在该函数内部进行系统堆的初始化 */
    rt_hw_board_init();

    /* 打印 RT-Thread 版本信息 */
    rt_show_version();

    /* 定时器初始化 */
    rt_system_timer_init();

    /* 调度器初始化 */
    rt_system_scheduler_init();

#ifdef RT_USING_SIGNALS
    /* 信号初始化 */
    rt_system_signal_init();
#endif

    /* 由此创建一个用户 main 线程 */
    rt_application_init();

    /* 定时器线程初始化 */
    rt_system_timer_thread_init();

    /* 空闲线程初始化 */
    rt_thread_idle_init();

    /* 启动调度器 */
    rt_system_scheduler_start();

    /* 不会执行至此 */
    return 0;
}

```

启动调度器之前，系统所创建的线程在执行 rt_thread_startup() 后并不会立马运行，它们会处于就绪状态等待系统调度；待启动调度器之后，系统才转入第一个线程开始运行，根据调度规则，选择的是就绪队列中优先级最高的线程。

rt_hw_board_init() 中完成系统时钟设置，为系统提供心跳、串口初始化，将系统输入输出终端绑定到这个串口，后续系统运行信息就会从串口打印出来。

main() 函数是 RT-Thread 的用户代码入口，用户可以在 main() 函数里添加自己的应用。

用户自定义main函数线程的参数

#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE 2048
#define RT_MAIN_THREAD_PRIORITY 10

## 2.程序内存分布

```
compiling my_uart.c...
linking...
Program Size: Code=97040 RO-data=15220 RW-data=1576 ZI-data=35168  
FromELF: creating hex file...
```

解释：

1> code : 代码段，存放程序的代码部分；

2> RO-data: 只读数据段，存放程序中定义的常量；

3> RW-data: 读写数据段，存放初始化为非0的全局变量；

4> ZI-data: 0数据段，存放未初始化的全局变量以及初始化为0的变量；

编译完生成的map文件

```

```

说明：

1> 	RO Size 包含了Code以及RO-data，表示程序占用flash空间的大小。

2>	RW Size包含了RW-data以及ZI-data,表示运行时所占用的RAM的大小。

3>  	ROM Size 包含了 Code、RO-data 以及 RW-data，表示烧写程序所占用的 Flash 空间的大小；。		

STM32 在上电启动之后默认从 Flash 启动，启动之后会将 RW 段中的 RW-data（初始化的全局变量）搬运到 RAM 中，但不会搬运 RO 段，即 CPU 的执行代码从 Flash 中读取，另外根据编译器给出的 ZI 地址和大小分配出 ZI 段，并将这块 RAM 区域清零。



## 3、IPC通信总结

**IPC：inter process communication    //进程间通信**

**线程间通信**: 邮箱mb消息队列msq；**<线程间或中断与线程间传输一定的数据（通信）>**

**线程间同步**：信号量sem；互斥量mutex；事件集event；**<只需要标记一种状态，并不需要传输具体的数据（同步）>**

### 3.1、RTT通信机制

消息队列、邮箱、信号量、互斥量、事件。

数据类：消息队列，邮箱。

消息队列：传输的数据大小用户自定义；邮箱：只能传输4个字节的数据。

非数据类：信号量，互斥量，事件。

信号量，互斥量是实现一对一的同步，事件可以实现1对多，多对多的同步。

### 3.2、各种通信IPC特点总结

#### 3.2.1、邮箱（mailbox）

- 一封邮件仅可传输4字节数据内容；
- 发送和接受邮件都支持超时阻塞机制（send函数无阻塞，send_wait函数有阻塞，recv函数有阻塞）；
- 线程、中断都可发送邮件，但注意，中断服务函数不允许任何阻塞，所以中断发送邮件必选选择非阻塞方式；
- 中断不能接受邮件；
- 一个线程可从任何一个邮箱接收和发送邮件；
- 多个线程可以向同一个邮箱发送和接收邮件；
- FIFO、PRIO两种阻塞唤醒方式；
- 若邮箱中有邮件，接收线程接收时，其实是复制邮箱中4字节的内容到接收地址中；

#### 3.2.2、消息队列（msq）

- 消息队列是邮箱的扩展，可接收不固定长度的消息；
- 读消息队列（recv）支持超时阻塞机制（send无阻塞，recv有阻塞）；
- 线程、中断都可以发送消息给消息队列；
- 中断服务函数中不可接收消息，因为中断不可阻塞；
- 1个线程可以从任何1个消息队列接收和发送邮件；
- 多个线程可以向同一个消息队列接收和发送邮件；
- FIFO,PRIO2种阻塞唤醒方式；
- 支持发送紧急消息，将消息放在队列的头部；（正常是放尾部）
- 当空闲消息链表上有空闲消息块时，线程或中断将消息复制到该消息块上，然后将该消息块挂在消息队列的尾部。若无空闲消息块，要发送消息的线程和中断会受到错误码；

#### 3.2.3 、信号量（semaphore）

- 信号量值非负，value值表示当前可用资源数，不传输数据内容，常用来同步；
- 二值信号量取值范围0-1；多值信号量取值范围0-65535；
- 获取信号量有超时阻塞机制（take函数有阻塞，try_take无阻塞，release函数无阻塞）
- 线程和中断都可以释放信号量，信号量无所有权，任何线程和中断都可以释放
- 获取信号量则value-1；释放信号量则value+1；
- FIFO、PRIO2种阻塞唤醒方式
- 一般不用信号量做互斥，容易出现优先级翻转问题；

#### 3.2.4 、互斥量（mutex）

- 初始化时，value=1，表示互斥量处于开锁状态，可被持有。常用来保护临界资源；
- 互斥量只能在线程中使用，不可在中断中使用；
- 获取互斥量有超时阻塞机制（take函数有阻塞，release函数无阻塞）；
- 互斥量有所有权，其只能被持有线程释放，该线程具有此互斥量的所有权，其余线程不能同时持有该互斥量；
- 已持有互斥量的线程，再次获取该互斥量时，不会挂起，但持有量计数加1；
- 保证每个时刻只有一个线程正在访问该互斥量保护的临界资源（闭锁）；其他任何优先级的线程想访问该资源时都会阻塞；
- 优先级继承：暂时提高持有互斥量线程的优先级，执行完毕后，释放互斥量时，恢复原有优先级。该方法不会彻底解决优先级翻转问题，只是缓解影响。

#### 3.2.5、事件集（event）







## 10、邮箱

### 10.1邮箱初始化

```
  rt_err_t rt_mb_init(rt_mailbox_t mb,
                    const char* name,
                    void* msgpool,
                    rt_size_t size,
                    rt_uint8_t flag)
```

**初始化邮箱时，该函数接口需要获得用户已经申请获得的邮箱对象控制块，缓冲区的指针，以及邮箱名称和邮箱容量（能够存储的邮件数）**

| mb      | 邮箱对象的句柄                                               |
| ------- | ------------------------------------------------------------ |
| name    | 邮箱名称                                                     |
| msgpool | 缓冲区指针                                                   |
| size    | 邮箱容量                                                     |
| flag    | 邮箱标志，它可以取如下数值： RT_IPC_FLAG_FIFO 或 RT_IPC_FLAG_PRIO |

eg :

```
static struct rt_mailbox MailboxSystemExcept;
#define MAX_MAIL_COUNT (8)
static rt_uint8_t MailboxPool[4*MAX_MAIL_COUNT];

result = rt_mb_init(&MailboxSystemExcept, "MailboxSystemExcept",
                    &MailboxPool, MAX_MAIL_COUNT, RT_IPC_FLAG_FIFO);
if (result != RT_EOK)
{
    rt_kprintf("Init %s Mailbox failed!", "MailboxSystemExcept");
    return RT_ERROR;
}
```

### 10.2 邮箱发送

```
rt_err_t rt_mb_detach(rt_mailbox_t mb);
```

```
rt_err_t rt_mb_send_wait (rt_mailbox_t mb,
                      rt_uint32_t value,
                      rt_int32_t timeout);   /*等待方式发送邮件*/
```

rt_mb_send_wait() 与 rt_mb_send() 的区别在于有等待时间，如果邮箱已经满了，那么发送线程将根据设定的 timeout 参数等待邮箱中因为收取邮件而空出空间。如果设置的超时时间到达依然没有空出空间，这时发送线程将被唤醒并返回错误码。

```
rt_err_t rt_mb_urgent (rt_mailbox_t mb, rt_ubase_t value);  
/*发送紧急邮箱，插入邮箱队头*/
```

### 10.3邮箱接收

```
rt_err_t rt_mb_recv (rt_mailbox_t mb, rt_uint32_t* value, rt_int32_t timeout);
```

只有当接收者接收的邮箱中有邮件时，接收者才能立即取到邮件并返回 RT_EOK 的返回值，否则接收线程会根据超时时间设置，或挂起在邮箱的等待线程队列上，或直接返回。

## 11.空闲线程





问题汇总:

一般 rt-thread 发布的 bsp 库默认的 rt_kprintf 函数的输出设备是串口1，想要更改输出设备为串口1，以 stm32 为例步骤如下：

　　首先，打开 UART2 设备

　　其次，在 menuconfig 中 RT-Thread Kernel --- Kernel Device Object --- Using console for rt_kprintf 修改 the device name for console 的值为 uart2

　　最后，在文件 <stm32f1xx_hal_msp.c> 中加入串口2相关的时钟、引脚配置信息即可