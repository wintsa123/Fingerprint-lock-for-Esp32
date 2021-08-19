#include "font.h"
#include "set.h"
void key_init()
{
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
}
void Mg966r()
{
  myServo.attach(18);//舵机接口
  myServo.write(180);
  myServo.write(0);
  Blinker.delay(5000);
  myServo.write(180);
  Blinker.delay(400);
  myServo.detach();
}
bool oState = false;
void Menu()    //主页
{
  int hours = timeClient.getHours();
  int minu =  timeClient.getMinutes();
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_gb16st_t_2);
    u8g2.setCursor(40, 10);
    u8g2.print("Indoor");
    u8g2.setFont(u8g2_font_helvB14_tf);
    u8g2.drawXBMP(27, 18, 17, 16, wendu);
    u8g2.setCursor(43, 34);
    u8g2.print(t);
    u8g2.drawCircle(91, 22, 2, U8G2_DRAW_ALL); //圆
    u8g2.setCursor(92, 35);
    u8g2.print("C");
    u8g2.drawXBMP(30, 33, 12, 16, WATER);
    u8g2.setCursor(43, 50);
    u8g2.print(h);
    u8g2.print("%");
    u8g2.drawHLine(0, 51, 128); //线
    u8g2.setFont(u8g2_font_t0_16b_mn);
    u8g2.setCursor(0, 64);
    u8g2.print(hours);
    u8g2.print(":");
    if (minu < 10) {
      u8g2.print("0");
      u8g2.print(minu);
    }
    else {
      u8g2.print(minu);
    }
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2.setCursor(40, 62);
    u8g2.print("语音开门");
  } while (u8g2.nextPage());
}


//APP
#define BUTTON_1 "ButtonKey"
BlinkerButton Button1(BUTTON_1);                  //点灯按键
void button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);

  if (state == BLINKER_CMD_BUTTON_TAP) {
    BLINKER_LOG("Button tap!");
    Button1.color("#FFFFFF");
    Button1.text("开门");
    Button1.print();
    myServo.attach(18); //D4
    myServo.write(0);
    Blinker.delay(5000);
    myServo.write(180);
    Blinker.delay(400);
    myServo.detach();
  }
  else {
    BLINKER_LOG("Get user setting: ", state);

    Button1.icon("icon_1");
    Button1.color("#FFFFFF");
    Button1.text("Your button name or describe");
    // Button1.text("Your button name", "describe");
    Button1.print();
  }
}



void miotPowerState(const String & state)              //点灯电源
{
  BLINKER_LOG("need set power state: ", state);

  if (state == BLINKER_CMD_ON) {
    myServo.attach(18); //D4
    myServo.write(180);

    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();

    oState = true;
    myServo.write(0);
    Blinker.delay(5000);
    myServo.write(180);
    Blinker.delay(400);
    myServo.detach();
    oState = false;
  }
  else if (state == BLINKER_CMD_OFF) {
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();

    oState = false;
  }
}

void miotQuery(int32_t queryCode)                      //状态回调
{
  BLINKER_LOG("MIOT Query codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_ALL_NUMBER :
      BLINKER_LOG("MIOT Query All");
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
    default :
      BlinkerMIOT.powerState(oState ? "on" : "off");
      BlinkerMIOT.print();
      break;
  }
}

void dataRead(const String & data)                //点灯数据保存
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}

