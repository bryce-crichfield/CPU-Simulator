#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <iostream>
#include <string>
#include "../bus/bus.hpp"

enum AddressingMode
{
    Immediate,
    Direct,
    Indirect,
    SingleInternal,
    DualInternal
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

std::string PrintAddressingMode(AddressingMode mode);

std::string PrintOperationCode(OperationCode code);

class CPU;
struct Flags;
class SubUnit;
class ControlUnit;
class AddressingUnit;
class ArithmeticLogicUnit;

// Abstract class for all components used by the cpu.  Gives each child unit of the
// CPU a reference to the CPU itself
class SubUnit
{
public:
    // Owner of this Sub Unit
    CPU &cpu;

    SubUnit(CPU &cpu)
        : cpu(cpu)
    {
    }

    virtual void Print() = 0;
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

    // Given a reference to the owning CPU of this addressing unit, set the
    // internal registers of this unit to zeros
    AddressingUnit(CPU &cpu);

    // Todo: This seems to call the CU more than itself, maybe it should be moved?
    void Fetch();

    // Read a 16-bit word from the bus by placing the address held at the
    // memory address register onto the memory address bus.  The bus itself
    // is expected to resolve any read-requests so long as the address
    // requested is well-formed.  Once the request is resolved, the resultant
    // value is placed on the data bus and then stored in the memory data register
    void Read();

    // Store the 16-bit word held in the memory data register at the 32-bit
    // address held in the memory address register.  The two values are handed off
    // onto the data bus and address bus respectively, and it is left up to the bus
    // to resolve the write to memory correctly.
    void Write();

    // The 32-bit address stored in the program counter is expected to contain
    // the next 16-bit data word.  The program counter's address is placed into
    // the memory address register, the counter rhen incremented.  A bus-read
    // operation is then performed accordingly
    void Immediate();

    void Direct();

    void Indirect();

    void Print() override;
};

// Responsible for decoding instructions retrieved rom the addressing unit
class ControlUnit : SubUnit
{
private:
    const word AddressingModeMask = 0b111 << 13;
    const word OperationCodeMask = 0b1111 << 8;
    const word DestinationRegisterMask = 0b111 << 6;
    const word SourceRegister1Mask = 0b111 << 3;
    const word SourceRegister2Mask = 0b111;

    // Decode the first 2-bits of the 16-bit control instruction
    // register, then store the decoded addressing mode in the address
    // mode register for execution during the decode phase
    void DecodeAddressingMode();

    // Decode the 4-bit operation code stored in the encoded
    // 16-bit control instruction register, then store the
    // decoded opcode in the operation code register
    void DecodeOperationCode();

    // If the current addressing mode is the Internal Mode,
    // decode 3-bits as the destination register, and store
    // the decoded internal address in the destination register
    void DecodeDestinationRegister();

    // If the current addressing mode is the Internal Mode,
    // decode 3-bits as the first source register, and store
    // the decoded internal address in the first source register
    void DecodeSourceRegister1();

    // If the current addressing mode is the Internal Mode,
    // decode 3-bits as the second source register, and store
    // the decoded internal address in the second source register
    void DecodeSourceRegister2();

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

    // Given a reference to the owning CPU of this control unit, set the program
    // counter, control instruction register, and addressing mode register to
    // defaults of zero and immediate mode as applicable.
    // Set destination and source register to the first register.
    ControlUnit(CPU &cpu);

    // After having contacted the addressing unit, this stores the memory data
    // register into the control instruction register, decodes the instruction
    // therein and dispatches the requisite addressing mode to the addressing
    // unit, preparing the registers for execution.
    // The Decode process begins by determining the addressing mode, then from
    // there determining the necessary sequence of decode operations needed.
    void Decode();

    // Program Counter Increment - increment the program counter,
    // if this were to result in overflow then set the error bit.
    void PrcInc();

    void Print() override;
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

    ArithmeticLogicUnit(CPU &cpu);

    void Execute();

    void Noop();

    // Used for registered writes 
    void Load();

    // Only for addressed writes, not for registered writes
    void Store();

    void Jump();

    void Halt();

    void Print() override;
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

    

public:
    // Flags
    Flags &flags;

    CPU();

    // I don't know why, but there has to be a destructor and it has to be virtual
    // otherwise you get the most obnoxius linking error
    virtual ~CPU();

    void Read() override;

    void Write() override;

    void Reset() override;

    void Cycle();

    void Print();
};

#endif