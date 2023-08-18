import os
import django

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'telemetry_project.settings')

django.setup()

import serial
import time
import api.models
import pathlib
import re

ser = serial.Serial('/dev/ttyUSB0', 57600)

all_text = ''
log = ''

while True:
    time.sleep(1)
    data_read = ser.read_all().decode()

    all_text += data_read

    if 'begin' in all_text:
        print('began')

        while 'end' not in data_read:
            time.sleep(1)
            data_read = ser.read_all().decode()

            print(data_read)

            all_text += data_read

        print(all_text)

        log = all_text[all_text.index('begin') : all_text.index('end')]

        all_text = all_text[all_text.index('end') + 3 :]

        print('end')

        all_data = [data.strip() for data in re.split("[\n:]+", log)]

        log = log.replace('begin', '================================================')
        log += '================================================\n'

        request_type = all_data[all_data.index('request_type') + 1]

        device_id = int(all_data[all_data.index('device_id') + 1])

        print(all_data)

        date = all_data[all_data.index('date') + 1]
        time_ = all_data[all_data.index('time') + 1].replace('.', ':')
        gps = all_data[all_data.index('gps') + 1]
        gps_find_satellite = all_data[all_data.index('gps_find_satellite') + 1]

        if request_type == 'NEW_DEVICE':
            api.models.Device.objects.create(
                date = date,
                time = time_,
                device_id = device_id,
                gps = gps,
                gps_find_satellite = gps_find_satellite,
                device_name = ''
            )

            pathlib.Path(f'api/logs/out{device_id}.log').touch()

        with open(f'api/logs/out{device_id}.log', 'a') as dest:
            dest.write(log)

        log = ''

    elif 'abcdefghijklmnopqrstuvwxyz' in all_text:
        ser.write('attaboy'.encode())

    print(data_read)

ser.close()