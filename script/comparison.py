from sys import exit
from argparse import *
import subprocess
import os
import sys
import time
import re

def comparison_df():
    print("[*] Exploitable")
    print("[*] Overlapping object: direct replicate object")
    print("[*] Overlapping object2: pointer obect")

    with open(os.path.join(workdir, "CA.log"), "w") as f:
        f.write("Exploitable\n")
        f.write("OO : direct\n")
        f.write("OO2 : ptr\n")

def comparison_wp():
    print("[*] Exploitable")
    print("[*] Overlapping object: direct replicate object")
    with open(os.path.join(workdir, "CA.log"), "w") as f:
        f.write("Exploitable\n")
        f.write("OO : direct\n")
        
def comparison_wv():
    # since corruption analysis logs the corruption
    # that the corruption size is less than 3 bytes
    # so all of WV maybe exploitable if precise object exists
    
    print("[*] Exploitable")
    print("[*] Overlapping object: indirect replicate object")
    with open(os.path.join(workdir, "CA.log"), "w") as f:
        f.write("Exploitable\n")
        f.write("OO : indirect\n")

def comparison_rp(ctu):
    if len(ctu) == 0:
        print("[-] Unexploitable")
    else:
        print("[*] Exploitable")
        print("[*] Overlapping object: pointer object")
        with open(os.path.join(workdir, "CA.log"), "w") as f:
            f.write("Exploitable\n")
            f.write("OO : ptr\n")


def comparison_rv(ctu):
    if len(ctu) == 0:
        print("[-] Unexploitable")
    else:
        print("[*] Exploitable")
        print("[*] Overlapping object: pointer object")
        with open(os.path.join(workdir, "CA.log"), "w") as f:
            f.write("Exploitable\n")
            f.write("OO : ptr\n")

# Compare the corruption information with exploit model
def comparison():
    # Read corruption information
    if not os.path.isfile(args.ci):
        print("[-] input the corruption information")
        sys.exit(1)
    data = open(args.ci).read()

    crash_type = ""
    zone = ""
    size = ""
    availability = []
    ctu = []

    for line in data.split("\n"):
        if "CRASH TYPE : " in line:
            crash_type = line.split("CRASH TYPE : ")[1].replace("\r", "")
        if "CRASH ZONE : " in line:
            zone = line.split("ZONE : ")[1].replace("\r", "")
        if "CTU" in line:
            ctu.append(line.split("CTU : ")[1])


    print("[+] TYPE : " +crash_type)
    print("[+] ZONE  : " + zone)

    if "kmalloc" not in zone:
        print("[*] Unexploitable since it uses special slab cache")
        sys.exit(1)

    if crash_type == "WP":
        comparison_wp()
    elif crash_type == "WV":
        comparison_wv()
    elif crash_type == "RP":
        comparison_rp(ctu)
    elif crash_type == "RV":
        comparison_rv(ctu)
    elif crash_type == "DF":
        comparison_df()
    else:
        print("[-] It's not a predetermined type " + crash_type)
        sys.exit(1)

if __name__ == "__main__":
    parser = ArgumentParser("description")
    parser.add_argument("--ci", required=True)
    parser.add_argument("--workdir", required=False, default=os.path.join(os.environ['PROJECT_HOME'], "tmp"))

    global args
    args = parser.parse_args()

    global workdir
    workdir = args.workdir
    if not os.path.isdir(workdir):
        os.mkdir(workdir)

    comparison()
