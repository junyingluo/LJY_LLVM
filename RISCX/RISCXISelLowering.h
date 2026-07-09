#ifndef LLVM_LIB_TARGET_RISCX_RISCXISELLOWERING_H
#define LLVM_LIB_TARGET_RISCX_RISCXISELLOWERING_H

#include "RISCXFrameLowering.h"
#include "llvm/CodeGen/GlobalISel/CallLowering.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

class RISCXSubtarget;

namespace RISCXISD {
enum NodeType : unsigned { FIRST_NUMBER = ISD::BUILTIN_OP_END, RET_GLUE, Call, HI, LO };
} // namespace RISCXISD

class RISCXTargetLowering : public TargetLowering {
  const RISCXSubtarget &Subtarget;

public:
  explicit RISCXTargetLowering(const TargetMachine &TM, const RISCXSubtarget &STI);

  const RISCXSubtarget &getSubtarget() const { return Subtarget; };

  SDValue LowerCall(CallLoweringInfo &, SmallVectorImpl<SDValue> &) const override;

  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;
  
  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  const char *getTargetNodeName(unsigned Opcode) const override;
};

} // namespace llvm

#endif