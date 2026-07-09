add_llvm_component_library(LLVMRISCXInfo
  RISCXTargetInfo.cpp

  LINK_COMPONENTS
  Support
  MC
  )
