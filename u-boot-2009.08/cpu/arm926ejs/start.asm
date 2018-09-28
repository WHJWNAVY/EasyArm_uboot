
start.o:     file format elf32-littlearm


Disassembly of section .text:

00000000 <_start>:
   0:	eafffffe 	b	50 <reset>
   4:	e59ff014 	ldr	pc, [pc, #20]	; 20 <_undefined_instruction>
   8:	e59ff014 	ldr	pc, [pc, #20]	; 24 <_software_interrupt>
   c:	e59ff014 	ldr	pc, [pc, #20]	; 28 <_prefetch_abort>
  10:	e59ff014 	ldr	pc, [pc, #20]	; 2c <_data_abort>
  14:	e59ff014 	ldr	pc, [pc, #20]	; 30 <_not_used>
  18:	e59ff014 	ldr	pc, [pc, #20]	; 34 <_irq>
  1c:	e59ff014 	ldr	pc, [pc, #20]	; 38 <_fiq>

00000020 <_undefined_instruction>:
  20:	000000e0 	.word	0x000000e0

00000024 <_software_interrupt>:
  24:	00000140 	.word	0x00000140

00000028 <_prefetch_abort>:
  28:	000001a0 	.word	0x000001a0

0000002c <_data_abort>:
  2c:	00000200 	.word	0x00000200

00000030 <_not_used>:
  30:	00000260 	.word	0x00000260

00000034 <_irq>:
  34:	000002c0 	.word	0x000002c0

00000038 <_fiq>:
  38:	00000320 	.word	0x00000320
  3c:	deadbeef 	.word	0xdeadbeef

00000040 <_TEXT_BASE>:
  40:	41008000 	.word	0x41008000

00000044 <_armboot_start>:
  44:	00000000 	.word	0x00000000

00000048 <_bss_start>:
  48:	00000000 	.word	0x00000000

0000004c <_bss_end>:
  4c:	00000000 	.word	0x00000000

00000050 <reset>:
  50:	e10f0000 	mrs	r0, CPSR
  54:	e3c0001f 	bic	r0, r0, #31
  58:	e38000d3 	orr	r0, r0, #211	; 0xd3
  5c:	e129f000 	msr	CPSR_fc, r0
  60:	eb00000e 	bl	a0 <cpu_init_crit>

00000064 <stack_setup>:
  64:	e51f002c 	ldr	r0, [pc, #-44]	; 40 <_TEXT_BASE>
  68:	e2400501 	sub	r0, r0, #4194304	; 0x400000
  6c:	e2400080 	sub	r0, r0, #128	; 0x80
  70:	e240d00c 	sub	sp, r0, #12

00000074 <clear_bss>:
  74:	e51f0034 	ldr	r0, [pc, #-52]	; 48 <_bss_start>
  78:	e51f1034 	ldr	r1, [pc, #-52]	; 4c <_bss_end>
  7c:	e3a02000 	mov	r2, #0

00000080 <clbss_l>:
  80:	e5802000 	str	r2, [r0]
  84:	e2800004 	add	r0, r0, #4
  88:	e1500001 	cmp	r0, r1
  8c:	dafffffb 	ble	80 <clbss_l>
  90:	ebfffffe 	bl	0 <coloured_LED_init>
  94:	ebfffffe 	bl	0 <red_LED_on>
  98:	e51ff004 	ldr	pc, [pc, #-4]	; 9c <_start_armboot>

0000009c <_start_armboot>:
  9c:	00000000 	.word	0x00000000

000000a0 <cpu_init_crit>:
  a0:	e3a00000 	mov	r0, #0
  a4:	ee070f17 	mcr	15, 0, r0, cr7, cr7, {0}
  a8:	ee080f17 	mcr	15, 0, r0, cr8, cr7, {0}
  ac:	ee110f10 	mrc	15, 0, r0, cr1, cr0, {0}
  b0:	e3c00c23 	bic	r0, r0, #8960	; 0x2300
  b4:	e3c00087 	bic	r0, r0, #135	; 0x87
  b8:	e3800002 	orr	r0, r0, #2
  bc:	e3800a01 	orr	r0, r0, #4096	; 0x1000
  c0:	ee010f10 	mcr	15, 0, r0, cr1, cr0, {0}
  c4:	e1a0c00e 	mov	ip, lr
  c8:	ebfffffe 	bl	0 <lowlevel_init>
  cc:	e1a0e00c 	mov	lr, ip
  d0:	e1a0f00e 	mov	pc, lr
  d4:	e1a00000 	nop			; (mov r0, r0)
  d8:	e1a00000 	nop			; (mov r0, r0)
  dc:	e1a00000 	nop			; (mov r0, r0)

000000e0 <undefined_instruction>:
  e0:	e51fd0a4 	ldr	sp, [pc, #-164]	; 44 <_armboot_start>
  e4:	e24dd842 	sub	sp, sp, #4325376	; 0x420000
  e8:	e24dd088 	sub	sp, sp, #136	; 0x88
  ec:	e58de000 	str	lr, [sp]
  f0:	e14fe000 	mrs	lr, SPSR
  f4:	e58de004 	str	lr, [sp, #4]
  f8:	e3a0d013 	mov	sp, #19
  fc:	e169f00d 	msr	SPSR_fc, sp
 100:	e1a0e00f 	mov	lr, pc
 104:	e1b0f00e 	movs	pc, lr
 108:	e24dd048 	sub	sp, sp, #72	; 0x48
 10c:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 110:	e51f20d4 	ldr	r2, [pc, #-212]	; 44 <_armboot_start>
 114:	e2422842 	sub	r2, r2, #4325376	; 0x420000
 118:	e2422088 	sub	r2, r2, #136	; 0x88
 11c:	e892000c 	ldm	r2, {r2, r3}
 120:	e28d0048 	add	r0, sp, #72	; 0x48
 124:	e28d5034 	add	r5, sp, #52	; 0x34
 128:	e1a0100e 	mov	r1, lr
 12c:	e885000f 	stm	r5, {r0, r1, r2, r3}
 130:	e1a0000d 	mov	r0, sp
 134:	ebfffffe 	bl	0 <do_undefined_instruction>
 138:	e1a00000 	nop			; (mov r0, r0)
 13c:	e1a00000 	nop			; (mov r0, r0)

00000140 <software_interrupt>:
 140:	e51fd104 	ldr	sp, [pc, #-260]	; 44 <_armboot_start>
 144:	e24dd842 	sub	sp, sp, #4325376	; 0x420000
 148:	e24dd088 	sub	sp, sp, #136	; 0x88
 14c:	e58de000 	str	lr, [sp]
 150:	e14fe000 	mrs	lr, SPSR
 154:	e58de004 	str	lr, [sp, #4]
 158:	e3a0d013 	mov	sp, #19
 15c:	e169f00d 	msr	SPSR_fc, sp
 160:	e1a0e00f 	mov	lr, pc
 164:	e1b0f00e 	movs	pc, lr
 168:	e24dd048 	sub	sp, sp, #72	; 0x48
 16c:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 170:	e51f2134 	ldr	r2, [pc, #-308]	; 44 <_armboot_start>
 174:	e2422842 	sub	r2, r2, #4325376	; 0x420000
 178:	e2422088 	sub	r2, r2, #136	; 0x88
 17c:	e892000c 	ldm	r2, {r2, r3}
 180:	e28d0048 	add	r0, sp, #72	; 0x48
 184:	e28d5034 	add	r5, sp, #52	; 0x34
 188:	e1a0100e 	mov	r1, lr
 18c:	e885000f 	stm	r5, {r0, r1, r2, r3}
 190:	e1a0000d 	mov	r0, sp
 194:	ebfffffe 	bl	0 <do_software_interrupt>
 198:	e1a00000 	nop			; (mov r0, r0)
 19c:	e1a00000 	nop			; (mov r0, r0)

000001a0 <prefetch_abort>:
 1a0:	e51fd164 	ldr	sp, [pc, #-356]	; 44 <_armboot_start>
 1a4:	e24dd842 	sub	sp, sp, #4325376	; 0x420000
 1a8:	e24dd088 	sub	sp, sp, #136	; 0x88
 1ac:	e58de000 	str	lr, [sp]
 1b0:	e14fe000 	mrs	lr, SPSR
 1b4:	e58de004 	str	lr, [sp, #4]
 1b8:	e3a0d013 	mov	sp, #19
 1bc:	e169f00d 	msr	SPSR_fc, sp
 1c0:	e1a0e00f 	mov	lr, pc
 1c4:	e1b0f00e 	movs	pc, lr
 1c8:	e24dd048 	sub	sp, sp, #72	; 0x48
 1cc:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 1d0:	e51f2194 	ldr	r2, [pc, #-404]	; 44 <_armboot_start>
 1d4:	e2422842 	sub	r2, r2, #4325376	; 0x420000
 1d8:	e2422088 	sub	r2, r2, #136	; 0x88
 1dc:	e892000c 	ldm	r2, {r2, r3}
 1e0:	e28d0048 	add	r0, sp, #72	; 0x48
 1e4:	e28d5034 	add	r5, sp, #52	; 0x34
 1e8:	e1a0100e 	mov	r1, lr
 1ec:	e885000f 	stm	r5, {r0, r1, r2, r3}
 1f0:	e1a0000d 	mov	r0, sp
 1f4:	ebfffffe 	bl	0 <do_prefetch_abort>
 1f8:	e1a00000 	nop			; (mov r0, r0)
 1fc:	e1a00000 	nop			; (mov r0, r0)

00000200 <data_abort>:
 200:	e51fd1c4 	ldr	sp, [pc, #-452]	; 44 <_armboot_start>
 204:	e24dd842 	sub	sp, sp, #4325376	; 0x420000
 208:	e24dd088 	sub	sp, sp, #136	; 0x88
 20c:	e58de000 	str	lr, [sp]
 210:	e14fe000 	mrs	lr, SPSR
 214:	e58de004 	str	lr, [sp, #4]
 218:	e3a0d013 	mov	sp, #19
 21c:	e169f00d 	msr	SPSR_fc, sp
 220:	e1a0e00f 	mov	lr, pc
 224:	e1b0f00e 	movs	pc, lr
 228:	e24dd048 	sub	sp, sp, #72	; 0x48
 22c:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 230:	e51f21f4 	ldr	r2, [pc, #-500]	; 44 <_armboot_start>
 234:	e2422842 	sub	r2, r2, #4325376	; 0x420000
 238:	e2422088 	sub	r2, r2, #136	; 0x88
 23c:	e892000c 	ldm	r2, {r2, r3}
 240:	e28d0048 	add	r0, sp, #72	; 0x48
 244:	e28d5034 	add	r5, sp, #52	; 0x34
 248:	e1a0100e 	mov	r1, lr
 24c:	e885000f 	stm	r5, {r0, r1, r2, r3}
 250:	e1a0000d 	mov	r0, sp
 254:	ebfffffe 	bl	0 <do_data_abort>
 258:	e1a00000 	nop			; (mov r0, r0)
 25c:	e1a00000 	nop			; (mov r0, r0)

00000260 <not_used>:
 260:	e51fd224 	ldr	sp, [pc, #-548]	; 44 <_armboot_start>
 264:	e24dd842 	sub	sp, sp, #4325376	; 0x420000
 268:	e24dd088 	sub	sp, sp, #136	; 0x88
 26c:	e58de000 	str	lr, [sp]
 270:	e14fe000 	mrs	lr, SPSR
 274:	e58de004 	str	lr, [sp, #4]
 278:	e3a0d013 	mov	sp, #19
 27c:	e169f00d 	msr	SPSR_fc, sp
 280:	e1a0e00f 	mov	lr, pc
 284:	e1b0f00e 	movs	pc, lr
 288:	e24dd048 	sub	sp, sp, #72	; 0x48
 28c:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 290:	e51f2254 	ldr	r2, [pc, #-596]	; 44 <_armboot_start>
 294:	e2422842 	sub	r2, r2, #4325376	; 0x420000
 298:	e2422088 	sub	r2, r2, #136	; 0x88
 29c:	e892000c 	ldm	r2, {r2, r3}
 2a0:	e28d0048 	add	r0, sp, #72	; 0x48
 2a4:	e28d5034 	add	r5, sp, #52	; 0x34
 2a8:	e1a0100e 	mov	r1, lr
 2ac:	e885000f 	stm	r5, {r0, r1, r2, r3}
 2b0:	e1a0000d 	mov	r0, sp
 2b4:	ebfffffe 	bl	0 <do_not_used>
 2b8:	e1a00000 	nop			; (mov r0, r0)
 2bc:	e1a00000 	nop			; (mov r0, r0)

000002c0 <irq>:
 2c0:	e51fd284 	ldr	sp, [pc, #-644]	; 44 <_armboot_start>
 2c4:	e24dd842 	sub	sp, sp, #4325376	; 0x420000
 2c8:	e24dd088 	sub	sp, sp, #136	; 0x88
 2cc:	e58de000 	str	lr, [sp]
 2d0:	e14fe000 	mrs	lr, SPSR
 2d4:	e58de004 	str	lr, [sp, #4]
 2d8:	e3a0d013 	mov	sp, #19
 2dc:	e169f00d 	msr	SPSR_fc, sp
 2e0:	e1a0e00f 	mov	lr, pc
 2e4:	e1b0f00e 	movs	pc, lr
 2e8:	e24dd048 	sub	sp, sp, #72	; 0x48
 2ec:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 2f0:	e51f22b4 	ldr	r2, [pc, #-692]	; 44 <_armboot_start>
 2f4:	e2422842 	sub	r2, r2, #4325376	; 0x420000
 2f8:	e2422088 	sub	r2, r2, #136	; 0x88
 2fc:	e892000c 	ldm	r2, {r2, r3}
 300:	e28d0048 	add	r0, sp, #72	; 0x48
 304:	e28d5034 	add	r5, sp, #52	; 0x34
 308:	e1a0100e 	mov	r1, lr
 30c:	e885000f 	stm	r5, {r0, r1, r2, r3}
 310:	e1a0000d 	mov	r0, sp
 314:	ebfffffe 	bl	0 <do_irq>
 318:	e1a00000 	nop			; (mov r0, r0)
 31c:	e1a00000 	nop			; (mov r0, r0)

00000320 <fiq>:
 320:	e51fd2e4 	ldr	sp, [pc, #-740]	; 44 <_armboot_start>
 324:	e24dd842 	sub	sp, sp, #4325376	; 0x420000
 328:	e24dd088 	sub	sp, sp, #136	; 0x88
 32c:	e58de000 	str	lr, [sp]
 330:	e14fe000 	mrs	lr, SPSR
 334:	e58de004 	str	lr, [sp, #4]
 338:	e3a0d013 	mov	sp, #19
 33c:	e169f00d 	msr	SPSR_fc, sp
 340:	e1a0e00f 	mov	lr, pc
 344:	e1b0f00e 	movs	pc, lr
 348:	e24dd048 	sub	sp, sp, #72	; 0x48
 34c:	e88d1fff 	stm	sp, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, sl, fp, ip}
 350:	e51f2314 	ldr	r2, [pc, #-788]	; 44 <_armboot_start>
 354:	e2422842 	sub	r2, r2, #4325376	; 0x420000
 358:	e2422088 	sub	r2, r2, #136	; 0x88
 35c:	e892000c 	ldm	r2, {r2, r3}
 360:	e28d0048 	add	r0, sp, #72	; 0x48
 364:	e28d5034 	add	r5, sp, #52	; 0x34
 368:	e1a0100e 	mov	r1, lr
 36c:	e885000f 	stm	r5, {r0, r1, r2, r3}
 370:	e1a0000d 	mov	r0, sp
 374:	ebfffffe 	bl	0 <do_fiq>
 378:	e1a00000 	nop			; (mov r0, r0)
 37c:	e1a00000 	nop			; (mov r0, r0)
