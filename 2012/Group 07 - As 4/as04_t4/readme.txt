De basis van de gebruikte exploit komt van http://www.phrack.org/issues.html?id=14&issue=49.

Aangepast is de werking. In plaats van een environment var aan te maken die in een nieuwe shell gebruikt kan worden, print onze versie van exploit.c de bufferoverflow direct naar de stdout. Met start.c kan deze output als parameter worden meegegeven aan as4_t4.

We hebben de lengte van de bufferoverflow bepaald door het adreswaarde 0x78 (dec 120) uit de dump te halen, en net zo lang te proberen tot we in de shell kwamen.

Uitvoeren:

Stap 1: anti-hack opties uitzetten:
$> sudo bash -c "echo 0 > /proc/sys/kernel/exec-shield"
$> sudo bash -c "echo 0 > /proc/sys/kernel/randomize_va_space"

Stap 2: compileren:
$> make

Stap 3: uitvoeren:
$> ./start

Als de bufferoverflow succesvol uitgevoerd wordt, wordt er een nieuwe shell geopend.