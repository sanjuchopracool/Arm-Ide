   .code 32  @means we are using arm mode not the thumb
   .align 4
   
.equ  I_BIT, 0x80               @when I bit is set, IRQ is disabled
.equ  F_BIT, 0x40               @ when F bit is set, FIQ is disabled
/*
Set the modes of processro i need only irq and sys modes
*/
.set USR_MODE  ,0x10
.set FIQ_MODE  ,0x11
.set IRQ_MODE  ,0x12
.set SVC_MODE  ,0x13
.set ABRT_MODE ,0x17
.set UND_MODE  ,0x1B
.set SYS_MODE  ,0x1F

/*
set the stack sizes
*/