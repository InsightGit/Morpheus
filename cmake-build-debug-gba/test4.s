
@{{BLOCK(test4)

@=======================================================================
@
@	test4, 32x32@4, 
@	+ palette 256 entries, not compressed
@	+ 16 tiles Metatiled by 4x4 not compressed
@	Total size: 512 + 512 = 1024
@
@	Time-stamp: 2020-12-22, 11:22:42
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.15
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global test4Tiles		@ 512 unsigned chars
	.hidden test4Tiles
test4Tiles:
	.word 0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0xF5000000,0x5F000000,0x5F000000
	.word 0x00000000,0x00000000,0xE3000000,0xF08888E0,0x60F77780,0x990001FF,0x55555555,0x55555555
	.word 0x00000000,0xDDDD1F50,0x444441AE,0xBBBB4444,0x66666666,0x48984999,0xB959B555,0xB959B555
	.word 0x00000000,0x0000008D,0x000000C4,0x000084CB,0x00004066,0x00006FA9,0x000000F5,0x000000F5
	.word 0x55000000,0x6B000000,0x60000000,0x60000000,0x24000000,0x60000000,0x12000000,0x60000000
	.word 0x55555555,0x55555555,0x55555555,0x55555555,0x66666666,0x6B000002,0x125555AB,0x67655517
	.word 0xB959B555,0x39593555,0x55555555,0x55555555,0x66666666,0x66666666,0x11111111,0x66666666
	.word 0x000000F5,0x000000F5,0x000000F5,0x000000F5,0x00000053,0x00000007,0x0000000B,0x00000007

	.word 0x3B000000,0x1B000000,0x60000000,0x12000000,0x60000000,0x3B000000,0x1B000000,0x60000000
	.word 0x32655512,0x12655512,0x67699917,0x12F111EB,0x67F111E7,0x32EFFF92,0x17777777,0x66666666
	.word 0x33333333,0x11111111,0x66666666,0x11111111,0x66666666,0x33333333,0x11111111,0x66666666
	.word 0x00000002,0x00000002,0x00000007,0x0000000B,0x00000007,0x00000002,0x00000002,0x00000007
	.word 0xEE000000,0x20000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
	.word 0x44EAAAAA,0xCCDAAAA9,0xCCDAAA3D,0xC7DAAA3D,0xCAAAAA3D,0xAAAAAA3D,0x222222D8,0xDDDDDDD0
	.word 0x44444444,0xCCCCCCCC,0x1CCCCCCC,0x1CCCCCCC,0x1CCCCCCC,0x1CCCCCC7,0x8C33333C,0x0DDDDDDD
	.word 0x00000004,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global test4Pal		@ 512 unsigned chars
	.hidden test4Pal
test4Pal:
	.hword 0x0000,0x4586,0x5075,0x4B86,0x51D5,0x6EAB,0x5E29,0x56B5
	.hword 0x0802,0x6CBC,0x66D0,0x7F75,0x6D9C,0x44D1,0x4631,0x3DFF
	.hword 0x0842,0x0842,0x0842,0x0842,0x0842,0x0842,0x0842,0x0842
	.hword 0x0C63,0x0C63,0x0C63,0x0C63,0x0C63,0x0C63,0x0C63,0x0C63
	.hword 0x1084,0x1084,0x1084,0x1084,0x1084,0x1084,0x1084,0x1084
	.hword 0x14A5,0x14A5,0x14A5,0x14A5,0x14A5,0x14A5,0x14A5,0x14A5
	.hword 0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6,0x18C6
	.hword 0x1CE7,0x1CE7,0x1CE7,0x1CE7,0x1CE7,0x1CE7,0x1CE7,0x1CE7

	.hword 0x2108,0x2108,0x2108,0x2108,0x2108,0x2108,0x2108,0x2108
	.hword 0x2529,0x2529,0x2529,0x2529,0x2529,0x2529,0x2529,0x2529
	.hword 0x294A,0x294A,0x294A,0x294A,0x294A,0x294A,0x294A,0x294A
	.hword 0x2D6B,0x2D6B,0x2D6B,0x2D6B,0x2D6B,0x2D6B,0x2D6B,0x2D6B
	.hword 0x318C,0x318C,0x318C,0x318C,0x318C,0x318C,0x318C,0x318C
	.hword 0x35AD,0x35AD,0x35AD,0x35AD,0x35AD,0x35AD,0x35AD,0x35AD
	.hword 0x39CE,0x39CE,0x39CE,0x39CE,0x39CE,0x39CE,0x39CE,0x39CE
	.hword 0x3DEF,0x3DEF,0x3DEF,0x3DEF,0x3DEF,0x3DEF,0x3DEF,0x3DEF

	.hword 0x4210,0x4210,0x4210,0x4210,0x4210,0x4210,0x4210,0x4210
	.hword 0x4631,0x4631,0x4631,0x4631,0x4631,0x4631,0x4631,0x4631
	.hword 0x4A52,0x4A52,0x4A52,0x4A52,0x4A52,0x4A52,0x4A52,0x4A52
	.hword 0x4E73,0x4E73,0x4E73,0x4E73,0x4E73,0x4E73,0x4E73,0x4E73
	.hword 0x5294,0x5294,0x5294,0x5294,0x5294,0x5294,0x5294,0x5294
	.hword 0x56B5,0x56B5,0x56B5,0x56B5,0x56B5,0x56B5,0x56B5,0x56B5
	.hword 0x5AD6,0x5AD6,0x5AD6,0x5AD6,0x5AD6,0x5AD6,0x5AD6,0x5AD6
	.hword 0x5EF7,0x5EF7,0x5EF7,0x5EF7,0x5EF7,0x5EF7,0x5EF7,0x5EF7

	.hword 0x6318,0x6318,0x6318,0x6318,0x6318,0x6318,0x6318,0x6318
	.hword 0x6739,0x6739,0x6739,0x6739,0x6739,0x6739,0x6739,0x6739
	.hword 0x6B5A,0x6B5A,0x6B5A,0x6B5A,0x6B5A,0x6B5A,0x6B5A,0x6B5A
	.hword 0x6F7B,0x6F7B,0x6F7B,0x6F7B,0x6F7B,0x6F7B,0x6F7B,0x6F7B
	.hword 0x739C,0x739C,0x739C,0x739C,0x739C,0x739C,0x739C,0x739C
	.hword 0x77BD,0x77BD,0x77BD,0x77BD,0x77BD,0x77BD,0x77BD,0x77BD
	.hword 0x7BDE,0x7BDE,0x7BDE,0x7BDE,0x7BDE,0x7BDE,0x7BDE,0x7BDE
	.hword 0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF,0x7FFF

@}}BLOCK(test4)
