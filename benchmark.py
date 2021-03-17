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


f.close()
