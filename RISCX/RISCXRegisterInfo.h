#ifndef LLVM_LIB_TARGET_RISCX_RISCXREGISTERINFO_H
#define LLVM_LIB_TARGET_RISCX_RISCXREGISTERINFO_H

#include "llvm/CodeGen/MachineFunction.h"
#include <cstdint>
#define GET_REGINFO_HEADER
#include "RISCXGenRegisterInfo.inc"

namespace llvm {
class RISCXSubtarget;
class RISCXRegisterInfo: public RISCXGenRegisterInfo {
    const RISCXSubtarget &STI;
public:
    RISCXRegisterInfo(const RISCXSubtarget &STI);
    const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;
    uint32_t const *getCallPreservedMask(const MachineFunction &MF, CallingConv::ID) const override;
    BitVector getReservedRegs(const MachineFunction &MF) const override;
    bool eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                            unsigned FIOperandNum,
                            RegScavenger *RS = nullptr) const override;
    Register getFrameRegister(const MachineFunction &MF) const override; 
};
} // namespace llvm

#endif