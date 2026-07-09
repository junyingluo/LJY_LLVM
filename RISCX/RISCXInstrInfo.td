include "RISCXInstrFormats.td"

def retglue : SDNode<"RISCXISD::RET_GLUE", SDTNone, 
  [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def imm12: Operand<i32>, ImmLeaf<i32, [{ return isInt<12>(Imm);}]>;
def imm20: Operand<i32>, ImmLeaf<i32, [{ return isInt<20>(Imm);}]>;

def mem : Operand<iPTR> {
    let MIOperandInfo = (ops GPR, imm12);
    let PrintMethod = "printMemOperand";
}

def ptr : Operand<iPTR> {
    let MIOperandInfo = (ops GPR, imm12);
    let PrintMethod = "printPtrOperand";
}

def addrFI : ComplexPattern<iPTR, 2, "SelectAddrFI", [frameindex], [SDNPWantParent]>;

multiclass LOAD<PatFrag op, string asm> {
  def FI : RISCXInst<(outs GPR: $ra), (ins mem: $addr), !strconcat(asm, "\t$ra, $addr"), [(set GPR: $ra, (op addrFI: $addr))]>;
  def R : RISCXInst<(outs GPR: $ra), (ins GPR: $rb), !strconcat(asm, "\t$ra, 0($rb)"), [(set GPR: $ra, (op GPR: $rb))]>;
}

multiclass STORE<PatFrag op, string asm> {
  def FI : RISCXInst<(outs), (ins GPR:$ra, mem: $addr), !strconcat(asm, "\t$ra, $addr"), [(op GPR:$ra, addrFI:$addr)]>;
  def R : RISCXInst<(outs), (ins GPR:$ra, GPR: $rb), !strconcat(asm, "\t$ra, 0($rb)"), [(op GPR:$ra, GPR:$rb)]>;
}

defm STOREW:STORE<store, "sw">;
defm STOREH:STORE<truncstorei16, "sh">;
defm STOREB:STORE<truncstorei8, "sb">;

defm LOADW:LOAD<load, "lw">;
defm LOADH:LOAD<extloadi16, "lh">;
defm LOADSH:LOAD<sextloadi16, "lh">;
defm LOADZH:LOAD<zextloadi16, "lh">;
defm LOADB:LOAD<extloadi8, "lb">;
defm LOADSB:LOAD<sextloadi8, "lb">;
defm LOADZB:LOAD<zextloadi8, "lb">;

class ArithLogicI<string inst, SDNode node>: I<
    (outs GPR:$rd),
    (ins GPR:$rs1, imm12: $imm12),
    !strconcat(inst, "\t$rd, $rs1, $imm12"),
    [(set GPR:$rd, (node GPR:$rs1, imm12: $imm12))]
  >;

def ADDI: ArithLogicI<"addi", add>;
def ANDI: ArithLogicI<"andi", and>;
def ORI: ArithLogicI<"ori", or>;
def XORI: ArithLogicI<"xori", xor>;
def SLLI: ArithLogicI<"slli", shl>;
def SRLI: ArithLogicI<"srli", srl>;
def SRAI: ArithLogicI<"srai", sra>;

class ArithLogicR<string inst, SDNode node>: R<
    (outs GPR:$rd),
    (ins GPR:$rs1, GPR:$rs2),
    !strconcat(inst, "\t$rd, $rs1, $rs2"),
    [(set GPR:$rd, (node GPR:$rs1, GPR:$rs2))]
  >;

def ADD: ArithLogicR<"add", add>;
def SUB: ArithLogicR<"sub", sub>;
def MUL: ArithLogicR<"mul", mul>;
def DIV: ArithLogicR<"div", sdiv>;
def REM: ArithLogicR<"rem", srem>;
def AND: ArithLogicR<"and", and>;
def OR: ArithLogicR<"or", or>;
def XOR: ArithLogicR<"xor", xor>;
def SLL: ArithLogicR<"sll", shl>;
def SRL: ArithLogicR<"srl", srl>;
def SRA: ArithLogicR<"sra", sra>;

def : Pat<(i32 (imm12:$imm12)), (ADDI ZERO, imm12:$imm12)>;

let isCall = 1 in {
  def JALR: I<(outs GPR:$rd), 
      (ins GPR:$rs1, imm12:$imm12),
      "jalr $rd, $rs1, $imm12", []>;
}

let isBarrier = 1, isReturn = 1, isTerminator = 1 in {
  def PseudoRET: PseudoInst<(outs), (ins), [(retglue)]>,
      PseudoInstExpansion<(JALR ZERO, RA, 0)>;
}

def RISCXSDT_Call : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;

def riscx_call : SDNode<"RISCXISD::Call", RISCXSDT_Call,
                      [SDNPHasChain, SDNPOutGlue,
                       SDNPOptInGlue, SDNPVariadic]>;

def call_symbol : Operand<iPTR>;

def CALL : I<(outs), (ins call_symbol:$rs1), "call\t$rs1", [(riscx_call tglobaladdr:$rs1)]> {
  let isCall = 1;
}

def HI: SDNode<"RISCXISD::HI", SDTIntUnaryOp>;
def LO: SDNode<"RISCXISD::LO", SDTIntUnaryOp>;

def LUI : U<(outs GPR:$rd), (ins imm20:$imm), "lui\t$rd, $imm", []>;

def : Pat<(HI tglobaladdr:$in), (LUI tglobaladdr:$in)>;
def : Pat<(add GPR:$hi, (LO tglobaladdr:$lo)), (ADDI GPR:$hi, tglobaladdr:$lo)>;

def : InstAlias<"ret", (JALR ZERO, RA, 0)>;
def : InstAlias<"li $rd, $imm", (ADDI GPR:$rd, ZERO, imm12:$imm)>;

def SLT : R<(outs GPR:$rd), (ins GPR:$rs1, GPR:$rs2), "slt\t$rd, $rs1, $rs2", []>;

def : Pat<(setgt GPR:$lhs, GPR:$rhs), (SLT GPR:$rhs, GPR:$lhs)>;
def : Pat<(setlt GPR:$lhs, GPR:$rhs), (SLT GPR:$lhs, GPR:$rhs)>;
def : Pat<(seteq GPR:$lhs, GPR:$rhs), (SLT (XOR GPR:$lhs, GPR:$rhs), 1)>;
def : Pat<(setne GPR:$lhs, GPR:$rhs), (SLT ZERO, (XOR GPR:$lhs, GPR:$rhs))>;
def : Pat<(setge GPR:$lhs, GPR:$rhs), (XORI (SLT GPR:$lhs, GPR:$rhs), 1)>;
def : Pat<(setle GPR:$lhs, GPR:$rhs), (XORI (SLT GPR:$rhs, GPR:$lhs), 1)>;

def brtarget : Operand<OtherVT>;

let isTerminator = 1 in {
  def J : J<(outs), (ins brtarget:$imm12), "j ${imm12}", []>;
  def BLT : B<(outs), (ins GPR:$rs1, GPR:$rs2, brtarget:$imm), "blt\t$rs1, $rs2, $imm", []>;
  def BEQ : B<(outs), (ins GPR:$rs1, GPR:$rs2, brtarget:$imm), "beq\t$rs1, $rs2, $imm", []>;
  def BNE : B<(outs), (ins GPR:$rs1, GPR:$rs2, brtarget:$imm), "bne\t$rs1, $rs2, $imm", []>;
}


def : Pat<(brcond (i32 (setlt GPR:$lhs, GPR:$rhs)), bb:$dst), (BLT GPR:$lhs, GPR:$rhs, bb:$dst)>;
def : Pat<(brcond (i32 (setgt GPR:$lhs, GPR:$rhs)), bb:$dst), (BLT GPR:$rhs, GPR:$lhs, bb:$dst)>;
def : Pat<(brcond (i32 (seteq GPR:$lhs, GPR:$rhs)), bb:$dst), (BEQ GPR:$lhs, GPR:$rhs, bb:$dst)>;
def : Pat<(brcond (i32 (setne GPR:$lhs, GPR:$rhs)), bb:$dst), (BNE GPR:$lhs, GPR:$rhs, bb:$dst)>;

def : Pat<(br bb:$imm), (J brtarget:$imm)>;

def : I<(outs GPR:$rd), (ins ptr:$p), "addi \t$rd, $p", [(set GPR:$rd, addrFI: $p)]>;