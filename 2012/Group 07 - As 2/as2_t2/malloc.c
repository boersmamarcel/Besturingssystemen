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
/* Align rounds up to the nearest multiple of 8*/
#define Align(x,a)	(((x) + (a - 1)) & ~(a - 1))
#define NextSlot(p)	(* (void **) ((p) - PTRSIZE)) //pak het nextslot pointer adres (8 byte voor de user visible part)
#define NextFree(p)	(* (void **) (p)) //pak de nextfree slot (eerste adres in user visible part)


/* Examples of what align does (used inorder to find out its use: */
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
  // Up the memory with the p calculated above (len upped to the nearest multiple of BRKSIZE)
  if (_brk(p) != 0) //attempt to increse the memory, _brk returns -1 if something goes wrong (e.g. out of memory)
	return(0); //return 0: no memory left
  NextSlot((char *)_top) = p; //set the next slot of _top to the new slot at p
  NextSlot(p) = 0; //set the next slot of p to 0
  free(_top); //free the slot at _top
  _top = p; //set the current top at p
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

	/* take the requested size, round up to closest multiple of PTRSIZE, add PTRSIZE */
	unsigned len = Align(size, PTRSIZE) + PTRSIZE; 
	if (len < 2 * PTRSIZE) { // Overflowcheck
		errno = ENOMEM;
		return NULL;
	}
	if (_bottom == 0) { // check if _bottom is not yet set
		if ((p = _sbrk(2 * PTRSIZE)) == (char *) -1) //create space for the first slot
			return NULL; //no space
		p = (char *) Align((ptrint)p, PTRSIZE); 
		p += PTRSIZE; //add pointer size
		_top = _bottom = p; //both _top and _bottom pointer start at p
		NextSlot(p) = 0; //Set the next slot of p to 0 (i.e. there is no new slot yet)
	}
#ifdef SLOWDEBUG
	for (p = _bottom; (next = NextSlot(p)) != 0; p = next)
		assert(next > p);
	assert(p == _top);
#endif
	// Look for a a free slot in the freeslot-linkedlist, no free slot, skip
	for (prev = 0, p = _empty; p != 0; prev = p, p = NextFree(p)) {
		// Check if this slot is big enough for the requested size
		next = NextSlot(p); //fetch the address of the next slot
		new = p + len;	/* easily overflows!! */ /* FIX HET DAN! */
		if (new > next || new <= p) 
		//if the calculated address of the new slot will be higher than the next slot in line.
			continue;		/* it's too small */
		if (new + PTRSIZE < next) {	/* it's too big, so split */
			/* + PTRSIZE avoids tiny slots on free list */
			NextSlot(new) = next;
			NextSlot(p) = new;
			NextFree(new) = NextFree(p);
			NextFree(p) = new;
		}
		if (prev) //prev is not null
			NextFree(prev) = NextFree(p);
		else
			_empty = NextFree(p); //prev is null, the last empty slot is new p
		return p; //and we're done
	}
	// no new slot find.
	if (grow(len) == 0) //try to allocate more memory
		break; //NOPE, this goes wrong
  }
  // make sure the loop has been run twice
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
  //Walk through the freeslot list, look for a next slot that has an higher address than p
  //The list is inorder of the memory
  for (prev = 0, next = _empty; next != 0; prev = next, next = NextFree(next))
	if (p < next)
		break;
  //puts p in the linked list.
  NextFree(p) = next;
  if (prev)
	NextFree(prev) = p;
  else
	_empty = p;
  
  //Merge the possible adjacent freeslots
  if (next) { //look if there is a slot next to this one
	assert((char *) NextSlot(p) <= next);
	if (NextSlot(p) == next) {		/* merge p and next */
		NextSlot(p) = NextSlot(next);
		NextFree(p) = NextFree(next);
	}
  }
  if (prev) {//look if there is a slot directly before p
	assert((char *) NextSlot(prev) <= p);
	if (NextSlot(prev) == p) {		/* merge prev and p */
		NextSlot(prev) = NextSlot(p);
		NextFree(prev) = NextFree(p);
	}
  }
}

