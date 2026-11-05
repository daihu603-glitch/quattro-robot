# Arduino 使用流程详解（以 Arduino IDE 2.x + Arduino Uno 为例）

Arduino 把硬件开发板、简化的 C/C++ 框架和 Arduino IDE 结合在一起，上手门槛低。整体流程：

**安装 IDE → 连接开发板 → 选择板型和端口 → 编写程序 → 验证编译 → 上传 → 串口调试**

---

## 1. 准备工作

### 1.1 安装 Arduino IDE

1. 打开 Arduino 官网（arduino.cc）的 Software 页面，下载适合你系统的 Arduino IDE 2.x。
2. 按提示安装。Windows 下安装过程中会询问是否安装驱动，选择同意。
3. 官网下载较慢时，也可以使用国内镜像站，但建议确认文件来源可靠。

### 1.2 连接开发板

- 用 USB 线把 Arduino 连接到电脑。
- 板子上的电源指示灯（ON）亮起，表示供电正常。
- 注意：有些 USB 线只能充电，不能传输数据。连接后识别不到端口时，先换一根数据线试试。

### 1.3 驱动问题（国产兼容板常见）

| 板子使用的串口芯片 | 需要安装的驱动 |
|---|---|
| ATmega16U2（官方 Uno） | 通常自动识别 |
| CH340 / CH341（常见兼容板） | 安装 CH340 驱动 |
| CP2102 | 安装 CP210x 驱动 |
| FT232 | 安装 FTDI 驱动 |

---

## 2. 选择开发板和端口

1. 打开 IDE，顶部工具栏有一个开发板选择下拉框，点击后选择 `Arduino Uno`（按实际板型选择）。
2. 选择板型后，选择对应的串口（Windows 显示为 `COMx`，macOS 显示为 `/dev/cu.usbmodemXXXX`，Linux 显示为 `/dev/ttyUSB0` 或 `/dev/ttyACM0`）。
3. 也可以通过菜单 `Tools → Board` 和 `Tools → Port` 设置。
4. 找不到端口时，对比插拔 USB 前后端口列表的变化，多出来的那一项就是你的板子。

### 安装其他类型的开发板（如 ESP32、ESP8266）

1. 打开 `File → Preferences`，在 **Additional boards manager URLs** 中填入对应开发板的包索引地址。
2. 打开 `Tools → Board → Boards Manager`，搜索并安装对应的包（如 `esp32`）。
3. 安装完成后，在开发板列表里选择具体型号。

---

## 3. 程序结构

每个 Arduino 程序（称为 sketch）都有两个必需的函数：

```cpp
void setup() {
  // 上电或复位后只执行一次，用于初始化
}

void loop() {
  // 初始化完成后无限循环执行
}
```

### 示例：LED 闪烁（Blink）

```cpp
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);   // 设置板载 LED 引脚为输出
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); // 点亮
  delay(1000);                     // 等待 1 秒
  digitalWrite(LED_BUILTIN, LOW);  // 熄灭
  delay(1000);                     // 等待 1 秒
}
```

可以直接从菜单打开官方示例：`File → Examples → 01.Basics → Blink`。

---

## 4. 验证（编译）与上传

| 按钮 | 作用 |
|---|---|
| ✓ Verify（对勾） | 只编译，检查语法错误，不下载 |
| → Upload（右箭头） | 编译并下载到开发板 |

操作步骤：

1. 点击 Verify，下方输出窗口显示 `Done compiling` 表示编译成功，并给出程序和内存占用。
2. 点击 Upload，板子上的 TX/RX 指示灯会闪烁，输出窗口出现 `Done uploading` 表示下载成功。
3. 上传完成后程序立即开始运行。

---

## 5. 串口调试（Serial Monitor）

串口监视器是最常用的调试手段，可以把程序里的数据打印到电脑上。

### 示例

