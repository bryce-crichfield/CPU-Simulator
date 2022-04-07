#include <stdio.h>
#include <iostream>
#include "../inc/cpu.hpp"
#include "../inc/def.hpp"

// Read from the MAR, and store to the MDR
void CPU::Read()
{
    bus->RaiseReadFlag();
    bus->WriteAddressBus(mar);
    bus->Notify();
    mdr = bus->ReadDataBus();
}

// Write from the MAR and MDR-
void CPU::Write() 
{
    bus->RaiseWriteFlag();
    bus->WriteAddressBus(mar);
    bus->WriteDataBus(mdr);
    bus->Notify();
}

void CPU::Reset()
{
    a = b = c = 0;
    prc = sysconfig->PRG_START;
    mar = mdr = cir = 0;
}

void CPU::Cycle()
{
    Fetch();
    Decode();
    Execute();
    if (error) exit(EXIT_FAILURE);
}

void CPU::PrcInc()
{
    if (prc + 1 > sysconfig->PRG_END) {
        std::cout << "PRC Overflow Dumping Core" << std::endl;
        exit(-1);
    }
    prc += 1;
}

void CPU::Fetch() 
{
    mar = prc;
    PrcInc();
    Read();
    cir = mdr;
}

void ControlUnit::Decode()
{
    // Check the Addressing Mode
    switch ((0b11 << 14) & cir)
    {
        case 0b00: am = &CPU::IMM; break;
        case 0b01: am = &CPU::DIR; break;
        case 0b10: am = &CPU::IND; break;
    }
    // Check the Operation Code
    switch((0b1111 << 10) & cir)
    {
        case 0b0000: 
    }
    switch (cir)
    {
        case 
    // Load
    // case 0x00: am =  &CPU::IMM; op = &CPU::LDA; break;
    // case 0x01: am =  &CPU::IMM; op = &CPU::LDB; break;
    // case 0x02: am =  &CPU::IMM; op = &CPU::LDC; break;
    // case 0x03: am =  &CPU::DIR; op = &CPU::LDA; break;
    // case 0x04: am =  &CPU::DIR; op = &CPU::LDB; break;
    // case 0x05: am =  &CPU::DIR; op = &CPU::LDC; break;
    // case 0x06: am =  &CPU::IND; op = &CPU::LDA; break;
    // case 0x07: am =  &CPU::IND; op = &CPU::LDB; break;
    // case 0x08: am =  &CPU::IND; op = &CPU::LDC; break;
    // case 0x0A: am =  &CPU::INT; op = &CPU::LDA; break;
    // case 0x0B: am =  &CPU::INT; op = &CPU::LDB; break;
    // case 0x0C: am =  &CPU::INT; op = &CPU::LDC; break;
    // // Store
    // case 0x0D: am =  &CPU::DIR; op = &CPU::STA; break;
    // case 0x0E: am =  &CPU::DIR; op = &CPU::STB; break;
    // case 0x0F: am =  &CPU::DIR; op = &CPU::STC; break;
    // case 0x10: am =  &CPU::IND; op = &CPU::STA; break;
    // case 0x11: am =  &CPU::IND; op = &CPU::STB; break;
    // case 0x12: am =  &CPU::IND; op = &CPU::STC; break;
    // // Addition
    // case 0x13: am =  &CPU::IMM; op = &CPU::ADDA; break;
    // case 0x14: am =  &CPU::IMM; op = &CPU::ADDB; break;
    // case 0x15: am =  &CPU::IMM; op = &CPU::ADDC; break;
    // case 0x16: am =  &CPU::DIR; op = &CPU::ADDA; break;
    // case 0x17: am =  &CPU::DIR; op = &CPU::ADDB; break;
    // case 0x18: am =  &CPU::DIR; op = &CPU::ADDC; break;
    // case 0x1A: am =  &CPU::IND; op = &CPU::ADDA; break;
    // case 0x1B: am =  &CPU::IND; op = &CPU::ADDB; break;
    // case 0x1C: am =  &CPU::IND; op = &CPU::ADDC; break;
    // case 0x1D: am =  &CPU::INT; op = &CPU::ADDA; break;
    // case 0x1E: am =  &CPU::INT; op = &CPU::ADDB; break;
    // case 0x1F: am =  &CPU::INT; op = &CPU::ADDC; break;
    // // Multiplication
    // case 0x20: am =  &CPU::IMM; op = &CPU::MULA; break;
    // case 0x21: am =  &CPU::IMM; op = &CPU::MULB; break;
    // case 0x22: am =  &CPU::IMM; op = &CPU::MULC; break;
    // case 0x23: am =  &CPU::IMM; op = &CPU::MULA; break;
    // case 0x24: am =  &CPU::IMM; op = &CPU::MULB; break;
    // case 0x25: am =  &CPU::IMM; op = &CPU::MULC; break;
    // case 0x26: am =  &CPU::DIR; op = &CPU::MULA; break;
    // case 0x27: am =  &CPU::DIR; op = &CPU::MULB; break;
    // case 0x28: am =  &CPU::DIR; op = &CPU::MULC; break;
    // case 0x2A: am =  &CPU::IND; op = &CPU::MULA; break;
    // case 0x2B: am =  &CPU::IND; op = &CPU::MULB; break;
    // case 0x2C: am =  &CPU::IND; op = &CPU::MULC; break;
    // // Branch
    // case 0x2D: am =  &CPU::DIR; op = &CPU::JMP; break;
    // case 0x2E: am =  &CPU::DIR; op = &CPU::JCA; break;
    // case 0x2F: am =  &CPU::DIR; op = &CPU::JCB; break;
    // case 0x30: am =  &CPU::DIR; op = &CPU::JCC; break;

    default: 
        std::cout << "Unknown Instruction" << std::endl;
        exit(-1);
    }
}

