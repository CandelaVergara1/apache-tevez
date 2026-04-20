# conversor.s
# int procesar_gini(float value)
# Convencion System V AMD64:
#   xmm0 = parametro (float)
#   rax  = retorno (int)

    .text
    .globl procesar_gini  # Export symbol

procesar_gini:
    enter $0, $0          # Prologo
    
    cvttss2si %xmm0, %eax # eax = (int) value
    add       $1, %eax

    leave
    ret

