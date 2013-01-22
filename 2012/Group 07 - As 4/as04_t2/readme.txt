In dit bestand leggen we per script uit wat er fout aan is.

De verbeterde versies zijn telkens safe_N.c

script 1:
	onveilige code:
		int main( int ac, char** av ) {
		    char buf[90];
		    if( ac > 1 )
			strcpy( buf, av[1] );
		}
	veilige code:
		#include <stdio.h>
		#include <string.h>
		#include <stdlib.h>

		int main( int ac, char** av ) {
			if( ac > 1 ) {
				char *buf = malloc(strlen(av[1]));
				strcpy( buf, av[1] );
			}
		}
	
	Door een argument mee te geven dat langer is dan de buffer kan bevatten (90 karakters) kan je data buiten de gereserveerde buffer plaatsen.
	Het misbruiken hiervan heet "stack smashing", en is vrij makkelijke te demonstreren op de oude code met het volgende commando:
	cc buffOverflow.c && ./a.out "aaaaa(90 tekens)aaaaa [en hier de nare code om te hacken]"

script 2:
	onveilige code;
		int main( int ac, char** av ) {
		    printf( av[1] );
		}
	
	verbeterde versie;
		#include <string.h>
		
		int main( int ac, char** av ) {
		    printf("%s", av[1] );
		}
	
	Hier kan "format string attack" worden toegepast. Doordat av als eerste argument wordt meegegeven kunnen we er formats aan meegeven.
	Bijvoorbeeld:
	$./a.out "%x %x %x %x"
	
	Hierdoor kunnen we stukken uit de stack uitlezen, wat natuurlijk niet de bedoeling is!
	Verder kan je met %n ook nog eens de stack gaan aanpassen, en zo (bijvoorbeeld) privlege elevation toepassen.
	
	(hier komt ook al een waarschuwing van bij het compileren mbv cc)
	
script 3:
	