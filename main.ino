#define PIN_SENS_MOTR 8
#define PIN_SENS_MOTL 7
#define PINPWMR 10
#define PINPWML 11

static int State_lumfinder = 0;

void setup() {

    /*  Setup pinouts   */
    Serial.begin(9600);
    pinMode(PINPWML,OUTPUT);
    pinMode(PINPWMR,OUTPUT);
    pinMode(PIN_SENS_MOTR,OUTPUT);
    pinMode(PIN_SENS_MOTL,OUTPUT);

    // init sens de rotation
    digitalWrite(PIN_SENS_MOTR, HIGH);
    digitalWrite(PIN_SENS_MOTL, HIGH);
}

void loop() {
    Serial.println(LightadjL());

    
    while (State_lumfinder == 0)// while light isn't strong enough
    {
        //search for light by spinning
        PwrMotorR(50);
        PwrMotorL(-50);
        // light threshold to stop searching
        if (analogRead(A1)<35)
        {
            State_lumfinder = 1;
        }
    }
    PwrMotorL(60-LightadjL());
    PwrMotorR(60-LightadjR());
}

int PwrMotorR(int PWM)//    Right motor management
{
    if (PWM>=0)
    {
        digitalWrite(PIN_SENS_MOTR,LOW); //  set forward
    }else
    {
        digitalWrite(PIN_SENS_MOTR,HIGH); //  Set Backward
        PWM = -PWM;
    }
    analogWrite(PINPWMR,PWM*2.56-1); //set pwm
    return 0;
}

int PwrMotorL(int PWM)//    Left motor management
{
    if (PWM>=0)
    {
        digitalWrite(PIN_SENS_MOTL,LOW); //  set forward
    }else
    {
        digitalWrite(PIN_SENS_MOTL,HIGH); //  Set Backward
        PWM = -PWM;
    }
    analogWrite(PINPWML,PWM*2.56-1); //set pwm
    return 0;
}
int LightadjR() // speed adjustment right motor
{
    int Output_value;
    int derivee = Photo_deriv();
    
    if (derivee<0) 
    {
        Output_value = -derivee*10;
    }else Output_value = 0;

    if (Output_value>60)
    {
        Output_value=60;
    }
    return Output_value;
}

int Photo_deriv()
{
    /*int SensorValueD = analogRead(A0);
    int SensorValueG = analogRead(A1);*/

    // as the light increase the value of the sensor became closser to 0
    int derivee = analogRead(A0) - analogRead(A1);

    //if derivee positive robot is turning right
    //if derivee negative robot is turning left
    return derivee;
}

int LightadjL()// speed adjustment left motor
{
    int Output_value;
    int derivee = Photo_deriv();
    if (derivee>0) 
    {
        Output_value = derivee*10;
    }else Output_value = 0;

    if (Output_value>60)
    {
        Output_value=60;
    }
    return Output_value;
}