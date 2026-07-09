#ifndef LLVM_LIB_TARGET_RISCX_RISCXSUBTARGET_H
#define LLVM_LIB_TARGET_RISCX_RISCXSUBTARGET_H

#include "RISCXFrameLowering.h"
#include "RISCXISelLowering.h"
#include "RISCXInstrInfo.h"
#include "RISCXRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "RISCXGenSubtargetInfo.inc"

namespace llvm {
class StringRef;

class RISCXSubtarget : public RISCXGenSubtargetInfo {
private:
  RISCXInstrInfo InstrInfo;
  RISCXRegisterInfo RegInfo;
  RISCXFrameLowering FrameLowering;
  RISCXTargetLowering TLI;

public:
  RISCXSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
               const TargetMachine &TM);
  RISCXSubtarget &initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                                const TargetMachine &TM);
  const RISCXInstrInfo *getInstrInfo() const override { return &InstrInfo; }
  const RISCXRegisterInfo *getRegisterInfo() const override { return &RegInfo; }
  const RISCXFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const TargetLowering *getTargetLowering() const override { return &TLI; }
  void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);
};
} // namespace llvm

#endif