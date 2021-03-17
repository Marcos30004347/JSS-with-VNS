import os
import subprocess
import json

f = open("instances.json")
jfile = json.load(f)

for case in jfile["instances"]:
    out = open("output.txt", "w+")
    print(case["path"])
    subprocess.Popen(["./jss", case["path"], "15", "15", "15"], stdout=out)
    total_cost = out.read()
    print(total_cost)

f.close()
# buffer=[]
# for filename in os.listdir("instances/"):   
#     print(filename)
#     proc = subprocess.run(["./jss", filename, "15", "15", "15", ">", "teste.txt"])
#     print(proc.stdout)
