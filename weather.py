import pyowm
owm = pyowm.OWM("81553685bbd49eaa92bea218e44a87d0")  # You MUST provide a valid API key
observation = owm.weather_at_id(1277333)  
w = observation.get_weather()
print('Wind ',w.get_wind())
print('temperature ',w.get_temperature('celsius')) 
print('Humidity ',w.get_humidity())
print("Clouds ",w.get_clouds())
print("Rain Volume ",w.get_rain())
print("Detailed Weather status ",w.get_detailed_status())  


