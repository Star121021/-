//硬件版本： ESP32多功能负载仪
//软件版本： 1.0
//创建时间： 2025.12.22
//作者：Star



/* 

ADC衰减量程参数：
ADC_ATTEN_DB_0    0~950mV
ADC_ATTEN_DB_2_5  0~1250mV
ADC_ATTEN_DB_6    0~1750mV
ADC_ATTEN_DB_11   0~3100mV
*/

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include "TFT1.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "INA226.h"

INA226 INA1(0x40);

#define ADC_ATTEN     ADC_ATTEN_DB_11
#define ADC_ATTEN0    ADC_ATTEN_DB_0
static esp_adc_cal_characteristics_t adc1_chars;
static esp_adc_cal_characteristics_t adc1_chars0;

byte sda = 13;                  //I2C SDA
byte scl = 14;                  //I2C SCL

byte K1 = 0;                    //按键1
byte K2 = 46;                   //按键2
byte K3 = 44;                   //按键3
byte K4 = 43;                   //按键3

byte avi = 6;                   //供电电压采集
byte ant = 7;                   //NTC电压采集
byte aua = 8;                   //uA级电流采集
byte avr = 9;                   //基准电压采集
byte ada = 10;                  //DAC电压采集

byte VG1 = 5;                   //输入保护MOS开关
byte VG2 = 4;                   //反接保护MOS开关
byte VG3 = 36;                  //测小电流MOS开关
byte VG4 = 35;                  //输出保护MOS开关
byte VG5 = 34;                  //负载恒流MOS开关
byte VG6 = 2;                   //USB协议电源开关

byte BUZ = 38;                  //蜂鸣器开关
byte FAN = 37;                  //散热风扇开关
byte BLK = 33;                  //TFT背光开关

float VBU;                      //电源电压
float NTC;                      //NTC温度
float VRE;                      //基准电压
float VDA;                      //DAC电压
float VIN;                      //输入电压
float AIN;                      //输入电流A
float mAI;                      //输入电流mA
float ESR;                      //等效电阻
float VAW;                      //输入功率
float PWH;                      //输入电量
float Wh;                       //功率积分

float SAX;                      //设定恒流
float SVX;                      //设定恒压
float SRX;                      //设定恒阻
float SWX;                      //设定恒功率
float SVL;                      //设定欠压保护
float STX;                      //设定运行时间

float MVX;                      //最大保护电压
float MAX;                      //最大保护电流
float MWX;                      //最大保护功率

float RV1 = 2.12;              //分压电阻比值
float RA1 = 4;                  //A分流电阻值mR
float RA2 = 10;                 //uA分流电阻值R
float LCD = 0;                  //TFT显示方向
float LED = 10;                 //TFT背光参数

int DAC = 0;                    //DAC参数
int MS1 = 0;                    //页面模式
int SZ1 = 0;                    //设置模式
int WX1 = 0;                    //位选参数
int EN1 = 0;                    //开关参数
int CS1 = 0;                    //参数
int CS2 = 0;                    //参数
float CS3 = 0;                  //参数

float MVI = 1;                  //测量最大电压
float MAI = 1;                  //测量最大电流
extern float VIA[80];           //输入电压数组
extern float AIA[80];           //外部声明
float VIA[80];                  //定义和初始化
float AIA[80];                  //输入电流数组

unsigned long TM1 = 0;          // 时间标记1(主循环刷新计时)
unsigned long TM2 = 0;          // 时间标记2(欠压保护计时)
unsigned long TM3 = 0;          // 时间标记3(功率积分计时10ms)
unsigned long TM4 = 0;          // 时间标记4(波形更新计时20ms)


// ============= ADC初始化函数 =============
// 功能：初始化ESP32的ADC1通道和校准参数
void adc_init(){
  adc1_config_channel_atten(ADC1_CHANNEL_5, ADC_ATTEN  ); //配置ADC通道6的衰减为0dB
  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN0 ); //配置ADC通道7的衰减为0dB

  //获取ADC特征值(ADC通道, 衰减值, ADC位宽, 参考电压, 结构体指针)
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN , ADC_WIDTH_BIT_12, 0, &adc1_chars );  
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN0, ADC_WIDTH_BIT_12, 0, &adc1_chars0);
}

// ============= Flash数据保存函数 =============
// 功能：将所有参数保存到内置Flash存储器(EEPROM模拟)
// 参数：无
// 返回：无
// 说明：参数地址必须4字节对齐,总计60字节(地址0-59)
void sjx(){  
  // 设定参数保存(地址0-23,共6个float,每个4字节)
  EEPROM.put( 0, SAX);    // 恒流设定值(A)   [地址0-3]
  EEPROM.put( 4, SVX);    // 恒压设定值(V)   [地址4-7]
  EEPROM.put( 8, SRX);    // 恒阻设定值(Ω)  [地址8-11]
  EEPROM.put(12, SWX);    // 恒功率设定值(W) [地址12-15]
  EEPROM.put(16, SVL);    // 欠压保护值(V)   [地址16-19]
  EEPROM.put(20, STX);    // 时间限制(秒)    [地址20-23]
  
  // 保护参数保存(地址24-35,共3个float)
  EEPROM.put(24, MVX);    // 最大保护电压(V) [地址24-27]
  EEPROM.put(28, MAX);    // 最大保护电流(A) [地址28-31]
  EEPROM.put(32, MWX);    // 最大保护功率(W) [地址32-35]
  
  // 校准参数保存(地址36-55,共5个float)
  EEPROM.put(36, RV1);    // 分压比校准系数 [地址36-39]
  EEPROM.put(40, RA1);    // A分流电阻(mΩ)  [地址40-43]
  EEPROM.put(44, RA2);    // uA分流电阻(Ω) [地址44-47]
  
  // 显示配置保存(地址48-59,共3个float)
  EEPROM.put(48, LCD);    // TFT方向设置    [地址48-51]
  EEPROM.put(52, LED);    // 背光亮度等级   [地址52-55]
  EEPROM.put(56, MS1);    // 启动默认页面   [地址56-59]
  
  EEPROM.commit();        // 提交改动到Flash(必须调用)
  delay(1);
}

