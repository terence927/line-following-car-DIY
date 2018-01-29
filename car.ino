long sensors[] = {0, 0, 0, 0};
int ratio[] = {0, 4, 5, 9};
int pos, sum, proportional, integral, derivative, last_proportional, error_value;
long avg;
int right_speed, left_speed;
#define max_speed 110
#define set_point 5500
#define spdoffset 0 //9V 51
#define Kp 0.045

#define Ki 0
#define Kd 0
void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  sensor();
}
void left(int spd)
{
  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);
  analogWrite(9, spd*(0.000785*spd+0.78)); //
}
void right(int spd)
{
  digitalWrite(8, HIGH);
  digitalWrite(7, LOW);
  analogWrite(6, spd); //
}
void turn()
{ //Restricting the error value between +256.
  if (error_value < -255)
  {
    error_value = -255;
  }
  if (error_value > 255)
  {
    error_value = 255;
  }

  if (error_value < 0)
  {
    right_speed = max_speed + error_value;
    left_speed = max_speed;
  }
  // Iferror_value is greater than zero calculate left turn values
  else
  {
    right_speed = max_speed;
    left_speed = max_speed - error_value;
  }
  if (left_speed < 0) {
    right_speed = right_speed - left_speed;
    left_speed = 0;
  }
  else if (right_speed < 0) {
    left_speed = left_speed - right_speed;
    right_speed = 0;
  }
  left(left_speed);
  right(right_speed);
  Serial.print(left_speed);
  Serial.print(right_speed);
  sensor();
}
void pid()
{ pos = int(avg / sum);
  proportional = pos - set_point; // Replace set_point by your set point
  integral = integral + proportional;
  derivative = proportional - last_proportional;
  last_proportional = proportional;
  error_value = int(proportional * Kp + integral * Ki + derivative * Kd);
  Serial.println(error_value);
}
void sensor()
{
  avg = 0;
  sum = 0;
  Serial.println("-----------------------------------------------");
  for (int i = 0; i <= 3; i++)
  {
    sensors[i] = analogRead(i);
    avg += sensors[i] * ratio[i] * 1000; //Calculating the weighted mean
    sum += int(sensors[i]);
    Serial.print(sensors[i]);
    Serial.print(" ");
  } //Calculating sum of sensor readings
  /**Serial.print("*********");
    pos = int(avg / sum);
    Serial.print(avg);
    Serial.print(" ");
    Serial.print(sum);
    Serial.print(" ");
    Serial.print(pos);
    Serial.println();**/
  pid();
  turn();
}
void loop() {
  //pid();
  //turn();
  //left(max_speed);221??2?
  //right(max_speed)3;
}
