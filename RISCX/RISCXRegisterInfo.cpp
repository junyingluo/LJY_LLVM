#include "RISCXRegisterInfo.h"
#include "MCTargetDesc/RISCXMCTargetDesc.h"
#include "RISCX.h"
#include "RISCXSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "RISCXGenRegisterInfo.inc"

RISCXRegisterInfo::RISCXRegisterInfo(const RISCXSubtarget &STI):  RISCXGenRegisterInfo(RISCX::RA), STI(STI) {
}

uint32_t const *RISCXRegisterInfo::getCallPreservedMask(const MachineFunction &MF, CallingConv::ID) const {
    return CSR_RegMask;
}

const MCPhysReg *RISCXRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
    // static const MCPhysReg CalleeSaveRegs[] = {RISCX::X2, 0};
    // return CalleeSaveRegs;
    return CSR_SaveList;
}

BitVector RISCXRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
    BitVector Reserved(getNumRegs());
    Reserved.set(RISCX::ZERO);
    Reserved.set(RISCX::RA);
    Reserved.set(RISCX::SP);
    return Reserved;
}

bool RISCXRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                        unsigned FIOperandNum,
                        RegScavenger *RS) const {
    uint i = 0;
    while (!MI->getOperand(i).isFI()) {
        i++;
    }
    const int FI = MI->getOperand(i).getIndex();
    const MachineFunction* MF = MI->getParent()->getParent();
    const MachineFrameInfo &MFI = MF->getFrameInfo();
    int64_t Offset = MFI.getObjectOffset(FI);
    uint64_t STACKSIZE = ROUND_UP(MFI.getStackSize(), STI.getFrameLowering()->getStackAlignment());
    Offset += static_cast<int64_t>(STACKSIZE);

    int64_t O = MI->getOperand(i + 1).getImm();
    Offset += O;

    MI->getOperand(i).ChangeToRegister(RISCX::SP,  false);
    MI->getOperand(i + 1).ChangeToImmediate(Offset);
    return true;
}

Register RISCXRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
    return RISCX::SP;
}