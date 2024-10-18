import sys
import pandas as pd
from meteostat import Point, Daily, Stations
from datetime import datetime, timedelta

if len(sys.argv) < 2:
    print("Usage: python meteostat_test.py <City Name>")
    sys.exit(1)

city_name = sys.argv[1]
# Set number_of days a power of 2 (i.e., 2048 = 2^11)
for i in range(10, 16):
    number_of_days = 2**i

    end_date = datetime.now()
    start_date = end_date - timedelta(days=number_of_days)

    stations = Stations()
    stations = stations.fetch()
    station = stations[stations['name'].str.contains(city_name, case=False)].iloc[0]
    location = Point(station['latitude'], station['longitude'])

    data = Daily(location, start_date, end_date)
    data = data.fetch()
    average_temperatures = data['tavg'].tolist()

    output_file = f'{city_name}_temps_{number_of_days}_days.csv'
    with open(output_file, 'w') as file:
        for temp in average_temperatures:
            file.write(f"{temp}\n")

    print(f"Data saved to {output_file}")