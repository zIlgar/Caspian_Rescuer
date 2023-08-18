# include <SoftwareSerial.h>

# define POT A0

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

SoftwareSerial ss(19, 18);

void send_msg(char buf[], short send_size) {
    Serial.print("begin");

    for (int i = 0; i < strlen(buf); i ++) {

        Serial.print(buf[i]);

        delay(1000 / send_size);

    }

    Serial.print("end");
}

void setup() {

    day_s = 28;
    month_s = 6;
    year_s = 2023;

    hour_s = 17;
    minute_s = 14;
    second_s = 0;

    req_type = "NEW_DEVICE";
    req_id = 0;
    dev_id = 1653;

    gps_w = 1;
    gps_f_s = 1;

    x = 15.0;
    y = 24.0;

    x_ = x * 1000;
    y_ = y * 1000;

    m_x = 1.0;
    m_y = 1.0;

    m_x_ = m_x * 1000;
    m_y_ = m_y * 1000;

    comp_ang = 90;

    ultra = 255;
    humid = 25;
    temp = 25;

    servo_ang = 90;

    Serial.begin(57600);
    ss.begin(57600);

    while (true) {

        Serial.write("abcdefghijklmnopqrstuvwxyz");

        if (Serial.available()) {
            break;
        }

        delay(1000);

    }

    sprintf(buf, new_device, year_s, month_s, day_s, hour_s, minute_s, second_s, req_type, req_id, dev_id, gps_w, gps_f_s, x_, y_, comp_ang, ultra, humid, temp, servo_ang);

    send_msg(buf, 8);

    req_type = "CUR_DATA";

}

void loop() {

    dis = 10;

    comp_ang = analogRead(POT);
    m_comp_ang = 90;

    sprintf(buf, cur_req, year_s, month_s, day_s, hour_s, minute_s, second_s, req_type, req_id, dev_id, gps_w, gps_f_s, x_, y_, m_x_, m_y_, dis, comp_ang, m_comp_ang, ultra, humid, temp, servo_ang);

    send_msg(buf, 256);

    req_id ++;

}
