// register-register model

// load base address into r0
loadI 1024 => r0

// load the value of 'a'
loadI 5 => r1

// load the vaule of 'result'
loadI 1 => r2

loadI 1 => r5

while:
	cmp_GT r1, r5 => r3
	cbr r3, contin, exit
contin:
	mult r2, r1 => r2
	subI r1, 1 => r1
	br while
exit:
	// print the result
	loadI 1020 => r4
	store r2 => r4
	output 1020