// ============= Flash数据读取函数 =============
// 功能：从内置Flash读取所有保存的参数并初始化,损坏时恢复默认值
// 参数：无
// 返回：无
// 说明：首次运行或Flash损坏会使用默认值(系统出厂值)
void sjd(){ 
  EEPROM.begin(1024);  // 初始化EEPROM(1KB存储空间)
  delay(1);
  
  // 读取所有保存的参数(地址和顺序必须与sjx()一致)
  EEPROM.get( 0, SAX);    // 读取恒流设定
  EEPROM.get( 4, SVX);    // 读取恒压设定
  EEPROM.get( 8, SRX);    // 读取恒阻设定
  EEPROM.get(12, SWX);    // 读取恒功率设定
  EEPROM.get(16, SVL);    // 读取欠压保护值
  EEPROM.get(20, STX);    // 读取时间限制
  EEPROM.get(24, MVX);    // 读取最大电压
  EEPROM.get(28, MAX);    // 读取最大电流
  EEPROM.get(32, MWX);    // 读取最大功率
  EEPROM.get(36, RV1);    // 读取分压比
  EEPROM.get(40, RA1);    // 读取A分流阻值
  EEPROM.get(44, RA2);    // 读取uA分流阻值
  EEPROM.get(48, LCD);    // 读取TFT方向
  EEPROM.get(52, LED);    // 读取背光亮度
  EEPROM.get(56, MS1);    // 读取默认页面
  
  // 检查数据完整性:若读到NaN表示Flash未初始化或已损坏
  // isnan(x)返回1表示是NaN,返回0表示正常数字
  if(isnan(SAX)== 1 || isnan(SVL) == 1|| isnan(LED) == 1){  
    // 恢复所有参数到出厂默认值
    SAX = 0;      SVX = 50;    SRX = 999;    // 设定参数(0A/50V/999Ω)
    SWX = 0;      SVL = 0;     STX = 7200;   // STX=7200秒=2小时
    RV1 = 2.12;   RA1 = 4;     RA2 = 10;     // 校准参数(硬件特性)
    MVX = 50;     MAX = 20.2;  MWX = 200;    // 保护参数(安全限制)
    LCD = 0;      LED = 10;    MS1 = 0;      // 显示参数,LED最大亮度,从第0页启动
  }
}

// ============= 按键函数1:位选切换(循环切换编辑位置) =============
// 功能：处理位选按键,用于切换要编辑的数字位(个/十/百位等),长按切换编辑页面
// 参数：
//   int kw - GPIO引脚编号(按键输入脚)
//   int &wx - 当前位选位置(0=个位,-1=小数第1位,...最大值=wi)
//   int wi - 位选最大值(最高位)
//   int wf - 位选最小值(最低位)
//   int &b - 编辑页面计数器(0-3四个页面循环)
//   int bx - 页面最大值(通常3表示4个页面)
// 时序：按下时播放20ms嘟声,长按(>600ms)播放30ms嘟声并切换页面
// 返回：无(通过引用修改wx和b)
// 前向声明：在文件顶部或使用处之前声明外部函数，避免在调用点之前未声明导致编译错误
void BUZZ(int b);

void key1(int kw, int &wx, int wi, int wf, int &b, int bx){
  if(digitalRead(kw) == LOW){  // 按键按下(有效电平为低)
    BUZZ(20);                   // 播放短嘟声反馈
    long int t =  millis();     // 记录按键按下时间
    int i = wx;                 // 保存当前位选值
    wx++;                        // 位选值加1,向高位移动
    if(wx > wi || wx < wf){     // 超出范围则回到最低位
      wx = wf; 
    }
    
    while(digitalRead(kw) == LOW){ // 等待按键释放
      if(millis() - t > 600){   // 长按判断:超过600ms
        BUZZ(30);               // 播放长嘟声
        wx = i;                 // 恢复原位选值
        b++;                    // 页面计数器加1
        if(b > bx){ b = 0; }    // 循环回到第一页
        break;  
      }
    }
  }
}

// ============= 按键函数2:数值增减(加/减键快速调整参数) =============
// 功能：处理增减按键,根据按键持续时间实现慢调(小步进)和快调(大步进)
// 参数：
//   float &a - 参数值(引用,会被修改)
//   float mi - 参数最小值(下限)
//   float mx - 参数最大值(上限)
//   int kh - 增加按键的GPIO引脚
//   int kl - 减少按键的GPIO引脚
//   int tx - 快调判断阈值(ms,超过此时间切换为大步进)
//   float xi - 小步进值(快速点按时的增量)
//   float xm - 大步进值(长按时的增量)
// 时序：按键按下播放30ms嘟声,短按小步进,长按(>tx)切换大步进
// 返回：无(通过引用修改参数a和保存到Flash)
void key2(float &a, float mi, float mx, int kh, int kl, int tx, float xi, float xm){   
  if(digitalRead(kh) !=  digitalRead(kl)){ // 两个按键有一个被按下
    BUZZ(30);  
    long int t =  millis();               // 记录按键按下时间
    if(digitalRead(kh) == LOW){           // 加键被按下
      int i = a;                          // 保存初始值
      a = a + xi;                         // 先增加小步进值
      if(a > mx){  a = mx ; }             // 检查上限
      while(digitalRead(kh) == LOW){      // 等待按键释放
        if(millis() - t > tx){            // 长按判断(如tx=500ms)
          a = i + xm;                     // 切换为大步进
          if(a > mx){  a = mx ; } 
          break;  
        }
      }   
    }else{                                // 减键被按下
      int i = a; 
      a = a - xi;                         // 先减少小步进值
      if(a < mi){  a = mi ; }             // 检查下限
      while(digitalRead(kl) == LOW){      // 等待按键释放
        if(millis() - t > tx){            // 长按判断
          a = i - xm;                     // 切换为大步进
          if(a < mi){  a = mi ; }
          break;  
        }
      }  
    }
    if(MS1 != 2){ sjx(); }                // 不在校准模式才保存参数
  }
}

