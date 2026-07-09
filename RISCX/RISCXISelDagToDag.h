#ifndef LLVM_LIB_TARGET_RISCX_RISCXISELDAGTODAG_H
#define LLVM_LIB_TARGET_RISCX_RISCXISELDAGTODAG_H

#include "MCTargetDesc/RISCXMCTargetDesc.h"
#include "RISCX.h"
#include "RISCXSubtarget.h"
#include "RISCXTargetMachine.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

namespace llvm {

class RISCXDAGToDAGISel : public SelectionDAGISel {
public:
  RISCXDAGToDAGISel() = delete;

  explicit RISCXDAGToDAGISel(RISCXTargetMachine &TM, CodeGenOptLevel OL)
      : SelectionDAGISel(TM, OL) {}

  bool runOnMachineFunction(MachineFunction &MF) override;
  bool SelectAddrFI(SDNode *Parent, SDValue AddrFI, SDValue &Base, SDValue &Offset);

private:
  const RISCXSubtarget *Subtarget = nullptr;

#include "RISCXGenDAGISel.inc"

  const RISCXTargetMachine &getTargetMachine() const {
    return static_cast<const RISCXTargetMachine &>(TM);
  }

  void Select(SDNode *Node) override;
};

class RISCXDAGToDAGISelLegacy : public SelectionDAGISelLegacy {
public:
  static char ID;
  explicit RISCXDAGToDAGISelLegacy(RISCXTargetMachine &TM)
      : SelectionDAGISelLegacy(ID, std::make_unique<RISCXDAGToDAGISel>(TM, TM.getOptLevel())) {}
};

} // namespace llvm

#endif