from sys import exit
from argparse import *
import subprocess
import os
import sys
import time
import re

def manipulate_poc(poc_file):
    print("[+] Manipulate PoC to logging the system call")

    manipulated_poc = os.path.join(workdir, "poc.c")

    code = open(poc_file).read()

    loop = ""
    main = ""
    header = ""
    if "void loop" in code:
        header = code.split("void loop")[0]
        loop = code.split("void loop")[1].split("\n{\n")[1].split("\n}\n")[0]
    else:
        header = code.split("int main")[0]

    main  = code.split("int main")[1].split("\n{\n")[1].split("\n}\n")[0]
    header += "\nint kmsg_fd;\n"

    header_file = os.path.join(workdir, "header.h")
    with open(header_file, "w") as f:
        f.write(header)

    with open(manipulated_poc, "w") as f:
        f.write("#include \"header.h\"\n")
        f.write("#include <fcntl.h>\n\n")
        
        if loop != "":
            f.write("void loop()\n{\n")
            for line in loop.split("\n"):
                line = line.strip()
                if "memset(" in line or "cpy(" in line or "printf(" in line or "strchr(" in line or "sleep(" in line:
                    f.write(line + "\n")
                elif "(" in line and ");" in line:
                    msg = line.replace("\"", "\\\"").replace("\\x", "\\\\x") + "\\n\""
                    f.write("\twrite(kmsg_fd, \"[-] " + msg + ", strlen(\"[-] " + msg + "));\n")
                    f.write("\t" + line + "\n")
                else:
                    f.write("\t" + line + "\n")

            f.write("}\n")

        f.write("int main()\n{\n")
        f.write("\tkmsg_fd = open(\"/dev/kmsg\", O_RDWR);\n")
        f.write("\twrite(kmsg_fd, \"\\n\\n\", 2);\n")
        f.write("\twrite(kmsg_fd, \"[-] Debugging Start\\n\", strlen(\"[-] Debugging Start\\n\"));\n")
        f.write("\tclose(kmsg_fd);\n")
        for line in main.split("\n"):
            line = line.strip()
            if "memset(" in line or "cpy(" in line or "printf(" in line or "strchr(" in line or "irda_set_ias" in line or "unshare" in line or "mmap" in line:
                f.write(line + "\n")
            elif "(" in line and ");" in line:
                msg = line.replace("\"", "\\\"").replace("\\x", "\\\\x") + "\\n\""
                f.write("\tkmsg_fd = open(\"/dev/kmsg\", O_RDWR);\n")
                f.write("\twrite(kmsg_fd, \"[-] " + msg + ", strlen(\"[-] " + msg + "));\n")
                f.write("\tclose(kmsg_fd);\n")
                f.write("\t"+line + "\n")
            elif "return" in line:
                continue
            else:
                f.write("\t"+line + "\n")

        if(args.close_fds == True):
            f.write("\tkmsg_fd = open(\"/dev/kmsg\", O_RDWR);\n")
            f.write("\twrite(kmsg_fd, \"[-] close(4);\\n\", strlen(\"[-] close(4);\\n\"));\n")
            f.write("\tclose(4);\n")
            f.write("\twrite(kmsg_fd, \"[-] close(5);\\n\", strlen(\"[-] close(5);\\n\"));\n")
            f.write("\tclose(5);\n")
            f.write("\tclose(kmsg_fd);\n")
        
        f.write("\tkmsg_fd = open(\"/dev/kmsg\", O_RDWR);\n")
        f.write("\twrite(kmsg_fd, \"[-] Debugging End\\n\", strlen(\"[-] Debugging End\\n\"));\n")
        f.write("}\n")
        f.write("\tclose(kmsg_fd);\n")

    return manipulated_poc


