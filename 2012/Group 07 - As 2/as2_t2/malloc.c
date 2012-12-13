/* $Header$ */

/* replace undef by define */
#define  ALIGN_EIGHT_BYTES /* Use 8-byte alignment. */
#define	 DEBUG		   /* check assertions */
#undef	 SLOWDEBUG	   /* some extra test loops (requires DEBUG) */

#ifndef DEBUG
#define NDEBUG
#endif

#include	<stdlib.h>
#include	<string.h>
#include	<errno.h>
#include	<assert.h>

#if _EM_WSIZE == _EM_PSIZE
#define	ptrint		int
#else
#define	ptrint		long
#endif

#if	_EM_PSIZE == 2
#define BRKSIZE		1024
#else
#define BRKSIZE		4096
#endif
#ifdef ALIGN_EIGHT_BYTES
#define PTRSIZE		8
#else
#define	PTRSIZE		((int) sizeof(void *))
#endif
/* Align pakt a -1, telt dat op bij x, en bitwise AND het met de inverse van a - 1 */
#define Align(x,a)	(((x) + (a - 1)) & ~(a - 1))
// maak p magisch met sterrenstof
#define NextSlot(p)	(* (void **) ((p) - PTRSIZE)) //pak het nextslot pointer adres (8 byte voor de user visible part)
#define NextFree(p)	(* (void **) (p)) //pak de nextfree slot (eerste adres in user visible part)


/* Wat de @#$& doet align, vijf voorbeelden: */
/* rond af naar de bovenste meervoud van a(PTRSIZE) */
x = 	0001 0000 //16
a =  	0000 1000
a-1 = 	0000 0111
x+a-1=	0001 0111
~(a-1) =1111 1000
& 	   =0001 0000 //16


x = 	0001 0001 //17
a =  	0000 1000
a-1 = 	0000 0111
x+a-1=  0001 1000
~(a-1) =1111 1000
&       0001 1000 //24

x =		0001 1000 //24
a =  	0000 1000
a-1 = 	0000 0111
x+a-1 = 0001 1111
~(a-1) =1111 1000
&  	= 	0001 1000 // 24

x =		0000 0011 //3
a =  	0000 1000
a-1 = 	0000 0111
x+a-1 = 0000 1010
~(a-1) =1111 1000
&  	= 	0000 1000 //8

x =		  0000 0000 1111 1111 //255
a =  	  0000 0000 0000 1000
a-1 = 	  0000 0000 0000 0111
x+a-1 =   0000 0001 0000 0110 
~(a-1) =  1111 1111 1111 1000
&  	= 	  0000 0001 0000 0000 //256:
/*
 * A short explanation of the data structure and algorithms.
 * An area returned by malloc() is called a slot. Each slot
 * contains the number of bytes requested, but preceeded by
 * an extra pointer to the next the slot in memory.
 * '_bottom' and '_top' point to the first/last slot.
 * More memory is asked for using brk() and appended to top.
 * The list of free slots is maintained to keep malloc() fast.
 * '_empty' points  the first free slot. Free slots are
 * linked together by a pointer at the start of the
 * user visable part, so just after the next-slot pointer.
 * Free slots are merged together by free().
 *
 * Since computers came out of the stoneage, processors prefer 8-byte alignment, we now pretend
 * our pointers are 8 bytes wide.
 */

extern void *_sbrk(int);
extern int _brk(void *);
static void *_bottom, *_top, *_empty;

static int grow(size_t len)
{
  register char *p;

  assert(NextSlot((char *)_top) == 0);
  if ((char *) _top + len < (char *) _top // Overflow check op de _top + len
      || (p = (char *)Align((ptrint)_top + len, BRKSIZE)) < (char *) _top ) { //overflow check als ceiled op BRKSIZE 
	errno = ENOMEM;
	return(0);
  }
  // Jas er in een keer maar een aantal keer BRKSIZE bij (len geceiled op BRKSIZE)
  if (_brk(p) != 0) //probeer memory op te hogen, _brk returnt -1 als er iets fout gaat
	return(0); //return 0: er ging iets fout
  NextSlot((char *)_top) = p; //zet de next slot van _top naar p
  NextSlot(p) = 0; //zet de next slot van p naar 0
  free(_top); //maak een free slot van de nieuw gemaakte data
  _top = p; //zet de _top van de memory shit op p.
  return 1;
}

