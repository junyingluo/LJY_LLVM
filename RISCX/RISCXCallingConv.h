#ifndef RISCXCALLINGCONV_H
#define RISCXCALLINGCONV_H

#include "MCTargetDesc/RISCXMCTargetDesc.h"
#include "llvm/CodeGen/CallingConvLower.h"

namespace llvm {
bool CC_RISCX(unsigned ValNo, MVT ValVT, MVT LocVT,
            CCValAssign::LocInfo LocInfo, ISD::ArgFlagsTy ArgFlags,
            CCState &State);
} // namespace llvm

#endif //RISCXCALLINGCONV_H