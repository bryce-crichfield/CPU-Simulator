#include "cpu.hpp"

#include <iostream>

void ControlUnit::DecodeAddressingMode()
{
    switch (AddressingModeMask & cir)
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
    // case 3:
    //     amr = Internal;
    //     break;
    }
}


// This ideally would store a function pointer in the ocr, but as of now
// this is largely redundant
void ControlUnit::DecodeOperationCode()
{
    std::cout << "Decoding Opcode" << std::endl;
    switch (OperationCodeMask & cir)
    {
    case 0:
        ocr = Noop;
        break;
    case 1:
        ocr = Load;
        break;
    case 2:
        // Prime the MAR with the address 
        // This will invalidate the MDR, but Store will overwrite that anyways
        cpu.addressing_unit.Direct();   
        ocr = Store;
        break;
    case 3:
        ocr = Jump;
        break;
    case 15:
        ocr = Halt;
        break;
    default:
        std::cout << "Opcode Decoding Error" << std::endl;
        break;
    }
}

void ControlUnit::DecodeDestinationRegister()
{
    switch (DestinationRegisterMask & cir)
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

void ControlUnit::DecodeSourceRegister1()
{
    switch (SourceRegister1Mask & cir)
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

void ControlUnit::DecodeSourceRegister2()
{
    switch (SourceRegister2Mask & cir)
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

ControlUnit::ControlUnit(CPU &cpu) : SubUnit(cpu)
{
    prc = 0x0;
    cir = 0x0;
    amr = Immediate;
    destination_register = &cpu.arithmetic_logic_unit.r1;
    source_register_1 = &cpu.arithmetic_logic_unit.r2;
    source_register_2 = &cpu.arithmetic_logic_unit.r3;
}


void ControlUnit::Decode()
{
    cir = cpu.addressing_unit.mdr;
    DecodeAddressingMode();
    DecodeOperationCode();
    switch (amr)
    {
    case Immediate:
        DecodeDestinationRegister();
        cpu.addressing_unit.Immediate();
        break;
    case Direct:
        DecodeDestinationRegister();
        cpu.addressing_unit.Direct();
        break;
    case Indirect:
        DecodeDestinationRegister();
        cpu.addressing_unit.Indirect();
        break;
    case SingleInternal:
        DecodeDestinationRegister();
        DecodeSourceRegister1();
        break;
    case DualInternal:
        DecodeDestinationRegister();
        DecodeSourceRegister1();
        DecodeSourceRegister2();
        break;
    // case Internal:
    //     DecodeDestinationRegister();
    //     DecodeSourceRegister1();
    //     DecodeSourceRegister2();
    //     break; THis oesn't make
    default:
        break;
    }
}

void ControlUnit::PrcInc()
{
    if (prc + 1 > 0xffffffff)
        cpu.flags.error = true;
    else
        prc += 1;
}

void ControlUnit::Print()
{
    using namespace std;
    cout << "----- Control Unit -----" << endl;
    cout << "----- --- PRC = 0x" << hex << prc << endl;
    cout << "----- --- CIR = 0x" << hex << cir << endl;
    cout << "----- --- AMR = " << PrintAddressingMode(amr) << endl;
    cout << "----- --- OCR = " << PrintOperationCode(ocr) << endl;
}