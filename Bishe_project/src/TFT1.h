/*
<TFT_eSPI.h>版本:2.5.0
配置<TFT_eSPI.h>库中User_Setup.h文件：
ST7789 1.54 240*240:
#define ST7789_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 240

SPI引脚：ESP32-S3
#define TFT_SCLK  18  
#define TFT_MOSI  17
#define TFT_RST   21     
#define TFT_DC    15   
#define TFT_CS    16
*/

/*RGB565 颜色数值
黑色： TFT_BLACK       0x0000      
深蓝色 TFT_NAVY        0x0011   
深绿色 TFT_DARKGREEN   0x03E0     
深青色 TFT_DARKCYAN    0x03EF     
褐红色 TFT_MAROON      0x7800    
紫色   TFT_PURPLE      0x780F     
黄绿色 TFT_OLIVE       0x7BE0     
浅灰色 TFT_LIGHTGREY   0xD69A     
暗灰色 TFT_DARKGREY    0x7BEF     
蓝色   TFT_BLUE        0x001F     
绿色   TFT_GREEN       0x07E0    
青色   TFT_CYAN        0x07FF     
红色   TFT_RED         0xF800    
洋红色 TFT_MAGENTA     0xF81F    
黄色   TFT_YELLOW      0xFFE0     
白色   TFT_WHITE       0xFFFF     
橙色   TFT_ORANGE      0xFDA0    
黄绿色 TFT_GREENYELLOW 0xB7E0     
粉色   TFT_PINK        0xFE19       
褐色   TFT_BROWN       0x9A60      
金色   TFT_GOLD        0xFEA0      
银色   TFT_SILVER      0xC618      
天蓝色 TFT_SKYBLUE     0x05FF 
灰色：0x8410      
*/

#ifndef TFT1_H
#define TFT1_H
#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(240,240);              // 定义彩屏对象（ST7789 1.54 240*240 屏）
TFT_eSprite tfts = TFT_eSprite(&tft);   // 创建缓冲精灵对象，用于缓冲绘图减少闪烁


// ============= 汉字字模数据库 =============
// 以下为 16x16 的汉字字模，采用逐行式、顺向、高位在前、C51 阴码格式
// 每个汉字占 32 字节（16x16 = 256 bits = 32 bytes）
// 可通过 tfts.drawBitmap() 绘制在屏幕上

static const unsigned char PROGMEM zui4[] = {
0x1F,0xF0,0x10,0x10,0x1F,0xF0,0x10,0x10,0x1F,0xF0,0x00,0x00,0xFF,0xFE,0x22,0x00,
0x3E,0xF8,0x22,0x88,0x3E,0x90,0x22,0x50,0x2F,0x20,0xF2,0x50,0x42,0x88,0x03,0x06};/*最*/
static const unsigned char PROGMEM da4[] = {
0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,
0x02,0x80,0x02,0x80,0x04,0x40,0x04,0x40,0x08,0x20,0x10,0x10,0x20,0x08,0xC0,0x06};/*大*/

static const unsigned char PROGMEM dian4[] = {
0x01,0x00,0x01,0x00,0x01,0x00,0x3F,0xF8,0x21,0x08,0x21,0x08,0x21,0x08,0x3F,0xF8,
0x21,0x08,0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x0A,0x01,0x02,0x01,0x02,0x00,0xFE};/*电*/
static const unsigned char PROGMEM ya4[]   = {
0x00,0x00,0x3F,0xFE,0x20,0x00,0x20,0x80,0x20,0x80,0x20,0x80,0x20,0x80,0x2F,0xFC,
0x20,0x80,0x20,0x80,0x20,0x90,0x20,0x88,0x20,0x88,0x40,0x80,0x5F,0xFE,0x80,0x00};/*压*/
static const unsigned char PROGMEM liu2[]  = {
0x00,0x80,0x20,0x40,0x17,0xFE,0x10,0x80,0x81,0x10,0x42,0x08,0x47,0xFC,0x10,0x04,
0x10,0x00,0x22,0x48,0xE2,0x48,0x22,0x48,0x22,0x48,0x22,0x4A,0x24,0x4A,0x08,0x46};/*流*/
static const unsigned char PROGMEM zu3[]  = {
0x00,0x00,0x7D,0xF8,0x45,0x08,0x49,0x08,0x49,0x08,0x51,0xF8,0x49,0x08,0x49,0x08,
0x45,0x08,0x45,0xF8,0x45,0x08,0x69,0x08,0x51,0x08,0x41,0x08,0x47,0xFE,0x40,0x00};/*阻*/
static const unsigned char PROGMEM liang4[]  = {
0x00,0x00,0x1F,0xF0,0x10,0x10,0x1F,0xF0,0x10,0x10,0xFF,0xFE,0x00,0x00,0x1F,0xF0,
0x11,0x10,0x1F,0xF0,0x11,0x10,0x1F,0xF0,0x01,0x00,0x1F,0xF0,0x01,0x00,0x7F,0xFC};/*量*/
static const unsigned char PROGMEM yuan2[]  = {
0x00,0x00,0x27,0xFE,0x14,0x20,0x14,0x40,0x85,0xFC,0x45,0x04,0x45,0xFC,0x15,0x04,
0x15,0xFC,0x25,0x24,0xE4,0x20,0x24,0xA8,0x29,0x24,0x2A,0x22,0x30,0xA0,0x00,0x40};/*源*/

static const unsigned char PROGMEM gong1[]  = {
0x00,0x40,0x00,0x40,0x00,0x40,0xFE,0x40,0x11,0xFC,0x10,0x44,0x10,0x44,0x10,0x44,
0x10,0x44,0x10,0x84,0x10,0x84,0x1E,0x84,0xF1,0x04,0x41,0x04,0x02,0x28,0x04,0x10};/*功*/
static const unsigned char PROGMEM lu4[]  = {
0x02,0x00,0x01,0x00,0x7F,0xFC,0x02,0x00,0x44,0x44,0x2F,0x88,0x11,0x10,0x22,0x48,
0x4F,0xE4,0x00,0x20,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00};/*率*/

static const unsigned char PROGMEM jiao4[] = {
0x10,0x40,0x10,0x20,0x10,0x20,0x11,0xFE,0xFC,0x00,0x10,0x88,0x31,0x04,0x3A,0x02,
0x54,0x88,0x50,0x88,0x90,0x50,0x10,0x50,0x10,0x20,0x10,0x50,0x10,0x88,0x13,0x06};/*校*/
static const unsigned char PROGMEM zhun3[] = {
0x01,0x40,0x41,0x20,0x21,0x20,0x23,0xFE,0x02,0x20,0x16,0x20,0x1B,0xFC,0x12,0x20,
0x22,0x20,0x23,0xFC,0xE2,0x20,0x22,0x20,0x22,0x20,0x23,0xFE,0x22,0x00,0x02,0x00};/*准*/

