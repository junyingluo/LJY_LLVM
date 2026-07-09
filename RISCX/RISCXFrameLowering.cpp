#include "RISCXFrameLowering.h"
#include "MCTargetDesc/RISCXMCTargetDesc.h"

#include "RISCX.h"
#include "RISCXSubtarget.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"

using namespace llvm;

uint64_t RISCXFrameLowering::computeStackSize(MachineFunction &MF) const {
  uint64_t STACK_SIZE = MF.getFrameInfo().getStackSize();

  if (getStackAlignment() > 0) {
    STACK_SIZE = ROUND_UP(STACK_SIZE, getStackAlignment());
  }

  return STACK_SIZE;
}

void RISCXFrameLowering::emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.begin();

  const TargetInstrInfo &TII = *STI.getInstrInfo();

  int STACKSIZE = computeStackSize(MF);

  if (STACKSIZE == 0) return;

  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  BuildMI(MBB, MBBI, DL, TII.get(RISCX::ADDI), RISCX::SP) // MachineInstr
      .addReg( RISCX::SP)
      .addImm(-STACKSIZE)
      .setMIFlag(MachineInstr::FrameSetup);
}

void RISCXFrameLowering::emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();

  const TargetInstrInfo &TII = *STI.getInstrInfo();

  int STACKSIZE = computeStackSize(MF);

  if (STACKSIZE == 0) return;

  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  BuildMI(MBB, MBBI, DL, TII.get(RISCX::ADDI), RISCX::SP) 
      .addReg( RISCX::SP)
      .addImm(STACKSIZE)
      .setMIFlag(MachineInstr::FrameDestroy);
}

bool RISCXFrameLowering::hasFP(const MachineFunction &MF) const {
    return true;
}

void RISCXFrameLowering::determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs, RegScavenger *RS) const {
    TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);

    if (MF.getFrameInfo().hasCalls()) {
        SavedRegs.set(RISCX::RA);
    }
}