// ============= 按键函数3:小数位调整(精细微调) =============
// 功能：按小数位为单位调整参数,提供0.001、0.01、0.1、1、10、100的精细步进
// 参数：
//   float &ma - 要调整的参数值(引用修改)
//   float mi - 参数最小值
//   float mx - 参数最大值
//   int kh - 增加按键的GPIO引脚
//   int kl - 减少按键的GPIO引脚
//   int wx - 当前小数位选择(-3到+2,对应0.001到100)
// 时序：按键按下播放30ms嘟声,按住400-300ms后释放,单次调整一个位
// 返回：无(通过引用修改参数ma和自动保存)
void key3(float &ma, float mi, float mx, int kh, int kl, int wx){ 
  if(digitalRead(kh) !=  digitalRead(kl)){ // 任一按键被按下
    long int t =  millis(); 
    BUZZ(30);  
    if(digitalRead(kh) == LOW){           // 增加按键
      // 根据位选wx决定增量值(从最低位0.001到最高位100)
      if(wx == -3){       ma = ma + 0.001; // 千分位:+0.001
      }else if(wx == -2){ ma = ma + 0.01;  // 百分位:+0.01
      }else if(wx == -1){ ma = ma + 0.1;   // 十分位:+0.1
      }else if(wx == 0 ){ ma = ma + 1;     // 个位:+1
      }else if(wx == 1 ){ ma = ma + 10;    // 十位:+10
      }else if(wx == 2 ){ ma = ma + 100;   // 百位:+100
      }
      if(ma > mx){ ma = mx; }             // 上限检查
      while(digitalRead(kh) == LOW){      // 等待按键释放
        if(millis() - t > 400){ break; }  // 防止重复触发(400ms)
      }   
    }else{                                // 减少按键
      if(wx == -3){       ma = ma - 0.001; 
      }else if(wx == -2){ ma = ma - 0.01;  
      }else if(wx == -1){ ma = ma - 0.1;  
      }else if(wx == 0 ){ ma = ma - 1;
      }else if(wx == 1 ){ ma = ma - 10; 
      }else if(wx == 2 ){ ma = ma - 100;   
      }
      if(ma < mi){  ma = mi ; }           // 下限检查
      while(digitalRead(kl) == LOW){      // 等待按键释放
        if(millis() - t > 300){ break; }  // 防止重复触发(300ms)
      }  
    }
    if(MS1 != 2){ sjx(); }                // 不在校准模式才自动保存参数到Flash
  }
}

// ============= 蜂鸣器驱动函数 =============
// 功能：通过PWM驱动蜂鸣器发出3kHz的嘟声,用于按键反馈和警告提示
// 参数：
//   int b - 蜂鸣时长(毫秒),范围20~400ms
// 返回：无
// 时序：立即发声,延时b毫秒后停止
// 说明：PWM频率3kHz,占空比180/255≈71%,音量较大
void BUZZ(int b){
  ledcWrite( 1,180);      // 启动PWM,输出功率(占空比180/255)
  delay(b);               // 延时指定时长
  ledcWrite( 1, 0);       // 关闭PWM,停止发声
}

// ============= 散热风扇温度控制 =============
// 功能：根据NTC温度传感器读数自动调整风扇速度(PWM)
// 参数：无
// 返回：无
// 温度策略：
//   NTC < 34℃：停转(0%)
//   34℃ ≤ NTC ≤ 40℃：慢速(30/255≈12%)
//   NTC > 40℃：全速(255/255=100%)
// 说明：用于防止长时间高功率运行时散热器过热
void VFAN(){
  if(NTC > 35 && NTC < 40){        // 中温区间(35-40℃)
    ledcWrite( 0, 30);             // 降速运行(减少噪音)
  }else if(NTC > 40){              // 高温(>40℃)
    ledcWrite( 0, 255);            // 全速运行(最大散热)
  }else if(NTC < 34){              // 低温(<34℃)
    ledcWrite( 0, 0);              // 停转节能
  }   
}

// ============= 欠压和超时保护 =============
// 功能：监测输入电压低于设定值和测试时间超限,自动关闭负载
// 参数：无
// 返回：无
// 保护条件：
//   1) 时间超限：测试时间 > STX秒 且 STX > 20秒
//   2) 欠压保护：输入电压 < SVL 且 VIN > 1V(防止测量噪声误触发)
// 说明：触发保护时播放400ms蜂鸣,关闭负载EN1=0,记录触发时刻
void VTP(){
  if(EN1 == 1){                      // 只在负载启用时执行保护
    float ti = (millis() - TM2)/1000; // 计算从启用到现在的运行时长(秒)
    if(STX < ti && STX > 20){        // 时间超限保护(STX大于20秒才启用)
      BUZZ(400);                     // 警告蜂鸣400ms
      EN1 = 0;                       // 关闭负载
    }else if(VIN < SVL && VIN > 1){ // 欠压保护
      BUZZ(400);                     // 警告蜂鸣400ms
      EN1 = 0;                       // 关闭负载
    } 
  }else{
    TM2 = millis();                  // 未启用时记录参考时刻
  } 
}

