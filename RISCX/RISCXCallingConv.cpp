#include "RISCXCallingConv.h"
#include "llvm/CodeGen/TargetLowering.h"

using namespace llvm;

bool llvm::CC_RISCX(unsigned ValNo, MVT ValVT,
                   MVT LocVT, CCValAssign::LocInfo LocInfo,
                   ISD::ArgFlagsTy ArgFlags, CCState &State) {
    unsigned XLen = 32;

    if (LocVT == MVT::i32) {
        static const MCPhysReg RegList1[] = {
        RISCX::A0, RISCX::A1, RISCX::A2
        };

        unsigned TwoXLenInBytes = (2 * XLen) / 8;
        if (ArgFlags.getNonZeroOrigAlign() == TwoXLenInBytes) {
            unsigned RegIdx = State.getFirstUnallocated(RegList1);
            if (RegIdx != std::size(RegList1) && RegIdx % 2 == 1)
                State.AllocateReg(RegList1);
        }

        if (unsigned Reg = State.AllocateReg(RegList1)) {
            State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
            return false;
        }
    }

    int64_t Offset2 = State.AllocateStack(4, Align(4));
    State.addLoc(CCValAssign::getMem(ValNo, ValVT, Offset2, LocVT, LocInfo));
    return false;
}