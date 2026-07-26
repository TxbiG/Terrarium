; TerrariumOS AArch64 boot entry placeholder.
;
; Preferred path: UEFI loads /boot/aarch64/terrarium-kernel.efi.
; Board firmware path must:
; - enter at EL1 or transition cleanly from EL2
; - preserve x0 as the device tree pointer when supplied
; - set up an early stack
; - branch into the common Terrarium loader
