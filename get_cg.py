import re
import sys

# kasan, kmasn, info hang
kasan_pattern = "Call Trace:\n([\s\S]*?)\n(RIP: 00|Allocated by task|===)" # group 0
kasan_pattern2 = "Call Trace:\n([\s\S]*?)\nAllocated by task" # uaf
kasan_pattern3 = "Call Trace:\n([\s\S]*?)\n===" # kasan_null_ptr

# group 0 and 1
kernel_bug = "RIP: 0010:([\s\S]*?)Code[\s\S]*R13:[\s\S]*Call Trace:\n([\s\S]*?)\nModules linked in"

pattern2 = "R13:.*\n([\s\S]*?)Kernel Offset"
pattern3 = "Call Trace:\n([\s\S]*?)\n(Modules linked in| ret_from_fork)"


pattern4 = "RIP: 0010:([\s\S]*)Code[\s\S]*?Call Trace:\n([\s\S]*?)(Kernel Offset|entry_SYSCALL)"

DEBUG=False

def dprint(s):
    if DEBUG:
        print(s)

def get_call_trace(pattern, report):
    p = re.compile(pattern)
    m = p.search(report)
    if not m:
        return None
    trace = m.group(1)
    if "invalid_op" in trace: return None
    if "Code: " in trace: return None
    return m

def get_calls():
    if len(sys.argv) == 1:
        print("%s report"%sys.argv[0])
    report = open(sys.argv[1]).read()
    
    new_report = ""
    crash = ""
    for line in report.split("\n"):
        if "] " not in line:
            continue
        new_report = new_report + line.split("] ")[1].strip() + "\n"

        if "BUG: KASAN:" in line:
            crash = line.split("] ")[1].strip().split(" in ")[1].split("+")[0]
            
    
    
    if "KASAN" in new_report:
        found = get_call_trace(kasan_pattern, new_report)
        if found:
            return crash, found.group(1)
        found = get_call_trace(kasan_pattern2, new_report)
        if found:
            return crash, found.group(1)
        found = get_call_trace(kasan_pattern3, new_report)
        if found:
            return crash, found.group(1)

        return crash, ""

def get_cg():
    cgs = ""
    crash, calls = get_calls()
        
    tracing = False
    count = 0
    cgs = ""
    for call in calls.split("\n"):
        if call.startswith("? "):
            continue
        if tracing and count < 5:
            new_call = call
            if "+" in call:
                new_call = call.split("+")[0]
            if ".isra" in call:
                new_call = call.split(".isra")[0]
            cgs = cgs + new_call + "\n" 
            count = count + 1
        if crash in call:
            tracing = True
    return crash, cgs

if __name__ == "__main__":
    cr, outs = get_cg()
    print(outs)
    i = 1
    argument = ""
    for line in outs.split("\n"):
        if i > 5:
            break
        argument = argument +" -par" + str(i) + " " + line
        i = i + 1

    print("opt -load ./llvm-linux/build/testk/libtestk.so -testk ./linux/vmlinux.bc -crash " + cr + " " + argument)
