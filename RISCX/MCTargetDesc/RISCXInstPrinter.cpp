#include "RISCXInstPrinter.h"
#include "RISCXInstrInfo.h"
#include "RISCXRegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

#define PRINT_ALIAS_INSTR
#include "RISCXGenAsmWriter.inc"

void RISCXInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) const {
  OS << getRegisterName(Reg);
}

void RISCXInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                               StringRef Annot, const MCSubtargetInfo &STI,
                               raw_ostream &O) {
  if (!printAliasInstr(MI, Address, O))
    printInstruction(MI, Address, O);

  printAnnotation(O, Annot);
}

void RISCXInstPrinter::printMemOperand(const MCInst *MI, unsigned opNum, raw_ostream &O) {
  printOperand(MI, opNum+1, O);
  O << "(";
  printOperand(MI, opNum, O);
  O << ")";
}

void RISCXInstPrinter::printPtrOperand(const MCInst *MI, unsigned opNum, raw_ostream &O) {
  printOperand(MI, opNum, O);
  O << ", ";
  printOperand(MI, opNum+1, O);
}

void RISCXInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                  raw_ostream &O) {
  const MCOperand &MO = MI->getOperand(OpNo);
  if (MO.isReg()) {
    printRegName(O, MO.getReg());
    return;
  }

  if (MO.isImm()) {
    printImmediate(MI, OpNo, O);
    return;
  }

  assert(MO.isExpr() && "Unknown operand kind in printOperand");
  MO.getExpr()->print(O, &MAI);
}

void RISCXInstPrinter::printImmediate(const MCInst *MI, unsigned opNum,
                                    raw_ostream &O) {
  const MCOperand &MO = MI->getOperand(opNum);
  if (MO.isImm())
    O << MO.getImm();
  else if (MO.isExpr()) {
    MO.getExpr()->print(O, &MAI);
  } else
    llvm_unreachable("Unknown immediate kind");
}
