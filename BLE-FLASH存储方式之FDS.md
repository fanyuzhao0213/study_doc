## BLE->FLASH存储方式之FDS

### 1. 蓝牙FLASH存储FDS模块介绍

- 自查

### 2. FDS 库函数 API 详解 

FDS 已经是脱离了底层的内部 FLASH 读写方式， 这种方式需要通过官方提供的
FDS 驱动库编写。 那么首先我们就来认识下 FDS 驱动库提供的 API 函数： 

#### 2.1  FDS 注册函数 fds_register 

ret_code_t fds_register(fds_cb_t cb);

**功能**： 功能是注册一个 FDS 事件回调处理函数。

* 你可以注册最大数量的回调函数， 这个最大数量的值可以通过配置 fds_config.h 文件中的参数
  FDS_MAX_USERS 来实现。

**参数**： cb 事件回调处理功能

**返回值**： FDS_SUCCESS 如果回调处理函数注册成功

​		FDS_ERR_USER_LIMIT_REACHED 如何达到可最大数量的回调注册函数

#### 2.2 FDS 统计函数 fds_stat 

ret_code_t fds_stat(fds_stat_t * p_stat);

**功能**： 检索文件系统统计信息的函数

* 这个函数检索文件系统统计信息， 例如打开记录的数量， 可以通过垃圾收集回收的空间， 等等。

**参数[out]**： p_stat 文件系统统计数据。

**返回值**： FDS_SUCCESS 如果统计数据被成功返回。

​		FDS_ERR_NOT_INITIALIZED 如果模块没有初始化。

 		FDS_ERR_NULL_ARG 如果参数 p_stat 是空的.

#### 2.3 FDS 初始化函数 fds_init 

ret_code_t fds_init(void);

***功能** ：初始化功能模块组件 Function for initializing the module.

- 这个功能是初始化功能组件和安装文件系统（除非文件系统已经安装）

* 这个功能是异步的， 完成报告通过事件来实现。 在你调用函数 fds_init 初始化之前， 确保首先调
  用 fds_register 函数， 这样你就能收到完成事件。

**返回值** ：FDS_SUCCESS 如果操作被成功排队。

​	         FDS_ERR_NO_PAGES 如果在闪存 flash 中没有可用的空间来安装文件系统。

#### 2.4 FDS 记录搜索函数 fds_record_find 

函数： ret_code_t fds_record_find(	uint16_t file_id,
​								uint16_t record_key,
​								fds_record_desc_t * p_desc,
​								fds_find_token_t * p_token);

**功能描述**： 在文件中通过给定记录键 key 搜索记录的函数。

- 这个函数的功能能找到了具有给定记录键 key 的文件中的第一个记录。要在文件中使用相同的键key 搜索下一个记录， 再次调用该函数并提供相同的功能。 从最后的记录中恢复搜索的fds_find_token_t 结构。

**参数[in]**  file_id 文件标识 ID。**File ID不能设置为0xFFFF，0xFFFF标识无效的file id。**

​		record_key 记录键 key。**record key 不能设置为0x0000,0x0000表示该record是无效的。**

​		p_desc 找到的记录的描述符。

 		p_token 包含关于操作进展的信息的记录。

**返回值**： FDS_SUCCESS 如果找到一个记录

​		 FDS_ERR_NOT_INITIALIZED 如果模块没有初始化。

​		 FDS_ERR_NULL_ARG 如果参数 p_desc 或者参数 p_token 是空的.

​		 FDS_ERR_NOT_FOUND 如果没有找到匹配的记录

#### 2.5 FDS 记录删除函数 fds_record_delete 

函数： ret_code_t fds_record_delete(fds_record_desc_t * p_desc);

**功能描述** 删除记录的函数

- 删除的记录不能使用 参数 fds_record_find, 参数 fds_record_find_by_key, or 参数fds_record_find_in_file. 此外， 他们不能再使用 参数 fds_record_open.
- 注意， 删除一个记录并不能释放它在闪存中占用的空间。 要回收被删除的记录所使用的 flash 空间， 调用@ref fds_gc 去进行垃圾回收.
- *这个函数是异步的。 完成是通过一个事件报告的， 该事件被发送到注册的事件处理程序函数。

**参数[in]** p_desc 应该删除的记录的描述符。

**返回值**： FDS_SUCCESS 如果操作成功地排队。
​	 	FDS_ERR_NOT_INITIALIZED 如果模块没有初始化。
​		FDS_ERR_NULL_ARG 如果指定的记录描述符参数 p_desc 为空.
​		FDS_ERR_NO_SPACE_IN_QUEUES 如果操作队列满了。

