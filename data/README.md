# Weather Data

## Export Meteostat

### Description
This script exports weather data from Meteostat API and saves it to a file.

### Dependencies
Make sure to have `pandas` and `meteostat` installed (you can install them with `pip`).

### Usage
1. Ensure you have installed the required dependencies
2. Run the script: `python export_meteostat.py CITY_NAME`
3. You can adjuts the number of days by changing `number_of_days` in the python file (must be a power of 2)
4. The exported weather data will be saved to a file named `{CITY_NAME}_average_temps_for_{number_of_days}_days.csv`

## CSV to Line

### Description
This script is just used to convert our csv datasets to a single line, with entries seperated by commas, so that we can use online tools to test our dataset.