#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-non-const-parameter"

#include "W65C02.h"

const int NEGATIVE_FLAG = 0b10000000;
const int OVERFLOW_FLAG =  0b01000000;
const int UNDEFINED_FLAG =  0b00100000;
const int BREAK_FLAG = 0b00010000;
const int DECIMAL_FLAG =  0b00001000;
const int INTERRUPT_FLAG =  0b00000100;
const int ZERO_FLAG =  0b00000010;
const int CARRY_FLAG =  0b00000001;

W65C02 cpu;
Opcode opcode;
Opcode opcodeMatrix[256] = {
        {BRK, stackB    }, {ORA, zeroPageIndexedIndirect }, {XXX, immediate         }, {XXX, immediate }, {TSB, zeroPageB   }, {ORA, zeroPageA  }, {ASL, zeroPageB  }, {RMB0, zeroPageC }, {PHP, stackE      }, {ORA, immediate  }, {ASL, accumulator    }, {XXX, immediate  }, {TSB, absoluteB                  }, {ORA, absoluteA  }, {ASL, absoluteB  }, {BBR0, relativeB },
        {BPL, relativeA }, {ORA, zeroPageIndirectIndexed }, {ORA, zeroPageIndirect  }, {XXX, immediate }, {TRB, zeroPageB   }, {ORA, zeroPageXA }, {ASL, zeroPageXB }, {RMB1, zeroPageC }, {CLC, impliedA    }, {ORA, absoluteY  }, {INC, accumulator    }, {XXX, immediate  }, {TRB, absoluteB                  }, {ORA, absoluteXA }, {ASL, absoluteXB }, {BBR1, relativeB },
        {JSR, absoluteD }, {AND, zeroPageIndexedIndirect }, {XXX, immediate         }, {XXX, immediate }, {BIT, zeroPageA   }, {AND, zeroPageA  }, {ROL, zeroPageB  }, {RMB2, zeroPageC }, {PLP, stackF      }, {AND, immediate  }, {ROL, accumulator    }, {XXX, immediate  }, {BIT, absoluteA                  }, {AND, absoluteA  }, {ROL, absoluteB  }, {BBR2, relativeB },
        {BMI, relativeA }, {AND, zeroPageIndirectIndexed }, {AND, zeroPageIndirect  }, {XXX, immediate }, {BIT, zeroPageXA  }, {AND, zeroPageXA }, {ROL, zeroPageXB }, {RMB3, zeroPageC }, {SEC, impliedA    }, {AND, absoluteY  }, {DEC, accumulator    }, {XXX, immediate  }, {BIT, absoluteXA                 }, {AND, absoluteXA }, {ROL, absoluteXB }, {BBR3, relativeB },
        {RTI, stackC    }, {EOR, zeroPageIndexedIndirect }, {XXX, immediate         }, {XXX, immediate }, {XXX, immediate   }, {EOR, zeroPageA  }, {LSR, zeroPageB  }, {RMB4, zeroPageC }, {PHA, stackE      }, {EOR, immediate  }, {LSR, accumulator    }, {XXX, immediate  }, {JMP, absoluteC                  }, {EOR, absoluteA  }, {LSR, absoluteB  }, {BBR4, relativeB },
        {BVC, relativeA }, {EOR, zeroPageIndirectIndexed }, {EOR, zeroPageIndirect  }, {XXX, immediate }, {XXX, immediate   }, {EOR, zeroPageXA }, {LSR, zeroPageXB }, {RMB5, zeroPageC }, {CLI, impliedA    }, {EOR, absoluteY  }, {PHY, stackE         }, {XXX, immediate  }, {XXX, immediate                  }, {EOR, absoluteXA }, {LSR, absoluteXB }, {BBR5, relativeB },
        {RTS, stackD    }, {ADC, zeroPageIndexedIndirect }, {XXX, immediate         }, {XXX, immediate }, {STZ, zeroPageA   }, {ADC, zeroPageA  }, {ROR, zeroPageB  }, {RMB6, zeroPageC }, {PLA, stackF      }, {ADC, immediate  }, {ROR, accumulator    }, {XXX, immediate  }, {JMP, absoluteIndirect           }, {ADC, absoluteA  }, {ROR, absoluteB  }, {BBR6, relativeB },
        {BVS, relativeA }, {ADC, zeroPageIndirectIndexed }, {ADC, zeroPageIndirect  }, {XXX, immediate }, {STZ, zeroPageXA  }, {ADC, zeroPageXA }, {ROR, zeroPageXB }, {RMB7, zeroPageC }, {SEI, impliedA    }, {ADC, absoluteY  }, {PLY, stackF         }, {XXX, immediate  }, {JMP, absoluteIndexedIndirect    }, {ADC, absoluteXA }, {ROR, absoluteXB }, {BBR7, relativeB },
        {BRA, relativeA }, {STA, zeroPageIndexedIndirect }, {XXX, immediate         }, {XXX, immediate }, {STY, zeroPageA   }, {STA, zeroPageA  }, {STX, zeroPageA  }, {SMB0, zeroPageC }, {DEY, impliedA    }, {BIT, immediate  }, {TXA, impliedA      }, {XXX, immediate  }, {STY, absoluteA                  }, {STA, absoluteA  }, {STX, absoluteA  }, {BBS0, relativeB},
        {BCC, relativeA }, {STA, zeroPageIndirectIndexed }, {STA, zeroPageIndirect  }, {XXX, immediate }, {STY, zeroPageXA  }, {STA, zeroPageXA }, {STX, zeroPageY  }, {SMB1, zeroPageC }, {TYA, impliedA    }, {STA, absoluteY  }, {TXS, impliedA       }, {XXX, immediate  }, {STZ, absoluteA                  }, {STA, absoluteXA }, {STZ, absoluteXA }, {BBS1, relativeB },
        {LDY, immediate }, {LDA, zeroPageIndexedIndirect }, {LDX, immediate         }, {XXX, immediate }, {LDY, zeroPageA   }, {LDA, zeroPageA  }, {LDX, zeroPageA  }, {SMB2, zeroPageC }, {TAY, impliedA    }, {LDA, immediate  }, {TAX, impliedA      }, {XXX, immediate  }, {LDY, absoluteA                  }, {LDA, absoluteA  }, {LDX, absoluteA  }, {BBS2, relativeB },
        {BCS, relativeA }, {LDA, zeroPageIndirectIndexed }, {LDA, zeroPageIndirect  }, {XXX, immediate }, {LDY, zeroPageXA  }, {LDA, zeroPageXA }, {LDX, zeroPageY  }, {SMB3, zeroPageC }, {CLV, impliedA    }, {LDA, absoluteY  }, {TSX, impliedA       }, {XXX, immediate  }, {LDY, absoluteXA                 }, {LDA, absoluteXA }, {LDX, absoluteY  }, {BBS3, relativeB },
        {CPY, immediate }, {CMP, zeroPageIndexedIndirect }, {XXX, zeroPageIndirect  }, {XXX, immediate }, {CPY, zeroPageA   }, {CMP, zeroPageA  }, {DEC, zeroPageB  }, {SMB4, zeroPageC }, {INY, impliedA    }, {CMP, immediate  }, {DEX, impliedA       }, {WAI, impliedB  }, {CPY, absoluteA                  }, {CMP, absoluteA  }, {DEC, absoluteB  }, {BBS4, relativeB },
        {BNE, relativeA }, {CMP, zeroPageIndirectIndexed }, {CMP, zeroPageIndirect  }, {XXX, immediate }, {XXX, immediate   }, {CMP, zeroPageXA }, {DEC, zeroPageXB }, {SMB5, zeroPageC }, {CLD, impliedA    }, {CMP, absoluteY  }, {PHX, stackE         }, {STP, immediate  }, {XXX, immediate                  }, {CMP, absoluteXA }, {DEC, absoluteXB }, {BBS5, relativeB },
        {CPX, immediate }, {SBC, zeroPageIndexedIndirect }, {XXX, immediate         }, {XXX, immediate }, {CPX, zeroPageA   }, {SBC, zeroPageA  }, {INC, zeroPageB  }, {SMB6, zeroPageC }, {INX, impliedA    }, {SBC, immediate  }, {NOP, impliedA       }, {XXX, immediate  }, {CPX, absoluteA                  }, {SBC, absoluteA  }, {INC, absoluteB  }, {BBS6, relativeB },
        {BEQ, relativeA }, {SBC, zeroPageIndirectIndexed }, {SBC, zeroPageIndirect  }, {XXX, immediate }, {XXX, immediate   }, {SBC, zeroPageXA }, {INC, zeroPageXB }, {SMB7, zeroPageC }, {SED, impliedA    }, {SBC, absoluteY  }, {PLX, stackF         }, {XXX, immediate  }, {XXX, immediate                  }, {SBC, absoluteXA }, {INC, absoluteXB }, {BBS7, relativeB }
};

