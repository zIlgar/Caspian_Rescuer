# include <Adafruit_MPU6050.h>
# include <Adafruit_Sensor.h>
# include <SoftwareSerial.h>
# include <Servo.h>
# include <Wire.h>
# include "DHT.h"

# define DHTTYPE DHT11
# define POT A2

const int TRIG = 3;
const int ECHO = 2;
Servo servo;
Servo esc;

const int DHT11_PIN = 11;
const int SERVO_PIN = 4;
const int SERVO_TRANSISTOR = 7;
const int ESC_TRANSISTOR = 5;
const int ESC_PIN = 8;

DHT dht(DHT11_PIN, DHTTYPE);

int humidity, temperature;
float speed_of_sound = 0.034f;
int duration, distance;

char new_device[] = "\ndate:%d-%d-%d\ntime:%d.%d.%d\nrequest_type:%s\nrequest_id:%d\ndevice_id:%d\ngps:%d\ngps_find_satellite:%d\ngps_data:%d,%d\ncompass_angle:%d\nultrasonic:%d\nhumidity:%d\ntemperature:%d\nservo_angle:%d\n";
char cur_req[] = "\ndate:%d-%d-%d\ntime:%d.%d.%d\nrequest_type:%s\nrequest_id:%d\ndevice_id:%d\ngps:%d\ngps_find_satellite:%d\ngps_data:%d,%d\nmust_go:%d,%d\ndistance:%d\ncompass_angle:%d\nmust_go_angle:%d\nultrasonic:%d\nhumidity:%d\ntemperature:%d\nservo_angle:%d\n";

char buf[300];

short day_s, month_s, year_s;
short hour_s, minute_s, second_s;
char * req_type;
short req_id, dev_id;
short gps_w;
short gps_f_s;
float x, y;
float m_x, m_y;
int x_, y_;
int m_x_, m_y_;
int dis;
short comp_ang, m_comp_ang;
short ultra, humid, temp;
short servo_ang;

int pot_value;
int safe_strt=0; //test_for_esc
String msg = "";

SoftwareSerial ss(19, 18);

// Adafruit_MPU6050 mpu;
double gyro_value = 0;

void readDHT11() {
    humidity = dht.readHumidity() * 100;
    temperature = dht.readTemperature() * 100;
}

void readHCSR04() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH);
    distance = duration * speed_of_sound / 2;
}
void esc_safe() {
  if (safe_strt<45) {
    esc.write(safe_strt);
    safe_strt=safe_strt+5;
    delay(150);
    }
  else{
    esc.write(safe_strt);
    }
 //   Serial.print(safe_strt);
 //   Serial.print("\n");
 // ESC.write(safe_strt);
}

void servo_strt() {
  servo.write(90);
}

void send_msg(char buf[], short send_size) {
    Serial.print("begin");

    for (int i = 0; i < strlen(buf); i ++) {
        Serial.print(buf[i]);

        delay(1000 / send_size);
    }
    
    Serial.print("end");
}

void setup() {
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    pinMode(SERVO_TRANSISTOR, OUTPUT);
    pinMode(ESC_TRANSISTOR, OUTPUT);

    readDHT11();
    readHCSR04();

    day_s = 14;
    month_s = 9;
    year_s = 2023;

    hour_s = 16;
    minute_s = 14;
    second_s = 0;

    req_type = "NEW_DEVICE";
    req_id = 0;
    dev_id = 1991;

    gps_w = 1;
    gps_f_s = 1;

    x = 40.426;
    y = 49.854;

    x_ = x * 1000;
    y_ = y * 1000;

    m_x = 1.0;
    m_y = 1.0;

    m_x_ = m_x * 1000;
    m_y_ = m_y * 1000;

    comp_ang = 90;
    servo_ang = 0;

    Serial.begin(57600);
    ss.begin(57600);

    dht.begin();

    while (true) {
        Serial.write("abcdefghijklmnopqrstuvwxyz");

        if (Serial.available()) {
            while (Serial.available() > 0) { Serial.read(); }
            break;
        }
        
        delay(1000);
    }

    sprintf(buf, new_device, year_s, month_s, day_s, hour_s, minute_s, second_s, req_type, req_id, dev_id, gps_w, gps_f_s, x_, y_, comp_ang, distance, humidity, temperature, servo_ang);

    send_msg(buf, 64);

    req_type = "CUR_DATA";

    servo.attach(SERVO_PIN);
    esc.attach(ESC_PIN, 1000, 2000);

    // while (!mpu.begin()) {
    //     delay(10);
    // }

    // mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    // mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    // mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

      esc.write(safe_strt); //test_for_esc
      //delay(1000);  //test_for_esc
      digitalWrite(ESC_TRANSISTOR, HIGH);
      digitalWrite(SERVO_TRANSISTOR, HIGH);
}

void loop() {
    //pot_value = analogRead(POT);
    //pot_value = map(pot_value, 0, 1023, 0, 180);

    //esc.write(pot_value)

    // sensors_event_t a, g, temp;
    // mpu.getEvent(&a, &g, &temp);
    // gyro_value += g.gyro.z;

    //Serial.println(gyro_value * 24);

    // if (gyro_value > 0)
    //     gyro_value = gyro_value - ((int) gyro_value / 60)  * 60;
    // else
    //     gyro_value = gyro_value - ((int) gyro_value / - 60) * - 60;

    delay(100);

    readDHT11();
    readHCSR04();
    esc_safe();
    servo_strt();

    char c;

    while (Serial.available() > 0) {
        c = (char) Serial.read();
        msg.concat(c);
    }

    msg.trim();

    Serial.println(msg);

    //Serial.println(msg);

    if (msg == "servo") {
        servo.write(20);
        delay(1000);
        servo.write(140);
        delay(1000);
        servo.write(90);
        delay(1000);
        msg = "";
    } else if (msg == "motor") {
        esc.write(40);
        delay(2000);
        esc.write(0);
        delay(4000);
        esc.write(40);
        delay(2000);
        msg = "";
      }
else if (msg == "auto") {
        digitalWrite(ESC_TRANSISTOR, HIGH);
        digitalWrite(SERVO_TRANSISTOR, HIGH);
//        esc.write(0);
//        delay(2000);
//        esc.write(40);
        msg = "";
      }
      else if (msg == "manual") {
        digitalWrite(ESC_TRANSISTOR, LOW);
        digitalWrite(SERVO_TRANSISTOR, LOW);
        esc.write(0);
        Serial.println(msg);
      }

    msg = "";

    // comp_ang = gyro_value * 24;
    m_comp_ang = 90;
    servo_ang = servo.read();

    // servo.write(45 + min(max(gyro_value * - 24, - 45), 45));
    // Serial.println(45 + min(max(gyro_value * - 24, - 45), 45));

    sprintf(buf, cur_req, year_s, month_s, day_s, hour_s, minute_s, second_s, req_type, req_id, dev_id, gps_w, gps_f_s, x_, y_, m_x_, m_y_, dis, comp_ang, m_comp_ang, distance, humidity, temperature, servo_ang);

    send_msg(buf, 256);

    req_id ++;
}
