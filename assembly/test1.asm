
.65816

.org 0x8000
start:
  ; Disable interrupts
  sei

  ; Set native mode
  clc
  xce
  SEP #$20
  REP #$20
  lda #$DEAD