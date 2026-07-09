add_llvm_component_library(LLVMRISCXDesc
  RISCXMCTargetDesc.cpp
  RISCXMCAsmInfo.cpp
  RISCXInstPrinter.cpp
  RISCXMCExpr.cpp

  LINK_COMPONENTS
  MC
  RISCXInfo
  Support
  TargetParser

  ADD_TO_COMPONENT
  RISCX
  )