#### 2.6 FDS 记录写入函数 fds_record_write 

函数： ret_code_t fds_record_write( fds_record_desc_t * p_desc,
​								fds_record_t const * p_record);

**功能描述** : 写入一个记录到闪存的函数。

* 这个函数对于文件 ID 和记录键 key 没有限制,除了记录键必须不同于参数
  FDS_RECORD_KEY_DIRTY 和文件 ID 必须不同于参FDS_FILE_ID_INVALID。
* 特别地， 对于文件 ID 或记录键 key 的唯一性没有任何限制。 所有具有相同文件 ID 的记录都被分组到一个文件中。 如果没有带有指定 ID 的文件， 则会创建它。 在一个文件中可以有多个记录，具有相同的记录键 key。
* 有些模块需要使用特定的文件 id 和记录键 key。 看参数 lib_fds_functionality_keys 相关详细信息.
* 记录数据可以由多个块组成。 数据必须与 4 字节的边界对齐， 并且由于它没有内部缓冲， 所以必须将其保存在内存中， 直到收到操作的回调为止。 数据的长度不能超过参数FDS_VIRTUAL_PAGE_SIZE 的字长度减去 14 字节。
* 这个函数是异步的。 完成是通过一个事件报告的， 该事件被发送到注册的事件处理程序函数。

**参数[out]** p_desc 所写的记录的描述符。 如果您不需要描述符， 则传递 NULL。

**参数[in]** 	  p_record 将被写入 flash 的记录。

返回值： FDS_SUCCESS 如果操作成功地排队。

 		FDS_ERR_NOT_INITIALIZED 如果模块没有初始化。

 		FDS_ERR_NULL_ARG 如果参数 p_record 是空.

​		FDS_ERR_INVALID_ARG 如果文件 ID 或记录键无效。

 		FDS_ERR_UNALIGNED_ADDR 如果记录数据不对齐到 4 字节的边界。

 		FDS_ERR_RECORD_TOO_LARGE 如果记录数据超过了最大长度。

 		FDS_ERR_NO_SPACE_IN_QUEUES 如果操作队列满了， 或者有更多的记录块，而不是可以缓冲的。

 		FDS_ERR_NO_SPACE_IN_FLASH 如果在 flash 中没有足够的空闲空间来存储记录。

#### **2.7 FDS 的记录阅读函数 fds_record_open** 

函数： ret_code_t fds_record_open( fds_record_desc_t * p_desc,
​								fds_flash_record_t * p_flash_record);

**功能描述** ：用于打开阅读记录的功能。

* 这个函数打开一个存储在 flash 中的记录， 这样就可以读取它了。 这个函数初始化一个参数fds_flash_record_t 结构， 它可以用来访问记录数据以及它的相关元数据。 在参数fds_flash_record_t 结构中提供的指针是指向闪存的针。
* 用操作 fds_record_open 打开一个记录， 可以防止垃圾收集在存储记录的虚拟闪存页面上运行，这样， 只要记录保持打开， 就保证在参数 fds_flash_record_t 中由字段指向的内存内容不会被修改。
* 当您完成读取记录时， 请调用操作 fds_record_close 关闭它。 垃圾收集可以在记录存储的虚拟页面上回收空间。 请注意， 您必须为操fds_record_close 提供相同的描述符， 就像您为本函数所做的那样。

**参数[in]**    p_desc 打开的记录的描述符.

**参数[out]** p_flash_record 记录， 存储在 flash 中

**返回值：** FDS_SUCCESS 如果记录成功打开.

​		FDS_ERR_NULL_ARG 如果参数 p_desc 或者参数 p_flash_record 是空的.

 		FDS_ERR_NOT_FOUND 如果没有找到记录。 它可能已经被删除了， 或者它可能还没有被写出来。

​		FDS_ERR_CRC_CHECK_FAILED 如果对记录的 CRC 校验失败了。

#### 2.8 FDS 记录关闭函数 fds_record_close 

函数： ret_code_t fds_record_close(fds_record_desc_t * p_desc);

**功能描述** ：闭合记录的功能

* 关闭一个记录可以让垃圾收集在记录存储的虚拟页面上运行（如果该页面上没有其他记录） 。 作为一个参数传递的描述符必须与使用操作 fds_record_open 打开记录的那个相同。
* 请注意， 关闭一个记录并不能使其描述符无效。 您仍然可以向所有接受记录描述符作为参数的函数提供描述符。

**参数[in]** p_desc 记录的描述符被关闭。

返回值： FDS_SUCCESS 如果记录被成功关闭。

