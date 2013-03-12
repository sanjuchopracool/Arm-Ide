.section "vectors"
reset:	b  start	@jump to start
undef: 	b     undef
swi:    b     swi
pabt:   b     pabt
dabt:   b     dabt
        nop
irq:   ldr   pc,[pc,#-0xFF0]            
fiq:    b    error
	.text
start:
	@@copy data to ram
	LDR	 	r0,=flash_sdata   @start of ram data in flash
	LDR		r1,=ram_sdata		@address of ram
	LDR		r2,=data_size

	@@handle zero data size
	CMP 	r2,#0
	BEQ		init_bss	@@ branch if equal to  either jump to init_bss

copy:

	LDRB	r4,[r0],#1    	@ r4 =mem[r0++]
	STRB	r4,[r1],#1		@ mem[r1++]=r4
	SUBS	r2,r2,#1		@ r2-- ; and update the flags
	BNE		copy			@branch ifnot equal to zero

init_bss:
	@@copy .bssto ram
	LDR	 	r0,=sbss   @start of bss data in flash
	LDR		r1,=ebss		@address of ram
	LDR		r2,=bss_size

	@@handle zero data size
	CMP 	r2,#0
	BEQ		init_stack	@@ branch if equal to  either jumpto init_stack

	MOV 	r4,#0

zero:
	@@make all bss data zero
	@LDRB	r4,[r0],#1    	@ r4 =mem[r0++]
	STRB	r4,[r0],#1		@ mem[r1++]=r4
	SUBS	r2,r2,#1		@ r2-- ; and update the flags
	BNE		zero			@branch ifnot equal to zero

init_stack:
        @@ Initialize the stack pointer
        @change ti irq mode  ,disable interrupt,set stack pointer
        LDR  r0 ,=0x40008000
        MSR  CPSR_c ,#IRQ_MODE|I_BIT|F_BIT
        MOV sp,r0 
        SUB r0,r0,#IRQ_STACK
        
        @change ti fiq mode  ,disable interrupt,set stack pointer
        MSR  CPSR_c ,#FIQ_MODE|I_BIT|F_BIT
        MOV sp,r0 
        SUB r0,r0,#FIQ_STACK
        
        @change ti svc mode  ,disable interrupt,set stack pointer
        MSR  CPSR_c ,#SVC_MODE|I_BIT|F_BIT
        MOV sp,r0 
        SUB r0,r0,#SVC_STACK
        
        @change ti abort mode  ,disable interrupt,set stack pointer
        MSR  CPSR_c ,#ABRT_MODE|I_BIT|F_BIT
        MOV sp,r0 
        SUB r0,r0,#ABRT_STACK
        
        @change ti undefined  mode  ,disable interrupt,set stack pointer
        MSR  CPSR_c ,#UND_MODE|I_BIT|F_BIT
        MOV sp,r0 
        SUB r0,r0,#UND_STACK

        @come back to system mode and set stack pointer
        MSR  CPSR_c ,#USR_MODE
        MOV sp,r0    @because we use full descending stack first it will decrement and store val
/*
Branch to c codes first go to the low level initialization so that clock , peripheral clock and MAM can se configured
*/
