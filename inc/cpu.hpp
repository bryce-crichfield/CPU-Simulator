#ifndef CPU_H
#define CPU_H

#include "bus.hpp"

enum AddressingMode
{
    Immediate,
    Direct,
    Indirect,
    Internal
};

enum OperationCode
{
    Noop,
    Load,
    Store,
    Jump,
    Add,
    Sub,
    Multiply,
    Halt,
};

class CPU;
struct Flags;
class SubUnit;
class ControlUnit;
class AddressingUnit;
class ArithmeticLogicUnit;

class SubUnit
{
public:
    // Owner of this Sub Unit
    CPU &cpu;

    SubUnit(CPU &cpu)
        : cpu(cpu)
    {
    }
};

// Responsible for writing and reading instructions and data to and from the bus
class AddressingUnit : SubUnit
{
private:
    // Construction Register - used as temporary buffer to construct a 32-bit 
    // value from two 16-bit values.  This is done during a download of of two
    // word of memory 
    word cra, crb;

public:

    // Memory Address Register - stores the address of the current fetched address
    // used during read and writes as a global threshold between cpu and address bus
    sentence mar;

    // Memory Data Register - stores 16-bit unencoded data items that have been 
    // fetched from memory and to be written to memory, acting as global theshold
    // between cpu and data bus
    word mdr;

    AddressingUnit(CPU &cpu) : SubUnit(cpu)
    {
        cra = crb = 0;
        mar = mdr = 0;
    }

    void Read()
    {
        cpu.bus->RaiseReadFlag();
        cpu.bus->WriteAddressBus(mar);
        cpu.bus->Notify();
        mdr = cpu.bus->ReadDataBus();
    }

    void Write()
    {
        cpu.bus->RaiseWriteFlag();
        cpu.bus->WriteAddressBus(mar);
        cpu.bus->WriteDataBus(mar);
        cpu.bus->Notify();
    }

    void Immediate()
    {
        mar = cpu.control_unit.prc;
        cpu.control_unit.PrcInc();
        Read();
    }

    void Direct()
    {
        Immediate();
        cra = mdr;
        Immediate();
        crb = mdr;
        mar = (cra << 16) | crb;
        Read();
    }

    void Indirect()
    {
        Direct();
        cra = mdr;
        Read();
        crb = mdr;
        mar = (cra << 16) | crb;
        Read();
    }
};

// Responsible for decoding instructions retrieved rom the addressing unit
class ControlUnit : SubUnit
{
private:
    // Decode the first 2-bits of the 16-bit control instruction
    // register, then store the decoded addressing mode in the address
    // mode register for execution during the decode phase
    void DecodeAddressingMode()
    {
        switch ((0b11 << 14) & cir)
        {
        case 0:
            amr = Immediate;
            break;
        case 1:
            amr = Direct;
            break;
        case 2:
            amr = Indirect;
            break;
        case 3:
            amr = Internal;
            break;
        }
    }

    // Decode the 4-bit operation code stored in the encoded
    // 16-bit control instruction register, then store the
    // decoded opcode in the operation code register
    void DecodeOperationCode()
    {
        switch ((0b1111 << 10) & cir)
        {
            case 0:
                ocr = Noop;
                break;
            case 1:
                ocr = Load;
                break;
            case 2: 
                ocr = Store;
                break;
            case 3: 
                ocr = Jump;
                break;
            case 15: 
                ocr = Halt;
                break;
        }
    }

    // If the current addressing mode is the Internal Mode,
    // decode 3-bits as the destination register, and store
    // the decoded internal address in the destination register
    void DecodeDestinationRegister()
    {
        switch ((0b111 << 8) & cir)
        {
        case 1:
            destination_register = &cpu.arithmetic_logic_unit.r1;
            break;
        case 2:
            destination_register = &cpu.arithmetic_logic_unit.r2;
            break;
        case 3:
            destination_register = &cpu.arithmetic_logic_unit.r3;
            break;
        case 4:
            destination_register = &cpu.arithmetic_logic_unit.r4;
            break;
        case 5:
            destination_register = &cpu.arithmetic_logic_unit.r5;
            break;
        case 6:
            destination_register = &cpu.arithmetic_logic_unit.r6;
            break;
        }
    }

    // If the current addressing mode is the Internal Mode,
    // decode 3-bits as the first source register, and store
    // the decoded internal address in the first source register
    void DecodeSourceRegister1()
    {
        switch ((0b111 << 5) & cir)
        {
        case 1:
            source_register_1 = &cpu.arithmetic_logic_unit.r1;
            break;
        case 2:
            source_register_1 = &cpu.arithmetic_logic_unit.r2;
            break;
        case 3:
            source_register_1 = &cpu.arithmetic_logic_unit.r3;
            break;
        case 4:
            source_register_1 = &cpu.arithmetic_logic_unit.r4;
            break;
        case 5:
            source_register_1 = &cpu.arithmetic_logic_unit.r5;
            break;
        case 6:
            source_register_1 = &cpu.arithmetic_logic_unit.r6;
            break;
        }
    }