​		 FDS_ERR_NULL_ARG 如果参数 p_desc 为空的。

​		 FDS_ERR_NO_OPEN_RECORDS 如果记录没有打开。

​		 FDS_ERR_NOT_FOUND 如果无法找到记录。

#### 2.9 FDS 记录更新函数 fds_record_update 

函数： ret_code_t fds_record_update(	fds_record_desc_t * p_desc,
​									fds_record_t const * p_record);

**功能描述**： 用于更新记录的函数。

- 更新一个记录首先会写一个新的记录（参数 precord） 到 flash， 然后删除旧记录（由参数 p_desc识别） 。

- *文件 ID 和记录钥匙 key 没有任何限制， 除了记录键必须与参数 FDS_RECORD_KEY_DIRTY 不同， 并且文件 ID 必须与参数 FDS_FILE_ID_INVALID 不同。 特别地， 对于文件 ID 或记录钥匙 key的唯一性没有任何限制。 所有具有相同文件 ID 的记录都被分组到一个文件中。 如果没有带有指定ID 的文件， 则会创建它。 在一个文件中可以有多个记录， 具有相同的记录键。
- 记录数据可以由多个块组成。 数据必须与 4 字节的边界对齐(字对齐）， 并且由于它没有内部缓冲，所以必须将其保存在内存中， 直到收到操作的回调为止。 数据的长度不能超过参数FDS_VIRTUAL_PAGE_SIZE 的定义字长减去 14 字节。
- 这个函数是异步的。 完成是通过一个事件报告的， 该事件被发送到注册的事件处理程序函数。

**参数[in, out]** p_desc 记录更新的描述符。 当函数返回 FDS_SUCCESS 时， 该参数包含新写入记录的描述符。

**参数[in]** 		p_record 更新的记录将被写入 flash。

**返回值**： FDS_SUCCESS 如果操作成功地排队。

 		FDS_ERR_NOT_INITIALIZED 如果模块没有初始化。

​		FDS_ERR_INVALID_ARG 如果文件 ID 或记录键 key 无效。

 		FDS_ERR_UNALIGNED_ADDR 如果记录数据不对齐到 4 字节的边界（不是字对齐） 。

​		FDS_ERR_RECORD_TOO_LARGE 如果记录数据超过了最大长度。

​		FDS_ERR_NO_SPACE_IN_QUEUES 如果操作队列满了， 或者有更多的记录块，而不是可以缓冲的。

​		FDS_ERR_NO_SPACE_IN_FLASH 如果 flash 中没有足够的空闲空间来存储更新的记录。

### 3. FDS 方式编程方法 

#### 3.1FDS 配置参数 

对应 FDS 初始化的时候实际上需要首先在 sdk_config.h 文件中对 FDS 进行使能， 同时配置下面 5 种参数， 我们先进行介绍下： 

**#define FDS_ENABLED 1** 

​	对 FDS 进行使能， 在实现 FDS 库函数之前， 需要首先将其设置为 1。 

**#define FDS_VIRTUAL_PAGES 3
#define FDS_VIRTUAL_PAGE_SIZE 1024** 

这两个参数用于配置要使用的虚拟页面数量及其大小。
FDS_VIRTUAL_PAGES -要使用的虚拟 flash 页面的数量。 系统为垃圾收集预留了一个虚拟页面。 因此， 最少是两个虚拟页面:一个用于存储数据的页面和一个用于系统垃圾收集的页面。 FDS使用的闪存总量为@ref FDS_VIRTUAL_PAGES * @ref FDS_VIRTUAL_PAGE_SIZE * 4 字节
FDS_VIRTUAL_PAGE_SIZE -虚拟 flash 页面的大小。 用 4 字节的倍数表示。 默认情况下，
虚拟页面的大小与物理页面相同。 虚拟页面的大小必须是物理页面大小的倍数。 

**#define FDS_BACKEND 2**

配置 nrf_fstorage 后台被 FDS 模式用于写入 flash。
参数选择为 NRF_FSTORAGE_NVMC 时， FDS_BACKEND 定义为 1
参数选择为 NRF_FSTORAGE_SD 时， FDS_BACKEND 定义为 2
使用 SoftDevice API 使用 NRF_FSTORAGE_SD 实现后端， 如果你使用协议栈的时候， 就用
这个。
使用外设的时候使用 nrf_fstorage_nvmc 实现后端， 如果你没有使用协议栈的时候， 就用这个。 

**#define FDS_OP_QUEUE_SIZE 4**

内部队列的大小。 如果您经常得到同步的 FDS_ERR_NO_SPACE_IN_QUEUES 错误， 请增
加这个值 

**#define FDS_CRC_CHECK_ON_READ 1
#define FDS_CRC_CHECK_ON_WRITE 0** 

FDS_CRC_CHECK_ON_READ -使能 CRC 检查。 当记录写入闪存时保存记录的 CRC， 并在
记录打开时检查它。 使用 FDS 函数的用户仍然可以“看到” 不正确的 CRC 记录， 但是不能打开它
们。 此外， 它们在被删除之前不会被垃圾收集。
FDS_CRC_CHECK_ON_WRITE -对新记录进行 CRC 检查。 此设置可用于确保记录数据在写
入 flash 时不会发生更改。 

**#define FDS_MAX_USERS 4**

可以注册的回调的最大数量。 

#### 3.2 FDS 配置流程 

- 1、FDS 配置首先是注册一个 FDS 事件回调处理函数。 你可以注册最大数量的回调函数， 这个最						  			大数量的值可以通过前面配置 fds_config.h 文件中的参数 FDS_MAX_USERS 来实现。
- 2、就是初始化 FDS 功能组件和安装文件系统， FDS 内部实际上是把文件存储和读取当做类似的一个 fileystem 文件系统处理。 这个功能是异步的， 完成报告通过事件来实现。FDS_EVT_INIT 事件报告初始化完成， 完成后我们把标志位 m_fds_initialized 置位。 因此需要通过判断 m_fds_initialized 是否置位来判断初始化是否成功， 没成功之前系统代码， 处于 system on 状态。 (注意： 在你调用函数 fds_init 初始化之前， 确保首先调用 fds_register 函数， 这样你就能收到完成事件。 ） 

- 3、 用 fds_stat 函数检索文件系统统计信息， 例如打开记录的数量， 可以通过垃圾收集回收的空间等。 这些数据提供一个结构体， 其结构如下所示， 可以通过这个函数观察此时 FDS 所处的状态：

```
typedef struct
{
	uint16_t pages_available; //可用页数
	uint16_t open_records; //打开记录的数量
	uint16_t valid_records; //有效记录的数量
	uint16_t dirty_records; //删除(“脏” )记录的数量
	uint16_t words_reserved; //通过参数函数 fds_reserve()保留的字数.
	uint16_t words_used;//给 flash 写的字的数量， 包括那些预留给将来写的字
	//该参数表示文件系统中最大的自由连续字数。 这个数字表示 FDS 可以存储的最大记录。
它考虑了为将来写所预留的空间。
	uint16_t largest_contig;
	//碎片回收后可以释放的最大的字节数。 如果在运行垃圾收集时， 记录是打开的， 那么碎片
收集后释放的实际空间数量可能小于此值。
	uint16_t freeable_words;
	//检测到文件系统损坏。 检测到一个或多个损坏的记录。 FDS 将自动修复文件系统，
下次碎片收集时， 可能会丢失一些数据。 此标志与 CRC 是否正确无关
    16. bool corruption;
	} fds_stat_t;
```

- 4、开始测试。 可以先使用函数 fds_record_find 找到指定记录， 再使用 fds_record_delete先清除所有记录。 然后再声明记录描述符 fds_record_desc_t 和操作进展的信息的令牌fds_find_token_t。 这两个描述符简单进行说明： 

```
功能： 用来操作记录的记录描述符结构。 FDS 模式使用这种结构。 在操作现有记录时， 必须向 FDS
模块提供描述符。 但是， 您不应该修改它或使用它的任何字段。 永远不要为不同的记录重用相同的
描述符。
typedef struct
{
	uint32_t record_id; 		//唯一的记录 ID。
	uint32_t const * p_record; 	//在 flash 中最后已知的记录的位置。
	uint16_t gc_run_count; 		//运行碎片收集的次数
	bool record_is_open; 		//记录是否现在打开。
} fds_record_desc_
```

```
功能： 一个令牌， 用来保存关于 fds_record_find 进程的信息， fds_record_find_by_key 和fds_record_find_in_file.总是在第一次使用令牌之前对其进行零初始化。永远不要重用相同的令牌来
搜索不同的记录。
typedef struct
{
	uint32_t const * p_addr;
	uint16_t page;
} fds_find_token_t;
```

这两个参数是用来标志操作记录的和操作的进程。 表征了本次操作， 用于后面整体打开、 删除、
关闭本次记录所使用。
测试 FDS 模块的数据流程就是： 写入数据， 然后查找数据， 找到对应记录后读取数据， 对比
写入和读出的数据是否正确。 



### 参考文档：

https://www.cnblogs.com/iini/p/9338169.html