void reset(word address) {
    cpu.PC = address;
    cpu.SP = 0xFF;
    cpu.PS = cpu.A = cpu.X = cpu.Y = cpu.STOP = cpu.WAIT = 0;
    setStatusFlags(UNDEFINED_FLAG, 1);
    setStatusFlags(BREAK_FLAG, 1);
    setStatusFlags(INTERRUPT_FLAG, 1);
}

void interruptRequest() {
    cpu.WAIT = 0;
    if(testStatusFlags(INTERRUPT_FLAG)) return;
    setStatusFlags(BREAK_FLAG, 0);
    readByte(cpu.PC);
    readByte(cpu.PC);
    pushWordToStack(cpu.PC);
    pushByteToStack(cpu.PS);
    cpu.PC = readByte(0xFFFE) | readByte(0xFFFF) << 8;
    setStatusFlags(INTERRUPT_FLAG, 1);
    setStatusFlags(DECIMAL_FLAG, 0);
}

void nonMaskableInterrupt() {
    cpu.WAIT = 0;
    readByte(cpu.PC);
    readByte(cpu.PC);
    pushWordToStack(cpu.PC);
    pushByteToStack(cpu.PS);
    cpu.PC = readByte(0xFFFA) | readByte(0xFFFB) << 8;
    setStatusFlags(INTERRUPT_FLAG, 1);
    setStatusFlags(DECIMAL_FLAG, 0);
}

