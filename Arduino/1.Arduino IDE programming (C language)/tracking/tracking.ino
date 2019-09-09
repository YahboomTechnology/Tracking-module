/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         tracking.c
* @author       Danny
* @version      V1.0
* @date         2017.07.26
* @brief        tracking
* @details
* @par History  
*
*/
int Left_motor_go = 8;       //AIN2 connects to port 8 of Arduino UNO for control Left motor forward 
int Left_motor_back = 7;     //AIN2 connects to port 7 of Arduino UNO for control Left motor back

int Right_motor_go = 2;      //BIN1 connects to port 2 of Arduino UNO for control Right motor forward 
int Right_motor_back = 4;    //BIN2 connects to port 4 of Arduino UNO for control Right motor back

int Left_motor_pwm = 6;      //PWMA connects to port 6 of Arduino UNO for control the speed of the left motor
int Right_motor_pwm = 5;     //PWMA connects to port 5 of Arduino UNO for control the speed of the right motor

int key = A0;                //Key connects to wiringPi port 10 of Arduino UNO 

//TrackSensorLeftPin1 TrackSensorLeftPin2 TrackSensorRightPin1 TrackSensorRightPin2
//      A2                  A1                  A3                   A4
const int TrackSensorLeftPin1  =  A2;  //The first tracking infrared sensor pin on the left is connected to  wiringPi port A2 of Arduino UNO
const int TrackSensorLeftPin2  =  A1;  //The second tracking infrared sensor pin on the left is connected to  wiringPi port A1 of Arduino UNO
const int TrackSensorRightPin1 =  A3;  //The first tracking infrared sensor pin on the right is connected to  wiringPi port A3 of Arduino UNO
const int TrackSensorRightPin2 =  A4;  //The second tracking infrared sensor pin on the right is connected to  wiringPi port A4 of Arduino UNO

//Define variables to store data collected by each tracking infrared pin
int TrackSensorLeftValue1;
int TrackSensorLeftValue2;
int TrackSensorRightValue1;
int TrackSensorRightValue2;

/**
* Function       setup
* @author        Danny
* @date          2017.07.25
* @brief         Initialization configure
* @param[in]     void
* @retval        void
* @par History   
*/
void setup()
{
  //Initialize the motor drive IO as the output mode
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);

  //Initialize the key interface as the input mode
  pinMode(key, INPUT);

  //Initialize the infrared sensor interface as the input mode
  pinMode(TrackSensorLeftPin1, INPUT);
  pinMode(TrackSensorLeftPin2, INPUT);
  pinMode(TrackSensorRightPin1, INPUT);
  pinMode(TrackSensorRightPin2, INPUT);

  //Initialize the key interface is high level
  digitalWrite(key, HIGH);

  //Initialize the infrared sensor interface is high level
  digitalWrite(TrackSensorLeftPin1, HIGH);
  digitalWrite(TrackSensorLeftPin2, HIGH);
  digitalWrite(TrackSensorRightPin1, HIGH);
  digitalWrite(TrackSensorRightPin2, HIGH);
  
  key_scan();
}

/**
* Function       run
* @author        Danny
* @date          2017.07.26
* @brief         advance
* @param[in1]    left_speed
* @param[in2]    right_speed
* @param[out]    void
* @retval        void
* @par History   
*/
void run(int left_speed, int right_speed)
{
  //Left motor advance
  digitalWrite(Left_motor_go, HIGH);   
  digitalWrite(Left_motor_back, LOW); 
  analogWrite(Left_motor_pwm, left_speed );

  //Right motor advance
  digitalWrite(Right_motor_go, HIGH); 
  digitalWrite(Right_motor_back, LOW); 
  analogWrite(Right_motor_pwm, right_speed);
}

/**
* Function       brake
* @author        Danny
* @date          2017.07.25
* @brief         brake
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   
*/
void brake(int time)
{
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);

  delay(time * 100);
}

/**
* Function       left
* @author        Danny
* @date          2017.07.25
* @brief         turn left (left aheel stop,right wheel advance)
* @param[in1]    left_speed
* @param[in2]    right_speed
* @param[out]    void
* @retval        void
* @par History   
*/
void left(int left_speed, int right_speed)
{
  //Left motor stop
  digitalWrite(Left_motor_go, LOW);    
  digitalWrite(Left_motor_back, LOW); 
  analogWrite(Left_motor_pwm, left_speed);

  //Right motor advance
  digitalWrite(Right_motor_go, HIGH);  
  digitalWrite(Right_motor_back, LOW); 
  analogWrite(Right_motor_pwm, right_speed);
}

/**
* Function       right
* @author        Danny
* @date          2017.07.25
* @brief         turn right(right wheel stop,left wheel advance)
* @param[in1]    left_speed
* @param[in2]    right_speed
* @param[out]    void
* @retval        void
* @par History   
*/
void right(int left_speed, int right_speed)
{
  //Left motor advance
  digitalWrite(Left_motor_go, HIGH);   
  digitalWrite(Left_motor_back, LOW); 
  analogWrite(Left_motor_pwm, left_speed);

  //Roght motor stop
  digitalWrite(Right_motor_go, LOW);  
  digitalWrite(Right_motor_back, LOW); 
  analogWrite(Right_motor_pwm, right_speed);
}

