import os

date = os.popen('date').read()
uptime = os.popen("uptime -p").read()
cpuspeed = os.popen("cat /proc/cpuinfo | grep -m1 -e MHz").read().split(":")[1].strip()
cpumodel = os.popen("cat /proc/cpuinfo | grep -m1 -e 'model name'").read().split(":")[1].strip()
topInfo = os.popen("top -n 1 -b").read().splitlines()
cpuusage = topInfo[2].split(":")[1].strip()
memusage = topInfo[3].split(":")[1].strip()

def get_proc_list(array: [str]):
    res = []
    for line in array:
        filtered = line.strip().split()
        res.append((filtered[0], filtered[11]))
    return res

procs = get_proc_list(topInfo[7:])

print(date)
print(uptime)
print(cpumodel, cpuspeed, cpuusage)
print(memusage)
print(topInfo[7:])
print(procs)