void loadRegister(byte* reg, byte value) {
    *reg = value;
    NZSetStatus(value);
}

void NZSetStatus(byte value) {
    setStatusFlags(ZERO_FLAG, value == 0);
    setStatusFlags(NEGATIVE_FLAG, value >> SIGN_BIT_POS);
}

void setStatusFlags(byte flag, byte status) {
    if(status) //if we want to set the flag to on
        cpu.PS |= flag;
    else
        cpu.PS &= ~flag;
}

byte testStatusFlags(byte flag) {
    if(cpu.PS & flag) return 1;
    return 0;
}

byte readByte(word address) {
    incCycles();
    return readBus(address);
}

byte fetchByte() {
    return readByte(cpu.PC++);
}

word fetchWord() {
    word data = fetchByte();
    return data | (fetchByte() << 8);
}

void writeByte(byte data, word address) {
    writeBus(address, data);
    incCycles();
}

void pushByteToStack(byte data) {
    writeByte(data, SPToAddress());
    --cpu.SP;
}

void pushWordToStack(word data) {
    pushByteToStack((data & 0xFF00) >> 8);
    return pushByteToStack(data);
}

byte pullByteFromStack() {
    byte data = readByte(SPToAddress());
    ++cpu.SP;
    return data;
}

byte readByteFromStack() {
    return readByte(SPToAddress());
}

word SPToAddress() {
    return 0x100 | cpu.SP;
}

word immediate(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    return fetchByte();
}

word absoluteA(byte* reg, __attribute__((unused)) operation op) {
    word address = fetchWord();
    if(reg) {
        writeByte(*reg, address);
        return 0;
    } return readByte(address);
}

word absoluteB(__attribute__((unused)) byte* reg, operation op) {
    word address = fetchWord();
    byte data = readByte(address);
    readByte(address);
    writeByte(op(data), address);
    return 0;
}

word absoluteC(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    return fetchWord();
}

word absoluteD(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    byte subAddrLow = fetchByte();
    readByte(SPToAddress());
    pushWordToStack(cpu.PC);
    return (fetchByte() << 8) | subAddrLow;
}

word zeroPageA(byte* reg, __attribute__((unused)) operation op) {
    byte zeroPage = fetchByte();
    if(reg) {
        writeByte(*reg, zeroPage);
        return 0;
    } return readByte(zeroPage);
}

word zeroPageB(__attribute__((unused)) byte* reg, operation op) {
    byte zeroPage = fetchByte();
    byte data = readByte(zeroPage);
    readByte(zeroPage);
    writeByte(op(data), zeroPage);
    return 0;
}

word zeroPageC(__attribute__((unused)) byte* reg, operation op) {
    byte zeroPage = fetchByte();
    byte data = readByte(zeroPage);
    readByte(zeroPage);
    writeByte(op(data), zeroPage);
    return 0;
}

word accumulator(byte* reg, operation op) {
    *reg = op(*reg);
    return readByte(cpu.PC);
}

word impliedA(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    return readByte(cpu.PC);
}

word impliedB(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    readByte(cpu.PC);
    readByte(cpu.PC);
    return 0;
}

word impliedC(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    readByte(cpu.PC);
    readByte(cpu.PC);
    return 0;
}

