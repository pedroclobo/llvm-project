add_llvm_component_library(LLVMInstrCount
  InstrCount.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Transforms/Utils

  LINK_COMPONENTS
  Core
  Support
  )