void Add_FR()      //添加指纹
{

  int i, ensure, processnum = 0;
  int ID_NUM = 0;
  char str2[10];
  while (1)
  {
    switch (processnum)
    {
      case 0:
        i++;
        u8g2.firstPage();
        do
        {
          u8g2.drawXBMP(32, 24, 64, 16, State5); /* 字串 请按手指   64x16  */
        }
        while (u8g2.nextPage());
        ensure = finger.getImage();
        if (ensure == FINGERPRINT_OK)
        {
          ensure = finger.image2Tz(1); //生成特征
          if (ensure == FINGERPRINT_OK)
          {
            u8g2.firstPage();
            do
            {
              u8g2.drawXBMP(32, 24, 64, 16, State6); // 字串 指纹正常  64x16
            }
            while (u8g2.nextPage());
            // Serial.println(" 000 is true");
            i = 0;
            processnum = 1; //跳到第二步
          }
          else {};
        }
        else {};
        break;

      case 1:
        i++;
        u8g2.firstPage();
        do
        {
          u8g2.drawXBMP(32, 24, 64, 16, State7); //字串 再按一次   64x16
        }
        while (u8g2.nextPage());
        ensure = finger.getImage();
        if (ensure == FINGERPRINT_OK)
        {
          ensure = finger.image2Tz(2); //生成特征
          if (ensure == FINGERPRINT_OK)
          {
            u8g2.firstPage();
            do
            {
              u8g2.drawXBMP(32, 24, 64, 16, State6); // 字串 指纹正常  64x16
            }
            while (u8g2.nextPage());
            i = 0;
            processnum = 2; //跳到第三步
          }
          else {};
        }
        else {};
        break;

      case 2:
        u8g2.firstPage();
        do
        {
          u8g2.drawXBMP(32, 24, 64, 16, State8); // 字串 创建模板   64x16
        }
        while (u8g2.nextPage());
        ensure = finger.createModel();
        if (ensure == FINGERPRINT_OK)
        {
          u8g2.firstPage();
          do
          {
            u8g2.drawXBMP(16, 24, 96, 16, State9); // 字串 模板创建成功   96x16
          }
          while (u8g2.nextPage());
          processnum = 3; //跳到第四步
        }
        else
        {
          u8g2.firstPage();
          do
          {
            u8g2.drawXBMP(16, 24, 96, 16, State10); // 字串 模板创建失败   96x16
          }
          while (u8g2.nextPage());
          i = 0;
          processnum = 0; //跳回第一步
        }
        delay(500);
        break;
      case 3:

        u8g2.firstPage();
        do
        {
          u8g2.drawXBMP(1, 0, 128, 48, State11);
          // 字串  按K4加，按K2减 按K3保存 0=< ID <=99 128x48
          u8g2.setFont(u8g2_font_gb16st_t_2); // 选择字体
          u8g2.drawStr(40, 62, "ID=00");
        }
        while (u8g2.nextPage());

        while (digitalRead(RIGHT) != 0)
        {

          if (digitalRead(DOWN) == 0)
          {
            //key_num = 0;
            if (ID_NUM > 0)
              ID_NUM--;
            if (ID_NUM < 10)
              sprintf(str2, "ID=0%d", ID_NUM);
            else
              sprintf(str2, "ID=%d", ID_NUM);
            u8g2.firstPage();
            do
            {
              u8g2.setFont(u8g2_font_gb16st_t_2); // 选择字体
              u8g2.drawXBMP(1, 0, 128, 48, State11);
              u8g2.drawStr(40, 62, str2);
            }
            while (u8g2.nextPage());
          }
          if (digitalRead(UP) == 0)
          {
            if (ID_NUM < 99)
              ID_NUM++;
            if (ID_NUM < 10)
              sprintf(str2, "ID=0%d", ID_NUM);
            else
              sprintf(str2, "ID=%d", ID_NUM);
            u8g2.firstPage();
            do
            {
              u8g2.setFont(u8g2_font_gb16st_t_2);
              u8g2.drawStr(40, 62, str2);
              u8g2.drawXBMP(1, 0, 128, 48, State11);
            }
            while (u8g2.nextPage());
          }
        }
        ensure = finger.storeModel(ID_NUM); //储存模板
        if (ensure == 0x00)
        {
          u8g2.firstPage();
          do
          {
            u8g2.drawXBMP(16, 24, 96, 16, State12); /* 字串 录入指纹成功   96x16  */
          }
          while (u8g2.nextPage());
          Serial.println("FR receive OK");
          delay(1500);
          return ;
        }
        else
        {
          processnum = 0;
        }
        break;
    }
    delay(400);
    if (i == 10) //超过5次没有按手指则退出
    {
      break;
    }
  }
}
void Del_FR()          //删除指纹
{

  int  ensure;
  int ID_NUM = 0;
  char str2[10];
  sprintf(str2, "ID=0%d", ID_NUM);
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_gb16st_t_2);
    u8g2.drawStr(44, 62, str2);
    u8g2.drawXBMP(1, 0, 128, 48, State15); //显示字模汉字
  }
  while (u8g2.nextPage());



  while (digitalRead(RIGHT) != 0)
  {

    if (digitalRead(DOWN) == 0)
    {
      //key_num = 0;
      if (ID_NUM > 0)
        ID_NUM--;
      if (ID_NUM < 10)
        sprintf(str2, "ID=0%d", ID_NUM);
      else
        sprintf(str2, "ID=%d", ID_NUM);
      u8g2.firstPage();
      do
      {
        u8g2.drawStr(44, 62, str2);
      }
      while (u8g2.nextPage());

    }
    if (digitalRead(UP) == 0)
    {
      if (ID_NUM < 99)
        ID_NUM++;
      if (ID_NUM < 10)
        sprintf(str2, "ID=0%d", ID_NUM);
      else
        sprintf(str2, "ID=%d", ID_NUM);
      u8g2.firstPage();
      do
      {
        u8g2.drawStr(44, 62, str2);

      }
      while (u8g2.nextPage());

    }
    if (digitalRead(LEFT) == 0) {
      return ;
    }
    if (digitalRead(UP) == 0 && digitalRead(DOWN) == 0)
    {

      ensure = finger.emptyDatabase(); //清空指纹库
      if (ensure == 0)
      {
        u8g2.firstPage();
        do
        {
          u8g2.drawXBMP(8, 16, 80, 16, State17); //显示字模汉字
          u8g2.drawXBMP(88, 16, 32, 16, State19); //显示字模汉字
        }
        while (u8g2.nextPage());

      }
      else
      {
        u8g2.firstPage();
        do
        {
          u8g2.drawXBMP(8, 16, 80, 16, State17); //显示字模汉字
          u8g2.drawXBMP(88, 16, 32, 16, State20); //显示字模汉字

        }
        while (u8g2.nextPage());

      }
      delay(1500);
      return ;
    }
  }
  ensure = finger.deleteModel(ID_NUM); //删除单个指纹
  if (ensure == 0)
  {
    u8g2.firstPage();
    do
    {
      u8g2.drawXBMP(16, 16, 64, 16, State18); //显示字模汉字
      u8g2.drawXBMP(80, 16, 32, 16, State19); //显示字模汉字
    }
    while (u8g2.nextPage());


  }
  else
  {
    u8g2.firstPage();
    do
    {
      u8g2.drawXBMP(16, 16, 64, 16, State18); //显示字模汉字
      u8g2.drawXBMP(80, 16, 32, 16, State20); //显示字模汉字

    }
    while (u8g2.nextPage());

  }
  delay(1500);
}