word zeroPageIndirectIndexed(byte* reg, __attribute__((unused)) operation op) {
    byte zpAddress = fetchByte();
    word address = readByte(zpAddress) | (readByte((byte)(zpAddress + 1)) << 8);
    if(((address & 0xFF) + cpu.Y) > 0xFF && !reg)
        readByte((byte)(zpAddress + 1));
    if(reg) {
        readByte((byte)(zpAddress + 1));
        writeByte(*reg, address + cpu.Y);
        return 0;
    } return readByte(address + cpu.Y);
}

word zeroPageIndexedIndirect(byte* reg, __attribute__((unused)) operation op) {
    byte zeroPage = readByte(cpu.PC);
    fetchByte();
    word effectiveAddress = readByte((byte)(zeroPage + cpu.X)) | (readByte((byte)(zeroPage + cpu.X + 1)) << 8);
    if(reg) {
        writeByte(*reg, effectiveAddress);
        return 0;
    } return readByte(effectiveAddress);
}

word zeroPageXA(byte* reg, __attribute__((unused)) operation op) {
    byte address = fetchByte();
    byte effectiveAddress = address + cpu.X;
    readByte(cpu.PC - 1);
    if(reg) {
        writeByte(*reg, effectiveAddress);
        return 0;
    } return readByte(effectiveAddress);
}

word zeroPageXB(__attribute__((unused)) byte* reg, operation op) {
    byte zeroPage = readByte(cpu.PC);
    byte effectiveAddress = zeroPage + cpu.X;
    fetchByte();
    byte data = readByte(effectiveAddress);
    readByte(effectiveAddress);
    writeByte(op(data), effectiveAddress);
    return 0;
}

//TODO - we need to fix this function
word zeroPageY(byte* reg, __attribute__((unused)) operation op) {
    byte address = fetchByte();
    byte effectiveAddress = address + cpu.Y;
    readByte(cpu.PC - 1);
    //cycles += (address + Y > 0xFF);
    if(reg) {
        writeByte(*reg, effectiveAddress);
        return 0;
    } else return readByte(effectiveAddress);
}

word absoluteXA(byte* reg, __attribute__((unused)) operation op) {
    word address = fetchWord();
    dword effectiveAddress = address + cpu.X;
    if(reg) {
        if(((address & 0xFF) + cpu.X) > 0xFF) {
            readByte(cpu.PC - 1);
            writeByte(*reg, effectiveAddress);
        } else {
            readByte(effectiveAddress);
            writeByte(*reg, effectiveAddress);
        }
    }
    else if(((address & 0xFF) + cpu.X) > 0xFF) {
        readByte(cpu.PC - 1);
        return readByte(effectiveAddress);
    } else return readByte(effectiveAddress);
    return 0;
}

word absoluteXB(__attribute__((unused)) byte* reg, operation op) {
    instruction ins = opcode.instruction;
    word address = fetchWord();
    word effectiveAddress = address + cpu.X;
    if(((address & 0xFF) + cpu.X) > 0xFF)
        readByte(cpu.PC - 1);
    else if(ins != &ASL && ins != &LSR && ins != &ROL && ins != &ROR)
        readByte(effectiveAddress);
    readByte(effectiveAddress);
    byte data = readByte(effectiveAddress);
    writeByte(op(data), effectiveAddress);
    return 0;
}

word absoluteY(byte* reg, __attribute__((unused)) operation op) {
    word address = fetchWord();
    dword effectiveAddress = address + cpu.Y;
    if(reg) {
        if(((address & 0xFF) + cpu.Y) > 0xFF) {
            readByte(cpu.PC - 1);
            writeByte(*reg, effectiveAddress);
        } else {
            readByte(effectiveAddress);
            writeByte(*reg, effectiveAddress);
        }
    }
    else if(((address & 0xFF) + cpu.Y) > 0xFF) {
        readByte(cpu.PC - 1);
        return readByte(effectiveAddress);
    } else return readByte(effectiveAddress);
    return 0;
}

word relativeA(__attribute__((unused)) byte* reg, operation op) {
    byte value = fetchByte();
    if(op(0)) {     //Then we should branch. the value passed to op is garbage. It isn't needed.
        readByte(cpu.PC);
        sbyte svalue = (sbyte)value;
        if((cpu.PC & 0xFF) + svalue < 0 || (cpu.PC & 0xFF) + svalue > 0xFF)
            readByte(cpu.PC);
        return cpu.PC + svalue;
    }
    return cpu.PC;
}

word relativeB(__attribute__((unused)) byte* reg, operation op) {
    byte value = fetchByte();
    byte zeroPage= fetchByte();
    readByte(zeroPage);
    readByte(zeroPage);
    readByte(zeroPage);
    if(op(0)) {     //Then we should branch. the value passed to op is garbage. It isn't needed.
        readByte(cpu.PC);
        sbyte svalue = (sbyte)value;
        if((cpu.PC & 0xFF) + svalue < 0 || (cpu.PC & 0xFF) + svalue > 0xFF)
            readByte(cpu.PC);
        return cpu.PC + svalue;
    }
    return cpu.PC;
}

