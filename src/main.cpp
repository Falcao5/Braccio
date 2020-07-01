#include <Arduino.h>
#include <Braccio.h>
#include <Servo.h>

/* COMMANDS FROM KEYBOARD */
#define BASE_KEY_INC        '4'
#define BASE_KEY_DEC        '6'
#define SHOULDER_KEY_INC    '8'
#define SHOULDER_KEY_DEC    '2'
#define ELBOW_KEY_INC       'w'
#define ELBOW_KEY_DEC       's'
#define WRIST_ROT_KEY_INC   'r'
#define WRIST_ROT_KEY_DEC   'f'
#define WRIST_VER_KEY_INC   'a'
#define WRIST_VER_KEY_DEC   'd'
#define GRIPPER_KEY_INC     '5'
#define GRIPPER_KEY_DEC     '0'

#define DEG_INFO_KEY        'i'
#define RESET_POSITION_KEY  ' '

/* BOUNDS FOR SERVOS LIMITS */
#define BASE_MAX_DEG        180
#define SHOULDER_MAX_DEG    170
#define ELBOW_MAX_DEG       180
#define WRIST_ROT_MAX_DEG   200
#define WRIST_VER_MAX_DEG   180
#define GRIPPER_MAX_DEG     73

#define BASE_MIN_DEG        0
#define SHOULDER_MIN_DEG    0
#define ELBOW_MIN_DEG       0
#define WRIST_ROT_MIN_DEG   0
#define WRIST_VER_MIN_DEG   0
#define GRIPPER_MIN_DEG     0

/* VERTICAL POSITION DEFINITIONS */
#define STEP_DELAY_VERTICAL 10
#define BASE_VERTICAL       90
#define SHOULDER_VERTICAL   96
#define ELBOW_VERTICAL      94
#define WRIST_ROT_VERTICAL  90
#define WRIST_VER_VERTICAL  90
#define GRIPPER_VERTICAL    73

/* MISC */
#define DEG_OFFSET          2

/* DECLARED AS EXTERN IN Braccio.h */
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

/* GLOBAL VARIABLES */
int step_delay;
int count;

/* GLOBAL VARIABLES FOR SERVOS DEGREES */
int base_deg;
int shoulder_deg;
int elbow_deg;
int wrist_rot_deg;
int wrist_ver_deg;
int gripper_deg;

void printServosDegrees()
{
    Serial.println("Servos degrees *******************\n");

    Serial.print("Base: ");
    Serial.println(base_deg);

    Serial.print("Shoulder: ");
    Serial.println(shoulder_deg);

    Serial.print("Elbow: ");
    Serial.println(elbow_deg);

    Serial.print("Wrist rotation: ");
    Serial.println(wrist_rot_deg);

    Serial.print("Wrist vertical: ");
    Serial.println(wrist_ver_deg);

    Serial.print("Gripper: ");
    Serial.println(gripper_deg);
}

void setToVerticalPosition()
{
    base_deg =      BASE_VERTICAL;
    shoulder_deg =  SHOULDER_VERTICAL;
    elbow_deg =     ELBOW_VERTICAL;
    wrist_rot_deg = WRIST_ROT_VERTICAL;
    wrist_ver_deg = WRIST_VER_VERTICAL;
    gripper_deg =   GRIPPER_VERTICAL;
    Serial.println("Set in vertical position");
}

void setup() 
{  
    Braccio.begin();
    Serial.begin(9600);

    /* INITIZATION */
    step_delay = STEP_DELAY_VERTICAL;
    setToVerticalPosition();
    count = 0;

    /* Sets the Braccio in the VERTICAL position */
    /*                    (step delay,          M1,         M2,            M3,      M4,             M5,             M6);    */
    Braccio.ServoMovement(step_delay,         base_deg, shoulder_deg, elbow_deg, wrist_rot_deg, wrist_ver_deg, gripper_deg);  
}

char readSerialData()
{
    char result;

    if(Serial.available())
    {
        result = Serial.read();

        Serial.print("(");
        Serial.print(count);
        Serial.print(") ");
        count++;
    }

    return result;
}

/**
 * degrees: the variable i want to modify
 * bound:   the max/min limit
 * incDec:  if 0 increases degrees in a range between the bound range
 *          if 1 decreases degrees in a range between the bound range
*/
void move(int *degrees, int bound, int incDec)
{

    if(incDec == 0)
    {
        if(*degrees < bound)
        {
            *degrees+=DEG_OFFSET;
            Serial.println(*degrees);
        }
        else
        {
            Serial.println("Can't move more");
        }
    }
    else 
    if (incDec == 1)
    {
        if(*degrees > bound)
        {
            *degrees-=DEG_OFFSET;
            Serial.println(*degrees);
        }
        else
        {
            Serial.println("Can't move more");
        }
    }
}

void loop() 
{
    char c;

    c = readSerialData();

    /* M1 */
    if(c == BASE_KEY_INC)
    {
        move(&base_deg, BASE_MAX_DEG, 0);
    }

    if(c == BASE_KEY_DEC)
    {
        move(&base_deg, BASE_MIN_DEG, 1);
    }

    /* M2 */
    if(c == SHOULDER_KEY_INC)
    {
        move(&shoulder_deg, SHOULDER_MAX_DEG, 0);
    }

    if(c == SHOULDER_KEY_DEC)
    {
        move(&shoulder_deg, SHOULDER_MIN_DEG, 1);
    }

    /* M3 */
    if(c == ELBOW_KEY_INC)
    {
        move(&elbow_deg, ELBOW_MAX_DEG, 0);
    }

    if(c == ELBOW_KEY_DEC)
    {
        move(&elbow_deg, ELBOW_MIN_DEG, 1);
    }

    /* M4 */
    if(c == WRIST_ROT_KEY_INC)
    {
        move(&wrist_rot_deg, WRIST_ROT_MAX_DEG, 0);
    }

    if(c == WRIST_ROT_KEY_DEC)
    {
        move(&wrist_rot_deg, WRIST_ROT_MIN_DEG, 1);
    }

    /* M5 */
    if(c == WRIST_VER_KEY_INC)
    {
        move(&wrist_ver_deg, WRIST_VER_MAX_DEG, 0);
    }

    if(c == WRIST_VER_KEY_DEC)
    {
        move(&wrist_ver_deg, WRIST_VER_MIN_DEG, 1);
    }

    /* M6 */
    if(c == GRIPPER_KEY_INC)
    {
        move(&gripper_deg, GRIPPER_MAX_DEG, 0);
    }

    if(c == GRIPPER_KEY_DEC)
    {
        move(&gripper_deg, GRIPPER_MIN_DEG, 1);
    }

    /* WRITE INFORMATION ABOUT SERVOS DEGREES */
    if(c == DEG_INFO_KEY)
    {
        printServosDegrees();
    }

    /* RESET TO THE DEFAULT POSITION */
    if(c == RESET_POSITION_KEY)
    {
        setToVerticalPosition();    // Vertical
    }

    // Move the Braccio in selected position
	Braccio.ServoMovement(step_delay, base_deg, shoulder_deg, elbow_deg, wrist_rot_deg, wrist_ver_deg, gripper_deg);  
}
