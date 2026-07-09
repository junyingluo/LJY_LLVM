#include "RISCXISelDagToDag.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"

using namespace llvm;

#define DEBUG_TYPE "riscx-isel"
#define PASS_NAME "RISCX DAG->DAG Pattern Instruction Selection"

bool RISCXDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  Subtarget = &MF.getSubtarget<RISCXSubtarget>();
  return SelectionDAGISel::runOnMachineFunction(MF);
}

void RISCXDAGToDAGISel::Select(SDNode *Node) {
  SDLoc DL(Node);

  LLVM_DEBUG(dbgs() << "Selecting: "; Node->dump(CurDAG); dbgs() << '\n');

  SelectCode(Node);
}

bool RISCXDAGToDAGISel::SelectAddrFI(SDNode *Parent, SDValue AddrFI, SDValue &Base, SDValue &Offset) {
  if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(AddrFI)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), AddrFI.getValueType());
    Offset = CurDAG->getTargetConstant(0, SDLoc(AddrFI), AddrFI.getValueType());
    return true;
  }
  if (CurDAG->isBaseWithConstantOffset(AddrFI)) {
    EVT ValTy = AddrFI.getValueType();
    ConstantSDNode *CN = dyn_cast<ConstantSDNode>(AddrFI.getOperand(1));
    if (FrameIndexSDNode *FIN = dyn_cast<FrameIndexSDNode>(AddrFI.getOperand(0)))
      Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), ValTy);
    else {
      Base = AddrFI.getOperand(0);
    }
    Offset = CurDAG->getTargetConstant(CN->getZExtValue(), SDLoc(AddrFI), ValTy);
    return true;
  }
  return false;
}

char RISCXDAGToDAGISelLegacy::ID = 0;

INITIALIZE_PASS(RISCXDAGToDAGISelLegacy, DEBUG_TYPE, PASS_NAME, false, false)

FunctionPass *llvm::createRISCXISelDag(RISCXTargetMachine &TM) {
  return new RISCXDAGToDAGISelLegacy(TM);
}