word absoluteIndirect(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    word address = fetchWord();
    readByte(--cpu.PC);
    return readByte(address) | (readByte(address + 1) << 8);
}

word stackA(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    readByte(cpu.PC);
    readByte(cpu.PC);
    pushWordToStack(SPToAddress());
    return 0;
}

word stackB(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    fetchByte();
    pushWordToStack(cpu.PC);
    setStatusFlags(BREAK_FLAG, 1);
    pushByteToStack(cpu.PS);
    setStatusFlags(INTERRUPT_FLAG, 1);
    return readByte(0xFFFE) | readByte(0xFFFF) << 8;
}

word stackC(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    pullByteFromStack();
    cpu.PS = pullByteFromStack();
    byte PCL = pullByteFromStack();
    byte PCH = readByteFromStack();
    return (PCH << 8) | PCL;
}

word stackD(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    readByte(cpu.PC);
    pullByteFromStack();
    byte PCL = pullByteFromStack();
    byte PCH = readByteFromStack();
    cpu.PC = (PCH << 8) | PCL;
    readByte(cpu.PC);
    return cpu.PC + 1;
}

word stackE(byte* reg, __attribute__((unused)) operation op) {
    readByte(cpu.PC);
    pushByteToStack(*reg);
    return 0;
}

word stackF(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    readByte(cpu.PC);
    pullByteFromStack();
    return readByte(SPToAddress());
}

word absoluteIndexedIndirect(__attribute__((unused)) byte* reg, __attribute__((unused)) operation op) {
    word address = fetchWord();
    readByte(--cpu.PC);
    return readByte(address + cpu.X) | (readByte(address + cpu.X + 1) << 8);
}

word zeroPageIndirect(byte* reg, __attribute__((unused)) operation op) {
    byte ZPAddr = fetchByte();
    word effectiveAddress = readByte(ZPAddr) | (readByte((byte)(ZPAddr + 1)) << 8);
    if(reg) {
        writeByte(*reg, effectiveAddress);
        return 0;
    } else return readByte(effectiveAddress);
}

void execute(qword numInstructionsToExecute) {
    while(numInstructionsToExecute--) {
        if(cpu.STOP) continue;
        if(cpu.IRQB) interruptRequest();
        if(cpu.NMIB) {
            nonMaskableInterrupt();
            cpu.NMIB = 0;
        }
        if(cpu.WAIT) continue;
        opcode = opcodeMatrix[fetchByte()];
        opcode.instruction(opcode.addressMode);
    }
}

void ADC(addressMode addrMode) {
    byte value = addrMode(NULL, NULL);
    word result = (word)cpu.A + (word)value + (word) testStatusFlags(CARRY_FLAG);
    byte SB1 = cpu.A >> SIGN_BIT_POS, SB2 = value >> SIGN_BIT_POS, SBR = (result & SIGN_BIT_MASK) >> SIGN_BIT_POS;
    setStatusFlags(OVERFLOW_FLAG, (!(SB1 ^ SB2) && (SB2 ^ SBR)) ? 1 : 0);
    setStatusFlags(CARRY_FLAG, (result >> CARRY_BIT_POS) ? 1 : 0);
    loadRegister(&cpu.A, (byte)(result & MAX_BYTE));
}

void AND(addressMode addrMode) {
    loadRegister(&cpu.A, cpu.A & addrMode(NULL, NULL));
}

byte opASL(byte a) {
    setStatusFlags(CARRY_FLAG, (a & 0x80) ? 1 : 0);
    NZSetStatus(a << 1);
    return a << 1;
}

void ASL(addressMode addrMode) {
    addrMode(&cpu.A, opASL);
}

byte opBBR0(byte data) {return !(data & 0b00000001);}
byte opBBR1(byte data) {return !(data & 0b00000010);}
byte opBBR2(byte data) {return !(data & 0b00000100);}
byte opBBR3(byte data) {return !(data & 0b00001000);}
byte opBBR4(byte data) {return !(data & 0b00010000);}
byte opBBR5(byte data) {return !(data & 0b00100000);}
byte opBBR6(byte data) {return !(data & 0b01000000);}
byte opBBR7(byte data) {return !(data & 0b10000000);}

void BBR0(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBR0);
}

void BBR1(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBR1);
}

void BBR2(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBR2);
}

void BBR3(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBR3);
}

void BBR4(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBR4);
}

