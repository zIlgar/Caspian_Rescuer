from django.urls import path
from .views import getRoutes, getDevices, getLog, getLatest

urlpatterns = [
    path('', getRoutes),
    path('devices/', getDevices),
    path('getLog/<int:pck>', getLog),
    path('getLatest/<int:pck>', getLatest)
]