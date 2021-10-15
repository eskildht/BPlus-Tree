import random

input_file = input("File name: ")
input_file = input_file if input_file.endswith(".txt") else input_file + ".txt"
entries = int(input("Number of entries: "))

with open(input_file, "w") as f:
    for i in range(entries):
        val = "Value" + str(i)
        key = round(random.uniform(-1500, 1500), 2)
        insert = "Insert(" + str(key) + "," + str(val) + ")\n"
        f.write(insert)

finished_msg = str(entries) + " entries written to " + input_file
print(finished_msg)
