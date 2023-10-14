#ifndef __KNOBS_H_
#define __KNOBS_H_
#include "llvm/Support/CommandLine.h"
using namespace llvm;

extern cl::opt<std::string> knob_crash_func;
extern cl::opt<std::string> knob_parent1;
extern cl::opt<std::string> knob_parent2;
extern cl::opt<std::string> knob_parent3;
extern cl::opt<std::string> knob_parent4;
extern cl::opt<std::string> knob_parent5;
extern cl::opt<std::string> knob_obj;

extern cl::opt<std::string> knob_target_obj;
extern cl::opt<std::string> knob_dangling_obj;
#endif // __KNOBS_H_
