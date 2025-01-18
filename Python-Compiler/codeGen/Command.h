#pragma once

enum Command {
	_new = 0xBB,
	
	istore = 0x36,
	astore = 0x3A,
	aload = 0x19,

	sipush = 0x11,

	ldc_w = 0x13,

	invokevirtual = 0xB6,
	invokespecial = 0xB7,

	dup = 0x59,

	ireturn = 0xAC,
	areturn = 0xB0,
	_return = 0xB1,
};
