#include "RISCXSubtarget.h"

using namespace llvm;

#define DEBUG_TYPE "RISCXSubtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "RISCXGenSubtargetInfo.inc"

RISCXSubtarget::RISCXSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                           const TargetMachine &TM)
    : RISCXGenSubtargetInfo(TT, CPU, CPU, FS), RegInfo(*this), FrameLowering(*this),
      TLI(TM, *this) {}

RISCXSubtarget &
RISCXSubtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                              const TargetMachine &TM) {
  if (CPU.empty()) {
    CPU = "riscx";
  }

  ParseSubtargetFeatures(CPU, CPU, FS);

  return *this;
}