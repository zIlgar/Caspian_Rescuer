from rest_framework.decorators import api_view
from rest_framework.response import Response
from .serializers import DeviceSerializer
from .models import Device
from pathlib import Path
from re import split

@api_view(['GET'])
def getRoutes(request):
    routes = [
        'GET /api',
        'GET /devices',
        'GET /getLog/<int:pck>'
    ]

    return Response(routes)

@api_view(['GET'])
def getDevices(request):
    devices = Device.objects.all()
    serializer = DeviceSerializer(devices, many = True)

    return Response(serializer.data)

@api_view(['GET'])
def getLog(request, pck):
    logs = [path.name for path in Path('api/logs').iterdir()]
    context = {}

    if f'out{pck}.log' in logs:
        with open(f'api/logs/out{pck}.log', 'r') as dest:
            context['status'] = 'attaboy'

            data = dest.read().split("""================================================
================================================""")

            context['data'] = []

            for row in data:
                row_data = [data.strip('=') for data in split("[\n:]+", row) if data.strip('=')]

                row_json = {row_data[i]: row_data[i + 1] for i in range(0, len(row_data), 2)}
                row_json['Time'] = row_json['time'].replace('.', ':')

                context['data'].append(row_json)

    else:
        context['status'] = 'not good'
        context['data'] = 'Not Received'

    return Response(context)

@api_view(['GET'])
def getLatest(request, pck):
    logs = [path.name for path in Path('api/logs').iterdir()]
    context = {}

    if f'out{pck}.log' in logs:
        with open(f'api/logs/out{pck}.log', 'r') as dest:
            context['status'] = 'attaboy'

            data = dest.read().split("""================================================
================================================""")

            context['data'] = []

            row = data[- 1]

            row_data = [data.strip('=') for data in split("[\n:]+", row) if data.strip('=')]

            row_json = {row_data[i]: row_data[i + 1] for i in range(0, len(row_data), 2)}
            row_json['Time'] = row_json['time'].replace('.', ':')

            context['data'].append(row_json)

    else:
        context['status'] = 'not good'
        context['data'] = 'Not Received'

    return Response(context)