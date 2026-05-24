.code

InvSqrt PROC
; https://www.felixcloutier.com/x86/rsqrtss
; all this effort for one instruction to exclude a single import
; yeah...
    rsqrtss xmm0, xmm0

    ret

InvSqrt ENDP

END