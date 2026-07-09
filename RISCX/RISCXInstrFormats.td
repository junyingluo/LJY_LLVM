
class RISCXInst<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> SoftFail = 0;

  let Size = 4;

  let Namespace = "RISCX";

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr; 
  let Pattern = pattern;
}

class PseudoInst<dag outs, dag ins, list<dag> pattern, string asmstr = "">
    : RISCXInst<outs, ins, asmstr, pattern> {
  let isPseudo = 1;
  let isCodeGenOnly = 1; 
}

class I<dag outs, dag ins, string asmstr, list<dag> pattern> 
  : RISCXInst<outs, ins, asmstr, pattern> {
}

class U<dag outs, dag ins, string asmstr, list<dag> pattern> 
  : RISCXInst<outs, ins, asmstr, pattern> {
}

class R<dag outs, dag ins, string asmstr, list<dag> pattern> 
  : RISCXInst<outs, ins, asmstr, pattern> {
}

class J<dag outs, dag ins, string asmstr, list<dag> pattern> 
  : RISCXInst<outs, ins, asmstr, pattern> {
}

class B<dag outs, dag ins, string asmstr, list<dag> pattern> 
  : RISCXInst<outs, ins, asmstr, pattern> {
}

class S<dag outs, dag ins, string asmstr, list<dag> pattern> 
  : RISCXInst<outs, ins, asmstr, pattern> {
}