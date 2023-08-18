from django.urls import path
from .views import devices, track, about_us, code_circuit, detailed, test

urlpatterns = [
    path('devices/', devices, name = 'devices'),
    path('track/<int:pck>', track, name = 'track'),
    path('detailed/<int:pck>', detailed, name = 'detailed'),
    path('test/<int:pck>', test, name = 'test'),
    path('about_us/', about_us, name = 'about_us'),
    path('code_circuit/', code_circuit, name = 'code_circuit')
]