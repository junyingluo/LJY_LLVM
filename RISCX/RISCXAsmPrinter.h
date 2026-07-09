#ifndef LLVM_LIB_TARGET_RISCX_RISCXASMPRINTER_H
#define LLVM_LIB_TARGET_RISCX_RISCXASMPRINTER_H

#include "RISCXTargetMachine.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Target/TargetMachine.h"
#include <memory>
#include <utility>

namespace llvm {

class RISCXSubtarget;
class LLVM_LIBRARY_VISIBILITY RISCXAsmPrinter : public AsmPrinter {
public:
  const RISCXSubtarget *Subtarget;

  explicit RISCXAsmPrinter(TargetMachine &TM,
                         std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {
    Subtarget = static_cast<RISCXTargetMachine &>(TM).getSubtargetImpl();
  }

  StringRef getPassName() const override { return "RISCX Assembly Printer"; }

  virtual bool runOnMachineFunction(MachineFunction &MF) override;

  void emitInstruction(const MachineInstr *MI) override;
  bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,
                                   const MachineInstr *MI);

private:
  void lowerToMCInst(const MachineInstr *MI, MCInst &Out);
  MCOperand LowerSymbolOperand(const MachineOperand &MO) const;
};
} // namespace llvm

#endif