void BBR5(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBR5);
}

void BBR6(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBR6);
}

void BBR7(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBR7);
}

byte opBBS0(byte data) {return (data & 0b00000001);}
byte opBBS1(byte data) {return (data & 0b00000010);}
byte opBBS2(byte data) {return (data & 0b00000100);}
byte opBBS3(byte data) {return (data & 0b00001000);}
byte opBBS4(byte data) {return (data & 0b00010000);}
byte opBBS5(byte data) {return (data & 0b00100000);}
byte opBBS6(byte data) {return (data & 0b01000000);}
byte opBBS7(byte data) {return (data & 0b10000000);}

void BBS0(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBS0);
}

void BBS1(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBS1);
}

void BBS2(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBS2);
}

void BBS3(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBS3);
}

void BBS4(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBS4);
}

void BBS5(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBS5);
}

void BBS6(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBS6);
}

void BBS7(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBBS7);
}

byte opBCC(__attribute__((unused)) byte data) {return !testStatusFlags(CARRY_FLAG);}

void BCC(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBCC);
}

byte opBCS(__attribute__((unused)) byte data) {return testStatusFlags(CARRY_FLAG) ? 1 : 0;}

void BCS(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBCS);
}

byte opBEQ(__attribute__((unused)) byte data) {return testStatusFlags(ZERO_FLAG);}

void BEQ(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBEQ);
}

void BIT(addressMode addrMode) {
    byte memVal = addrMode(NULL, NULL);
    byte result = cpu.A & memVal;
    setStatusFlags(ZERO_FLAG, result == 0);
    if(addrMode != &immediate) {
        setStatusFlags(NEGATIVE_FLAG, (memVal & 0x80) ? 1 : 0);
        setStatusFlags(OVERFLOW_FLAG, (memVal & 0x40) ? 1 : 0);
    }
}

byte opBMI(__attribute__((unused)) byte val) {return testStatusFlags(NEGATIVE_FLAG);}

void BMI(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBMI);
}

byte opBNE(__attribute__((unused)) byte val) {return !testStatusFlags(ZERO_FLAG);}

void BNE(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBNE);
}

byte opBPL(__attribute__((unused)) byte val) {return !testStatusFlags(NEGATIVE_FLAG);}

void BPL(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBPL);
}

byte opBRA(__attribute__((unused)) byte val) {return 1;}

void BRA(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBRA);
}

void BRK(addressMode addrMode) {
    cpu.PC = addrMode(NULL, NULL);
}

byte opBVC(__attribute__((unused)) byte val) {return !testStatusFlags(OVERFLOW_FLAG);}

void BVC(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBVC);
}

byte opBVS(__attribute__((unused)) byte val) {return testStatusFlags(OVERFLOW_FLAG);}

void BVS(addressMode addrMode) {
    cpu.PC = addrMode(NULL, opBVS);
}

void CLC(addressMode addrMode) {
    addrMode(NULL, NULL);
    setStatusFlags(CARRY_FLAG, 0);
}

void CLD(addressMode addrMode) {
    addrMode(NULL, NULL);
    setStatusFlags(DECIMAL_FLAG, 0);
}

void CLI(addressMode addrMode) {
    addrMode(NULL, NULL);
    setStatusFlags(INTERRUPT_FLAG, 0);
}

void CLV(addressMode addrMode) {
    addrMode(NULL, NULL);
    setStatusFlags(OVERFLOW_FLAG, 0);
}

void CMP(addressMode addrMode) {
    byte value = addrMode(NULL, NULL);
    setStatusFlags(CARRY_FLAG, cpu.A >= value);
    setStatusFlags(ZERO_FLAG, cpu.A == value);
    setStatusFlags(NEGATIVE_FLAG, (byte)(cpu.A - value) >> 7);
}

void CPX(addressMode addrMode) {
    byte value = addrMode(NULL, NULL);
    setStatusFlags(CARRY_FLAG, cpu.X >= value);
    setStatusFlags(ZERO_FLAG, cpu.X == value);
    setStatusFlags(NEGATIVE_FLAG, (byte)(cpu.X - value) >> 7);
}

void CPY(addressMode addrMode) {
    byte value = addrMode(NULL, NULL);
    setStatusFlags(CARRY_FLAG, cpu.Y >= value);
    setStatusFlags(ZERO_FLAG, cpu.Y == value);
    setStatusFlags(NEGATIVE_FLAG, (byte)(cpu.Y - value) >> 7);
}

byte opDEC(byte a) {
    NZSetStatus(a - 1);
    return a - 1;
}

void DEC(addressMode addrMode) {
    addrMode(&cpu.A, opDEC);
}

