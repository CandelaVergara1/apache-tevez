.code16 # genera instrucciones de 16 bits
    cli # deshabilita interrupciones
    ljmp $0, $start # ponemos CS en 0

start:
    xor %ax, %ax # ponemos el Ax en 0 
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov $0x7C00, %sp 


    lgdt gdt_descriptor # se carga la gdt
    
    # Activo modo protegido - PE=1 en CR0
    mov %cr0, %eax
    orl $0x1, %eax
    mov %eax, %cr0

    ljmp $0x08, $protected_mode 

gdt_start:
gdt_null:
    .long 0x0
    .long 0x0
gdt_code:
    .word 0xffff # limite
    .word 0x0    # base
    .byte 0x0    
    .byte 0b10011010
    .byte 0b11001111
    .byte 0x0
gdt_data:
    .word 0xffff
    .word 0x0
    .byte 0x0
    .byte 0b10010010
    .byte 0b11001111
    .byte 0x0
gdt_end:
gdt_descriptor:
    .word gdt_end - gdt_start
    .long gdt_start


.code32
protected_mode:
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    mov $0X7000, %ebp
    mov %ebp, %esp

