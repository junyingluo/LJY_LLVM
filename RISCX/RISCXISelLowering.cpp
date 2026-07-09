#include "RISCXISelLowering.h"
#include "MCTargetDesc/RISCXMCTargetDesc.h"
#include "RISCXFrameLowering.h"
#include "RISCXSubtarget.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/CodeGenTypes/MachineValueType.h"
#include "MCTargetDesc/RISCXMCExpr.h"
#include "RISCXCallingConv.h"

using namespace llvm;

#include "RISCXGenCallingConv.inc"

RISCXTargetLowering::RISCXTargetLowering(const TargetMachine &TM,
                                     const RISCXSubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {
  addRegisterClass(MVT::i32, &RISCX::GPRRegClass);

  setOperationAction(ISD::GlobalAddress, MVT::i32, Custom);
  setOperationAction(ISD::BR_CC, MVT::i32, Expand);

  computeRegisterProperties(STI.getRegisterInfo());
}

SDValue RISCXTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const  {
  switch (Op.getOpcode()) {
  case ISD::GlobalAddress:
    return LowerGlobalAddress(Op, DAG);
  default:
    llvm::llvm_unreachable_internal("unknown op");
  }
}

SDValue RISCXTargetLowering::LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const {
  EVT VT = Op.getValueType();
  GlobalAddressSDNode *N = dyn_cast<GlobalAddressSDNode>(Op);
  SDLoc DL(N);

  SDValue Hi = DAG.getTargetGlobalAddress(N->getGlobal(), DL, VT, 0, RISCXMCExpr::HI);
  SDValue Lo = DAG.getTargetGlobalAddress(N->getGlobal(), DL, VT, 0, RISCXMCExpr::LO);

  SDValue HiNode = DAG.getNode(RISCXISD::HI, DL, VT, Hi);
  SDValue LoNode = DAG.getNode(RISCXISD::LO, DL, VT, Lo);
  
  SDValue BaseAddr = DAG.getNode(ISD::ADD, DL, VT, HiNode, LoNode);
  int64_t Offset = N->getOffset();

  if (Offset) {
    return DAG.getNode(ISD::ADD, DL, VT, BaseAddr, DAG.getConstant(Offset, DL, VT));
  }
  return BaseAddr;
}

SDValue RISCXTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_RISCX);

  SDValue ArgValue;
  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    if (VA.isRegLoc()) {
      MVT RegVT = VA.getLocVT();
      Register Reg = MF.addLiveIn(VA.getLocReg(), &RISCX::GPRRegClass);
      ArgValue = DAG.getCopyFromReg(Chain, DL, Reg, RegVT);
      InVals.push_back(ArgValue);
    } else {
      assert(VA.isMemLoc());
      MVT ValVT = VA.getValVT();
      int Offset = VA.getLocMemOffset();
      int FI = MFI.CreateFixedObject(ValVT.getSizeInBits() / 8, Offset, true);
      SDValue FIN = DAG.getFrameIndex(FI, getPointerTy(DAG.getDataLayout()));
      SDValue Val = DAG.getLoad(
          ValVT, DL, Chain, FIN,
          MachinePointerInfo::getFixedStack(DAG.getMachineFunction(), FI)
      );
      InVals.push_back(Val);
    }
  }

  return Chain;
}