def debugging(manipulated_poc):
    print("[+] Reproduce")
    outfile = open(os.path.join(workdir, "stdout"), "w")
    errfile = open(os.path.join(workdir, "stderr"), "w")

    os.system("sudo pkill qemu-system-x86")

    cmd = ["sudo", os.path.join(os.environ['PROJECT_HOME'], "script", "qemu-sa.sh")]
    task = subprocess.Popen(cmd, stdout=outfile, stderr=errfile)

    time.sleep(10)
    pubkey = os.path.join(os.environ['PROJECT_HOME'], "image", "stretch.id_rsa")
    header = os.path.join(workdir, "header.h")
    poc = manipulated_poc

    os.system("scp -i " + pubkey + " -P 10021 -o \"StrictHostKeyChecking no\" " + header +  " root@localhost:./header.h 2> /dev/null")
    os.system("scp -i " + pubkey + " -P 10021 -o \"StrictHostKeyChecking no\" " + poc +  " root@localhost:./poc.c 2> /dev/null")

    gcc = "gcc -o poc poc.c -lpthread"
    os.system("ssh -i " + pubkey + " -p 10021 -o \"StrictHostKeyChecking no\" root@localhost " + gcc + " 2> /dev/null")
    os.system("ssh -i " + pubkey + " -p 10021 -o \"StrictHostKeyChecking no\" root@localhost " + "./poc 2> /dev/null &")
    time.sleep(20)

    os.system("sudo pkill qemu-system-x86")

    return os.path.join(workdir, "stdout")

def debug_strip(debug):
    log = open(debug).read()

    logging = False
    stripped_log = ""
   
    for line in log.split("\n"):
        if "[-] Debugging End" in line:
            logging = False
        
        if logging == True:
            stripped_log += line + "\n"

        if "[-] Debugging Start" in line:
            logging = True

    with open(os.path.join(workdir, "debug.log"), "w") as f:
        f.write(stripped_log)

    return os.path.join(workdir, "debug.log")

def extract_ci(log_file):
    log = open(log_file).read()

    cache = []
    for line in log.split("\n"):
        if "[X]" in line:
            tmp_cache = line.split("Cache Name : ")[1].split(",")[0]
            cache.append(tmp_cache)

    ci_log = ""
    for line in log.split("\n"):
        if "[-]" in line:
            ci_log += line + "\n"
        elif "[*]" in line:
            tmp = line.split("cache : ")[1].split(",")[0]
            if tmp in cache:
                ci_log += line + "\n"
        elif "[X]" in line:
            ci_log += line + "\n"

    stripped_path = os.path.join(workdir, "stripped.log")
    with open(stripped_path, "w") as f:
        f.write(ci_log)

    return stripped_path

