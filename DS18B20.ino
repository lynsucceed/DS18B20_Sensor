#define DSIO 5
boolean Ds18b20_Init()
{
  boolean check = false;
  pinMode(DSIO,OUTPUT);
  digitalWrite(DSIO,1);    //数据总线复位
 delayMicroseconds(10);
  digitalWrite(DSIO,0); //将总线拉低480us~960us
  delayMicroseconds(720);
  digitalWrite(DSIO,1); //然后拉高总线
  pinMode(DSIO,INPUT);
  delayMicroseconds(60); //若DS18B20做出反应会将在15us~60us后将总线拉低
  if(digitalRead(DSIO)==0)
   {
      delayMicroseconds(240); //DS18b20将总线拉低60us~240us
      check = true;           //DS18b20初始化成功，上拉电阻拉高总线
      digitalWrite(DSIO,1);
   }
    return  check;
}

void Ds18b20_writeByte(byte dat)
{
  int j = 0x01;
  pinMode(DSIO,OUTPUT);
  for(int i=1;i<=8;i++)
  {
    digitalWrite(DSIO,0);  //主机拉低总线至少60us
    delayMicroseconds(15);
    digitalWrite(DSIO,dat&j);
    delayMicroseconds(45);
    digitalWrite(DSIO,1);  //主机拉高总线
    j<<=1;
  }
  
}

int Ds18b20_readByte()
{
  int data = 0;
  for(int i=1;i<=8;i++)
  {
    pinMode(DSIO,OUTPUT);
    digitalWrite(DSIO,0);
    data>>=1;
    digitalWrite(DSIO,1);
    pinMode(DSIO,INPUT);                 
    if(digitalRead(DSIO)==1)       
    {
      data |= 0x80;
    }
   delayMicroseconds(45); //60us为一次读周期
  }
  return data;
}

void  Ds18b20_ChangTemp()   //温度转换函数
{
  Ds18b20_Init();
  delay(1);
  Ds18b20_writeByte(0xcc);   //跳过ROM操作命令    
  Ds18b20_writeByte(0x44);   //温度转换命令
  delay(100);
}

void  Ds18b20_ReadTempCom()  //温度读取命令
{  
  Ds18b20_Init();
  delay(1);
  Ds18b20_writeByte(0xcc);  //跳过ROM操作命令
  Ds18b20_writeByte(0xbe);  //发送读取温度命令
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Ds18b20_Init()==true)
  {
    int TempL=0;
    int TempH=0;
    int Temp=0;    
    Ds18b20_ChangTemp();
    Ds18b20_ReadTempCom();
    TempL = Ds18b20_readByte();
    TempH = Ds18b20_readByte();
    Temp =  (TempH<<8)|TempL;
    Serial.print("Temperature:");
    Serial.print(Temp*0.0625);
    Serial.println("℃");
    delay(1000);
  }
    Serial.println("DS18B20 Sensor not exist");
    delay(1000);
}
