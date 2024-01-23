  .syntax unified
	.cpu cortex-m4
	.fpu vfpv3
	.thumb

.global getSqrt

getSqrt:
  vsqrt.f32 s0, s0
  mov pc,lr
