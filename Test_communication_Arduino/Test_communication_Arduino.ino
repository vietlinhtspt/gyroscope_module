//Arduino code
#include <SoftwareSerial.h>
SoftwareSerial s(5,6);
 
void setup() {
s.begin(115200);
}
 
void loop() {
int data=50;
if(s.available()>0)
{
 s.write(data);
}
}
