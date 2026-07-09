//===-- RISCVMCExpr.h - RISC-V specific MC expression classes----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes RISC-V specific MCExprs, used for modifiers like
// "%hi" or "%lo" etc.,
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RISCX_MCTARGETDESC_RISCVMCEXPR_H
#define LLVM_LIB_TARGET_RISCX_MCTARGETDESC_RISCVMCEXPR_H

#include "llvm/MC/MCExpr.h"

namespace llvm {
class RISCXMCExpr : public MCTargetExpr {
public:
  enum Kind { None, HI, LO };

  RISCXMCExpr(Kind K, const MCExpr *Expr) : Kd(K), Expr(Expr) {}

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;

  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAssembler *Asm,
                                const MCFixup *Fixup) const override { return false; }

  void visitUsedExpr(MCStreamer &Streamer) const override {}

  MCFragment *findAssociatedFragment() const override { return nullptr; }

  void fixELFSymbolsInTLSFixups(MCAssembler &) const override {}

private:
  const Kind Kd;
  const MCExpr *Expr;
};
} // end namespace llvm.

#endif
