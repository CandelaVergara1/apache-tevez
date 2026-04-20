import requests
import os
import ctypes
url = 'https://api.worldbank.org/v2/en/country/AR/indicator/SI.POV.GINI?format=json&date=2011:2020&per_page=100'
req = requests.get(url)

# Cargar librería
path = os.path.abspath('./libgini.so')
libgini = ctypes.CDLL(path)
libgini.procesar_gini.argtypes = (ctypes.c_float,)
libgini.procesar_gini.restype = ctypes.c_int

if req:
    print("Response OK - Status: ", req.status_code)
    data = req.json()
    registros = data[1]
    
    for i in registros:
        pais = i['country']['value']
        anio = i['date']
        valor = i['value']
        if valor is not None:
            resultado = libgini.procesar_gini(valor)
            print(f'{pais} - {anio}: {resultado}')
else:
    print('Response failed - Status: ', req.status_code)

