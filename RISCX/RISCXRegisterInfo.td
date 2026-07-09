class RISCXReg<string n> : Register<n> {
  let Namespace = "RISCX";
}

def ZERO : RISCXReg<"zero">, DwarfRegNum<[0]>;
def RA : RISCXReg<"ra">, DwarfRegNum<[1]>;
def SP : RISCXReg<"sp">, DwarfRegNum<[2]>;
def T0 : RISCXReg<"t0">, DwarfRegNum<[5]>;
def T1 : RISCXReg<"t1">, DwarfRegNum<[6]>;
def T2 : RISCXReg<"t2">, DwarfRegNum<[7]>;
def S0 : RISCXReg<"s0">, DwarfRegNum<[8]>;
def S1 : RISCXReg<"s1">, DwarfRegNum<[9]>;
def A0 : RISCXReg<"a0">, DwarfRegNum<[10]>;
def A1 : RISCXReg<"a1">, DwarfRegNum<[11]>;
def A2 : RISCXReg<"a2">, DwarfRegNum<[12]>;

def GPR: RegisterClass<"RISCX", [i32], 32, (add ZERO, RA, SP, A0, A1, A2, T0, T1, S0, S1)>;
