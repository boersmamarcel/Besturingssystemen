Benchmark van malloc vs. onze oplossing:

# cc benchmark.c solution.c
# ./a.out
Benchmarking Malloc:
Time: 400000 microsec

Benchmarking our solution:
Time: 233333 microsec

# ./a.out
Benchmarking Malloc:
Time: 383334 microsec

Benchmarking our solution:
Time: 250000 microsec

# ./a.out
Benchmarking Malloc:
Time: 416666 microsec

Benchmarking our solution:
Time: 266667 microsec


We zien dat onze oplossing bijna 2 keer zo snel is. 
Wat ons opviel, is dat de time functie zoals door de kernel geleverd wordt. De door ons gebruikte functie "gettimeofday()" doet het best aardig, maar we zien dat hij niet echt op microseconden nauwkeurig is. In plaats daarvan geeft hij een benadering ervan, maar deze is al goed genoeg om te laten zien dat onze oplossing significant sneller is dan malloc.
