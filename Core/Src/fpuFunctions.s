  .syntax unified
	.cpu cortex-m4
	.fpu vfpv3
	.thumb

.global getSqrt

getSqrt:
  vsqrt.f32 s0, s0
  vcvtr.u32.f32 s0, s0
  vmov.f32 a1, s0
  mov pc,lr