// ============= 多重保护函数(过压/过流/过温/过功率) =============
// 功能：监测电路工作状态,触发时切断电源保护负载
// 参数：无
// 返回：无
// 说明：分为两个工作模式
//   MS1=0(控制模式,CC/CV/CR/CP)：保护输入和输出端
//   MS1≥1(测量模式)：重点保护输出端
// 
// 控制模式(MS1=0)保护策略：
//   - 过压(VIN>MVX)：立即切断输入保护(VG1),播放200ms警告
//   - 过流/过功/过温(AIN>MAX 或 VAW>MWX 或 NTC>80℃)：
//     若EN1未启动则额外关闭VG2反向保护,然后关闭EN1,播放300ms警告
//
// 测量模式(MS1≥1)保护策略：
//   - 过压(VIN>MVX)：同控制模式
//   - 过流/过温(AIN>MAX 或 NTC>80℃)：
//     若EN1未启动则关闭VG1,播放300ms警告;
//     然后关闭输出保护(VG4),关闭EN1,再播放100ms声音
void VACTP(){
  if(MS1 == 0){                          // 控制模式保护逻辑
    // 过压保护:输入电压超过MVX(通常50V)
    if(VIN > MVX){ 
      digitalWrite(VG1, LOW);            // 切断输入保护FET
      BUZZ(200);                         // 短警告声
      EN1 = 0;                           // 关闭负载
    }
    // 过流/过功/过温保护
    if(AIN > MAX || VAW > MWX || NTC > 80){ 
      if(EN1 == 0){                      // 首次触发保护
        digitalWrite(VG1, LOW);          // 切断输入
        digitalWrite(VG2, LOW);          // 切断反向保护 
        BUZZ(300);                       // 长警告声
      }
      EN1 = 0;                           // 持续关闭负载
    }  
  }else if(MS1 > 0){                     // 测量模式保护逻辑(MS1=1,2,3)
    // 过压保护
    if(VIN > MVX){ 
      digitalWrite(VG1, LOW);            // 切断输入保护
      BUZZ(200);                         // 短警告声
      EN1 = 0; 
    }
    // 过流/过温保护
    if(AIN > MAX || NTC > 80){
      if(EN1 == 0){                      // 首次触发
        digitalWrite(VG1, LOW);          // 切断输入
        BUZZ(300);                       // 长警告声
      } 
      digitalWrite(VG4, LOW);            // 切断输出保护 
      EN1 = 0;                           // 关闭负载
      BUZZ(100);                         // 再加一声反馈
    }    
  }
}

// ============= ADC模拟量采样(电压/参考/DAC反馈/温度) =============
// 功能：读取四路ADC输入,经过校准转换为实际电压和温度值
// 参数：无
// 返回：无
// 输入信号说明：
//   avi(GPIO6)  - 输入电压采样,经过分压网络RV1衰减
//   avr(GPIO9)  - 参考电压(应为1.25),用于校准
//   ada(GPIO10) - DAC输出反馈,验证I2C DAC是否正常
//   ant(GPIO7)  - NTC热敏电阻,通过log计算温度
// 
// 工作原理：
//   1. 采样原始ADC值(12位,0-4095)
//   2. 使用校准参数转换为电压值(mV)
//   3. 计算分压网络两端电压(VBU = v1*2/1000 → 0-50V)
//   4. 使用Steinhart-Hart方程计算NTC温度
void VADC(){
  // 采样四个ADC通道的原始值
  float v1 = analogRead(avi);       // 输入电压ADC值
  float v2 = analogRead(avr);       // 参考电压ADC值
  float v3 = analogRead(ada);       // DAC反馈ADC值
  float v4 = analogRead(ant);       // NTC温度ADC值

  // 使用校准参数将原始ADC值转换为实际电压(mV)
  v1 = esp_adc_cal_raw_to_voltage(v1, &adc1_chars);   // 高衰减档(3.1V范围)
  v2 = esp_adc_cal_raw_to_voltage(v2, &adc1_chars);
  v3 = esp_adc_cal_raw_to_voltage(v3, &adc1_chars);
  v4 = esp_adc_cal_raw_to_voltage(v4, &adc1_chars);

  // 计算实际电压值(mV→V,并考虑分压比)
  VBU = v1*2/1000;    // 输入电源电压(V,分压比2倍)
  VRE = v2;           // 参考电压值(mV)
  VDA = v3;           // DAC输出电压反馈(mV)

  // 通过NTC热敏电阻计算温度(Steinhart-Hart公式)
  // R = V_bias×R_ntc/(V_ref - V_bias)
  float r = v4*10000/(3300 - v4);  // 计算NTC电阻值(单位Ω,标称25℃=10kΩ)
  NTC = (3950*298.15)/(3950+(298.15*log(r/10000))) - 273.15; // Steinhart-Hart,结果为℃
}

