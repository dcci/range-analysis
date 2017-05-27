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
    llvm::errs() << "tinkywinky\n";
    return false;
  }
};
}

char RangePropagation::ID = 0;
static RegisterPass<RangePropagation>
    X("rangeprop", "Intraprocedural Range Propagation", false, false);
