#ifndef LLVM_LIB_TARGET_RISCX_RISCXFRAMELOWERING_H
#define LLVM_LIB_TARGET_RISCX_RISCXFRAMELOWERING_H

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class RISCXSubtarget;

class RISCXFrameLowering : public TargetFrameLowering {
  const RISCXSubtarget &STI;

public:
  explicit RISCXFrameLowering(const RISCXSubtarget &STI)
      : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(16), 0,
                            Align(16)),
        STI(STI) {}

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasFP(const MachineFunction &MF) const override;

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs, RegScavenger *RS = nullptr) const override;
  
  uint64_t computeStackSize(MachineFunction &MF) const;
};

} // namespace llvm

#endif