#include "testk.h"
#include <fstream>
#include <regex>
#include <iostream>

char testk::ID;

using namespace llvm;


void testk::TrackSource(Value *V)
{
	/* For debug */
	/*
	if(auto *dI = dyn_cast<Instruction>(V))
		errs() << "[=] Inst : " << *dI << "\n";
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
				Function* F = CI->getParent()->getParent();
				if(knob_parent1 != F->getName() && 
						knob_parent2 != F->getName() &&
						knob_parent3 != F->getName() &&
						knob_parent4 != F->getName() &&
						knob_parent5 != F->getName())
					continue;
				errs() <<  "<<<<<<<<< Cross Analyzing " << F->getName().str() <<  "()\n";
				TrackSource(arg);
			}
		}
	}
	else if(GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(V))
	{
		Type *Ty = GEP->getSourceElementType();
		if(Ty->isStructTy())
		{
			errs() << "[=] Find : Type " << Ty->getStructName() << "\n";
		}
		return;
	}
	else if(BitCastInst *BCI = dyn_cast<BitCastInst>(V))
	{
        	TrackSource(BCI->getOperand(0));
	}
	else if (PHINode* PN = dyn_cast<PHINode>(V)) 
	{
		for (unsigned i = 0, e = PN->getNumIncomingValues(); i != e; i++) 
		{
			Value* IV = PN->getIncomingValue(i);
			TrackSource(IV);
        	}
    	}

}

void testk::TrackStruct(Value *V)
{
	/* For debug */
	/*
	if(auto *dI = dyn_cast<Instruction>(V))
		errs() << "[=] Inst : " << *dI << "\n";
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
				Function* F = CI->getParent()->getParent();
				if(knob_parent1 != F->getName() && 
						knob_parent2 != F->getName() &&
						knob_parent3 != F->getName() &&
						knob_parent4 != F->getName() &&
						knob_parent5 != F->getName())
					continue;
				errs() <<  "<<<<<<<<< Cross Analyzing " << F->getName().str() <<  "()\n";
				TrackStruct(arg);
			}
		}
	}
	else if(GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(V))
	{
		TrackStruct(GEP->getOperand(0));
	}
	else if(BitCastInst *BCI = dyn_cast<BitCastInst>(V))
	{
        	TrackStruct(BCI->getOperand(0));
	}
	else if (PHINode* PN = dyn_cast<PHINode>(V)) 
	{
		for (unsigned i = 0, e = PN->getNumIncomingValues(); i != e; i++) 
		{
			Value* IV = PN->getIncomingValue(i);
			TrackStruct(IV);
        	}
    	}
	else if(LoadInst *LI = dyn_cast<LoadInst>(V))
	{
		Type *Ty = LI->getPointerOperandType();
		errs() << "[=] LoadInst " << *Ty << "\n";
		TrackSource(LI->getPointerOperand());
		return;
	}
}

bool testk::extractGEP(Instruction *I)
{
	if(auto *GEP = dyn_cast<GetElementPtrInst>(I))
	{
		if(GEP->getSourceElementType()->isStructTy())
		{
			errs() << "\n\n >>>>> GEP " << GEP->getSourceElementType()->getStructName() << "\n";
			TrackStruct(GEP->getOperand(0));
		}
		return 1;
	}
	return 0;
}


bool testk::runOnModule(Module &M)
{
	errs() << "[+] runOnModule " << M.getName() << "\n";
	errs() << "Crash Function " << knob_crash_func << "\n";
	for(auto &F : M)
	{
		if(knob_crash_func != F.getName())
			continue;
		errs() << "[+] Function " << F.getName() << "\n";
		GetElementPtrInst *tmp_gep[100];
		int i = 0;
		int choice;
		for(auto &B : F)
		{
			for(auto &I : B)
			{
				if(auto *GEP = dyn_cast<GetElementPtrInst>(&I))
				{
					if(GEP->getSourceElementType()->isStructTy())
					{
						if(auto *Inst = dyn_cast<Instruction>(GEP))
						{
							DILocation* Loc = Inst->getDebugLoc();

							if (Loc == nullptr) 
							{
								errs() << "[" << i << "] " << GEP->getSourceElementType()->getStructName() << "\n";
        						}
							else
							{
								errs() << "[" << i << "] " << GEP->getSourceElementType()->getStructName() << " : " << Loc->getLine() << "\n";
							}
							errs() << "Desc: " << *Inst << "\n";
							tmp_gep[i++] = GEP;	
						}
					}
				}
				//errs() << "[+] Instruction " << I << "\n";
				//if( extractGEP(&I) )
				{
					//break;
				}
			}
		}
		errs() << ">> ";
		cin >> choice;
		TrackStruct(tmp_gep[choice]->getOperand(0));
	}

	return 0;
}

static RegisterPass<testk>
	XXX("testk", "Test Kernel Pass");

