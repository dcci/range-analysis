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
    IntraProceduralRA<Cousot> &RA = getAnalysis<IntraProceduralRA<Cousot>>();
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
    AU.addRequired<IntraProceduralRA<Cousot>>();
  }
};
}

char RangePropagation::ID = 0;
static RegisterPass<RangePropagation>
    X("rangeprop", "Intraprocedural Range Propagation", false, false);