void DEX(addressMode addrMode) {
    addrMode(NULL, NULL);
    loadRegister(&cpu.X, cpu.X - 1);
}

void DEY(addressMode addrMode) {
    addrMode(NULL, NULL);
    loadRegister(&cpu.Y, cpu.Y - 1);
}

void EOR(addressMode addrMode) {
    loadRegister(&cpu.A, cpu.A & addrMode(NULL, NULL));
}

byte opINC(byte a) {
    NZSetStatus(a + 1);
    return a + 1;
}

void INC(addressMode addrMode) {
    addrMode(&cpu.A, opINC);
}

void INX(addressMode addrMode) {
    addrMode(NULL, NULL);
    loadRegister(&cpu.X, cpu.X + 1);
}

void INY(addressMode addrMode) {
    addrMode(NULL, NULL);
    loadRegister(&cpu.Y, cpu.Y + 1);
}

void JMP(addressMode addrMode) {
    cpu.PC = addrMode(NULL, NULL);
}

void JSR(addressMode addrMode) {
    cpu.PC = addrMode(NULL, NULL);
}

void LDA(addressMode addrMode) {
    loadRegister(&cpu.A, addrMode(NULL, NULL));
}

void LDX(addressMode addrMode) {
    loadRegister(&cpu.X, addrMode(NULL, NULL));
}

void LDY(addressMode addrMode) {
    loadRegister(&cpu.Y, addrMode(NULL, NULL));
}

byte opLSR(byte a) {
    setStatusFlags(CARRY_FLAG, a & 0x1);
    NZSetStatus(a >> 1);
    return a >> 1;
}

void LSR(addressMode addrMode) {
    addrMode(&cpu.A, opLSR);
}

void NOP(addressMode addrMode) {
    addrMode(NULL, NULL);
}

void ORA(addressMode addrMode) {
    loadRegister(&cpu.A, cpu.A | addrMode(NULL, NULL));
}

void PHA(addressMode addrMode) {
    addrMode(&cpu.A, NULL);
}

void PHP(addressMode addrMode) {
    setStatusFlags(BREAK_FLAG, 1);
    addrMode(&cpu.PS, NULL);
}

void PHX(addressMode addrMode) {
    addrMode(&cpu.A, NULL);
}

void PHY(addressMode addrMode) {
    addrMode(&cpu.Y, NULL);
}

void PLA(addressMode addrMode) {
    loadRegister(&cpu.A, addrMode(NULL, NULL));
}

void PLP(addressMode addrMode) {
    cpu.PS = addrMode(NULL, NULL);
}

void PLX(addressMode addrMode) {
    loadRegister(&cpu.X, addrMode(NULL, NULL));
}

void PLY(addressMode addrMode) {
    loadRegister(&cpu.Y, addrMode(NULL, NULL));
}

byte opRMB0(byte a) {return a & 0b11111110;}
byte opRMB1(byte a) {return a & 0b11111101;}
byte opRMB2(byte a) {return a & 0b11111011;}
byte opRMB3(byte a) {return a & 0b11110111;}
byte opRMB4(byte a) {return a & 0b11101111;}
byte opRMB5(byte a) {return a & 0b11011111;}
byte opRMB6(byte a) {return a & 0b10111111;}
byte opRMB7(byte a) {return a & 0b01111111;}

void RMB0(addressMode addrMode) {
    addrMode(NULL, opRMB0);
}

void RMB1(addressMode addrMode) {
    addrMode(NULL, opRMB1);
}

void RMB2(addressMode addrMode) {
    addrMode(NULL, opRMB2);
}

void RMB3(addressMode addrMode) {
    addrMode(NULL, opRMB3);
}

void RMB4(addressMode addrMode) {
    addrMode(NULL, opRMB4);
}

void RMB5(addressMode addrMode) {
    addrMode(NULL, opRMB5);
}

void RMB6(addressMode addrMode) {
    addrMode(NULL, opRMB6);
}

void RMB7(addressMode addrMode) {
    addrMode(NULL, opRMB7);
}

byte opROL(byte a) {
    byte result = (a << 1) | testStatusFlags(CARRY_FLAG);
    setStatusFlags(CARRY_FLAG, a & 0x80);
    NZSetStatus(result);
    return result;
}

void ROL(addressMode addrMode) {
    addrMode(&cpu.A, opROL);
}

byte opROR(byte a) {
    byte result = (a >> 1) | testStatusFlags(CARRY_FLAG << SIGN_BIT_POS);
    setStatusFlags(CARRY_FLAG, a & 0x01);
    NZSetStatus(result);
    return result;
}

