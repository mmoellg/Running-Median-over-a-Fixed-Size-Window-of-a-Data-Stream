//Tested in https://www.onlinegdb.com
//Published on zenodo.org: 10.5281/zenodo.23670710
//Copyright (c) 2026, M. Møllgaard — MIT License
//LEAN ALGORITHM: Running Median of a Fixed-Size Window of a Data Stream
//using Basic Nonsorting Algorithm
//BEGIN PROGRAM CODE****************************************************************** 
#include <stdio.h>
#include <stdint.h> //Used for fixed-width integer types

#define NBUF            5  //[<251] ODD! Number of values in buffer X(0..U); NBUF=U+1
#define NSTREAM        10  //*TEST* Number of values from the stream
#define MEDIANTYP int16_t  //Datatype for median buffer
#define XTYPMAX     32767  //Max. pos. value for datatype in median buffer X()
#define XTYPMIN    -32768  //Min. neg. value for datatype in median buffer X()

typedef struct {      //Data type for the median using sorting
  MEDIANTYP Xmedian;  //Current median value
  uint8_t   Nbuf;     //ODD; Total number of values in buffer X(0..U)
  uint8_t   Ubuf;     //EVEN; Upper index of buffer X(0..U), where U=Nbuf-1
  uint8_t   Uhalf;    //= U/2; Index of the median
  uint8_t   UhalfP1;  //= Uhalf+1
  uint8_t   Iold;     //Index of the oldest value in buffer X(0..U)
  uint16_t  Isteps;   //Number of steps for calculating the median
  uint16_t  CntO1;    //Count O(1) calculations 
  MEDIANTYP X[NBUF];  //Fixed-size window buffer X(0..U)
} typRunMedianNoSort;

//Initializes the running-median structure for the No-Sort algorithm.
void InitRunMedianNoSort(    //Init. running median based on No-Sorting
  typRunMedianNoSort *S,     //Structure for median using a sorting algorithm
  uint8_t            Nbuf )  //ODD; Number of values in buffer X(0..U), where U=Nbuf-1
{
  S->Nbuf   = Nbuf;          //Total number of values in buffer X(0..U)
  S->Ubuf   = Nbuf - 1;      //Upper index of buffer X(0..U), where U=Nbuf-1
  S->Uhalf  = S->Ubuf >> 1;  //Index of the median in buffer X(0..U)
  S->UhalfP1= S->Uhalf + 1;  //= Uhalf+1
  S->Iold   = S->Ubuf;       //Index of oldest value in buffer X(0..U)
}

//Copyright (c) 2026, M. Møllgaard, Open-source MIT License
//Running Median of a Fixed-Size Window of a Data Stream
//Uses a basic nonsorting algorithm.
//A buffer array X() stores the values in the fixed-size window.
//For each new stream value, Xnew replaces the oldest value Xold.
//The buffer remains unsorted during median calculation.
//The algorithm balances the number of values on each side of the median.
//Values equal to the median are not tracked separately.
//Time complexity:   best case O(1), worst case O(N).
//Memory complexity: O(N); only one buffer is required.
//Cases for calculating the median:
// CASE1: O(1): The median remains unchanged. RETURN.
// CASE2: O(N): Scan for the smallest value strictly greater than the median.
// CASE3: O(N): Scan for the largest value strictly less than the median.
// CASE2 and CASE3 exception: may terminate early when enough values are found
// on the relevant side of the median, including cases with duplicate values.

