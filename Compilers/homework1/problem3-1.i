// problem 3.1
// iloc code for register-register

// load the base address into 1024
loadI 1024 => r0

loadI 1 => r1 		// a =1
loadI 2 => r2 		// b =2
subI r2, 4 => r3  	// c = b-4
add r1, r2 => r4 	// d = a+b
addI r4, 1 => r5	// e = d+1
mult r3, r5 => r6	// f = e - c * e
sub r5, r6 => r7  	// f is really in r7
add r4, r5 => r8	// g = (d + e) + f
add r7, r8 => r9 	// g is really in r9
add r9, r1 => r10	// h = g + a

loadI 1020 => r11
store r10 => r11
output 1020