```cpp
void setup() {
  Serial.begin(9600);            // 初始化串口，波特率 9600
}

void loop() {
  int value = analogRead(A0);    // 读取 A0 引脚的模拟值
  Serial.print("A0 = ");
  Serial.println(value);         // 打印并换行
  delay(500);
}
```

操作步骤：

1. 上传程序后，点击右上角放大镜图标，或按 `Ctrl+Shift+M` 打开 Serial Monitor。
2. 右下角的波特率要与程序中 `Serial.begin()` 的值一致，否则会显示乱码。
3. 在输入框中输入内容并发送，可以向板子发送数据。

---

## 6. 使用库（Library）

库可以快速驱动传感器、显示屏、舵机等模块。

### 方法一：库管理器安装（推荐）

1. 点击左侧边栏的库管理器图标，或使用 `Sketch → Include Library → Manage Libraries`。
2. 搜索库名（如 `Servo`、`DHT sensor library`、`Adafruit SSD1306`）。
3. 点击 Install。部分库会提示同时安装依赖库，选择 Install all。
4. 在代码开头引入：

```cpp
#include <Servo.h>
```

### 方法二：手动安装 ZIP 库

使用 `Sketch → Include Library → Add .ZIP Library...`，选择下载好的库压缩包即可。

---

## 7. 常用函数速查

| 函数 | 作用 |
|---|---|
| `pinMode(pin, mode)` | 设置引脚模式（`INPUT` / `OUTPUT` / `INPUT_PULLUP`） |
| `digitalWrite(pin, value)` | 输出高/低电平 |
| `digitalRead(pin)` | 读取数字引脚电平 |
| `analogRead(pin)` | 读取模拟值（Uno 返回 0–1023） |
| `analogWrite(pin, value)` | 输出 PWM（0–255，仅限带 ~ 标记的引脚） |
| `delay(ms)` | 延时毫秒 |
| `millis()` | 返回上电后经过的毫秒数，可用于不阻塞的计时 |
| `Serial.begin(baud)` | 初始化串口 |
| `Serial.print()` / `println()` | 串口输出 |

> 提示：用 `delay()` 时程序会被阻塞，需要同时处理多个任务时，改用 `millis()` 做非阻塞计时。

---

## 8. 常见问题

| 问题 | 原因与处理 |
|---|---|
| 找不到端口（Port 灰色） | 检查数据线、驱动，换 USB 口 |
| 上传报 `avrdude: stk500_recv(): programmer is not responding` | 板型或端口选错、驱动未装、线路有问题；也可能是 0、1 引脚外接了设备占用串口 |
| 编译报 `'xxx' was not declared in this scope` | 变量或函数拼写错误，或缺少 `#include` |
| 编译报 `xxx.h: No such file or directory` | 没有安装对应的库 |
| 串口监视器显示乱码 | 波特率与程序里的设置不一致 |
| 上传成功但程序没有反应 | 检查接线、引脚编号，用 Serial 打印调试 |
| 程序运行一段时间后异常 | 注意 RAM 占用，字符串尽量用 `F()` 宏存放在 Flash 中 |

---

## 9. 快速流程清单

- [ ] 安装 Arduino IDE 和驱动
- [ ] 用数据线连接开发板
- [ ] 选择正确的开发板型号和端口
- [ ] 编写或打开示例程序
- [ ] 点击 Verify 确认编译通过
- [ ] 点击 Upload 下载到板子
- [ ] 需要调试时打开 Serial Monitor，波特率保持一致
- [ ] 用到传感器或模块时，通过库管理器安装对应的库

---

## 10. 与 STM32CubeIDE 的简单对比

| 项目 | Arduino | STM32CubeIDE |
|---|---|---|
| 上手难度 | 很低，几行代码就能运行 | 较高，需要理解时钟、外设和 HAL |
| 配置方式 | 代码为主，底层已封装 | 图形化配置加代码 |
| 适合场景 | 快速原型、教学、小项目 | 需要精细控制外设和性能的项目 |
| 调试能力 | 以串口打印为主 | 支持断点、单步和寄存器查看 |
