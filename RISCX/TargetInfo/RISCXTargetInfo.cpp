#include "llvm/MC/TargetRegistry.h"
#include "RISCXTargetInfo.h"

using namespace llvm;

Target &llvm::getTheRISCXTarget() {
  static Target TheRISCXTarget;
  return TheRISCXTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void 
LLVMInitializeRISCXTargetInfo() {
  RegisterTarget<Triple::riscx,
                 /*HasJIT=*/false>
      X(getTheRISCXTarget(), "riscx", "RISCX (32-bit little endian)", "riscx");
}


