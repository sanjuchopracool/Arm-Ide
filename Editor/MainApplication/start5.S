    .global enableIRQ
enableIRQ:
	MRS r0,CPSR
	BIC  r0,r0,#I_BIT|F_BIT      @only irq 
	MSR CPSR ,r0
	MOV pc,lr
	    .global disableIRQ
disableIRQ:
	MRS r0,CPSR
	ORR  r0,r0,#I_BIT|F_BIT
	MSR CPSR ,r0
	MOV pc,lr