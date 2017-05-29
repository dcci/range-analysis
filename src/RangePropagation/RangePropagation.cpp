#include "../RangeAnalysis/RangeAnalysis.h"

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
struct RangePropagation : public FunctionPass {
  static char ID;
  RangePropagation() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    IntraProceduralRA<CropDFS> &RA = getAnalysis<IntraProceduralRA<CropDFS>>();
    errs() << "Analysis for function: " << F.getName() << "\n";
    for (BasicBlock &BB : F) {
      for (Instruction &I : BB) {
        const Value *V = &I;
        Range R = RA.getRange(V);
        if (!R.isUnknown()) {
          R.print(errs());
          errs() << I << "\n";
        }
      }
    }
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<IntraProceduralRA<CropDFS>>();
  }
};

// Interprocedural version.
struct IPRangePropagation : public ModulePass {
  static char ID;
  IPRangePropagation() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    InterProceduralRA<CropDFS> &RA = getAnalysis<InterProceduralRA<CropDFS>>();
    for (Function &F : M) {
      errs() << "Analysis for function: " << F.getName() << "\n";
      for (BasicBlock &BB : F) {
        for (Instruction &I : BB) {
          const Value *V = &I;
          Range R = RA.getRange(V);
          if (!R.isUnknown()) {
            R.print(errs());
            errs() << I << "\n";
          }
        }
      }
    }
    return false;
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<InterProceduralRA<CropDFS>>();
  }
};

}

char RangePropagation::ID = 0;
static RegisterPass<RangePropagation>
    X("rangeprop", "Intraprocedural Range Propagation", false, false);
char IPRangePropagation::ID = 0;
static RegisterPass<IPRangePropagation>
    Y("iprangeprop", "Interprocedural Range Propagation", false, false);
