# include <SoftwareSerial.h>
#include <Servo.h>
# include "DHT.h"

# define DHTTYPE DHT11
# define POT A0


enum HCSR04_PIN {
    TRIG = 3,
    ECHO = 2
};

Servo servo;

const int DHT11_PIN = 11;
const int SERVO_PIN = 13;

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

String msg = "";

SoftwareSerial ss(19, 18);

void readDHT11() {
    humidity = dht.readHumidity() * 100;
    temperature = dht.readTemperature() * 100;
}

void readHCSR04() {
    servo.attach(SERVO_PIN);

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH);
    distance = duration * speed_of_sound / 2;
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

    readDHT11();
    readHCSR04();

    day_s = 31;
    month_s = 8;
    year_s = 2023;

    hour_s = 17;
    minute_s = 14;
    second_s = 0;

    req_type = "NEW_DEVICE";
    req_id = 0;
    dev_id = 1984;

    gps_w = 1;
    gps_f_s = 1;

    x = 40.4;
    y = 49.9;

    x_ = x * 1000;
    y_ = y * 1000;

    m_x = 1.0;
    m_y = 1.0;

    m_x_ = m_x * 1000;
    m_y_ = m_y * 1000;

    comp_ang = 90;

    servo_ang = 90;

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

    send_msg(buf, 8);

    req_type = "CUR_DATA";
}

void loop() {
    delay(2000);

    readDHT11();
    readHCSR04();

    while (Serial.available() > 0)
        msg += (char) Serial.read();

    msg.trim();

    if (msg == "servo") {
        servo.write(0);
        delay(1000);
        servo.write(90);
        delay(1000);
        servo.write(180);
        delay(1000);

        msg = "";
    }
    else{
        Serial.print(msg);
    }

    comp_ang = analogRead(POT);
    m_comp_ang = 90;

    sprintf(buf, cur_req, year_s, month_s, day_s, hour_s, minute_s, second_s, req_type, req_id, dev_id, gps_w, gps_f_s, x_, y_, m_x_, m_y_, dis, comp_ang, m_comp_ang, distance, humidity, temperature, servo_ang);

    send_msg(buf, 256);

    req_id ++;
}