// ============= 电压电流双重采样测量(关键!) =============
// 功能：使用INA226芯片和可选的op-amp放大ADC测量电压/电流,并自动检测切换微电流模式
// 参数：无
// 返回：无
// 说明：这是最复杂的测量函数,包含5个关键操作：
//   1) INA226采样与反向检测
//   2) 微电流自动检测与切换
//   3) ADC衰减动态选择
//   4) 反向保护(VG2)控制
//   5) 功率和能量计算
//
// 测量原理：
//   - 大电流(>3mA)：INA226直接测量4mΩ分流电阻
//     AIN = 分流电压/RA1
//   - 微电流(≤3mA)：VG3切换到op-amp放大电路(101×增益)
//     mAI = ADC采样/RA2/101×1000
//
// 反向检测：用于判断电源极性
//   - VIN<1V且AIN<-0.1A：反向→关闭反向保护FET(VG2),停止
//   - VIN>1V且AIN>0.5A：正向→启用反向保护
void INA(){
  float vv = 0;
  float aa = 0;
  
  // 第1步：INA226原始采样(20次过采样求平均)
  for(int i = 0; i < 20; i++){
    VIN = INA1.getBusVoltage()*RV1;              // 读取母线电压(经分压系数修正)
    AIN = INA1.getShuntVoltage_mV()/RA1;         // 读取分流电压(mV)并转为电流(A)
    vv = vv + VIN;
    aa = aa + AIN;
  }
  VIN = vv/20;  // 平均电压(V)
  AIN = aa/20;  // 平均电流(A)
  
  // 第2步：反向保护控制(检测极性错误)
  if(VIN < 1 && AIN < -0.1){           // 极性反向(负电流+低压)
    digitalWrite(VG2, LOW);             // 关闭反向保护FET
    EN1 = 0;                           // 关闭负载防止烧毁
    BUZZ(500);                          // 长蜂鸣警告
  }else if(VIN > 1 && AIN > 0.5){     // 确认正向连接(高压+正电流)
    digitalWrite(VG2, HIGH);            // 启用反向保护FET
  }else if(VIN > 3 && AIN > 0){       // 条件放宽(≥3V就认可)
    digitalWrite(VG2, HIGH);
  }
  
  // 第3步：微电流模式自动检测与切换(核心!)
  if(AIN <= 0.003 && MS1 > 0){         // 电流≤3mA且在测量模式(MS1=1,2,3)
    digitalWrite(VG3, LOW);             // 启用微电流测量电路(VG3控制MOSFET)
    float v5 = 0;
    
    // 第3.1步：ADC衰减档位动态选择(自适应)
    // 阈值：9.4mA(即0.094V/RA2), 若上次mAI小于此值则用低衰减获得更高精度
    if(mAI < 9.4/RA2){
      adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN0 );   // 0dB低衰减(0-950mV),精度高
      esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN0, ADC_WIDTH_BIT_12, 0, &adc1_chars0);
    }else{
      adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN );    // 11dB高衰减(0-3100mV),量程大
      esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN, ADC_WIDTH_BIT_12, 0, &adc1_chars0);     
    }
    
    // 第3.2步：采样20次并转换为电压
    for (int i = 0; i < 20; i++) { 
      v5 += adc1_get_raw(ADC1_CHANNEL_7);  // 累加原始ADC值
    }
    v5 = v5/20;                            // 平均值
    v5 = esp_adc_cal_raw_to_voltage(v5, &adc1_chars0);  // 用校准参数转换为电压(mV)
    
    // 第3.3步：计算微电流值(考虑op-amp 101×增益)
    mAI = v5/RA2/101.0;                 // mV/(Ω×101) = mA
    AIN = mAI/1000.0;                   // mA → A
  }else{
    digitalWrite(VG3, HIGH);             // 禁用微电流电路,保持主路INA测量
    mAI = 0;
  }
  
  // 第4步：电流值下限处理(防止负数噪声)
  if( AIN < 0){ AIN = 0; }
  
  // 第5步：功率、能量和阻值计算
  if(VIN <= 0.2){                        // 输入电压过低视为无负载
    VIN = 0; VAW = 0; ESR = 0; 
  }else{
    if( AIN == 0){                       // 无电流(开路或超低流)
      VAW = 0;                           // 功率为0
      ESR = 99999.9;                     // 等效电阻设为极大值(开路)
    }else{ 
      VAW = VIN*AIN;                     // 功率(W)
      ESR = VIN/AIN;                     // 等效电阻(Ω)
      if(ESR > 9999990){ ESR = 9999990; } // 上限保护
    }
    
    // 能量积分(10ms更新一次)
    if((millis() - TM3) > 10){
      TM3 = millis();
      Wh = Wh + VAW;                     // 瓦时累加(粗略值)
      PWH = Wh/3600.0/100.0;            // 转换为Wh显示值(缩放因子)
      if(PWH > 99999.9){ PWH = 99999.9; } // 显示上限
    }
    
    // 防止NaN错误(浮点运算溢出)
    if(isnan(ESR)== 1 || isnan(PWH) == 1){ ESR = 0; PWH = 0;}
  } 
}


// ============= DAC数模转换输出(I2C控制) =============
void VDAC(int da,int cs){
  Wire.beginTransmission(0x60); 
  if(cs == 0){       
    Wire.write(0B01011011); //0B 010 11 01 1
  }else if(cs == 1){ 
    Wire.write(0B01011001);                  
  }else if(cs == 2){ 
    Wire.write(0B01011000);                  
  }else if(cs == 3){ 
    Wire.write(0B01000000);                  
  }
  Wire.write(da / 16);                       // 12位值的高8位
  Wire.write((da % 16) << 4);                // 12位值的低4位,左移4位填充
  Wire.endTransmission();                    // 提交I2C写入
}

// ============= NMOS栅极驱动反馈控制(恒流源) =============
// 功能：使用反馈闭环算法调节NMOS栅极电压,使实际电流跟踪设定电流(恒流模式)
// 参数：
//   float ax - 目标电流设定值(A),由CC模式的SAX设定
//   float ai - 当前实际电流测量值(A),来自INA226或ADC
//   float arf - 电流误差死区(A),小于此值不调整避免频繁振荡,通常0.001A
//   float mxa - 电流切换阈值(A),超过此值(如10A)使用不同DAC量程
// 返回：无
// 控制策略：
//   1. 比较设定值ax和实际值ai的偏差
//   2. 偏差超过arf则计算调整量a = (ax-ai)×200(粗调)或×10(细调)
//   3. DAC递增或递减,范围0-4095
//   4. 低电流(<mxa)用2.5V量程cs=2,高电流用5V量程cs=1
// 特点：模拟PID的简化版本,实时响应电流变化
void NMOS(float ax, float ai, float arf, float mxa){
  if(ax > 0 && EN1 == 1 && VIN > 0.5){      // 有设定值且负载启用且有输入电压
    digitalWrite(VG5, LOW);                  // 启用NMOS驱动(VG5低电平激活)
    int a = 0;
    float fab = ax - ai;                     // 计算电流偏差(目标-实际)
    if(fabs(fab) >= arf){                    // 偏差超过死区才调整
      a = fab*200;                           // 粗调:200倍系数(快速响应)
      if(fab > 1){ a = fab*10; }             // 细调:1A以上用10倍系数(防止过调)
      DAC = DAC + a;                         // DAC累加调整
      if(DAC > 4095){ DAC = 4095; }          // 上限保护
      else if(DAC < 0 ){ DAC = 0; }         // 下限保护
      
      // 根据电流大小选择DAC输出范围
      if(ax > mxa){ 
        VDAC(DAC, 1);                        // 高电流(>mxa): 0-5V量程(cs=1)
      }else{ 
        VDAC(DAC, 2);                        // 低电流(≤mxa): 0-2.5V量程(cs=2)
      }
    }
  }else{                                     // 禁止条件:无设定值或负载关闭
    VDAC(0, 0);                              // DAC输出0V(下拉1K)
    digitalWrite(VG5, HIGH);                 // 禁用NMOS驱动
  }   
}


