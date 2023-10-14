#include "knobs.h"

// VA
cl::opt<std::string> knob_crash_func("crash",
    cl::desc("Crash Function"),
    cl::init("crash.func"));
cl::opt<std::string> knob_parent1("par1",
    cl::desc("Parent function1"),
    cl::init("parent1"));
cl::opt<std::string> knob_parent2("par2",
    cl::desc("Parent function2"),
    cl::init("parent2"));
cl::opt<std::string> knob_parent3("par3",
    cl::desc("Parent function3"),
    cl::init("parent3"));
cl::opt<std::string> knob_parent4("par4",
    cl::desc("Parent function4"),
    cl::init("parent4"));
cl::opt<std::string> knob_parent5("par5",
    cl::desc("Parent function5"),
    cl::init("parent5"));
cl::opt<std::string> knob_obj("obj",
    cl::desc("object"),
    cl::init("obj"));

// SPOT
cl::opt<std::string> knob_target_obj("tobj",
    cl::desc("target object"),
    cl::init("obj"));
cl::opt<std::string> knob_dangling_obj("dobj",
    cl::desc("dangling object"),
    cl::init("obj"));


