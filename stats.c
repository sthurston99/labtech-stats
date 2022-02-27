/* stats_safest.c - work around brittle input system
 *
 * D Provine 28 Jan 2014
 * S Thurston 21 Feb 2020
 *
 * fgets() returns the next line of input, cutting it off if it's
 * longer than the size of the buffer being read.
 *
 * sscanf() does what scanf() does, but it works on a string that's
 * already been read in.  If something goes wrong, that string, in
 * program memory, can be examined with a debugger.
 *
 * To see this in, compile this program with "-g", and then
 * run it in GDB, type "q" or something, and when it loops hit
 * ^C.  Then try to examine the data with "print lineread".
 *
 * See also "stats_broken.c" and "stats_discard.c".
 */

#include <stdio.h>

#define BUFSIZE 256

int main()
{
    int    itemsread = 0;
    double item = -1;     // initial value so loop starts
    int    howmany = 0;
    double    sum;
    double    max;
    double    min;
    char   line[BUFSIZE]; // fgets() knows about the NULL

    // check last item wasn't zero; if not
    // read the line from input and copy it to memory as a string
    while ( item != 0 && fgets(line, BUFSIZE, stdin) != NULL ) {

        // scan the number from the line - extra stuff on line is ignored
        itemsread = sscanf(line, "%lf", &item);

        // check to see if we got valid input
        if (itemsread == 1 && item != 0) {
	  if (howmany == 0){
	    min = item;
	    max = item;
	  }
            howmany++;
	  if (item > max){
	    max = item;
	  }
	  if (item < min){
	    min = item;
	  }
	    sum += item;
        }

        // this will not get stuck in an infinite loop,
        // plus we can examine the input data
        if (itemsread == 0) {
            printf("bad input\n");
        }
    }
    printf("# items: %d\n", howmany);
    if (howmany !=0){
      printf("Sum: %lf\n", sum);
      printf("Maximum: %lf\n",max);
      printf("Minimum: %lf\n", min);
      printf("Mean: %lf\n", sum/howmany);
    }
    return 0;
}
