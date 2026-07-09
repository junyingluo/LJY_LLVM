#ifndef LLVM_LIB_TARGET_RISCX_RISCXMCINSTRINFO_H
#define LLVM_LIB_TARGET_RISCX_RISCXMCINSTRINFO_H

#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "RISCXGenInstrInfo.inc"

namespace llvm {

class RISCXInstrInfo : public RISCXGenInstrInfo {
public:
  explicit RISCXInstrInfo();
  
  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;

  void storeRegToStackSlot(MachineBasicBlock &MBB,
                           MachineBasicBlock::iterator MBBI, Register SrcReg,
                           bool isKill, int FrameIndex,
                           const TargetRegisterClass *RC,
                           const TargetRegisterInfo *TRI,
                           Register VReg) const override;
                           
  void loadRegFromStackSlot(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator MBBI, Register DestReg,
                            int FrameIndex, const TargetRegisterClass *RC,
                            const TargetRegisterInfo *TRI,
                            Register VReg) const override;
};
} // namespace llvm

#endif