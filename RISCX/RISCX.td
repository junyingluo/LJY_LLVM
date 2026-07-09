include "llvm/Target/Target.td"

include "RISCXRegisterInfo.td"
include "RISCXCallingConv.td"
include "RISCXInstrInfo.td"

def RISCXInstrInfo : InstrInfo;

def: ProcessorModel<"riscx", NoSchedModel, []>;

def RISCX : Target {
  let InstructionSet = RISCXInstrInfo;
}
