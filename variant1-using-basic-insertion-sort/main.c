//Tested in https://www.onlinegdb.com
//Published on zenodo.org: https://doi.org/10.5281/zenodo.23601248
//The C-Code is optimized for MSP430FR2355

//BEGIN PROGRAM CODE******************************************************************** 
#include <stdio.h>
#include <stdint.h> //Used for fixed-width integer types

#define NBUF            5  //[<255] ODD! Number of values in buffer X(0..U); NBUF=U+1
#define NSTREAM        10  //*TEST* Number of values from the stream
#define MEDIANTYP int16_t  //Datatype for median buffer

typedef struct {      //Data type for the median using sorting
  MEDIANTYP Xmedian;  //Current median value
  uint8_t   Nbuf;     //ODD; Total number of values in buffer X(0..U)
  uint8_t   Ubuf;     //EVEN; Upper index of buffer X(0..U), where U=Nbuf-1
  uint8_t   Uhalf;    //= U/2; Index of the median
  uint8_t   Iold;     //Index of the oldest value in buffer X(0..U)
  uint16_t  Isteps;   //Number of steps for calculating the median
  MEDIANTYP X[NBUF];  //Fixed-size window buffer X(0..U)
  MEDIANTYP S[NBUF];  //Temporary buffer for sorting S(0..U)
} typRunMedianSort;

//Generates a pseudo-random integer using a 16-bit seed.
//Uses integer arithmetic without multiplication or division.
int16_t Xrandom(         //*TEST* Function to generate a pseudo-random number
  uint16_t *seed   )     //Seed for generating pseudo-random numbers
{
  *seed ^= *seed << 7;
  *seed ^= *seed >> 9;
  *seed ^= *seed << 8;
   return (int16_t)((*seed & 63) + ((*seed >> 8) & 127))-95; //Range: -95..95
}

//Initializes the running-median structure for the sorting algorithm.
void InitRunMedianSort(     //Init. running median based on sorting
  typRunMedianSort *S,      //Structure for median using a sorting algorithm
  uint8_t          Nbuf )   //ODD; Number of values in buffer X(0..U), where U=Nbuf-1
{
  S->Nbuf  = Nbuf;          //Total number of values in buffer X(0..U)
  S->Ubuf  = Nbuf - 1;      //Upper index of buffer X(0..U), where U=Nbuf-1
  S->Uhalf = S->Ubuf >> 1;  //Index of the median in buffer X(0..U)
  S->Iold  = S->Ubuf;       //Index of oldest value in buffer X(0..U)
}

//Lean Algorithm: (c) 2026, M. Møllgaard, Open-source MIT License
//Calculates the running median for a fixed-size window of a data stream.
//Uses a code-level optimized Insertion Sort algorithm.
//Copies the fixed-size window buffer to a separate buffer for sorting.
//Beware: The entire window is sorted even when only one value changes.
//For each new stream value, replaces the oldest value in the window buffer.
//Returns the middle value of the sorted buffer as the running median.
//Time complexity: best case O(N), worst case O(N²), memory complexity: O(N).
//Insertion Sort is a well-known sorting method.
//The implementation presented here is original code released under the MIT License.

void RunMedianInsSort(       //Running median using basic Insertion-Sort algorithm
  MEDIANTYP        *Xnew,    //New value from the stream
  typRunMedianSort *S    )   //Structure for median incl. buffer X(0..U)
{
  uint8_t j, e;              //Indices for insertion sort
  uint8_t i = S->Nbuf;       //Index for copying and sorting
  MEDIANTYP Xinsert;         //Value to be inserted
  if (++S->Iold > S->Ubuf)   //if next oldest-value index exceeds buffer end
    {S->Iold = 0;}           //¦ Wrap to first value in CIRCULAR buffer
  S->X[S->Iold] = *Xnew;     //Replace oldest value with new stream value
  S->Isteps = 0;             //*TEST* Reset counter for median calculation steps
  while (i != 0) {           //Loop: Copy buffer values to sorting buffer
    --i;                     //¦ Next index to be copied
    S->S[i] = S->X[i];       //¦ Copy value to sorting buffer
    ++S->Isteps;             //¦ *TEST* Count steps for median calculation
  }                          //End loop
  //i = 0 already set above  //Start sorting at first position
  while (i < S->Ubuf) {      //Loop: Sort values using insertion sort
    j = i;                   //¦ Index of preceding sorted value
    e = ++i;                 //¦ Index of value to insert
    Xinsert = S->S[e];       //¦ Value to insert into sorted order
    ++S->Isteps;             //¦ *TEST* Count steps for median calculation
    while (Xinsert<S->S[j]){ //¦ Loop: Shift larger values one position right
      ++S->Isteps;           //¦ ¦ *TEST* Count steps for median calculation
      S->S[e] = S->S[j];     //¦ ¦ Move preceding value to the right
      e = j--;               //¦ ¦ Update insertion position and move left
      if (e == 0) break;     //¦ ¦ if at start of buffer then RETURN
    }                        //¦ End loop
    S->S[e] = Xinsert;       //¦ Insert value at the correct position
  }                          //End loop
  S->Xmedian=S->S[S->Uhalf]; //Median at the middle of the sorted buffer
}

int main(void) {             //Test program
  uint16_t   Seed = 1119;    //*TEST* Seed for random generator
  uint16_t   i,j;            //Indices for stream traversal and printing
  int16_t    Xnew;           //New simulated value from the stream
  typRunMedianSort M = {0};  //Running median based on insertion sort; init. to 0
  InitRunMedianSort(&M, NBUF);       //Init. running median using basic insertion sort
  for (i=0; i<NSTREAM; i++) {        //Loop through stream values
    Xnew = Xrandom(&Seed);           //¦ Generate simulated stream value
    RunMedianInsSort(&Xnew, &M);     //¦ Calculate running median using insertion sort
    printf("i=%04d",      i       ); //¦ Print stream-value index
    printf(" e=%02d",     M.Iold  ); //¦ Print index of oldest value in buffer
    printf(" n=%04d",     M.Isteps); //¦ Print steps for median calculation
    printf(" Xnew=%+03d", Xnew    ); //¦ Print new stream value
    printf(" X="                  ); //¦ Print buffer label
    for (j=0; j<=M.Ubuf; j++)        //¦ Loop through window buffer
      printf("%+03d,", M.X[j]);      //¦ ¦ Print buffer values X(0..U)
    printf(" Xm=%+03d", M.Xmedian);  //¦ Print median
    printf(" S=");                   //¦ Print sorted-buffer label
    for (j=0; j<=M.Ubuf; j++)        //¦ Loop through sorted buffer
      printf("%+03d,", M.S[j]);      //¦ ¦ Print sorted values S(0..U)
    printf("\n");                    //¦ Print new line
  }                                  //End loop over stream
  printf("i:Stream index, e:Oldest-value index, n:Steps for median calculation\n");
  printf("X=..:Window buffer, Xm:Median, S=..:Sorted buffer");
}
//END PROGRAM CODE******************************************************************** 
     