// ============= 数控负载工作模式(四种运行模式) =============
// 功能：执行CC/CV/CR/CP四种工作模式,每种模式有独立的设定值和控制逻辑,支持欠压/超时保护设置
// 参数：无
// 返回：无
// 说明：
//   SZ1=0: CC(恒流)模式, SAX设定目标电流
//   SZ1=1: CV(恒压)模式, SVX设定目标电压
//   SZ1=2: CR(恒阻)模式, SRX设定目标阻值
//   SZ1=3: CP(恒功)模式, SWX设定目标功率
//   SZ1=4: SVL(欠压值)设置
//   SZ1=5: STX(超时)设置
//
// 各模式工作原理：
//   CC模式: 实际电流跟踪SAX目标值(使用NMOS反馈控制)
//   CV模式: 实际电压跟踪SVX目标值(DAC直接调节,误差<±20mV)
//   CR模式: A = V/R, 根据测量电压和设定阻值计算目标电流, 由NMOS控制
//   CP模式: A = P/V, 根据测量电压和设定功率计算目标电流, 由NMOS控制
void CVRP(){
  // 高功率运行时启动散热风扇(>100W时全速)
  if(VAW > 100){ ledcWrite( 0, 255); }
  
  // 按键K3切换运行/停止(仅在模式0-3)
  if(SZ1 <= 3 && digitalRead(K3) == LOW){
    BUZZ(30); 
    EN1 = !EN1;                              // 负载开关切换
    DAC = 0;                                 // 重置DAC值
    while(digitalRead(K3) == LOW){}
    if(EN1 == 0){                            // 停止运行
      VDAC(0, 0);                            // DAC输出0V
      digitalWrite(VG5, HIGH);               // 禁用NMOS驱动
    }else{                                   // 开始运行
      digitalWrite(VG5, LOW);                // 启用NMOS驱动
    } 
  }
  
  // ---- CC(恒流)模式 ----
  if(SZ1 == 0){
    key1(K4, WX1, 1, -3, SZ1, 5);           // 位选:个位到小数3位,共6档
    key3(SAX, 0, MAX, K2, K1, WX1);        // 调整目标电流(0-MAX A)
    NMOS(SAX, AIN, 0.001, 10);              // 闭环控制:精度1mA
    
  }else if(SZ1 == 1){                       // CV(恒压)模式
    key1(K4, WX1, 1, -3, SZ1, 5);           // 位选:个位到小数3位
    key3(SVX, 1, MVX, K2, K1, WX1);        // 调整目标电压(1-MVX V)
    // 恒压控制:检查输入电压是否超过目标值
    if(VIN > SVX && EN1 == 1){
      digitalWrite(VG5, LOW);                // 启用驱动
      float V = VIN - SVX;                   // 计算超压量
      if(V > 0.02){ DAC++; }                 // 超压>20mV: DAC增加
      else if(V < -0.02){ DAC--; }           // 欠压<-20mV: DAC减少
      if(DAC > 4095){ DAC = 4095; }
      else if(DAC < 0 ){ DAC = 0; }
      VDAC(DAC, 1);                          // 输出调整(5V量程)
    }else{
      VDAC(0, 0);                            // 输入低于目标则断开
      digitalWrite(VG5, HIGH);
    }
    
  }else if(SZ1 == 2){                       // CR(恒阻)模式
    key1(K4, WX1, 2, -2, SZ1, 5);           // 位选:百位到小数2位
    key3(SRX, 0.1, 999.9, K2, K1, WX1);    // 调整目标阻值(0.1-999.9Ω)
    float A = VIN/SRX;                      // 根据V=IR计算所需电流
    if(A > MAX){ A = MAX; }                 // 限流保护
    else if( A < 0){ A = 0; }
    NMOS(A, AIN, 0.002, 10);                // 闭环控制目标电流
    
  }else if(SZ1 == 3){                       // CP(恒功)模式
    key1(K4, WX1, 2, -2, SZ1, 5);           // 位选:百位到小数2位
    key3(SWX, 0, MWX, K2, K1, WX1);        // 调整目标功率(0-MWX W)
    float A = SWX/VIN;                      // 根据P=VI计算所需电流
    if(A > MAX){ A = MAX; }                 // 限流保护
    else if( A < 0){ A = 0; }
    NMOS(A, AIN, 0.002, 10);                // 闭环控制目标电流
          
  }else if(SZ1 == 4){                       // 欠压保护值设置
    key1(K4, WX1, 1, -2, SZ1, 5); 
    key3(SVL, 0, 50, K2, K1, WX1);          // 设置欠压阈值(0-50V)
  
  }else if(SZ1 == 5){                       // 超时保护设置
    key1(K4, WX1, 1, -2, SZ1, 5);  
    key2(STX, 0,345600, K2, K1, 200, 60, 3600); // 设置超时时间(秒), 支持0-345600s(96小时)
  }      
}

