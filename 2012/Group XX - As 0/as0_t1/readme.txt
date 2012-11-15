1. We hebben eerst minix geinstalleerd.
2. Vervolgens de dummy.c in de /usr/src/lib/libc/other/ map gezet
3. We hebben dummy.c toegevoegd aan de makefile.inc
4. vervolgens hebben we de dummy functie gedefined in de stdlib.h in /usr/src/include/stdlib.h
5. we hebben make libraries uitgevoerd
6. we hebben make includes uitgevoerd
7. we as0_t1.c gecompiled met cc -Wall as0_t1.c command
8. vervolgens hebben we uitgevoerd met ./a.out en het gaf de gewenste output!!! TADA