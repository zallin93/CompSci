// memory-memory model

// load base address into r0
loadI 1024 => r0

// load the value of 'a'
loadI 5 => r1
// store a in memory, offset 0 from base
storeAI r1 => r0, 0

// load the vaule of 'result'
loadI 1 => r1
// store result in memory, offset 4 from base
storeAI r1 => r0, 4

loadI 1 => r3

while:
        loadAI r0, 0 => r1
	cmp_GT r1, r3 => r4
	cbr r4, contin, exit
contin:
        loadAI r0, 4 => r1 // result
        loadAI r0, 0 => r2 // a
	mult r1, r2  => r1
	storeAI r1 => r0, 4 // store 'result'
	subI r2, 1 => r2
	storeAI r2 => r0, 0 // store 'a'
	br while
exit:
	// print the result
	loadI 1020 => r1
        loadAI r0, 4 => r2
	store r2 => r1
	output 1020