// ============= 功率测量模式 =============
// 功能：记录实时功率数据,绘制V-I波形曲线,支持能量清零和超时保护
// 参数：无
// 返回：无
// 说明：
//   - 采样20ms刷新一次,记录80个历史点
//   - 自动显示波形的纵轴范围(基于MVI/MAI最大值)
//   - K3启停记录,K4长按(>500ms)清零能量和计时
//   - 超过MAX电流自动断电保护
void VAPW(){
  // 超流保护
  if(AIN > MAX){ 
    digitalWrite(VG4, LOW);                  // 关闭输出FET
    EN1 = 0; 
  }
  
  // 每1秒增加运行计时CS1(用于UI显示)
  if(millis()- TM2 > 1000){ 
    TM2 = millis();  
    CS1++;                                   // 计时器加1秒
  }
  
  // 每20ms更新一次波形数据
  if(millis()- TM4 > 20){                    // 20ms采样周期
    TM4 = millis(); 
    int size = sizeof(VIA)/sizeof(VIA[0]);  // 数组大小(80个点)
    // 数据左移:将新数据插入最前,旧数据向后移动
    for(int i = size; i > 1; i--){ 
      VIA[i-1] = VIA[i-2];                   // 电压历史数据后移
      AIA[i-1] = AIA[i-2];                   // 电流历史数据后移
    }
    // 更新波形纵轴自动缩放范围
    if(VIN > MVI){ MVI = int(VIN)+1; }       // 电压最大值(+1裕度)
    if(AIN > MAI){ MAI = int(AIN)+1; }       // 电流最大值(+1裕度)
    VIA[0] =  VIN;                           // 新数据插入最前
    AIA[0] =  AIN; 
  }

  // K3启停测量记录
  if(digitalRead(K3) == LOW){
    while(digitalRead(K3) == LOW){}
    BUZZ(100); 
    EN1 = !EN1;                              // 切换测量开关
    digitalWrite(VG4, EN1);                  // 控制输出FET
    MVI = 0; MAI = 0;                        // 清除自动缩放值(重新计算范围)
  }
  
  // K4长按(>500ms)清零能量和计时
  if(digitalRead(K4) == LOW){
    long int t =  millis();
    while(digitalRead(K4) == LOW){
      if(millis() - t > 500){                // 检测长按>500ms
        BUZZ(100);                           // 长蜂鸣反馈
        Wh = 0;                              // 清零能量累计
        CS1 = 0;                             // 清零计时器
        break;  
      }
    }
  }
}

//参数设置校准模式
void XZCS(){
  EN1 = 0;
  if( digitalRead(K3) == LOW){
    BUZZ(100); 
    EN1 = 1;
    while(digitalRead(K3) == LOW){} 
  }
  
  if(SZ1 > 2){
    if( digitalRead(K4) == LOW){
      long int t =  millis(); 
      while(digitalRead(K4) == LOW){
        if(millis() - t > 600){      
          BUZZ(50); 
          SZ1++;
          if(SZ1 > 8){ SZ1 = 0; }
          break;
        }
      }
    } 
  }
  if(SZ1 == 0){
    digitalWrite(VG3, HIGH);  digitalWrite(VG4, HIGH);
    key1(K4, WX1, 1, -3, SZ1, 8);  
    key3(CS3, 0, MVX, K2, K1, WX1);  
    if(EN1 == 1){ RV1 = CS3/VIN*RV1;  sjx(); }
    
  }else if(SZ1 == 1){
    digitalWrite(VG3, HIGH);  digitalWrite(VG4, HIGH);
    key1(K4, WX1, 1, -3, SZ1, 8);  
    key3(CS3, 0, MAX, K2, K1, WX1);  
    if(EN1 == 1){ RA1 = AIN/CS3*RA1;  sjx(); }
    
  }else if(SZ1 == 2){
    digitalWrite(VG3, LOW);  digitalWrite(VG4, HIGH);
    key1(K4, WX1, 1, -3, SZ1, 8);  
    key3(CS3, 0,  3, K2, K1, WX1);  
    if(EN1 == 1){ RA2 = mAI/CS3*RA2;  sjx(); }  

  }else if(SZ1 == 3){
    key2(MVX, 0, 60, K2, K1, 200, 1, 10);
    if(EN1 == 1){ sjx(); }
     
  }else if(SZ1 == 4){
    key2(MAX, 0, 20.2, K2, K1, 200, 0.1, 5);
    if(EN1 == 1){ sjx();  }  
      
  }else if(SZ1 == 5){
    key2(MWX, 0, 300, K2, K1, 200, 1, 50);
    if(EN1 == 1){ sjx();}  
     
  }else if(SZ1 == 6){
    key2(LCD, 0, 3, K2, K1, 200, 1, 1);
    if(EN1 == 1){   
      if(LCD == 0){       tft.setRotation(2);    
      }else if(LCD == 1){ tft.setRotation(3);
      }else if(LCD == 2){ tft.setRotation(0);
      }else if(LCD == 3){ tft.setRotation(1);   
      } 
      sjx();
    }
    
  }else if(SZ1 == 7){
    key2(LED, 0, 10, K2, K1, 200, 1, 2);
    if(EN1 == 1){
      ledcWrite(2, map(int(LED), 0, 10, 10, 255)); 
      sjx();   
    }
    
  }else if(SZ1 == 8){ 
    if(EN1 == 1){ //恢复默认设置
      SAX = 0;      SVX = 50;    SRX = 1000; 
      SWX = 0;      SVL = 0;     STX = 7200;
      RV1 = 2.12;   RA1 = 4;     RA2 = 10;  
      MVX = 50;     MAX = 20.2;  MWX = 200;  
      LCD = 0;      LED = 10;    
      sjx();
    }  
  }  
}

/*//USB测试模式
void USB(){

}
*/

