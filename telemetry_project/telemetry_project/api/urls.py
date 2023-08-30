from .views import getRoutes, getDevices, getLog, getLatest, servoControl
from django.urls import path

urlpatterns = [
    path('', getRoutes),
    path('devices/', getDevices),
    path('getLog/<int:pck>', getLog),
    path('getLatest/<int:pck>', getLatest),
    path('servoControl/', servoControl)
]