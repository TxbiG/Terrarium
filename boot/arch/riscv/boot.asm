; TerrariumOS RISC-V boot entry placeholder.
;
; Preferred path: firmware/SBI loads the kernel and passes a device tree.
; The real entry must:
; - preserve a0/a1 firmware arguments where the platform ABI uses them
; - set up an early stack
; - keep the device tree pointer for kernel handoff
; - branch into the common Terrarium loader