// ============= 系统初始化函数 =============
// 功能：初始化串口、按键输入、ADC引脚、FET控制引脚、I2C、INA226、显示与PWM
// 说明：在此完成所有外设初始化，确保主循环可直接调用测量与控制函数
void setup(){
  Serial.begin(115200);                    // 串口用于调试输出
  // 按键使用上拉输入
  pinMode(K1, INPUT_PULLUP);
  pinMode(K2, INPUT_PULLUP);
  pinMode(K3, INPUT_PULLUP);
  pinMode(K4, INPUT_PULLUP);

  // ADC采样引脚配置(模拟输入)
  pinMode(avi, INPUT); 
  pinMode(avr, INPUT);
  pinMode(ada, INPUT);
  pinMode(ant, INPUT); 
  pinMode(aua, INPUT);
  
  // FET和外设控制引脚配置(推挽输出)
  pinMode(VG1, OUTPUT);
  pinMode(VG2, OUTPUT);
  pinMode(VG3, OUTPUT);
  pinMode(VG4, OUTPUT);
  pinMode(VG5, OUTPUT);
  pinMode(VG6, OUTPUT);
  pinMode(BUZ, OUTPUT);
  pinMode(BLK, OUTPUT);

  // 初始状态: 大多数保护FET拉高(断开或默认状态),背光关闭
  digitalWrite(VG1, HIGH);
  digitalWrite(VG2, HIGH);  
  digitalWrite(VG3, LOW);   
  digitalWrite(VG4, LOW);
  digitalWrite(VG5, LOW);
  digitalWrite(VG6, LOW);
  digitalWrite(BLK, LOW);

  // 初始化I2C并配置INA226
  Wire.begin(sda, scl);                       // 指定I2C SDA/SCL引脚
  INA1.setMaxCurrentShunt(20.2,0.004);        // 设置INA226的最大测量范围(20.2A, 分流电阻0.004Ω)
  adc_init();                                 // ADC衰减与校准参数初始化
  csh(LCD);                                   // TFT屏幕初始化并传入显示方向

  // PWM通道初始化: ledcSetup(channel, freq, resolution)
  ledcSetup( 0, 40000, 8);    // 风扇PWM: 40kHz,8位
  ledcSetup( 1,  4000, 8);    // 蜂鸣器PWM: 4kHz,8位
  ledcSetup( 2, 20000, 8);    // 背光PWM: 20kHz,8位
  ledcAttachPin(FAN, 0);      // 将通道绑定到引脚
  ledcAttachPin(BUZ, 1);      
  ledcAttachPin(BLK, 2); 
  ledcWrite( 0, 0);           // 风扇初始0
  ledcWrite( 2, map(int(LED), 0, 10, 10, 255) ); // 背光按参数设置

  sjd();                     // 读取Flash参数到运行变量
  BUZZ(200);                 // 启动提示音
  TM1 =  millis();           // 时间标记初始化
  TM3 =  millis();
  TM4 =  millis();
}
 
// ============= 主循环 =============
// 功能：按键检测、测量/保护/控制逻辑调度、定时更新显示与外围设备
// 时序：主循环中核心测量周期为100ms(由TM1控制),波形更新为20ms(在VAPW中)
void loop(){
  // 长按K1(>800ms)用于切换主界面模式MS1(0:控制,1:测量,2:校准)
  if(digitalRead(K1) == LOW){ 
    long int t =  millis();
    while(digitalRead(K1) == LOW){
      if(millis() - t > 800){
        MS1++;  SZ1 = 0;  EN1 = 0; 
        if(MS1 == 1){ VDAC(0, 0);  digitalWrite(VG5, HIGH); } // 进入测量模式时断开驱动
        if(MS1 > 2){ MS1 = 0; }
        BUZZ(100);
        break;
      }
    } 
  }

  // 先做快速测量与动作判断(尽可能快执行以保证保护响应)
  INA();       // 读取INA226与微电流ADC,计算VIN/ I/功率/阻抗等
  VACTP();     // 立即执行过压/过流/过温/过功保护

  // 模式分发: 根据MS1选择不同功能模块
  if(MS1 == 0){       CVRP();   // 数控负载(CC/CV/CR/CP)
  }else if(MS1 == 1){ VAPW();   // 波形/功率测量模式
  }else if(MS1 == 2){ XZCS();   // 参数设置/校准模式                                         
  }else{ MS1 = 0; }

  // 定时100ms更新低频任务: ADC采样(慢)，风扇，显示刷新等
  if((millis() - TM1) > 100){
    VADC();    // 对输入电压/NTC/DAC反馈等做一次校准后的采样
    VFAN();    // 根据NTC温度控制风扇转速
    
    if(MS1 == 0){
      VTP();   // 欠压/超时保护
      // 根据当前子模式SZ1选择不同的显示参数并刷新界面
      if(SZ1 == 0){       ym1(SZ1, EN1, WX1, SAX, SVL, STX, VIN, AIN, VAW, ESR, PWH, VBU, NTC); 
      }else if(SZ1 == 1){ ym1(SZ1, EN1, WX1, SVX, SVL, STX, VIN, AIN, VAW, ESR, PWH, VBU, NTC);        
      }else if(SZ1 == 2){ ym1(SZ1, EN1, WX1, SRX, SVL, STX, VIN, AIN, VAW, ESR, PWH, VBU, NTC);
      }else if(SZ1 == 3){ ym1(SZ1, EN1, WX1, SWX, SVL, STX, VIN, AIN, VAW, ESR, PWH, VBU, NTC);
      }else if(SZ1 == 4){ ym1(SZ1, EN1, WX1, SVL, SVL, STX, VIN, AIN, VAW, ESR, PWH, VBU, NTC);
      }else if(SZ1 == 5){ ym1(SZ1, EN1, WX1, STX, SVL, STX, VIN, AIN, VAW, ESR, PWH, VBU, NTC);   
      }
    }else if(MS1 == 1){
      // 波形/功率测量界面
      ym2(EN1, CS1, VIN, AIN, mAI, VAW, ESR, PWH, VBU, NTC, MVI, MAI, VIA, AIA); 
    }else if(MS1 == 2){
      // 校准/参数设置页面(ym3负责显示当前选项和编辑)
      if(SZ1 == 0){       ym3(SZ1, WX1, EN1, LCD, LED, CS3, VIN, AIN, mAI, RV1, RA1, RA2, MVX, MAX, MWX); 
      }else if(SZ1 == 1){ ym3(SZ1, WX1, EN1, LCD, LED, CS3, VIN, AIN, mAI, RV1, RA1, RA2, MVX, MAX, MWX);
      }else if(SZ1 == 2){ ym3(SZ1, WX1, EN1, LCD, LED, CS3, VIN, AIN, mAI, RV1, RA1, RA2, MVX, MAX, MWX);   
      }else{              ym3(SZ1, WX1, EN1, LCD, LED, CS3, VIN, AIN, mAI, RV1, RA1, RA2, MVX, MAX, MWX);     
      }
    }
    TM1 =  millis();
  }
}

//
