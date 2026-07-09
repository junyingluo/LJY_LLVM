#include "RISCXAsmPrinter.h"
#include "MCTargetDesc/RISCXMCTargetDesc.h"
#include "TargetInfo/RISCXTargetInfo.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"
#include "MCTargetDesc/RISCXMCExpr.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "RISCXGenMCPseudoLowering.inc"

bool RISCXAsmPrinter::runOnMachineFunction(MachineFunction &MF) {
  AsmPrinter::runOnMachineFunction(MF);
  return true;
}

void RISCXAsmPrinter::lowerToMCInst(const MachineInstr *MI, MCInst &Out) {
  Out.setOpcode(MI->getOpcode());
  for (const MachineOperand &MO : MI->operands()) {
    MCOperand MCOp;
    switch (MO.getType()) {
    case MachineOperand::MO_MachineBasicBlock:
    case MachineOperand::MO_GlobalAddress: {
      MCOp = LowerSymbolOperand(MO);
      break;
    }
    case MachineOperand::MO_Register: {
      if (MO.isImplicit())
        continue;
      MCOp = MCOperand::createReg(MO.getReg());
      break;
    }
    case MachineOperand::MO_Immediate: {
      MCOp = MCOperand::createImm(MO.getImm());
      break;
    }
    case MachineOperand::MO_RegisterMask: {
      break;
    }
    default:
      llvm_unreachable("unknown operand type");
    }
    Out.addOperand(MCOp);
  }
}

MCOperand RISCXAsmPrinter::LowerSymbolOperand(const MachineOperand &MO) const {
  RISCXMCExpr::Kind kind = RISCXMCExpr::None;
  const MCSymbol *symbol = nullptr;

  switch (MO.getTargetFlags()) {
  case RISCXMCExpr::HI:
    kind = RISCXMCExpr::HI;
    break;
  case RISCXMCExpr::LO:
    kind = RISCXMCExpr::LO;
    break;
  default:
    break;
  }
  
  if (MO.getType() == MachineOperand::MO_MachineBasicBlock) {
    symbol = MO.getMBB()->getSymbol();
  } else {
    symbol = getSymbol(MO.getGlobal());
  }
  
  const MCExpr *Expr = MCSymbolRefExpr::create(symbol, OutContext);
  Expr = new RISCXMCExpr(kind, Expr);
  return MCOperand::createExpr(Expr);
}

void RISCXAsmPrinter::emitInstruction(const MachineInstr *MI) {
  if (emitPseudoExpansionLowering(*OutStreamer, MI))
    return;

  MCInst temp;
  lowerToMCInst(MI, temp);
  EmitToStreamer(*OutStreamer, temp);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCXAsmPrinter() {
  RegisterAsmPrinter<RISCXAsmPrinter> X(getTheRISCXTarget());
}