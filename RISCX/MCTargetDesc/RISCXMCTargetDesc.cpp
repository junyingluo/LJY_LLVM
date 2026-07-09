#include "RISCXMCTargetDesc.h"
#include "RISCXInstPrinter.h"
#include "RISCXInstrInfo.h"
#include "RISCXMCAsmInfo.h"
#include "RISCXRegisterInfo.h"
#include "RISCXSubtarget.h"
#include "TargetInfo/RISCXTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "RISCXGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "RISCXGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "RISCXGenRegisterInfo.inc"

static MCAsmInfo *createRISCXMCAsmInfo(const MCRegisterInfo &MRI,
                                     const Triple &TT,
                                     const MCTargetOptions &Options) {
  return new RISCXMCAsmInfo(TT);
}

static MCInstrInfo *createRISCXMCInstrInfo() {
  MCInstrInfo *x = new MCInstrInfo();
  InitRISCXMCInstrInfo(x);
  return x;
}

static MCRegisterInfo *createRISCXMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *x = new MCRegisterInfo();
  InitRISCXMCRegisterInfo(x, RISCX::RA);
  return x;
}

static MCSubtargetInfo *createRISCXMCSubtargetInfo(const Triple &TT,
                                                 StringRef CPU, StringRef FS) {
  if (CPU.empty()) {
    CPU = "riscx";
  }

  return createRISCXMCSubtargetInfoImpl(TT, CPU, CPU, FS);
}

static MCInstPrinter *createRISCXMCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  return new RISCXInstPrinter(MAI, MII, MRI);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeRISCXTargetMC() {
  TargetRegistry::RegisterMCAsmInfo(getTheRISCXTarget(), createRISCXMCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(getTheRISCXTarget(), createRISCXMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(getTheRISCXTarget(), createRISCXMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(getTheRISCXTarget(),
                                          createRISCXMCSubtargetInfo);
  TargetRegistry::RegisterMCInstPrinter(getTheRISCXTarget(),
                                        createRISCXMCInstPrinter);
}