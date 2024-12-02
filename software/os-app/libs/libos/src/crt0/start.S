.globl  _start
_start:
  mv s0, zero
  # mv sp, a0
  jal call_main