static const unsigned char PROGMEM mo2[] = {
0x11,0x10,0x11,0x10,0x17,0xFC,0x11,0x10,0xFC,0x00,0x13,0xF8,0x32,0x08,0x3B,0xF8,
0x56,0x08,0x53,0xF8,0x90,0x40,0x17,0xFC,0x10,0xA0,0x11,0x10,0x12,0x08,0x14,0x06};/*模*/
static const unsigned char PROGMEM shi4[]  = {
0x00,0x48,0x00,0x44,0x00,0x44,0x00,0x40,0xFF,0xFE,0x00,0x40,0x00,0x40,0x3E,0x40,
0x08,0x40,0x08,0x40,0x08,0x20,0x08,0x22,0x0F,0x12,0x78,0x0A,0x20,0x06,0x00,0x02};/*式*/

static const unsigned char PROGMEM shi2[] = {
0x00,0x08,0x00,0x08,0x7C,0x08,0x44,0x08,0x45,0xFE,0x44,0x08,0x44,0x08,0x7C,0x08,
0x44,0x88,0x44,0x48,0x44,0x48,0x44,0x08,0x7C,0x08,0x44,0x08,0x00,0x28,0x00,0x10};/*时*/
static const unsigned char PROGMEM jian2[] = {
0x20,0x00,0x13,0xFC,0x10,0x04,0x40,0x04,0x47,0xC4,0x44,0x44,0x44,0x44,0x44,0x44,
0x47,0xC4,0x44,0x44,0x44,0x44,0x44,0x44,0x47,0xC4,0x40,0x04,0x40,0x14,0x40,0x08};/*间*/

static const unsigned char PROGMEM she4[] = {
0x00,0x00,0x21,0xF0,0x11,0x10,0x11,0x10,0x01,0x10,0x02,0x0E,0xF4,0x00,0x13,0xF8,
0x11,0x08,0x11,0x10,0x10,0x90,0x14,0xA0,0x18,0x40,0x10,0xA0,0x03,0x18,0x0C,0x06};/*设*/
static const unsigned char PROGMEM zhi4[] = {
0x7F,0xFC,0x44,0x44,0x7F,0xFC,0x01,0x00,0x7F,0xFC,0x01,0x00,0x1F,0xF0,0x10,0x10,
0x1F,0xF0,0x10,0x10,0x1F,0xF0,0x10,0x10,0x1F,0xF0,0x10,0x10,0xFF,0xFE,0x00,0x00};/*置*/

static const unsigned char PROGMEM bao3[] = {
0x08,0x00,0x0B,0xF8,0x0A,0x08,0x12,0x08,0x12,0x08,0x33,0xF8,0x30,0x40,0x50,0x40,
0x97,0xFC,0x10,0xE0,0x11,0x50,0x12,0x48,0x14,0x44,0x18,0x42,0x10,0x40,0x10,0x40};/*保*/
static const unsigned char PROGMEM cun2[] = {
0x04,0x00,0x04,0x00,0xFF,0xFE,0x08,0x00,0x08,0x00,0x13,0xF8,0x10,0x10,0x30,0x20,
0x50,0x40,0x97,0xFE,0x10,0x40,0x10,0x40,0x10,0x40,0x10,0x40,0x11,0x40,0x10,0x80};/*存*/

static const unsigned char PROGMEM ping2[] = {
0x00,0x00,0x3F,0xF8,0x20,0x08,0x20,0x08,0x3F,0xF8,0x24,0x10,0x22,0x20,0x2F,0xF8,
0x22,0x20,0x22,0x20,0x3F,0xFC,0x22,0x20,0x42,0x20,0x44,0x20,0x84,0x20,0x08,0x20};/*屏*/
static const unsigned char PROGMEM xian3[] = {
0x00,0x00,0x1F,0xF0,0x10,0x10,0x10,0x10,0x1F,0xF0,0x10,0x10,0x10,0x10,0x1F,0xF0,
0x04,0x40,0x44,0x44,0x24,0x44,0x14,0x48,0x14,0x50,0x04,0x40,0xFF,0xFE,0x00,0x00};/*显*/
static const unsigned char PROGMEM fang1[] = {
0x02,0x00,0x01,0x00,0x01,0x00,0xFF,0xFE,0x04,0x00,0x04,0x00,0x04,0x00,0x07,0xF0,
0x04,0x10,0x04,0x10,0x04,0x10,0x08,0x10,0x08,0x10,0x10,0x10,0x20,0xA0,0x40,0x40};/*方*/
static const unsigned char PROGMEM xiang4[] = {
0x02,0x00,0x04,0x00,0x08,0x00,0x7F,0xFC,0x40,0x04,0x40,0x04,0x47,0xC4,0x44,0x44,
0x44,0x44,0x44,0x44,0x44,0x44,0x47,0xC4,0x44,0x44,0x40,0x04,0x40,0x14,0x40,0x08};/*向*/

static const unsigned char PROGMEM jie2[] = {
0x08,0x20,0x08,0x28,0x7F,0x24,0x08,0x20,0xFF,0xFE,0x14,0x20,0x22,0x24,0x7F,0xA4,
0xA4,0x24,0x3F,0x28,0x24,0x28,0x3F,0x10,0x24,0x12,0x3F,0xAA,0x20,0x46,0x20,0x82};/*截*/
static const unsigned char PROGMEM zhi3[] = {
0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x11,0x00,0x11,0x00,0x11,0xF8,0x11,0x00,
0x11,0x00,0x11,0x00,0x11,0x00,0x11,0x00,0x11,0x00,0x11,0x00,0xFF,0xFE,0x00,0x00};/*止*/

static const unsigned char PROGMEM xian4[] = {
0x00,0x00,0x7B,0xF8,0x4A,0x08,0x52,0x08,0x53,0xF8,0x62,0x08,0x52,0x08,0x4B,0xF8,
0x4A,0x44,0x4A,0x48,0x6A,0x30,0x52,0x20,0x42,0x10,0x42,0x88,0x43,0x06,0x42,0x00};/*限*/

static const unsigned char PROGMEM yun4[] = {
0x00,0x00,0x23,0xF8,0x10,0x00,0x10,0x00,0x00,0x00,0x07,0xFC,0xF0,0x40,0x10,0x80,
0x11,0x10,0x12,0x08,0x17,0xFC,0x12,0x04,0x10,0x00,0x28,0x00,0x47,0xFE,0x00,0x00};/*运*/
static const unsigned char PROGMEM xing2[] = {
0x08,0x00,0x09,0xFC,0x10,0x00,0x20,0x00,0x48,0x00,0x08,0x00,0x13,0xFE,0x30,0x20,
0x50,0x20,0x90,0x20,0x10,0x20,0x10,0x20,0x10,0x20,0x10,0x20,0x10,0xA0,0x10,0x40};/*行*/