    // If the current addressing mode is the Internal Mode,
    // decode 3-bits as the second source register, and store
    // the decoded internal address in the second source register
    void DecodeSourceRegister2()
    {
        switch (0b111 & cir)
        {
        case 1:
            source_register_2 = &cpu.arithmetic_logic_unit.r1;
            break;
        case 2:
            source_register_2 = &cpu.arithmetic_logic_unit.r2;
            break;
        case 3:
            source_register_2 = &cpu.arithmetic_logic_unit.r3;
            break;
        case 4:
            source_register_2 = &cpu.arithmetic_logic_unit.r4;
            break;
        case 5:
            source_register_2 = &cpu.arithmetic_logic_unit.r5;
            break;
        case 6:
            source_register_2 = &cpu.arithmetic_logic_unit.r6;
            break;
        }
    }

public:
    // Program Counter - the 32-bit address of the current 16-bit instruction
    sentence prc;

    // Control Instruction Register - the current 16-bit encoded instruction
    word cir;

    // Address Mode Register - stores the next addressing mode to be performed
    AddressingMode amr;

    // Operation Code Register - stores the current operation code to be performed
    OperationCode ocr;

    // Destination and Source Registers - stores the registers targeted by
    // the current instruction by destination and by source
    word *destination_register, *source_register_1, *source_register_2;

    ControlUnit(CPU &cpu) : SubUnit(cpu)
    {
        prc = 0x0;
        cir = 0x0;
        amr = Immediate;
        destination_register = &cpu.arithmetic_logic_unit.r1;
        source_register_1 = &cpu.arithmetic_logic_unit.r2;
        source_register_2 = &cpu.arithmetic_logic_unit.r3;
    }

    /* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ Decode ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
    // After having contacted the addressing unit, this stores the memory data
    // register into the control instruction register, decodes the instruction
    // therein and dispatches the requisite addressing mode to the addressing
    // unit, preparing the registers for execution.
    void Decode()
    {
        cir = cpu.addressing_unit.mdr;
        DecodeAddressingMode();
        DecodeOperationCode();
        switch (amr)
        {
        case Immediate:
            cpu.addressing_unit.Immediate();
            break;
        case Direct:
            cpu.addressing_unit.Direct();
            break;
        case Indirect:
            cpu.addressing_unit.Indirect();
            break;
        case Internal:
            DecodeDestinationRegister();
            DecodeSourceRegister1();
            DecodeSourceRegister2();
            break;
        default:
            break;
        }
    }

    // Program Counter Increment - increment the program counter,
    // if this were to result in overflow then set the error bit.
    void PrcInc()
    {
        if (prc + 1 > 0xffffffff)
            cpu.flags.error = true;
        else
            prc += 1;
    }
};

struct Flags
{
    bool error, halt;

    Flags()
    {
        halt = false;
        error = false;
    }
};

// Resposible for the execution of instructions and maintence of
// CPU registers.
class ArithmeticLogicUnit : SubUnit
{
public:
    // Data registers
    // Register 0 always contains 0
    word r0, r1, r2, r3, r4, r5, r6;

    ArithmeticLogicUnit(CPU &cpu) : SubUnit(cpu)
    {
        r0 = r1 = r2 = r3 = r4 = r5 = r6 = 0;
    }

    void Noop()
    {
        return;
    }

    void Load()
    {
        *cpu.control_unit.destination_register = cpu.addressing_unit.mdr;
    }

    void Store()
    {
        cpu.addressing_unit.mdr = *cpu.control_unit.destination_register;
    }

    void Jump()
    {
        cpu.control_unit.prc = cpu.addressing_unit.mar;
    }

    void Halt() 
    {
        cpu.flags.halt = true;
    }
};

class CPU : public Device
{
private:
    friend class ControlUnit;
    friend class AddressingUnit;
    friend class ArithmeticLogicUnit;

    AddressingUnit &addressing_unit;
    ControlUnit &control_unit;
    ArithmeticLogicUnit &arithmetic_logic_unit;

    // Flags
    Flags &flags;

    void PrcInc();

    void Fetch();
    void Decode();
    void Execute();

public:
    CPU()
        : addressing_unit(*(new AddressingUnit(*this))),
          control_unit(*(new ControlUnit(*this))),
          arithmetic_logic_unit(*(new ArithmeticLogicUnit(*this))),
          flags(*(new Flags()))
    {
    }

    ~CPU() {}

    void Reset();
    void Cycle();
    void Print();
};

#endif