void ROR(addressMode addrMode) {
    addrMode(&cpu.A, opROR);
}

void RTI(addressMode addrMode) {
    cpu.PC = addrMode(NULL, NULL);
}

void RTS(addressMode addrMode) {
    cpu.PC = addrMode(NULL, NULL);
}

void SBC(addressMode addrMode) {
    byte value = addrMode(NULL, NULL);
    word result = (word)cpu.A + (word)~value + (word) testStatusFlags(CARRY_FLAG);
    byte SB1 = cpu.A >> SIGN_BIT_POS, SB2 = value >> SIGN_BIT_POS, SBR = (result & SIGN_BIT_MASK) >> SIGN_BIT_POS;
    setStatusFlags(OVERFLOW_FLAG, (SB1 ^ SB2) && !(SB2 ^ SBR));
    setStatusFlags(CARRY_FLAG, !(result >> CARRY_BIT_POS));
    loadRegister(&cpu.A, (byte)(result & MAX_BYTE));
}

void SEC(addressMode addrMode) {
    addrMode(NULL, NULL);
    setStatusFlags(CARRY_FLAG, 1);
}

void SED(addressMode addrMode) {
    addrMode(NULL, NULL);
    setStatusFlags(DECIMAL_FLAG, 1);
}

void SEI(addressMode addrMode) {
    addrMode(NULL, NULL);
    setStatusFlags(INTERRUPT_FLAG, 1);
}

byte opSMB0(byte a) {return a | 0b00000001;}
byte opSMB1(byte a) {return a | 0b00000010;}
byte opSMB2(byte a) {return a | 0b00000100;}
byte opSMB3(byte a) {return a | 0b00001000;}
byte opSMB4(byte a) {return a | 0b00010000;}
byte opSMB5(byte a) {return a | 0b00100000;}
byte opSMB6(byte a) {return a | 0b01000000;}
byte opSMB7(byte a) {return a | 0b10000000;}

void SMB0(addressMode addrMode) {
    addrMode(NULL, opSMB0);
}

void SMB1(addressMode addrMode) {
    addrMode(NULL, opSMB1);
}

void SMB2(addressMode addrMode) {
    addrMode(NULL, opSMB2);
}

void SMB3(addressMode addrMode) {
    addrMode(NULL, opSMB3);
}

void SMB4(addressMode addrMode) {
    addrMode(NULL, opSMB4);
}

void SMB5(addressMode addrMode) {
    addrMode(NULL, opSMB5);
}

void SMB6(addressMode addrMode) {
    addrMode(NULL, opSMB6);
}

void SMB7(addressMode addrMode) {
    addrMode(NULL, opSMB7);
}

void STA(addressMode addrMode) {
    addrMode(&cpu.A, NULL);
}

void STP(addressMode addrMode) {
    addrMode(NULL, NULL);
    cpu.STOP = 1;
}

void STX(addressMode addrMode) {
    addrMode(&cpu.X, NULL);
}

void STY(addressMode addrMode) {
    addrMode(&cpu.Y, NULL);
}

void STZ(addressMode addrMode) {
    byte zero = 0;
    addrMode(&zero, NULL);
}

void TAX(addressMode addrMode) {
    addrMode(NULL, NULL);
    loadRegister(&cpu.X, cpu.A);
}

void TAY(addressMode addrMode) {
    addrMode(NULL, NULL);
    loadRegister(&cpu.Y, cpu.A);
}

byte opTRB(byte a) {
    setStatusFlags(ZERO_FLAG, (cpu.A & a) == 0);
    return ~cpu.A & a;
}

void TRB(addressMode addrMode) {
    addrMode(&cpu.A, opTRB);
}

byte opTSB(byte a) {
    setStatusFlags(ZERO_FLAG, (cpu.A & a) == 0);
    return cpu.A | a;
}

void TSB(addressMode addrMode) {
    addrMode(&cpu.A, opTSB);
}

void TSX(addressMode addrMode) {
    addrMode(NULL, NULL);
    loadRegister(&cpu.X, cpu.SP);
}

void TXA(addressMode addrMode) {
    addrMode(NULL, NULL);
    loadRegister(&cpu.A, cpu.X);
}

void TXS(addressMode addrMode) {
    addrMode(NULL, NULL);
    cpu.SP = cpu.X;
}

void TYA(addressMode addrMode) {
    addrMode(NULL, NULL);
    loadRegister(&cpu.A, cpu.Y);
}

void WAI(addressMode addrMode) {
    addrMode(NULL, NULL);
    cpu.WAIT = 1;
}

void XXX(addressMode addrMode) {
    NOP(addrMode);
}

#pragma clang diagnostic pop