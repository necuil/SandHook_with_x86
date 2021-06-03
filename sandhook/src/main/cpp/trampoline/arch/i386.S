#include "../../includes/arch_base.h"

#if defined(__i386__)

#define RegMethod %eax

FUNCTION_START(REPLACEMENT_HOOK_TRAMPOLINE)
    call replacement_hook_trampoline_entry
replacement_hook_trampoline_entry:
    popl RegMethod
    pushl %ebx
    movl 23(RegMethod), %ebx //addr_code_entry
    pushl (%ebx) //[addr_code_entry]
    movl 4(%esp), %ebx
    movl 19(RegMethod), RegMethod //addr_art_method
    retn $4 //[addr_code_entry]
    nop
    nop
addr_art_method:
    .long 0
addr_code_entry:
    .long 0
FUNCTION_END(REPLACEMENT_HOOK_TRAMPOLINE)


FUNCTION_START(DIRECT_JUMP_TRAMPOLINE)
    .byte 0xe9 //jmp $+5
    .long 0 // addr_target = (target - (DIRECT_JUMP_TRAMPOLINE + 5))
FUNCTION_END(DIRECT_JUMP_TRAMPOLINE)


FUNCTION_START(INLINE_HOOK_TRAMPOLINE)
    pushl %ebx
    call inline_hook_trampoline_entry
inline_hook_trampoline_entry:
    popl %ebx
    movl 78(%ebx), %ebx //origin_art_method
    cmp RegMethod, %ebx
    jne call_origin_code
    call inline_hook_trampoline_entry2
inline_hook_trampoline_entry2:
    popl RegMethod
    movl 77(RegMethod), %ebx //addr_hook_code_entry
    pushl (%ebx) //[addr_hook_code_entry]
    movl 4(%esp), %ebx
    movl 73(RegMethod), RegMethod //hook_art_method
    retn $4 //[addr_hook_code_entry]
call_origin_code:
    popl %ebx
origin_code: //备份原函数被覆盖的指令，每条指令最长可能为15个字节，inline hook覆盖了5个字节，可能涉及多条指令
    .octa 0x90909090909090909090909090909090
    .quad 0x9090909090909090
    .long 0x90909090
    .short 0x9090
    pushl %eax
    call inline_hook_trampoline_entry3
inline_hook_trampoline_entry3:
    popl %eax
    pushl 16(%eax) //addr_origin_code_after_hook
    movl 4(%esp), %eax
    retn $4
    nop
origin_art_method:
    .long 0
addr_origin_code_after_hook:
    .long 0
hook_art_method:
    .long 0
addr_hook_code_entry:
    .long 0
FUNCTION_END(INLINE_HOOK_TRAMPOLINE)


FUNCTION_START(CALL_ORIGIN_TRAMPOLINE)
    call call_origin_trampoline_entry
call_origin_trampoline_entry:
    popl RegMethod
    pushl 15(RegMethod) //addr_origin
    movl 11(RegMethod), RegMethod //origin_method
    ret
    nop
    nop
    nop
origin_method:
    .long 0
addr_origin:
    .long 0
FUNCTION_END(CALL_ORIGIN_TRAMPOLINE)

#endif