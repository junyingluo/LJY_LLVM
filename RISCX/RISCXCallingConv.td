def RetCC_RISCX: CallingConv<[
    CCIfType<[i32], CCAssignToReg<[A0, A1]>>,
]>;

// def CC_RISCX: CallingConv<[
//     CCIfType<[i32], CCAssignToReg<[A0, A1, A2]>>,
//     CCAssignToStack<4, 4>
// ]>;

def CSR: CalleeSavedRegs<(add S0, S1, RA)>;