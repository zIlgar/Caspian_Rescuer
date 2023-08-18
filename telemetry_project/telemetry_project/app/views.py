from django.shortcuts import render

def devices(request):
    context = {}

    return render(request, 'app/devices.html', context)

def track(request, pck):
    context = {'device_id' : pck}

    return render(request, 'app/device.html', context)

def about_us(request):
    context = {}

    return render(request, 'app/about-us.html', context)

def code_circuit(request):
    context = {}

    return render(request, 'app/code-circuit.html', context)

def detailed(request, pck):
    context = {'device_id' : pck}

    return render(request, 'app/detailed.html', context)

def test(request, pck):
    context = {'device_id' : pck}

    return render(request, 'app/test.html', context)