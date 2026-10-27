# STM32 + FreeRTOS & Linux 系统编程练习

哈工大仪器学院 · 嵌入式方向个人项目（陈烈）

基于 **STM32F103C8T6 真机** 的 FreeRTOS 多任务开发，与 Ubuntu(WSL) 环境下的 Linux 系统编程实践合集。

## 项目亮点

- **FreeRTOS 多任务真机验证**：LED / 按键 / UART / SPI 四任务同时调度，非仿真
- **SPI 硬件回环测试**：MOSI-MISO 短接，5 组边界数据全过
- **队列消息链路**：按键事件 → 队列 → UART 输出，真机跑通
- **Linux 系统编程**：文件 IO / 进程 / 线程 / Socket 网络编程完整练习
- **工程化工具链**：Makefile 多文件管理、GDB 调试、arm-none-eabi 交叉编译

## 目录结构

```
├── freertos/          STM32 + FreeRTOS 核心源码
│   ├── main.c         FreeRTOS 四任务 + SPI 回环 + 队列消息
│   └── FreeRTOSConfig.h
└── linux/             Linux 系统编程练习
    ├── fileio.c       文件 IO（open/read/write）
    ├── fork.c         进程创建（fork/wait）
    ├── exec.c         exec 程序替换
    ├── thread.c       pthread 多线程
    ├── thread_mutex.c 互斥锁（数据竞争修复）
    ├── tcp_server.c   TCP server（socket/bind/listen/accept）
    ├── udp_server.c   UDP server（recvfrom/sendto）
    ├── mk/            Makefile 多文件工程
    └── shell/         Shell 脚本练习
```

## freertos/ 说明

- 硬件：STM32F103C8T6 最小系统板 + ST-LINK SWD 烧录
- 功能：
  - FreeRTOS 任务创建与优先级调度（LED/KEY/UART/SPI 四任务）
  - 队列消息传递：按键 → 队列 → UART 输出
  - SPI1 主机模式硬件回环（PA5 MOSI 短接 PA6 MISO）
- 真机验证结果：
  - UART 115200 收发正常
  - SPI 回环 5 组边界数据（0xFF / 0x00 / 0xAA / 0x55 / 0x12）全部收发一致
- 注：完整工程基于 STM32 标准外设库 + FreeRTOS 内核，此处仅保留自写核心代码

## linux/ 说明

| 文件 | 内容 | 验证 |
|---|---|---|
| fileio.c | 文件读写 | 运行正常 |
| fork.c | 进程创建 | 运行正常 |
| exec.c | 程序替换 | 运行正常 |
| thread.c | 多线程 | 运行正常 |
| thread_mutex.c | 互斥锁 | counter 97798 → 加锁后 200000 |
| tcp_server.c | TCP 服务端 | nc 客户端实测通过 |
| udp_server.c | UDP 服务端 | nc 客户端实测通过 |
| mk/ | Makefile 工程 | make 编译通过 |
| shell/ | 脚本练习 | 运行正常 |

## 环境

- MCU 开发：Keil MDK5（ARMCC V5.06）+ ST-Link V2
- Linux：Ubuntu 24.04（WSL2）+ gcc
- 交叉编译：arm-none-eabi-gcc

## 待办

- [ ] I2C 外设（OLED 驱动）
- [ ] LoRa 无线通信（E22-900MM22S）
