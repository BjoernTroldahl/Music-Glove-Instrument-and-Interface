
int flex0 = A0;
int flex1 = A1;
int flex2 = A2; 
int flex3 = A3;

int data0;
int data1; 
int data2;
int data3; 
int currentMill = 1;

void setup()
{
  Serial.begin(9600); 
  pinMode(flex0, INPUT);  
  pinMode(flex1, INPUT);
  pinMode(flex2, INPUT);  
  pinMode(flex3, INPUT);
}
 
void loop()
{
  data0 = analogRead(flex0); 
  data1 = analogRead(flex1); 
  data2 = analogRead(flex2); 
  data3 = analogRead(flex3);
  
  String imuLine = "";
  //Index finger
  //imuLine.concat("index: ");
  imuLine.concat(String(data0));
  imuLine.concat(", ");
  //Middle finger
  //imuLine.concat("middle: ");
  imuLine.concat(String(data1));
  imuLine.concat(", ");
  //Ring finger
  //imuLine.concat("ring: ");
  imuLine.concat(String(data2));
  imuLine.concat(", ");
  //Pinky finger
  //imuLine.concat("pinky: ");
  imuLine.concat(String(data3));
  Serial.println(imuLine);
  delay(8); 
}
