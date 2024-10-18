import csv

def csv_to_line(csv_file, txt_file):
    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        values = [row[0] for row in reader]
        line = ','.join(values)

    with open(txt_file, 'w') as file:
        file.write(line)

csv_to_line('Marseille_average_temps_for_5_years.csv', 'Marseille_data.txt')