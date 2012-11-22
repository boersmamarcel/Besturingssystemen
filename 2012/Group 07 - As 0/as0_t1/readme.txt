1. We installed VMWare fusion (for Mac) and installed the Minix image
2. We created dummy.c with the appropriate code in /usr/src/lib/libc/other/
3. dummy.c was added to the makefile ( /usr/src/lib/libc/other/Makefile.inc)
4. The dummy() function was added in the stdlib.h header file in /usr/src/include/stdlib.h
5. Executed "make libraries"
6. Executed "make includes"
7. as0_t1.c compiled using "cc -Wall as0_t1.c"
8. Executed ./a.out and retrieved the output below



We will now invoke the dummy command in libc. If your implementation would print anything to the standard output, it should be printed here as well.
The test ends with the line "[End of Test]". If there is no such output, the program crashed and you're not done yet :).
-- Your friendly BeSys instructors.

[Begin of Test]
You're a dummy.

[End of Test]