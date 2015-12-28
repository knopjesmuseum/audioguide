#include <Servo.h> 

int sweep = 30;

class Leg {
public:
  float angle=0,speed=1;
  Servo servo;
  
  void update() {
    angle += speed;
    speed *= .98;
    servo.write(90+angle);
  }
  
} front,back;

void setup() { 
  back.servo.attach(8);
  front.servo.attach(9);
  
  front.angle = -sweep;
  back.angle = sweep;
} 
 
void loop() { 
  int r = random(100);
  if (r==1) front.angle = sweep;
  else if (r==2) front.angle = -sweep;
  else if (r==3) back.angle = sweep;
  else if (r==4) back.angle = -sweep;
  
  if (front.angle >= sweep) front.speed = -1;
  if (front.angle <= -sweep) front.speed = 1;
  if (back.angle >= sweep) back.speed = -1;
  if (back.angle <= -sweep) back.speed = 1;
  
  front.update();
  back.update();
  delay(10);
} 

