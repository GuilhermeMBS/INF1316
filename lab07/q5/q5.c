/**
 * 5) Escreva um programa que faz um mapeamento de arquivo para memória e depois escreve novos caracteres no final do arquivo. O que voce precisa fazer para evitar um SegFault?
O Linux também suporta uma função:
    void *mremap(void *addr, size_t old_sz, size_t
    new_sz, unsigned long flags) // addr precisa estar alinhado com o início da página
 */