static const unsigned char PROGMEM nei4[] = {
0x01,0x00,0x01,0x00,0x01,0x00,0x7F,0xFC,0x41,0x04,0x41,0x04,0x41,0x04,0x42,0x84,
0x42,0x44,0x44,0x24,0x48,0x14,0x50,0x14,0x40,0x04,0x40,0x04,0x40,0x14,0x40,0x08};/*内*/
static const unsigned char PROGMEM bu4[] = {
0x10,0x00,0x08,0x3E,0x7F,0xA2,0x00,0x24,0x21,0x24,0x12,0x28,0xFF,0xE4,0x00,0x24,
0x00,0x22,0x3F,0x22,0x21,0x22,0x21,0x34,0x21,0x28,0x3F,0x20,0x21,0x20,0x00,0x20};/*部*/
static const unsigned char PROGMEM wen1[] = {
0x00,0x00,0x23,0xF8,0x12,0x08,0x12,0x08,0x83,0xF8,0x42,0x08,0x42,0x08,0x13,0xF8,
0x10,0x00,0x27,0xFC,0xE4,0xA4,0x24,0xA4,0x24,0xA4,0x24,0xA4,0x2F,0xFE,0x00,0x00};/*温*/
static const unsigned char PROGMEM du4[] = {
0x01,0x00,0x00,0x80,0x3F,0xFE,0x22,0x20,0x22,0x20,0x3F,0xFC,0x22,0x20,0x22,0x20,
0x23,0xE0,0x20,0x00,0x2F,0xF0,0x24,0x10,0x42,0x20,0x41,0xC0,0x86,0x30,0x38,0x0E};/*度*/

static const unsigned char PROGMEM lei3[] = {
0x00,0x00,0x3F,0xF8,0x21,0x08,0x3F,0xF8,0x21,0x08,0x3F,0xF8,0x02,0x00,0x04,0x20,
0x1F,0xC0,0x01,0x80,0x06,0x10,0x3F,0xF8,0x01,0x08,0x11,0x20,0x25,0x10,0x42,0x08};/*累*/
static const unsigned char PROGMEM ji4[] = {
0x00,0x40,0x20,0x40,0x10,0x40,0x10,0x40,0x00,0x40,0x00,0x40,0xF7,0xFE,0x10,0x40,
0x10,0x40,0x10,0x40,0x10,0x40,0x10,0x40,0x14,0x40,0x18,0x40,0x10,0x40,0x00,0x40};/*计*/

static const unsigned char PROGMEM deng3[] = {
0x20,0x40,0x3F,0x7E,0x48,0x90,0x85,0x08,0x01,0x00,0x3F,0xF8,0x01,0x00,0x01,0x00,
0xFF,0xFE,0x00,0x00,0x00,0x20,0x7F,0xFC,0x08,0x20,0x04,0x20,0x04,0xA0,0x00,0x40};/*等*/
static const unsigned char PROGMEM xiao4[] = {
0x10,0x20,0x08,0x20,0x00,0x20,0xFF,0x3E,0x00,0x44,0x24,0x44,0x42,0x44,0x81,0xA4,
0x24,0x28,0x14,0x28,0x08,0x10,0x14,0x10,0x22,0x28,0x42,0x48,0x80,0x84,0x01,0x02};/*效*/

static const unsigned char PROGMEM bei4[] = {
0x04,0x80,0x04,0x98,0x7C,0xE0,0x04,0x84,0x1C,0x84,0xE4,0x7C,0x40,0x00,0x1F,0xF0,
0x10,0x10,0x1F,0xF0,0x10,0x10,0x1F,0xF0,0x10,0x10,0x10,0x10,0x10,0x50,0x10,0x20};/*背*/
static const unsigned char PROGMEM guang2[] = {
0x01,0x00,0x21,0x08,0x11,0x08,0x09,0x10,0x09,0x20,0x01,0x00,0xFF,0xFE,0x04,0x40,
0x04,0x40,0x04,0x40,0x04,0x40,0x08,0x42,0x08,0x42,0x10,0x42,0x20,0x3E,0xC0,0x00};/*光*/

static const unsigned char PROGMEM mo4[] = {
0x00,0x20,0xFE,0x28,0x92,0x24,0xD6,0x24,0xBA,0x20,0x93,0xFE,0xFE,0x20,0x10,0x20,
0xFE,0x20,0x10,0x50,0x1E,0x50,0xE0,0x50,0x02,0x88,0xAA,0x88,0xA9,0x04,0x82,0x02};/*默*/
static const unsigned char PROGMEM ren4[] = {
0x00,0x40,0x20,0x40,0x10,0x40,0x10,0x40,0x00,0x40,0x00,0x40,0xF0,0x40,0x10,0x40,
0x10,0xA0,0x10,0xA0,0x10,0xA0,0x15,0x10,0x19,0x10,0x12,0x08,0x04,0x04,0x08,0x02};/*认*/

static const unsigned char PROGMEM ce4[] = {
0x00,0x04,0x27,0xC4,0x14,0x44,0x14,0x54,0x85,0x54,0x45,0x54,0x45,0x54,0x15,0x54,
0x15,0x54,0x25,0x54,0xE5,0x54,0x21,0x04,0x22,0x84,0x22,0x44,0x24,0x14,0x08,0x08};/*测*/

static const unsigned char PROGMEM fen1[] = {
0x00,0x40,0x04,0x40,0x04,0x20,0x08,0x20,0x10,0x10,0x20,0x08,0x40,0x04,0x9F,0xE2,
0x04,0x20,0x04,0x20,0x04,0x20,0x08,0x20,0x08,0x20,0x10,0x20,0x21,0x40,0x40,0x80};/*分*/

static const unsigned char PROGMEM bi3[] = {
0x00,0x80,0x20,0x80,0x20,0x80,0x20,0x84,0x20,0x88,0x20,0x90,0x3E,0xA0,0x20,0xC0,
0x20,0x80,0x20,0x80,0x20,0x80,0x20,0x82,0x26,0x82,0x38,0x82,0x20,0x7E,0x00,0x00};/*比*/





// ============= 屏幕初始化函数 =============
// 功能：初始化 ST7789 TFT 屏幕，设置分辨率、方向和默认颜色
// 参数：lcd - 屏幕方向参数(0/90°, 1/270°, 2/180°, 3/0°)
void csh(float lcd){
  tft.init();                     // 初始化屏幕硬件（SPI、GPIO 等）
  tfts.createSprite(240,240);     // 创建 240x240 的缓冲精灵（避免屏幕闪烁）
  tfts.fillSprite(0x0000);        // 用黑色（0x0000）填充缓冲
  if(lcd == 0){       tft.setRotation(2);    // 设置屏幕旋转方向：0=0°, 1=90°, 2=180°, 3=270°
  }else if(lcd == 1){ tft.setRotation(3);
  }else if(lcd == 2){ tft.setRotation(0);
  }else if(lcd == 3){ tft.setRotation(1);   
  }
}