def ci_analysis(ci_log):
    log = open(ci_log).read()
    
    crash = []
    availability = {}
    for line in log.split("\n"):
        # UAF W
        if "[X]" in line:
            ptr =""
            cache = ""
            if "[X] Store" in line:
                # UAF W PTR
                if "Data: PTR" in line:
                    ptr = line.split("PTR : ")[1].split(",")[0]
                    cache = line.split("Cache Name : ")[1].split(",")[0]
                    offset = line.split("Offset : ")[1].split("-")[0]
                    if not ptr in availability:
                        availability["WP"+ptr] = []
                    availability["WP"+ptr].append(offset)

                    if not [cache, ptr] in crash:
                        crash.append([cache, ptr, "WP"])
                # UAF W VALUE
                else:
                    size = int(line.split("[X] Store Size : ")[1].split(",")[0])
                    # corruption must be less than 3 bytes
                    if size < 3:
                        ptr = line.split("PTR : ")[1].split(",")[0]
                        cache = line.split("Cache Name : ")[1].split(",")[0]
                        offset = line.split("Offset : ")[1].split("-")[0]
                        if not ptr in availability:
                            availability["WV"+ptr] = []
                        availability["WV"+ptr].append(offset)

                        if not [cache, ptr] in crash:
                            crash.append([cache, ptr, "WV"])
            elif "[X] Load" in line:
                # UAF R PTR
                if"Data: PTR" in line:
                    ptr = line.split("PTR : ")[1].split(",")[0]
                    cache = line.split("Cache Name : ")[1].split(",")[0]
                    offset = line.split("Offset : ")[1].split("-")[0]
                    if not ptr in availability:
                        availability["RP"+ptr] = []
                    availability["RP"+ptr].append(offset)

                    if not [cache, ptr] in crash:
                        crash.append([cache, ptr, "RP"])
                # UAF R VALUE
                else:
                    ptr = line.split("PTR : ")[1].split(",")[0]
                    cache = line.split("Cache Name : ")[1].split(",")[0]
                    offset = line.split("Offset : ")[1].split("-")[0]
                    if not ptr in availability:
                        availability["RV"+ptr] = []
                    availability["RV"+ptr].append(offset)

                    if not [cache, ptr] in crash:
                        crash.append([cache, ptr, "RV"])
            # DF
            elif "[X] Double Free" in line:
                ptr = line.split("PTR : ")[1].split(",")[0]
                cache = line.split("Cache Name : ")[1].split(",")[0]
                if not [cache, ptr] in crash:
                    crash.append([cache, ptr, "DF"])

    print(crash)
    for cache, ptr, crash in crash:
        
        dfree_syscall = ""
        spray = 0 
        if crash in ["WP", "WV", "RP", "RV"]:
            alloc_syscall = ""
            free_syscall = ""
            crash_syscall = ""
            for syscall in log.split("[-] ")[1:]:
                call = syscall.split("\n")[0]

                for line in syscall.split("\n"):
                    if "[*]" in line and ptr in line:
                        if "ALLOC" in line:
                            alloc_syscall = call
                        elif "FREE" in line:
                            free_syscall = call
                    if "[X]" in line and ptr in line:
                        crash_syscall = call

                spray_bool = False
                for line in syscall.split("\n"):
                    if "FREE" in line:
                        if spray_bool == True:
                            spray = spray + 1
                        
                        if cache in line and ptr in line:
                            spray_bool = True
    
        if crash in ["DF"]:
            alloc_syscall = ""
            free_syscall = ""
            crash_syscall = ""
           
            for syscall in log.split("[-] ")[1:]:
                call = syscall.split("\n")[0]

                for line in syscall.split("\n"):
                    if "[*]" in line and ptr in line:
                        if "ALLOC" in line:
                            alloc_syscall = call
                        elif "FREE" in line:
                            if free_syscall == "":
                                free_syscall = call
                            else:
                                dfree_syscall = call
                    if "[X]" in line and ptr in line:
                        crash_syscall = call

                spray_bool = False
                df_bool = False
                for line in syscall.split("\n"):
                    if "FREE" in line:
                        if spray_bool == True and df_bool == True:
                            spray = spray + 1
                        
                        if spray_bool == True and cache in line and ptr in line:
                            df_bool = True
                        if cache in line and ptr in line:
                            spray_bool = True
             
        filename = ""
        for i in range(100):
            filename = os.path.join(workdir, "CI_"+str(i)+".log")
            if not os.path.isfile(filename):
                break

        with open(filename, "w") as f:
            f.write("CRASH TYPE : " + crash + "\n")
            f.write("CRASH ZONE : " + cache + "\n")
            f.write("ALLOC SYSCALL : " + alloc_syscall + "\n")
            f.write("FREE SYSCALL : " + free_syscall + "\n")
            if dfree_syscall == "":
                f.write("CRASH SYSCALL : " + crash_syscall + "\n")
            else:
                f.write("DOUBLE FREE SYSCALL : " + crash_syscall + "\n")
            
            if crash+ptr in availability:
                for offset in availability[crash+ptr]:
                    f.write("AVAILABILITY : " + offset + "\n")
            f.write("SPRAY : " + str(spray) + "\n")
            print("[+] Abbreviated Critical Information file : " + filename)

if __name__ == "__main__":
    parser = ArgumentParser("description")
    parser.add_argument("--poc", required=True)
    parser.add_argument("--close_fds", required=False, default=True)
    parser.add_argument("--workdir", required=False, default=os.path.join(os.environ['PROJECT_HOME'], "tmp"))

    global args
    args = parser.parse_args()

    global poc
    poc = args.poc

    global workdir
    workdir = args.workdir
    if not os.path.isdir(workdir):
        os.mkdir(workdir)

    manipulated_poc = manipulate_poc(poc)
    print("[+] Manipulated PoC : " + manipulated_poc)

    debug_file = debugging(manipulated_poc)
    #debug_file = os.path.join(workdir, "stdout")
    print("[+] Debug File : " + debug_file)

    log_file = debug_strip(debug_file)
    print("[+] Logging file : " + log_file)

    strip_file = extract_ci(log_file)
    print("[+] Stripped Critical Infomation file : " + strip_file)
   
    for i in range(100):
        filename = os.path.join(workdir, "CI.log" + str(i))
        if os.path.isfile(filename):
            os.remove(filename)
    ci_analysis(strip_file)