void setup()
{
  key_init();
  dht.begin();
  myServo.attach(18);
  myServo.write(0);
  myServo.detach();
  Serial.begin(115200);
  u8g2.begin();
  finger.begin(57600);
  Blinker.begin(auth, ssid, pswd);
  timeClient.begin();
  Menu();
  Blinker.attachData(dataRead);
  Button1.attach(button1_callback);//APP
  BlinkerMIOT.attachPowerState(miotPowerState);
  BlinkerMIOT.attachQuery(miotQuery);
  u8g2.enableUTF8Print();
}


void loop()
{
  Blinker.run();
  timeClient.update();
  Menu();
  if (digitalRead(UP) == 0)           //按下K1键 调用Add_FR()
  {
    Add_FR();
  }
  if (digitalRead(DOWN) == 0)           //按下K2键 调用Del_FR();
  {
    Del_FR();
  }
  int ensure, i;          // 验证指纹并开锁 函数
  char str[20];
  char cishu[5];
  u8g2.firstPage();


  ensure = finger.getImage();
  if (ensure == 0x00) //获取图像成功
  {
    ensure = finger.image2Tz();
    if (ensure == 0x00) //生成特征成功
    {
      ensure = finger.fingerFastSearch();
      if (ensure == 0x00) //搜索成功
      {
        Mg966r();
        q++;
        sprintf(str, "ID: % d Score: % d", finger.fingerID, finger.confidence);
        sprintf(cishu, " % d", q);
        u8g2.firstPage();
        do
        {
          u8g2.setFont(u8g2_font_gb16st_t_2); // 选择字体
          u8g2.drawXBMP(16, 16, 96, 16, State13); //显示指纹搜索成功
          u8g2.drawStr(100, 16, cishu);
          u8g2.drawStr(1, 46, str);
        }
        while (u8g2.nextPage());
        //Serial.println("OPEN");
        delay(1500);
      }
      else
      {
        u8g2.firstPage();
        do
        {
          u8g2.drawXBMP(16, 16, 96, 16, State14); //State14_字串 未搜索到指纹 96x16
        }
        while (u8g2.nextPage());
        delay(1500);
      }
    }
  }
  else
  {
    //ShowErrMessage(ensure);
  }

  delay(200);








}
