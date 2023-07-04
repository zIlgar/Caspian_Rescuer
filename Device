# define POT A0
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
char new_device[500] = "\nDate: %d/%d/%d\nTime: %d:%d\nRequest_Type:%s\nRequest_ID:%d\nDeviceID: %d\n---------------GPS----------------\nGPS: %d\nGPS_find_satalite: %d\nGPS_Data: %s, %s\n-------------Compass--------------\nCompass_Angle: %d\n-------------Sensors--------------\nUltrasonic_Data: %d\nHumidity: %d\nTemperature: %d\nServo_Angle: %d\n";
char cur_req[500] = "\nDate: %d/%d/%d\nTime: %d:%d\nRequest_Type:%s\nRequest_ID: %d\nDeviceID: %d\n---------------GPS----------------\nGPS: %d\nGPS_find_satalite: %d\nGPS_Data: %s, %s\nMust_GO: %s, %s\nDistance: %d\n-------------Compass--------------\nCompass_Angle: %d\nMust_GO_Angle: %d\n-------------Sensors--------------\nUltrasonic_Data: %d\nHumidity: %d\nTemperature: %d\nServo_Angle: %d\n";

char buf[500];

short day_s, month_s, year_s;
short hour_s, minute_s;
char * req_type;
short req_id, dev_id;
short gps_w;
short gps_f_s;
char * x, * y;
char * m_x, * m_y;
int dis;
short comp_ang, m_comp_ang;
short ultra, humid, temp;
short servo_ang;

void setup() {
    dht.begin();
    day_s = 28;
    month_s = 6;
    year_s = 2023;

    hour_s = 17;
    minute_s = 14;

    req_type = "NEW_DEVICE";
    req_id = 0;
    dev_id = 1984;

    gps_w = 1;
    gps_f_s = 1;

    x = "0.0";
    y = "0.0";

    comp_ang = 90;

    ultra = 255;
    humid = 25;
    temp = 25;

    servo_ang = 90;

    Serial.begin(57600);
    
    while (true) {
    
        Serial.write("abcdefghijklmnopqrstuvwxyz");

        if (Serial.available()) {
            break;
        }

        delay(1000);

    }

    sprintf(buf, new_device, day_s, month_s, year_s, hour_s, minute_s, req_type, req_id, dev_id, gps_w, gps_f_s, x, y, comp_ang, ultra, humid, temp, servo_ang);

    Serial.print("begin");

    for (int i = 0; i < strlen(buf); i ++) {


        Serial.print(buf[i]);

        delay(1000 / 8);

    }

    Serial.print("end");
}

void loop() {

    req_type = "CUR_DATA";
    
    m_x = "1.0";
    m_y = "1.0";

    dis = 10;
    temp = dht.readTemperature();
    humid = dht.readHumidity();
    comp_ang = analogRead(POT);
    comp_ang=comp_ang/2.84;     //saxta deyer ucun
    m_comp_ang = 90;

    sprintf(buf, cur_req, day_s, month_s, year_s, hour_s, minute_s, req_type, req_id, dev_id, gps_w, gps_f_s, x, y, m_x, m_y, dis, comp_ang, m_comp_ang, ultra, humid, temp, servo_ang);

    Serial.print("begin");

    for (int i = 0; i < strlen(buf); i ++) {

        Serial.print(buf[i]);

        delay(1000 / 8);

    }

    Serial.print("end");

}