void CPU::Execute()
{
    (this->*am)();
    (this->*op)();
}

void CPU::IMM() 
{
    mar = prc;
    PrcInc();
    Read();
}

void CPU::DIR()
{
    IMM();
    cra = mdr;
    IMM();
    crb = mdr;
    mar = (cra << 16) | crb;
    Read();
}

void CPU::IND()
{
    DIR();
    cra = mdr;
    Read();
    crb = mdr;
    mar = (cra << 16) | crb;
    Read();
}

void CPU::INT()
{
    IMM();
    switch (mdr) 
    {
        case 0x00:
            mdr = a;
            break;
        case 0x01:
            mdr = b;
            break;
        case 0x02:
            mdr = c;
            break;
        default:
            error = 1;
            break;
    }
}

void CPU::NOOP()
{
    return;
}

void CPU::LDA()
{
    a = mdr;
}

void CPU::LDB()
{
    b = mdr;
}

void CPU::LDC()
{
    c = mdr;
}

void CPU::STA()
{
    mdr = a;
    Write();
}

void CPU::STB()
{
    mdr = b;
    Write();
}

void CPU::STC()
{
    mdr = c;
    Write();
}

void CPU::ADDA()
{
    a = a + mdr;
}

void CPU::ADDB()
{
    b = b + mdr;
}

void CPU::ADDC()
{
    c = c + mdr;
}

void CPU::MULA()
{
    a = a * mdr;
}

void CPU::MULB()
{
    b = b * mdr;
}

void CPU::MULC()
{
    c = c * mdr;
}

// This works under the assumption that the insertion into the mdr by the 
// preceeding DIR() call won't screw over any data dependency
// this avoids having to write a special case for the JMP instr
void CPU::JMP()
{
    prc = mar;
}

void CPU::JCA()
{
    if (a == 0) JMP();
}

void CPU::JCB()
{
    if (b == 0) JMP();
}

void CPU::JCC()
{
    if (c == 0) JMP();
}

void CPU::Print()
{
    printf("CPU State Report: ------------------------\n");
    printf("Register A = %x\n", a);
    printf("Register B = %x\n", b);
    printf("Register C = %x\n", c);
    printf("------------------------------------------\n");
    printf("Program Counter = %x\n", prc);
    printf("Memory Address Register = %x\n", mar);
    printf("Control Instruction Register = %x\n", cir);
    printf("Memory Data Register = %x\n", mdr);
    printf("------------------------------------------\n\n");
}
