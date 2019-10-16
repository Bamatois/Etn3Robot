#define PINSENSR 8
#define PINSENSL 7
#define PINPWMR 10
#define PINPWML 11

static int State_lumfinder = 0;

void setup() {

    /*  Setup pinouts   */
    Serial.begin(9600);
    pinMode(PINPWML,OUTPUT);
    pinMode(PINPWMR,OUTPUT);
    pinMode(PINSENSR,OUTPUT);
    pinMode(PINSENSL,OUTPUT);
    digitalWrite(PINSENSR, HIGH);
    digitalWrite(PINSENSL, HIGH);
}

void loop() {
    Serial.println(LightadjL());

    
    while (State_lumfinder == 0)
    {
        PwrMotorR(50);
        PwrMotorL(-50);
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
        digitalWrite(PINSENSR,LOW); //  set forward
    }else
    {
        digitalWrite(PINSENSR,HIGH); //  Set Backward
        PWM = -PWM;
    }
    analogWrite(PINPWMR,PWM*2.56-1); //set pwm
    return 0;
}

int PwrMotorL(int PWM)//    Left motor management
{
    if (PWM>=0)
    {
        digitalWrite(PINSENSL,LOW); //  set forward
    }else
    {
        digitalWrite(PINSENSL,HIGH); //  Set Backward
        PWM = -PWM;
    }
    analogWrite(PINPWML,PWM*2.56-1); //set pwm
    return 0;
}
int LightadjR()
{
    int Output_value;
    int derivee = Photo_deriv();
    
    if (derivee<0) 
    {
        Output_value = derivee*10;
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
    int derivee = analogRead(A0) - analogRead(A1);
    return derivee;
}

int LightadjL()
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