void RunMedianNoSort(        //Basic Nonsorting Running Median
  MEDIANTYP          *Xnew,  //New value from the stream
  typRunMedianNoSort *M    ) //Structure for median state and buffer X(0..N)
{  
  uint8_t i;                 //Buffer index: X(0..i..N)
  uint8_t j;                 //Counts values on the relevant side of the median
  MEDIANTYP Xm, X;           //Temporary values used during the scan of buffer
  MEDIANTYP Xnext, Xold;     //Temporary values used during the scan of buffer
  M->Isteps = 0;             //*TEST* Reset buffer traversal counter
  ++M->CntO1;                //*TEST* Count this as an O(1) calculation
  if (++M->Iold > M->Ubuf)   //if next index in buffer exceeds upper boundary
    {M->Iold = 0;}           //¦ set index to first position in CIRCULAR buffer 
  Xold = M->X[M->Iold];      //Read the oldest value being replaced
  M->X[M->Iold] = *Xnew;     //Store the new value in the buffer
  Xm = M->Xmedian;           //Temporary values for the median
  if (*Xnew == Xold)         //if the new and replaced values are equal
    return;                  //¦ CASE1: RETURN; The median remains unchanged   
  if (*Xnew == Xm)           //if the the new value equals the current median
    return;                  //¦ CASE1: RETURN; The median remains unchanged
  if (*Xnew > Xm) {          //if   the new value is above the median
    if (Xold > Xm)           //¦ if the replaced value was also above the median
      return;                //¦ ¦ CASE1: RETURN; The median remains unchanged
    else //(Xold <= Xm)      //¦ else the lower-side balance may change
    {                        //¦ ¦ CASE2: Search for nearest value above median
      --M->CntO1;            //¦ ¦ *TEST* Remove the O(1) count
      j = M->UhalfP1;        //¦ ¦ Set the required count for the scan
      i = M->Nbuf;           //¦ ¦ Set the scan index to the buffer size
      Xnext = XTYPMAX;       //¦ ¦ Initialize candidate to the maximum data value
      do {                   //¦ ¦ Loop Scan buffer X(i) for i=U..0
        ++M->Isteps;         //¦ ¦ ¦ *TEST* Count this buffer access
        X = M->X[--i];       //¦ ¦ ¦ Read next value X from the buffer X(i)
        if (X <= Xm) {       //¦ ¦ ¦ if the value is at or below the median
          if (--j == 0)      //¦ ¦ ¦ ¦ if enough values are found on this side
            return;          //¦ ¦ ¦ ¦ ¦ CASE2: The median remains unchanged  
        }                    //¦ ¦ ¦ ¦ ¦ ¦      exception due duplicate values 
        else //(X > Xm)      //¦ ¦ ¦ else the value is above the median                   
          if (X < Xnext)     //¦ ¦ ¦ ¦ if it is the closest candidate found so far         
            {Xnext = X;}     //¦ ¦ ¦ ¦ ¦ Store the new candidate 
      } while (i > 0);       //¦ ¦ End Loop scanning the buffer until i==0
    }                        //¦ 
  }                          //¦ 
  else { //(Xnew < Xm)       //else the new value is below the median 
    if (Xold < Xm)           //¦ if the replaced value was also below the median
      return;                //¦ ¦ CASE1: RETURN; The median remains unchanged
    else  //(Xold >= Xm)     //¦ else the upper-side balance may change
    {                        //¦ ¦ CASE3: Search for the nearest value below median
      --M->CntO1;            //¦ ¦ *TEST* Remove the O(1) count
      j = M->UhalfP1;        //¦ ¦ Set the required count for the scan
      i = M->Nbuf;           //¦ ¦ Set the scan index to the buffer size
      Xnext = XTYPMIN;       //¦ ¦ Initialize candidate to the minimum data value
      do {                   //¦ ¦ Loop Scan buffer X(i) for i=N..0
        ++M->Isteps;         //¦ ¦ ¦ *TEST* Count this buffer access
        X = M->X[--i];       //¦ ¦ ¦ Read the next buffer value
        if (X >= Xm ) {      //¦ ¦ ¦ if the value is at or above the median
          if (--j == 0)      //¦ ¦ ¦ ¦ if If enough values are found on this side
            return;          //¦ ¦ ¦ ¦ ¦ CASE3: RETURN; The median remains unchanged
        }                    //¦ ¦ ¦ ¦ ¦ ¦      exception due to duplicate values
        else //(X < Xm)      //¦ ¦ ¦ else the value is below the median 
          if (Xnext < X)     //¦ ¦ ¦ ¦ if it is the closest candidate found so far 
            {Xnext = X;}     //¦ ¦ ¦ ¦ ¦ Store the new candidate 
      } while (i > 0);       //¦ ¦ End Loop scanning the buffer until i=0
    }                        //¦
  }                          //¦ 
  M->Xmedian = Xnext;        //Update the median to the selected candidate
}                            // 

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

int main(void) {                     //Test program
  uint16_t   Seed = 1119;            //*TEST* Seed for random generator
  uint16_t   i, j;                   //Indices for stream traversal and printing
  int16_t    Xnew;                   //New simulated value from the stream
  typRunMedianNoSort  M = {};        //Init. to 0: Running median using insertion sort
  InitRunMedianNoSort ( &M, NBUF );  //Init. running median using optimized ins. sort
  for ( i=0; i<NSTREAM; i++ ) {      //Loop through stream values
    Xnew = Xrandom( &Seed );         //¦ Generate pseudo random stream value
    RunMedianNoSort( &Xnew, &M );    //¦ Calculate running median using ins. sort
    printf("i=%04d",      i       ); //¦ Print stream-value index
    printf(" e=%02d",     M.Iold  ); //¦ Print index of oldest value in buffer
    printf(" n=%04d",     M.Isteps); //¦ Print steps for median calculation
    printf(" Xnew=%+03d", Xnew    ); //¦ Print new stream value
    printf(" X="                  ); //¦ Print "X=" before buffer values 
    for ( j=0; j<=M.Ubuf; ++j )      //¦ Loop through window buffer
      printf("%+03d,",  M.X[j]    ); //¦ ¦ Print buffer values X(0..U)
    printf(" Xm=%+03d", M.Xmedian ); //¦ Print median
    printf("\n");                    //¦ Print new line
  }                                  //End loop over stream
  printf("Q=%3d %% O(1) calculations \n",100*M.CntO1/NSTREAM );      //% O(1) path 
  printf("i=id.Stream, e=id.oldest value, n=Steps median calc., Xnew=Newest stream data\n");
  printf("X=..:Unsorted buffer, Xm=Median");
}
//END PROGRAM CODE******************************************************************** 