SDValue RISCXTargetLowering::LowerCall(CallLoweringInfo &CLI, SmallVectorImpl<SDValue> &InVals) const {
    SelectionDAG &DAG = CLI.DAG;
    SDLoc &DL = CLI.DL;
    SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
    SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
    SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
    SDValue Chain = CLI.Chain;
    SDValue Callee = CLI.Callee;
    CallingConv::ID CallConv = CLI.CallConv;
    bool IsVarArg = CLI.IsVarArg;

    MachineFunction &MF = DAG.getMachineFunction();
    SmallVector<CCValAssign, 16> ArgLocs;
    CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
    CCInfo.AnalyzeCallOperands(Outs, CC_RISCX);

    SmallVector<std::pair<unsigned, SDValue>> RegsPairs;
    SDValue StackPtr;

    for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
        CCValAssign &VA = ArgLocs[i];
        if (VA.isRegLoc()) {
          RegsPairs.push_back(std::make_pair(VA.getLocReg(), OutVals[i]));
        } else {
          assert(VA.isMemLoc());
          if (!StackPtr.getNode()) {
            StackPtr = DAG.getCopyFromReg(Chain, DL, RISCX::SP,
                getPointerTy(DAG.getDataLayout()));
          }
          unsigned LocMemOffset = VA.getLocMemOffset();
          SDValue PtrOff = DAG.getIntPtrConstant(LocMemOffset, DL);
          PtrOff = DAG.getNode(ISD::ADD, DL, getPointerTy(DAG.getDataLayout()),
             StackPtr, PtrOff);
          Chain = DAG.getStore(Chain, DL, OutVals[i], PtrOff, 
            MachinePointerInfo::getStack(MF, LocMemOffset));
        }
    }

    GlobalAddressSDNode *N = dyn_cast<GlobalAddressSDNode>(Callee);
    Callee = DAG.getTargetGlobalAddress(N->getGlobal(), DL, getPointerTy(DAG.getDataLayout()));

    SmallVector<SDValue, 8> Ops(1, Chain);
    Ops.push_back(Callee);
    SDValue Glue;

    for (int i = 0, e = RegsPairs.size(); i != e; ++i) {
      auto &[reg, val] = RegsPairs[i];
      Chain = DAG.getCopyToReg(Chain, DL, reg, val, Glue);
      Glue = Chain.getValue(1);
      Ops.push_back(DAG.getRegister(reg, val.getValueType()));
    }

    const TargetRegisterInfo *TRI = Subtarget.getRegisterInfo();
    const uint32_t *Mask = TRI->getCallPreservedMask(DAG.getMachineFunction(), CallConv);
    Ops.push_back(DAG.getRegisterMask(Mask));
    if (Glue.getNode()) {
      Ops.push_back(Glue);
    }

    SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
    Chain = DAG.getNode(RISCXISD::Call, DL, NodeTys, Ops);

    {
      SDValue Glue = Chain.getValue(1);
      SmallVector<CCValAssign, 2> RVLos;
      CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLos, *DAG.getContext());
      CCInfo.AnalyzeCallResult(Ins, RetCC_RISCX);

      for (unsigned i = 0, e = RVLos.size(); i != e; ++i) {
        CCValAssign &VA = RVLos[i];
        EVT vt = RVLos[i].getLocVT();
        assert(VA.isRegLoc());
        Register RVReg = VA.getLocReg();
        SDValue Val = DAG.getCopyFromReg(Chain, DL, RVReg, vt, Glue);
        Chain = Val.getValue(1);
        Glue = Val.getValue(2);
        InVals.push_back(Val);
      }
    }

    return Chain;
}

SDValue
RISCXTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::OutputArg> &Outs,
                               const SmallVectorImpl<SDValue> &OutVals,
                               const SDLoc &DL, SelectionDAG &DAG) const {
  SmallVector<CCValAssign, 16> RVLocs;

  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  CCInfo.AnalyzeReturn(Outs, RetCC_RISCX);

  SDValue Glue;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  for (unsigned i = 0, e = RVLocs.size(); i < e; ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");
    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), OutVals[i], Glue);
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  RetOps[0] = Chain;

  if (Glue.getNode()) {
    RetOps.push_back(Glue);
  }

  return DAG.getNode(RISCXISD::RET_GLUE, DL, MVT::Other, RetOps);
}

const char *RISCXTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch(Opcode) {
    case RISCXISD::RET_GLUE:
      return "RISCXISO::RET_GLUE";
    case RISCXISD::Call:
      return "RISCXISO::Call";
    case RISCXISD::HI:
      return "RISCXISO::HI";
    case RISCXISD::LO:
      return "RISCXISO::LO";
    default:
      return nullptr;
  }
}