import json
import subprocess
import sys

# opening instances json
f = open("instances.json")
data = json.load(f)

# build main.cpp
cmd = "make"
subprocess.call(cmd)

for case in data["instances"]:
    subprocess.call(["./jss", case["path"], "15", "15", "15"])

r = open("out.txt")
r.readline()

for otimo in data["instances"]:
    bt = float(r.readline())
    gap = float(bt - float(otimo["optimum"]))/float(otimo["optimum"])
    print("{}:\notimo: {} | obtido: {} | gap {}".format(otimo["name"], otimo["optimum"], bt, gap))

f.close()
