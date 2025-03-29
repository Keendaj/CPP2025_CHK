import math
import csv

def count(filename):
    count = 0
    with open(filename, mode="r", encoding="utf-8") as file:
        reader = csv.reader(file, delimiter=';')
        for row in reader:
            if len(row) != 3:
                continue
            a, b, c = map(int, row)
            if abs(a) <= 10e-6 and abs(b) <= 10e-6:
                if(abs(c) > 10e-6):
                    count+=1
            discriminant = b**2 - 4*a*c
            if discriminant < 0:
                count += 1
    return count

filename = "dz.csv"

result = count(filename)
print(f"Количество уравнений без вещественных корней: {result}")