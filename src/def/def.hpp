#pragma once

// 1-bit
typedef bool bit;
// 8-bit
typedef unsigned char byte;
// 16-bit
typedef unsigned short int word;
// 32-bit
typedef unsigned int sentence;



class SysConfig;

class SysConfig
{
    public:
        word MEM_SIZE;

        word PRG_START;
        word PRG_END;

        word STK_START;
        word STK_END;

        SysConfig();
};
