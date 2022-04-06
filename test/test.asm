start:
    mov a, @0x0ff2
    add a, 0x01
    str a, 0x03!
    jmp lab2
lab1:
    jmp start
lab2:
    jmp lab1