void * malloc(const size_t size) {
  register char *prev, /* Points to the prev */
  	 *p, /* Points to the p */
  	 *next,   /* Points to the next */
  	 *new;  /* Points to the new */
  unsigned ntries; /* Holds the current try, 0 or 1 */

  /* Obviously, if the size of the requested memory is zero, return NULL */
  if (size == 0)
	return NULL;
  
  /* Try to run this two times,
   	first time: check for current free slots
   	second time: grow has created a new free slot at the top of the memoryspace, so use that
   	*/
  for (ntries = 0; ntries < 2; ntries++) {

	/* Neem size afgerond naar boven op meervoud PTRSIZE, tel PTRSIZE er bijop */
	unsigned len = Align(size, PTRSIZE) + PTRSIZE; 
	if (len < 2 * PTRSIZE) { // Overflowcheck: Check of de size van het blok in ptrsize niet groter is dan maximale memory, anders out of memory
		errno = ENOMEM;
		return NULL;
	}
	if (_bottom == 0) { //_bottom is nog niet geset (immers, 0 is geen geldig adres)
		if ((p = _sbrk(2 * PTRSIZE)) == (char *) -1) //Maak ruimte voor de eerste slot
			return NULL; //Geen ruimte, err err
		p = (char *) Align((ptrint)p, PTRSIZE); 
		p += PTRSIZE; //tel nogmaals ptrsize bij p op
		_top = _bottom = p; //de top en de bottom beginnen bij p
		NextSlot(p) = 0; //Zet de volgende  slot van p op 
	}
#ifdef SLOWDEBUG
	for (p = _bottom; (next = NextSlot(p)) != 0; p = next)
		assert(next > p);
	assert(p == _top);
#endif
	// Zoek naar een free slot in de freeslot-linkedlist, geen free slot: skip
	for (prev = 0, p = _empty; p != 0; prev = p, p = NextFree(p)) {
		// Check of dit freeslot groot genoeg is.
		next = NextSlot(p); //pak adres volgende slot
		new = p + len;	/* easily overflows!! */ /* FIX HET DAN! */
		if (new > next || new <= p) 
		//als het berekende adres van het nieuwe slot verder ligt dan het eerst volgende slot, dan is het huidige freeslot te klein! 
			continue;		/* too small */
		if (new + PTRSIZE < next) {	/* too big, so split */
			/* + PTRSIZE avoids tiny slots on free list */
			/* bovenstaande is helder, split als te groot */
			NextSlot(new) = next;
			NextSlot(p) = new;
			NextFree(new) = NextFree(p);
			NextFree(p) = new;
		}
		if (prev) //prev is niet null
			NextFree(prev) = NextFree(p);
		else
			_empty = NextFree(p); //prev is null, dus het laatste empty slot in de freeslots list is nu de laatste van p.
		return p; // Alweer een winnaar!
	}
	// geen nieuw slot gevonden.
	if (grow(len) == 0) //probeer nieuw memory te allocaten
		break; //NOPE, dit gaat fout.
  }
  // beter is dit wel twee keer gedaan.
  assert(ntries != 2);
  return NULL;
}

void *
realloc(void *oldp, size_t size)
{
  register char *prev, *p, *next, *new;
  char *old = oldp;
  register size_t len, n;

  if (old == 0)
	return malloc(size);
  if (size == 0) {
	free(old);
	return NULL;
  }
  len = Align(size, PTRSIZE) + PTRSIZE;
  next = NextSlot(old);
  n = (int)(next - old);			/* old length */
  /*
   * extend old if there is any free space just behind it
   */
  for (prev = 0, p = _empty; p != 0; prev = p, p = NextFree(p)) {
	if (p > next)
		break;
	if (p == next) {	/* 'next' is a free slot: merge */
		NextSlot(old) = NextSlot(p);
		if (prev)
			NextFree(prev) = NextFree(p);
		else
			_empty = NextFree(p);
		next = NextSlot(old);
		break;
	}
  }
  new = old + len;
  /*
   * Can we use the old, possibly extended slot?
   */
  if (new <= next && new >= old) {		/* it does fit */
	if (new + PTRSIZE < next) {		/* too big, so split */
		/* + PTRSIZE avoids tiny slots on free list */
		NextSlot(new) = next;
		NextSlot(old) = new;
		free(new);
	}
	return old;
  }
  if ((new = malloc(size)) == NULL)		/* it didn't fit */
	return NULL;
  memcpy(new, old, n);				/* n < size */
  free(old);
  return new;
}

void free(void *ptr) {
  register char *prev, *next;
  char *p = ptr; //puts the ptr in p as a char

  if (p == 0) //p is zero, so no use in freeing that
	return;

  //removed slowdebug, for debugging purposes

  assert((char *) NextSlot(p) > p);
  //Loop door de freelist list, zoek naar een next die meer is dan p
  //de list is opvolgorde
  for (prev = 0, next = _empty; next != 0; prev = next, next = NextFree(next))
	if (p < next)
		break;
  //Plaats p in de linkedlist van vrije slots
  NextFree(p) = next;
  if (prev)
	NextFree(prev) = p;
  else
	_empty = p;
  
  //Merge the possible adjacent freeslots
  if (next) { //Kijk of er een freeslot direct na p is
	assert((char *) NextSlot(p) <= next);
	if (NextSlot(p) == next) {		/* merge p and next */
		NextSlot(p) = NextSlot(next);
		NextFree(p) = NextFree(next);
	}
  }
  if (prev) {//Kijk of er een freeslot direct voor p is
	assert((char *) NextSlot(prev) <= p);
	if (NextSlot(prev) == p) {		/* merge prev and p */
		NextSlot(prev) = NextSlot(p);
		NextFree(prev) = NextFree(p);
	}
  }
}