/**
* Function       spin_left
* @author        Danny
* @date          2017.07.25
* @brief         turn left in place(left wheel back,right wheel advance)
* @param[in1]    left_speed
* @param[in2]    right_speed
* @param[out]    void
* @retval        void
* @par History   
*/
void spin_left(int left_speed, int right_speed)
{
  //Left motor back
  digitalWrite(Left_motor_go, LOW);     
  digitalWrite(Left_motor_back, HIGH);  
  analogWrite(Left_motor_pwm, left_speed);

  //Right motor advance
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW); 
  analogWrite(Right_motor_pwm, right_speed);
}

/**
* Function       spin_right
* @author        Danny
* @date          2017.07.25
* @brief          turn right in place(left wheel advance,right wheel back)
* @param[in1]    left_speed
* @param[in2]    right_speed
* @param[out]    void
* @retval        void
* @par History   
*/
void spin_right(int left_speed, int right_speed)
{
  //Left motor advance
  digitalWrite(Left_motor_go, HIGH);    
  digitalWrite(Left_motor_back, LOW);   
  analogWrite(Left_motor_pwm, left_speed);

  //Right motor back
  digitalWrite(Right_motor_go, LOW);    
  digitalWrite(Right_motor_back, HIGH); 
  analogWrite(Right_motor_pwm, right_speed);
}

/**
* Function       back
* @author        Danny
* @date          2017.07.25
* @brief         back
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   
*/
void back(int time)
{
  //Left motor back
  digitalWrite(Left_motor_go, LOW);     
  digitalWrite(Left_motor_back, HIGH); 
  analogWrite(Left_motor_pwm, 40);

  //Right motor back
  digitalWrite(Right_motor_go, LOW);    
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_pwm, 40);

  delay(time );
}

/**
* Function       key_scan
* @author        Danny
* @date          2017.07.25
* @brief         key detection (including software-key debounce)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   
*/
void key_scan()
{
  while (digitalRead(key));       //Loops this code when the key is not pressed
  while (!digitalRead(key))       //When the key is pressed
  {
    delay(10);	                 
    if (digitalRead(key)  ==  LOW)//Re-determine whether the key was pressed
    {
      delay(100);
      while (!digitalRead(key));  //Determine whether the key is released 
    }
  }
}

/**
* Function       loop
* @author        Danny
* @date          2017.07.25
* @brief         
* @param[in]     void
* @retval        void
* @par History   
*/
void loop()
{
   // When the black line is detected, the corresponding indicator of the tracking module is on, and the port level is LOW.
   // When the black line is not detected, the corresponding indicator of the tracking module is off, and the port level is HIGH.
  TrackSensorLeftValue1  = digitalRead(TrackSensorLeftPin1);
  TrackSensorLeftValue2  = digitalRead(TrackSensorLeftPin2);
  TrackSensorRightValue1 = digitalRead(TrackSensorRightPin1);
  TrackSensorRightValue2 = digitalRead(TrackSensorRightPin2);

  //4 tracking pins level status
  // 0 0 X 0
  // 1 0 X 0
  // 0 1 X 0
  if ( (TrackSensorLeftValue1 == LOW || TrackSensorLeftValue2 == LOW) &&  TrackSensorRightValue2 == LOW)
  {
    spin_right(250, 250);
    delay(80);
  }
  //4 tracking pins level status
  // 0 X 0 0       
  // 0 X 0 1 
  // 0 X 1 0       
  //Turn right in place,speed is 150,delay 80ms   
  //Handle left acute angle and left right angle 
  else if ( TrackSensorLeftValue1 == LOW && (TrackSensorRightValue1 == LOW ||  TrackSensorRightValue2 == LOW))
  {
    spin_left(250, 250);
    delay(80);
  }
  // 0 X X X
  //Left_sensor1 detected black line
  else if ( TrackSensorLeftValue1 == LOW)
  {
    spin_left(150, 150);
   //delay(2);
  }
  // X X X 0
  //Right_sensor2 detected black line
  else if ( TrackSensorRightValue2 == LOW )
  {
    spin_right(150, 150);
    //delay(2);
  }
  //4 tracking pins level status
  // X 0 1 X
  else if ( TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == HIGH)
  {
    left(0, 220);
  }
  //4 tracking pins level status
  // X 1 0 X  
  else if (TrackSensorLeftValue2 == HIGH && TrackSensorRightValue1 == LOW)
  {
    right(220, 0);
  }
  //4 tracking pins level status
  // X 0 0 X
  else if (TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == LOW)
  {
    run(255, 255);
  }
  //When the level of 4 pins are 1 1 1 1 , the car keeps the previous running state.
}


