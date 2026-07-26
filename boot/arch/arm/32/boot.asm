; TerrariumOS ARM32 boot entry placeholder.
;
; ARM boards usually enter through board firmware, U-Boot, or an EFI stub.
; This file marks the architecture slot; the real implementation must:
; - receive firmware registers according to the selected board ABI
; - preserve or locate the device tree
; - set up an early stack
; - branch into the common Terrarium loader
