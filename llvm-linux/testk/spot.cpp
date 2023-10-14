#include "spot.h"
#include <fstream>
#include <regex>
#include <iostream>

char spot::ID;
using namespace llvm;


void spot::findDang(Function *F, Value *V)
{
	/* For debug */
	/*
	if(auto *dI = dyn_cast<Instruction>(V))
		errs() << "[Dang] Inst : " << *dI << "\n";
	*/

	if(Argument* A = dyn_cast<Argument>(V))
	{
		Function *callee = A->getParent();
		if(callee == nullptr)
			return;

		for(auto *U : callee->users())
		{
			if(auto *CI = dyn_cast<CallInst>(U))
			{
				if(A->getArgNo() >= CI->getNumArgOperands())
					continue;
				Value* arg = CI->getArgOperand(A->getArgNo());
				if (arg == nullptr)
					continue;
				Function* Func = CI->getParent()->getParent();
				errs() <<  "<<<<<<<<< Cross Analyzing " << Func->getName().str() <<  "()\n";
				findDang(F, arg);
			}
		}
	}
	else if(GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(V))
	{
		Type *Ty = GEP->getSourceElementType();
		if(Ty->isStructTy())
		{
			if(Ty->getStructName() == knob_dangling_obj)
			{
				errs() << "[" << F->getName() << "] " << "\n";
			}
		}
		return;
	}
	else if(BitCastInst *BCI = dyn_cast<BitCastInst>(V))
	{
        	findDang(F, BCI->getOperand(0));
	}
	else if (PHINode* PN = dyn_cast<PHINode>(V))
	{
		for (unsigned i = 0, e = PN->getNumIncomingValues(); i != e; i++)
		{
			Value* IV = PN->getIncomingValue(i);
			findDang(F, IV);
        	}
    	}
}

bool spot::findTarget(LoadInst *LI)
{
	Type *Ty = LI->getPointerOperandType();
	if(auto *PT = dyn_cast<PointerType>(Ty))
	{
		Type *Ty2 = PT->getPointerElementType();
		if(auto *PT2 = dyn_cast<PointerType>(Ty2))
		{
			Type *Ty3 = PT2->getPointerElementType();
			if(Ty3->isStructTy())
			{
				if(Ty3->getStructName() == knob_target_obj)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool spot::runOnModule(Module &M)
{
	errs() << "[+] runOnModule " << M.getName() << "\n";
	for(auto &F : M)
	{
		for(auto &B : F)
		{
			for(auto &I : B)
			{
				if(auto *LI = dyn_cast<LoadInst>(&I))
				{
					if(findTarget(LI))
					{
						//errs() << "[" << F.getName() << "] " << I << "\n";
						findDang(&F, LI->getPointerOperand());
						break;
					}		
				}
			}
		}
	}

	return true;
}

static RegisterPass<spot>
	XXX("spot", "Find new spot");