// ============= 页面 1：数控恒流/恒压/恒阻/恒功率负载显示 =============
// 功能：显示数控负载的工作参数（电压、电流、功率、阻值、能量）和模式
// 参数说明：
//   sz1   - 当前设置模式(0=CC恒流,1=CV恒压,2=CR恒阻,3=CP恒功率,4=欠压,5=时间限制)
//   en1   - 是否启用(0=OFF,1=ON)
//   wx1   - 小数点位选(-3至2)
//   six   - 设定值（恒流/恒压/恒阻/恒功率）
//   svl   - 欠压保护值(V)
//   stx   - 运行时间(秒)
//   vin/ain/vaw/esr/pwh - 实时测量值(电压V/电流A/功率W/阻值Ω/能量Wh)
//   vbu/ntc - 辅助参数(电源电压V/温度°C)
void ym1(int sz1,int en1,int wx1,float six,float svl,float stx,float vin,float ain,float vaw,float esr,float pwh,float vbu,float ntc){
  int rgb0 = 0x07FF;  // 青色(标签)
  int rgb1 = 0x9A60;  // 棕色(分割线)
  int rgb2 = 0x07E0;  // 绿色
  int rgb3 = 0xFDA0;  // 橙色(ON/OFF显示)
  int rgb4 = 0x867D;  // 灰色(右侧副参数)
  int rgb5 = 0xFFFF;  // 白色(光标)
  int wx = 0;  // 光标 X 坐标
  
  // 根据小数点位选计算光标位置
  if(wx1 == -3 ){      wx = 148; 
  }else if(wx1 == -2){ wx = 130;
  }else if(wx1 == -1){ wx = 112;
  }else if(wx1 == 0 ){ wx = 76; 
  }else if(wx1 == 1 ){ wx = 58;
  }else if(wx1 == 2 ){ wx = 40;
  }
  
  // 将秒转换为小时和分钟显示
  int th = stx/3600;        // 小时
  int tm = int(stx/60)%60;  // 分钟
  
  tfts.fillSprite(0x0000);    // 用黑色填充缓冲
  tfts.setTextSize(3);        // 设置主显示文字大小
  tfts.setTextColor(0xFEA0);  // 金色
  
  // 显示设定值（自适应位数，左对齐补零）
  if(sz1 < 5){
    if(six < 10 ){        tfts.drawString("00", 40, 8 ); tfts.drawFloat(six, 3 ,76, 8 );
    }else if(six < 100 ){ tfts.drawString( "0", 40, 8 ); tfts.drawFloat(six, 3 ,58, 8 );
    }else if(six < 1000){ tfts.drawFloat(six, 3,40, 8 ); }
  }else if(sz1 == 5){  // 时间限制模式显示 HH:MM
    if(th < 10){        tfts.drawString("00", 40, 8 ); tfts.drawNumber(th, 76, 8); 
    }else if(th < 100){ tfts.drawString( "0", 40, 8 ); tfts.drawNumber(th, 58, 8); }
    tfts.drawString(":", 94, 8);
    if(tm < 10){        tfts.drawString("0", 112, 8 ); tfts.drawNumber(tm,130, 8);  tfts.drawString("0", 148, 8 );
    }else if(tm < 100){ tfts.drawNumber( tm, 112, 8 ); tfts.drawString("0",148, 8);}
  }
  
  // 显示输入电压(V)
  tfts.setTextColor(0xF800);  // 红色
  if(vin < 10 ){        tfts.drawString("00", 40, 50 ); tfts.drawFloat(vin, 3 ,76, 50);
  }else if(vin < 100 ){ tfts.drawString( "0", 40, 50 ); tfts.drawFloat(vin, 3 ,58, 50);
  }else if(vin < 1000){ tfts.drawFloat(vin, 3,40, 50);  }
  
  // 显示输入电流(A)
  tfts.setTextColor(0x07E0);  // 绿色
  if(ain < 10 ){        tfts.drawString("00", 40, 90); tfts.drawFloat(ain, 3 ,76, 90);
  }else if(ain < 100 ){ tfts.drawString( "0", 40, 90); tfts.drawFloat(ain, 3 ,58, 90);
  }else if(ain < 1000){ tfts.drawFloat(ain, 3,40, 90);  }
  
  // 显示输入功率(W)
  tfts.setTextColor(0xFFE0);  // 黄色
  if(vaw < 10 ){        tfts.drawString("00", 40, 130); tfts.drawFloat(vaw, 3 ,76, 130); 
  }else if(vaw < 100 ){ tfts.drawString( "0", 40, 130); tfts.drawFloat(vaw, 3 ,58, 130);
  }else if(vaw < 1000){ tfts.drawFloat(vaw, 3 ,40,130);  }
  
  // 显示等效电阻(Ω)，根据大小自适应小数位数
  tfts.setTextColor(0x001F);  // 蓝色
  if(esr < 10 ){           tfts.drawString("00", 40, 170); tfts.drawFloat(esr, 3 ,76, 170); 
  }else if(esr < 100    ){ tfts.drawString( "0", 40, 170); tfts.drawFloat(esr, 3 ,58, 170);
  }else if(esr < 1000   ){ tfts.drawFloat(esr, 3 ,40,170);
  }else if(esr < 10000  ){ tfts.drawFloat(esr, 2 ,40,170);
  }else if(esr < 100000 ){ tfts.drawFloat(esr, 1 ,40,170);
  }else if(esr < 9999990){ tfts.drawFloat(9999999, 0 ,40,170); }
   
  // 显示累计能量(Wh)
  tfts.setTextColor(0x03EF);  // 暗青色
  if(pwh < 10 ){          tfts.drawFloat(pwh, 5 ,40, 210); 
  }else if(pwh < 100   ){ tfts.drawFloat(pwh, 4 ,40, 210);
  }else if(pwh < 1000  ){ tfts.drawFloat(pwh, 3 ,40,210); 
  }else if(pwh < 10000 ){ tfts.drawFloat(pwh, 2 ,40,210); 
  }else if(pwh < 100000){ tfts.drawFloat(pwh, 1 ,40,210); 
  }else{                  tfts.drawFloat(pwh, 0 ,40,210); }
  
  // 显示 ON/OFF 状态和工作模式
  tfts.setTextSize(4);            
  tfts.setTextColor(rgb3);  // 橙色
  if(en1 == 0){       tfts.drawString("OFF", 172,207); 
  }else if(en1 == 1){ tfts.drawString( "ON", 182,207); }

  // 显示工作模式
  if(sz1 == 0){       tfts.drawString( "CC", 182, 6 );  // 恒流模式
  }else if(sz1 == 1){ tfts.drawString( "CV", 182, 6 );  // 恒压模式
  }else if(sz1 == 2){ tfts.drawString( "CR", 182, 6 );  // 恒阻模式
  }else if(sz1 == 3){ tfts.drawString( "CP", 182, 6 );  // 恒功率模式
  }else if(sz1 == 4){   // 欠压保护：显示汉字"截止电压"
    tfts.drawBitmap(174, 12,  jie2, 16, 16, rgb3);
    tfts.drawBitmap(190, 12,  zhi3, 16, 16, rgb3);
    tfts.drawBitmap(206, 12, dian4, 16, 16, rgb3);
    tfts.drawBitmap(222, 12,   ya4, 16, 16, rgb3); 
  }else if(sz1 == 5){  // 时间限制：显示汉字"限时运行"
    tfts.drawBitmap(174, 12, xian4, 16, 16, rgb3); 
    tfts.drawBitmap(190, 12,  shi2, 16, 16, rgb3);
    tfts.drawBitmap(206, 12,  yun4, 16, 16, rgb3); 
    tfts.drawBitmap(222, 12, xing2, 16, 16, rgb3);
  }
  
  // 左侧参数标签(字母)
  tfts.setTextSize(2);            
  tfts.setTextColor(rgb0);  // 青色
  tfts.drawString( "i", 10,  23);  // 设定值单位(i for 设定)
  tfts.drawString( "V", 10,  63);  // 电压标签
  tfts.drawString( "A", 10, 103);  // 电流标签
  tfts.drawString( "W", 10, 143);  // 功率标签
  tfts.drawString( "R", 10, 183);  // 阻值标签
  tfts.drawString("Wh",  4, 223);  // 能量标签
           
  // 右侧副参数显示(时间、欠压值、电源电压、温度)
  tfts.setTextColor(rgb4);  // 灰色
  if(th < 10){ tfts.drawString( "0", 174, 63); tfts.drawNumber(th, 188, 63);  // 显示时间（小时）
  }else{ tfts.drawNumber(th, 174, 63); }
  tfts.drawString(":", 200,  63);
  if(tm < 10){ tfts.drawString( "0", 212, 63); tfts.drawNumber(tm, 226, 63);  // 显示时间（分钟）
  }else{ tfts.drawNumber(tm, 214, 63); }
  
  tfts.drawFloat(svl, 2 ,174, 103); tfts.drawString("V", 226, 103);  // 欠压保护值
  tfts.drawFloat(vbu, 2 ,174, 143); tfts.drawString("V", 226, 143);  // 电源电压
  tfts.drawFloat(ntc, 1 ,174, 183); tfts.drawString("C", 226, 183);  // 芯片温度

  // 绘制光标、分割线和汉字标签
  tfts.fillRect( wx, 32 , 15 ,  4 , rgb5);  // 白色光标（指示当前设定值所在位）
  tfts.drawLine( 0 ,  40, 240,  40, rgb1);  // 水平分割线
  tfts.drawLine( 0 ,  80, 240,  80, rgb1);
  tfts.drawLine( 0 , 120, 240, 120, rgb1);
  tfts.drawLine( 0 , 160, 240, 160, rgb1);
  tfts.drawLine( 0 , 200, 240, 200, rgb1);
  tfts.drawLine(169,  0 , 169, 240, rgb1);  // 竖线（分割左右区域）

  // 左侧参数标签（汉字字模）
  tfts.drawBitmap(  0,  2,   mo2, 16, 16, rgb0);  // "模"
  tfts.drawBitmap( 16,  2,  shi4, 16, 16, rgb0);  // "式"
  
  tfts.drawBitmap(  0, 44, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap( 16, 44,   ya4, 16, 16, rgb0);  // "压"

  tfts.drawBitmap(  0, 84, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap( 16, 84,  liu2, 16, 16, rgb0);  // "流"

  tfts.drawBitmap(  0,124, gong1, 16, 16, rgb0);  // "功"
  tfts.drawBitmap( 16,124,   lu4, 16, 16, rgb0);  // "率"

  tfts.drawBitmap(  0,164, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap( 16,164,   zu3, 16, 16, rgb0);  // "阻"

  tfts.drawBitmap(  0,204, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap( 16,204,liang4, 16, 16, rgb0);  // "量"

  // 右侧参数标签（汉字字模）
  tfts.drawBitmap(174, 44, xian4, 16, 16, rgb0);  // "限"
  tfts.drawBitmap(190, 44,  shi2, 16, 16, rgb0);  // "时"
  tfts.drawBitmap(206, 44,  yun4, 16, 16, rgb0);  // "运"
  tfts.drawBitmap(222, 44, xing2, 16, 16, rgb0);  // "行"

  tfts.drawBitmap(174, 84,  jie2, 16, 16, rgb0);  // "截"
  tfts.drawBitmap(190, 84,  zhi3, 16, 16, rgb0);  // "止"
  tfts.drawBitmap(206, 84, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap(222, 84,   ya4, 16, 16, rgb0);  // "压"

  tfts.drawBitmap(174,124, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap(190,124, yuan2, 16, 16, rgb0);  // "源"
  tfts.drawBitmap(206,124, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap(222,124,   ya4, 16, 16, rgb0);  // "压"

  tfts.drawBitmap(174,164,  nei4, 16, 16, rgb0);  // "内"
  tfts.drawBitmap(190,164,   bu4, 16, 16, rgb0);  // "部"
  tfts.drawBitmap(206,164,  wen1, 16, 16, rgb0);  // "温"
  tfts.drawBitmap(222,164,   du4, 16, 16, rgb0);  // "度"
  
  tfts.pushSprite(0, 0);    // 将缓冲精灵刷新到屏幕显示                       
}


// ============= 页面 2：功率测量与波形显示 =============
// 功能：显示实时电压/电流/功率值和历史波形曲线图
// 参数说明：
//   en1     - 是否启用(0=OFF,1=ON)
//   stx     - 当前计数(用于时间显示)
//   vin/ain/mai/vaw/esr/pwh - 实时测量值
//   vbu/ntc - 电源电压/温度
//   mvx/max - 波形显示的电压/电流范围
//   via/aia - 存储的历史电压/电流数据数组指针
void ym2(int en1,int stx,float vin,float ain,float mai,float vaw,float esr,float pwh,float vbu,float ntc,float mvx,float max,float *via,float *aia){
  int rgb0 = 0x07FF;  // 青色
  int rgb1 = 0x9A60;  // 棕色
  int rgb2 = 0x07E0;  // 绿色
  int rgb3 = 0xFDA0;  // 橙色
  int rgb4 = 0x867D;  // 灰色
  int rgb5 = 0xFFFF;  // 白色

  int th = stx/3600;        // 转换为小时
  int tm = int(stx/60)%60;  // 转换为分钟
  int ai = 0;  // 是否使用微电流模式标志
  
  // 如果检测到电流很小，自动切换到微电流模式显示 mA
  if( ain <= 0.003){ ain = mai; vaw = vin*mai;  ai = 1; }
  
  tfts.fillSprite(0x0000);    // 用黑色填充缓冲
  tfts.setTextSize(4);   
  
  // 显示 ON/OFF 状态
  if(en1 == 0){ 
    tfts.setTextColor(0x07FF);  tfts.drawString("OFF", 172,127); 
  }else{ 
    tfts.setTextColor(0x07E0);  tfts.drawString( "ON", 182,127); 
  }  
     
  // 显示测量数据
  tfts.setTextSize(3);
  tfts.setTextColor(0xF800);  // 红色-电压
  if(vin < 10 ){        tfts.drawString("00", 40, 10 ); tfts.drawFloat(vin, 3 ,76, 10);
  }else if(vin < 100 ){ tfts.drawString( "0", 40, 10 ); tfts.drawFloat(vin, 3 ,58, 10);
  }else if(vin < 1000){ tfts.drawFloat(vin, 3,40, 10);  }
  
  tfts.setTextColor(0x07E0);  // 绿色-电流
  if(ain < 10 ){        tfts.drawString("00", 40, 50); tfts.drawFloat(ain, 3 ,76, 50);
  }else if(ain < 100 ){ tfts.drawString( "0", 40, 50); tfts.drawFloat(ain, 3 ,58, 50);
  }else if(ain < 1000){ tfts.drawFloat(ain, 3,40, 50);  }
  
  tfts.setTextColor(0xFFE0);  // 黄色-功率
  if(vaw < 10 ){        tfts.drawString("00", 40, 90); tfts.drawFloat(vaw, 3 ,76, 90); 
  }else if(vaw < 100 ){ tfts.drawString( "0", 40, 90); tfts.drawFloat(vaw, 3 ,58, 90);
  }else if(vaw < 1000){ tfts.drawFloat(vaw, 3,40, 90);  }
   
  // 显示阻值和能量
  tfts.setTextSize(2); 
  tfts.setTextColor(0xFE19);  // 粉红
  tfts.drawFloat(esr, 3 ,40,124);  tfts.drawString("R", 154, 124); 
  tfts.setTextColor(0x780F);  // 紫色
  if(pwh < 10){         tfts.drawFloat(pwh, 6 ,40,142);
  }else if(vaw < 100 ){ tfts.drawFloat(pwh, 5 ,40,142);
  }else if(vaw < 1000){ tfts.drawFloat(pwh, 4 ,40,142);
  }else if(vaw < 1000){ tfts.drawFloat(pwh, 3 ,40,142);
  }
  tfts.drawString("Wh", 144, 142);

  // 单位标签
  tfts.setTextSize(2);            
  tfts.setTextColor(rgb0); 
  tfts.drawString( "V", 10,  23); 
  if(ai == 0){  // 大电流模式显示 A、W
    tfts.drawString( "A", 10,  63);
    tfts.drawString( "W", 10, 103);
  }else{  // 微电流模式显示 mA、mW
    tfts.drawString( "mA", 4,  63);
    tfts.drawString( "mW", 4, 103);
  }
 
  // 右侧副参数显示
  tfts.setTextColor(rgb4);  // 灰色
  tfts.drawFloat(vbu, 2 ,174, 23); tfts.drawString("V", 226, 23);  // 电源电压
  tfts.drawFloat(ntc, 1 ,174, 63); tfts.drawString("C", 226, 63);  // 温度
  
  if(th < 10){ tfts.drawString( "0", 174, 103); tfts.drawNumber(th, 188, 103); 
  }else{ tfts.drawNumber(th, 174, 103); }
  tfts.drawString(":", 200, 103);  // 运行时间显示
  if(tm < 10){ tfts.drawString( "0", 212,103); tfts.drawNumber(tm, 226, 103); 
  }else{ tfts.drawNumber(tm, 214, 103); }

  // 左侧参数汉字标签
  tfts.drawBitmap(  0,  4, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap( 16,  4,   ya4, 16, 16, rgb0);  // "压"

  tfts.drawBitmap(  0, 44, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap( 16, 44,  liu2, 16, 16, rgb0);  // "流"

  tfts.drawBitmap(  0, 84, gong1, 16, 16, rgb0);  // "功"
  tfts.drawBitmap( 16, 84,   lu4, 16, 16, rgb0);  // "率"

  tfts.drawBitmap(  0,124, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap( 16,124,   zu3, 16, 16, rgb0);  // "阻"
  
  tfts.drawBitmap(  0,142, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap( 16,142,liang4, 16, 16, rgb0);  // "量"

  // 右侧参数汉字标签
  tfts.drawBitmap(174,  4, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap(190,  4, yuan2, 16, 16, rgb0);  // "源"
  tfts.drawBitmap(206,  4, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap(222,  4,   ya4, 16, 16, rgb0);  // "压"

  tfts.drawBitmap(174, 44,  nei4, 16, 16, rgb0);  // "内"
  tfts.drawBitmap(190, 44,   bu4, 16, 16, rgb0);  // "部"
  tfts.drawBitmap(206, 44,  wen1, 16, 16, rgb0);  // "温"
  tfts.drawBitmap(222, 44,   du4, 16, 16, rgb0);  // "度"

  tfts.drawBitmap(174, 84,  yun4, 16, 16, rgb0);  // "运"
  tfts.drawBitmap(190, 84, xing2, 16, 16, rgb0);  // "行"
  tfts.drawBitmap(206, 84,  shi2, 16, 16, rgb0);  // "时"
  tfts.drawBitmap(222, 84, jian2, 16, 16, rgb0);  // "间"

  // 绘制分割线
  tfts.drawLine( 0 ,  40, 240,  40, rgb1);
  tfts.drawLine( 0 ,  80, 240,  80, rgb1);
  tfts.drawLine( 0 , 120, 240, 120, rgb1);
  tfts.drawLine( 0 , 160, 240, 160, rgb1);
  tfts.drawLine(169,  0 , 169, 160, rgb1);
  
  // 波形显示参数标注
  tfts.setTextSize(1); 
  tfts.setTextColor(0xF800);  // 红色-电压范围
  tfts.drawFloat(mvx, 0 ,  0, 162);
  if(mvx < 10){ tfts.drawString("V", 8, 162); }else{ tfts.drawString( "V", 14,  162); }
  tfts.drawFloat(mvx/7.0, 2, 30,162);  tfts.drawString( "V/div", 56,  162);  // 电压分度值
  
  tfts.setTextColor(0x07E0);  // 绿色-电流范围
  tfts.drawFloat(max/7.0, 2, 154,162); tfts.drawString( "A/div", 180, 162);  // 电流分度值
  if(max < 10){ tfts.drawFloat(max, 0 , 226, 162); }else{ tfts.drawFloat(max, 0, 220, 162); }
  tfts.drawString( "A", 234, 162);

  // 绘制波形曲线
  int rgbx = 0x0011;  // 深蓝色网格
  tfts.drawRect( 0 , 170, 240,  70, rgbx);  // 波形边框
  for(int a = 180; a < 231; a+=10){  // 水平网格线
    tfts.drawLine( 0 , a , 240, a , rgbx);
  }
  for(int a = 10; a < 231; a+=10){  // 竖直网格线
    tfts.drawLine( a, 170, a , 240, rgbx);
  }
  
  int size = 79;  // 波形点数
  int vx1 = 0, vx2 = 0, vy1 = 0, vy2 = 0;  // 电压曲线坐标
  int ay1 = 0, ay2 = 0;  // 电流曲线坐标
  
  // 绘制历史数据曲线
  for(int i = 0; i < size; i++){
    vx1 = map( i, 0, size, 240, 0);   // 从右到左显示（时间从新到旧）
    vx2 = map( i+1, 0, size, 240, 0);
    vy1 = 240-(70.0/mvx*via[i]);  // 映射到显示高度
    vy2 = 240-(70.0/mvx*via[i+1]);
    if(vy1 < 170||vy2 < 170){ vy1 = 170; vy2 = 170; }  // 夹紧在波形区域内
    tfts.drawLine( vx1 , vy1 , vx2, vy2 , 0xF800);  // 红色电压曲线
    
    ay1 = 240-(70.0/max*aia[i]);
    ay2 = 240-(70.0/max*aia[i+1]);
    if(ay1 < 170||ay2 < 170){ ay1 = 170; ay2 = 170; }
    tfts.drawLine( vx1 , ay1 , vx2, ay2 , 0x07E0);  // 绿色电流曲线
  }
  
  tfts.pushSprite(0, 0);    // 刷新显示                      
}


// ============= 页面 3：参数设置与校准页面 =============
// 功能：显示和编辑系统的各种校准参数和配置
// 参数说明：
//   sz1   - 当前编辑模式(0=电压校准,1=电流校准,2=微电流校准,3-8=保护/显示参数)
//   wx1   - 小数点位选
//   en1   - 是否进入编辑模式(1=编辑中显示OK)
//   lcd/led - TFT方向和背光参数
//   six   - 校准输入值
//   vin/ain/mai - 实时测量值
//   rv1/ra1/ra2 - 校准系数(分压比/A分流电阻/uA分流电阻)
//   mvx/max/mwx - 保护参数(最大保护电压/电流/功率)
void ym3(int sz1,int wx1,int en1,float lcd,float led,float six,float vin,float ain,float mai,float rv1,float ra1,float ra2,float mvx,float max,float mwx){
  // 定义各模式的显示颜色
  int rgba = 0xFFE0;  // 黄色(编辑框)
  int rgbi = 0xFDA0;  // 橙色(分割线)
  int rgb0 = 0xF800;  // 红色(电压参数)
  int rgb1 = 0x07E0;  // 绿色(电流参数)
  int rgb2 = 0x03E0;  // 暗绿(微电流参数)
  int rgb3 = 0x867D;  // 灰色(最大电压-编辑前)
  int rgb4 = 0x867D;  // 灰色(最大电流-编辑前)
  int rgb5 = 0x867D;  // 灰色(最大功率-编辑前)
  int rgb6 = 0x07FF;  // 青色(TFT方向-编辑前)
  int rgb7 = 0x07FF;  // 青色(背光亮度-编辑前)
  int rgb8 = 0x07FF;  // 青色(默认值-编辑前)
  
  int wx = 0;  // 光标位置
  if(wx1 == -3 ){      wx = 142; 
  }else if(wx1 == -2){ wx = 124;
  }else if(wx1 == -1){ wx = 106;
  }else if(wx1 == 0 ){ wx = 70; 
  }else if(wx1 == 1 ){ wx = 52;
  }else if(wx1 == 2 ){ wx = 34;
  }
  
  tfts.fillSprite(0x0000);    // 用黑色填充缓冲
  tfts.setTextSize(4);
  
  // 编辑状态指示
  if(en1 == 1){ 
    tfts.setTextColor(rgb1);  tfts.drawString( "OK", 180, 5);  // 编辑中显示 OK
  }else{ 
    tfts.setTextColor(rgba);  tfts.drawString( "ON", 180, 5);  // 待命状态显示 ON
  }  
  
  // 显示当前编辑的校准值
  tfts.setTextSize(3);            
  tfts.setTextColor(rgba);  // 黄色(编辑区)
  if(sz1 < 3){  // 前3个模式(电压/电流/微电流校准)
    tfts.fillRect( wx, 32 , 15 ,  4 , rgba);    // 白色光标
    if(six < 10 ){        tfts.drawString("00", 34, 8 ); tfts.drawFloat(six, 3 ,70, 8 );
    }else if(six < 100 ){ tfts.drawString( "0", 34, 8 ); tfts.drawFloat(six, 3 ,52, 8 );
    }else if(six < 1000){ tfts.drawFloat(six, 3,34, 8 ); }
  }else{  // 模式 3-8：显示模式号
    tfts.drawNumber(sz1, 72, 8);   
  }
  
  // 显示实时测量值用于校准
  tfts.setTextColor(rgb0);  // 红色-电压测量值
  if(vin < 10 ){        tfts.drawString("00", 34, 50 ); tfts.drawFloat(vin, 3 ,70, 50);
  }else if(vin < 100 ){ tfts.drawString( "0", 34, 50 ); tfts.drawFloat(vin, 3 ,52, 50);
  }else if(vin < 1000){ tfts.drawFloat(vin, 3,34, 50);  }
  
  tfts.setTextColor(rgb1);  // 绿色-电流测量值
  if(ain < 10 ){        tfts.drawString("00", 34, 90); tfts.drawFloat(ain, 3 ,70, 90);
  }else if(ain < 100 ){ tfts.drawString( "0", 34, 90); tfts.drawFloat(ain, 3 ,52, 90);
  }else if(ain < 1000){ tfts.drawFloat(ain, 3,34, 90);  }
  
  tfts.setTextColor(rgb2);  // 暗绿-微电流测量值
  if(mai < 10 ){        tfts.drawString("00", 34, 130); tfts.drawFloat(mai, 3 ,70, 130); 
  }else if(mai < 100 ){ tfts.drawString( "0", 34, 130); tfts.drawFloat(mai, 3 ,52, 130);
  }else if(mai < 1000){ tfts.drawFloat(mai, 3 ,34,130);  }
  

  tfts.setTextSize(2);
  
  // 根据当前模式改变编辑区颜色，指示正在编辑的参数
  if(sz1 < 3){            
    if(sz1 == 0){       tfts.setTextColor(rgb0);  tfts.drawString( "V", 10,  22);  rgba = rgb0;  // 电压校准
    }else if(sz1 == 1){ tfts.setTextColor(rgb1);  tfts.drawString( "A", 10,  22);  rgba = rgb1;  // 电流校准
    }else if(sz1 == 2){ tfts.setTextColor(rgb2);  tfts.drawString("mA", 4, 22);    rgba = rgb2;  // 微电流校准
    }
    tfts.drawBitmap(  0,  2, jiao4, 16, 16, rgba);  // "校"
    tfts.drawBitmap( 16,  2, zhun3, 16, 16, rgba);  // "准"
  }else if(sz1 == 3){ rgb3 = rgba;  // 最大保护电压进入编辑
  }else if(sz1 == 4){ rgb4 = rgba;  // 最大保护电流进入编辑
  }else if(sz1 == 5){ rgb5 = rgba;  // 最大保护功率进入编辑
  }else if(sz1 == 6){ rgb6 = rgba;  // TFT 方向进入编辑
  }else if(sz1 == 7){ rgb7 = rgba;  // 背光亮度进入编辑
  }else if(sz1 == 8){ rgb8 = rgba;  // 默认值恢复进入编辑
  }

  // 显示各校准系数
  tfts.setTextSize(2);            
  tfts.setTextColor(rgb0);  // 电压分压比 RV1
  tfts.drawString( "V", 10,  62); 
  tfts.drawString( "R1:R2",172, 44);  // 分压比标签
  tfts.drawFloat(rv1, 4 , 164,  62);
  
  tfts.setTextColor(rgb1);  // 电流分流电阻 RA1
  tfts.drawString( "A", 10, 102); 
  tfts.drawString( "mR",188, 84);  // 毫欧标签
  tfts.drawFloat(ra1, 4 , 164, 102);
  
  tfts.setTextColor(rgb2);  // 微电流分流电阻 RA2
  tfts.drawString( "mA", 4, 142); 
  tfts.drawString( "R", 194, 124);  // 欧标签
  if(ra2 < 10){tfts.drawFloat(ra2, 4 , 164, 144); }else{ tfts.drawFloat(ra2, 3 , 164, 144);  }
  
  // 显示保护参数
  tfts.setTextColor(rgb3);  // 最大保护电压
  tfts.drawFloat(mvx, 2 ,  4, 182); tfts.drawString("V",  66, 182);
  
  tfts.setTextColor(rgb4);  // 最大保护电流
  tfts.drawFloat(max, 2 , 84, 182); tfts.drawString("A", 146, 182);
  
  tfts.setTextColor(rgb5);  // 最大保护功率
  tfts.drawFloat(mwx, 1 ,164, 182); tfts.drawString("W", 226, 182);
  
  // 显示配置参数
  tfts.setTextColor(rgb6);  tfts.drawNumber(lcd, 34, 222);  // TFT 显示方向
  
  tfts.setTextColor(rgb7);  tfts.drawNumber(led, 114, 222);  // 背光亮度等级
  
  tfts.setTextColor(rgb8);  tfts.drawNumber(en1, 194, 222);  // 恢复默认值标志

  // 绘制参数标签（汉字）
  // 电压参数区
  tfts.drawBitmap(  0, 44, dian4, 16, 16, rgb0);  // "电"
  tfts.drawBitmap( 16, 44,   ya4, 16, 16, rgb0);  // "压"

  // 电流参数区
  tfts.drawBitmap(  0, 84, dian4, 16, 16, rgb1);  // "电"
  tfts.drawBitmap( 16, 84,  liu2, 16, 16, rgb1);  // "流"

  // 微电流参数区
  tfts.drawBitmap(  0,124, dian4, 16, 16, rgb2);  // "电"
  tfts.drawBitmap( 16,124,  liu2, 16, 16, rgb2);  // "流"
  
  // 最大保护参数标签
  tfts.drawBitmap( 8 ,164,  zui4, 16, 16, rgb3);  // "最"
  tfts.drawBitmap( 24,164,   da4, 16, 16, rgb3);  // "大"
  tfts.drawBitmap( 40,164, dian4, 16, 16, rgb3);  // "电"
  tfts.drawBitmap( 56,164,   ya4, 16, 16, rgb3);  // "压"
  
  tfts.drawBitmap( 88,164,  zui4, 16, 16, rgb4);  // "最"
  tfts.drawBitmap(104,164,   da4, 16, 16, rgb4);  // "大"
  tfts.drawBitmap(120,164, dian4, 16, 16, rgb4);  // "电"
  tfts.drawBitmap(136,164,  liu2, 16, 16, rgb4);  // "流"
  
  tfts.drawBitmap(168,164,  zui4, 16, 16, rgb5);  // "最"
  tfts.drawBitmap(184,164,   da4, 16, 16, rgb5);  // "大"
  tfts.drawBitmap(200,164, gong1, 16, 16, rgb5);  // "功"
  tfts.drawBitmap(216,164,   lu4, 16, 16, rgb5);  // "率"
  
  // 配置参数标签
  tfts.drawBitmap( 8 ,204, ping2, 16, 16, rgb6);  // "屏"
  tfts.drawBitmap( 24,204, xian3, 16, 16, rgb6);  // "显"
  tfts.drawBitmap( 40,204, fang1, 16, 16, rgb6);  // "方"
  tfts.drawBitmap( 56,204,xiang4, 16, 16, rgb6);  // "向"
  
  tfts.drawBitmap( 88,204,  bei4, 16, 16, rgb7);  // "背"
  tfts.drawBitmap(104,204,guang2, 16, 16, rgb7);  // "光"
  tfts.drawBitmap(120,204,  she4, 16, 16, rgb7);  // "设"
  tfts.drawBitmap(136,204,  zhi4, 16, 16, rgb7);  // "置"
  
  tfts.drawBitmap(168,204,   mo4, 16, 16, rgb8);  // "默"
  tfts.drawBitmap(184,204,  ren4, 16, 16, rgb8);  // "认"
  tfts.drawBitmap(200,204,  she4, 16, 16, rgb8);  // "设"
  tfts.drawBitmap(216,204,  zhi4, 16, 16, rgb8);  // "置"

  // 绘制分割线
  tfts.drawLine( 0 ,  40, 240,  40, rgbi);
  tfts.drawLine( 0 ,  80, 240,  80, rgbi);  
  tfts.drawLine( 0 , 120, 240, 120, rgbi);
  tfts.drawLine( 0 , 160, 240, 160, rgbi);
  tfts.drawLine( 0 , 200, 240, 200, rgbi);
  tfts.drawLine(160,  0 , 160, 240, rgbi);  // 右侧保护参数竖线
  tfts.drawLine(80,  160 , 80, 240, rgbi);  // 下侧配置参数竖线

  tfts.pushSprite(0, 0);    // 刷新显示                       
}

#endif
