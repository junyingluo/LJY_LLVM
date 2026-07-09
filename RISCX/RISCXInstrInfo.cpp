#include "RISCXInstrInfo.h"

using namespace llvm;

#define GET_INSTRINFO_ENUM
#define GET_INSTRINFO_CTOR_DTOR
#include "RISCXGenInstrInfo.inc"

#define GET_REGINFO_ENUM
#include "RISCXGenRegisterInfo.inc"

RISCXInstrInfo::RISCXInstrInfo():  RISCXGenInstrInfo() {
}

void RISCXInstrInfo::copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const{
    MachineInstrBuilder MIB = BuildMI(MBB, I, DL, get(RISCX::ADD));
    MIB.addReg(DestReg, RegState::Define);
    MIB.addReg(RISCX::ZERO);
    MIB.addReg(SrcReg, getKillRegState(KillSrc));
}

void RISCXInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                        MachineBasicBlock::iterator MBBI, Register SrcReg,
                        bool isKill, int FrameIndex,
                        const TargetRegisterClass *RC,
                        const TargetRegisterInfo *TRI,
                        Register VReg) const {
    DebugLoc DL;
    BuildMI(MBB, MBBI, DL, get(RISCX::STOREWFI))
        .addReg(SrcReg, getKillRegState(isKill))
        .addFrameIndex(FrameIndex)
        .addImm(0);                     
}
                        
void RISCXInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                        MachineBasicBlock::iterator MBBI, Register DestReg,
                        int FrameIndex, const TargetRegisterClass *RC,
                        const TargetRegisterInfo *TRI,
                        Register VReg) const {
    DebugLoc DL;
    BuildMI(MBB, MBBI, DL, get(RISCX::LOADWFI), DestReg)
        .addFrameIndex(FrameIndex)
        .addImm(0); 
}