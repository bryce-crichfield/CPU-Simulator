#pragma once

typedef unsigned char byte;
typedef unsigned short int word;

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
