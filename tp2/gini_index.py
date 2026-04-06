# app.py
import ctypes
import requests
import os

path = os.path.abspath('./conversor_lib.so')
conversor_lib = ctypes.CDLL(path)
conversor_lib.num_conversor.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_int]
conversor_lib.num_conversor.restype = ctypes.POINTER(ctypes.c_int)

def convert_values(values):
    quantity = len(values)
    input_array = (ctypes.c_float * quantity)(*values)
    result_pointer = conversor_lib.num_conversor(input_array, quantity)
    integer_results = [result_pointer[i] for i in range(quantity)]
    return integer_results

def main():
    api_url = "https://api.worldbank.org/v2/en/country/AR/indicator/SI.POV.GINI?format=json&date=2011:2020"
    
    # API consumption
    try:
        response = requests.get(api_url)
        response.raise_for_status() # Check download status
        json_data = response.json()

        data_list = json_data[1]

        gini_values = [element['value'] for element in data_list if element['value'] is not None]
        print("Extracted values from API: ", gini_values)
    except requests.exceptions.RequestException as e:
        print(f"Error trying to connect to API: {e}")
    except Exception as e:
        print(f"Unexpected error: {e}")
    
    integer_values = convert_values(gini_values) # C function call
    print("Converted values from C:   ", integer_values) 

if __name__ == '__main__':
    main()

