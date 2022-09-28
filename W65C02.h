#ifndef CPROJECT_CPU_H
#define CPROJECT_CPU_H

#include <stddef.h>
#include "system_types.h"
#include "cycles.h"
#include "bus.h"

#define SIGN_BIT_POS 7
#define CARRY_BIT_POS 8
#define OVERFLOW_BIT_POS 6
#define OVERFLOW_BIT_MASK 0x40
#define SIGN_BIT_MASK 0x80
#define MAX_BYTE 0XFF

typedef byte (*operation)(byte);
typedef word (*addressMode)(byte*, operation);
typedef void (*instruction)(addressMode);

typedef struct {
    word PC;
    byte SP;
    byte A, X, Y;
    byte IRQB, NMIB, STOP, WAIT, IRQB_COMPLETED;
    byte PS;
} W65C02;

extern W65C02 cpu;

typedef struct {
    instruction instruction;
    addressMode addressMode;
} Opcode;

void reset(word address);
void interruptRequest();
void nonMaskableInterrupt();

void loadRegister(byte*, byte);
void setStatusFlags(byte, byte);
byte testStatusFlags(byte);
void NZSetStatus(byte);

byte readByte(word);
byte fetchByte();
word fetchWord();
void writeByte(byte, word);
void pushByteToStack(byte);
void pushWordToStack(word);
byte pullByteFromStack();
byte readByteFromStack();
word SPToAddress();

word immediate(__attribute__((unused)) byte*, __attribute__((unused)) operation);
word absoluteA(byte*, __attribute__((unused)) operation);
word absoluteB(__attribute__((unused)) byte*, __attribute__((unused)) operation);
word absoluteC(__attribute__((unused)) byte*, __attribute__((unused)) operation);
word absoluteD(__attribute__((unused)) byte*, __attribute__((unused)) operation);
word zeroPageA(byte*, __attribute__((unused)) operation);
word zeroPageB(__attribute__((unused)) byte*, operation);
word zeroPageC(__attribute__((unused)) byte*, operation);
word accumulator(byte*, operation);
word impliedA(__attribute__((unused)) byte*, __attribute__((unused)) operation);
word impliedB(__attribute__((unused)) byte*, __attribute__((unused)) operation);
word impliedC(__attribute__((unused)) byte*, __attribute__((unused)) operation);
word zeroPageIndirectIndexed(byte*, __attribute__((unused)) operation);
word zeroPageIndexedIndirect(byte*, __attribute__((unused)) operation);
word zeroPageXA(byte*, operation);
word zeroPageXB(__attribute__((unused)) byte*, operation);
word zeroPageY(byte*, __attribute__((unused)) operation);
word absoluteXA(byte*, __attribute__((unused)) operation);
word absoluteXB(__attribute__((unused)) byte*, operation);
word absoluteY(byte*, operation);
word relativeA(__attribute__((unused)) byte*, operation);
word relativeB(__attribute__((unused)) byte*, operation);
word absoluteIndirect(__attribute__((unused)) byte*, __attribute__((unused)) operation);
word stackA(byte*, operation);
word stackB(byte*, operation);
word stackC(byte*, operation);
word stackD(byte*, operation);
word stackE(byte*, operation);
word stackF(byte*, operation);
word absoluteIndexedIndirect(__attribute__((unused)) byte*, __attribute__((unused)) operation);
word zeroPageIndirect(byte*, __attribute__((unused)) operation);

void execute(qword);

void ADC(addressMode);  void AND(addressMode);	void ASL(addressMode);	void BBR0(addressMode); void BBR1(addressMode); void BBR2(addressMode); void BBR3(addressMode); void BBR4(addressMode);
void BBR5(addressMode); void BBR6(addressMode); void BBR7(addressMode); void BBS0(addressMode); void BBS1(addressMode); void BBS2(addressMode); void BBS3(addressMode); void BBS4(addressMode);
void BBS5(addressMode); void BBS6(addressMode); void BBS7(addressMode); void BCC(addressMode);  void BCS(addressMode);	void BEQ(addressMode);  void BIT(addressMode);  void BMI(addressMode);
void BNE(addressMode);	void BPL(addressMode);  void BRA(addressMode);  void BRK(addressMode);  void BVC(addressMode);  void BVS(addressMode);  void CLC(addressMode);	void CLD(addressMode);
void CLI(addressMode);  void CLV(addressMode);  void CMP(addressMode);	void CPX(addressMode);  void CPY(addressMode);  void DEC(addressMode);  void DEX(addressMode);	void DEY(addressMode);
void EOR(addressMode);  void INC(addressMode);  void INX(addressMode);	void INY(addressMode);  void JMP(addressMode);  void JSR(addressMode);  void LDA(addressMode);	void LDX(addressMode);
void LDY(addressMode);  void LSR(addressMode);  void NOP(addressMode);	void ORA(addressMode);  void PHA(addressMode);  void PHP(addressMode);  void PHX(addressMode);  void PHY(addressMode);
void PLA(addressMode);	void PLP(addressMode);  void PLX(addressMode);  void PLY(addressMode);  void RMB0(addressMode); void RMB1(addressMode); void RMB2(addressMode); void RMB3(addressMode);
void RMB4(addressMode); void RMB5(addressMode); void RMB6(addressMode); void RMB7(addressMode); void ROL(addressMode);  void ROR(addressMode);  void RTI(addressMode);	void RTS(addressMode);
void SBC(addressMode);  void SEC(addressMode);  void SED(addressMode);	void SEI(addressMode);  void SMB0(addressMode); void SMB1(addressMode); void SMB2(addressMode); void SMB3(addressMode);
void SMB4(addressMode); void SMB5(addressMode); void SMB6(addressMode); void SMB7(addressMode); void STA(addressMode);  void STP(addressMode);  void STX(addressMode);	void STY(addressMode);
void STZ(addressMode);  void TAX(addressMode);  void TAY(addressMode);  void TRB(addressMode);  void TSB(addressMode);  void TSX(addressMode);  void TXA(addressMode);	void TXS(addressMode);
void TYA(addressMode);  void WAI(addressMode);  void XXX(addressMode);


#endif //CPROJECT_CPU_H
