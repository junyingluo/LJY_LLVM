add_llvm_component_group(RISCX)

set(LLVM_TARGET_DEFINITIONS RISCX.td)

tablegen(LLVM RISCXGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM RISCXGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM RISCXGenCallingConv.inc -gen-callingconv)
tablegen(LLVM RISCXGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM RISCXGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM RISCXGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM RISCXGenMCPseudoLowering.inc -gen-pseudo-lowering)

add_public_tablegen_target(RISCXCommonTableGen)

add_llvm_target(RISCXCodeGen
  RISCXTargetMachine.cpp
  RISCXInstrInfo.cpp
  RISCXRegisterInfo.cpp
  RISCXSubtarget.cpp
  RISCXFrameLowering.cpp
  RISCXISelLowering.cpp
  RISCXISelDagToDag.cpp
  RISCXAsmPrinter.cpp
  RISCXCallingConv.cpp
  
  LINK_COMPONENTS
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  MC
  SelectionDAG
  RISCXDesc
  RISCXInfo
  Support
  Target
  TargetParser

  ADD_TO_COMPONENT
  RISCX
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)

