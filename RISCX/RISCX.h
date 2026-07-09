#ifndef LLVM_LIB_TARGET_RISCX_RISCX_H
#define LLVM_LIB_TARGET_RISCX_RISCX_H

#include "llvm/PassRegistry.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

#define DIV_ROUND_UP(n, d) (((n)+(d)-1)/(d))
#define ROUND_UP(x, align) (DIV_ROUND_UP(x, align) * (align))
  
class FunctionPass;
class RISCXTargetMachine;

FunctionPass *createRISCXISelDag(RISCXTargetMachine &TM);
void initializeRISCXDAGToDAGISelLegacyPass(PassRegistry &);

} // namespace llvm
#endif