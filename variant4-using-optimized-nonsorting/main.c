//Tested in https://www.onlinegdb.com
//Published on zenodo.org: 10.5281/zenodo.23671391
//LEAN ALGORITHM: Running Median of a Fixed-Size Window of a Data Stream
//  using Optimized Nonsorting Algorithm
//Copyright (c) 2026, M. Møllgaard — MIT License
//BEGIN PROGRAM CODE******************************************************************** 
#include <stdio.h>
#include <stdint.h> //Used for fixed-width integer types

#define NBUF           15  //[<251] ODD! Number of values in buffer X(0..U); NBUF=U+1
#define NSTREAM       100  //*TEST* Number of values from the stream
#define MEDIANTYP int16_t  //Datatype for median buffer
#define XTYPMAX     32767  //Max. pos. value for datatype in median buffer X()
#define XTYPMIN    -32768  //Min. neg. value for datatype in median buffer X()

typedef struct {      //Data type for the median using nonsorting
  MEDIANTYP Xmedian;  //Current median value
  uint8_t   Nbuf;     //ODD; Total number of values in buffer X(0..U)
  uint8_t   Ubuf;     //EVEN; Upper index of buffer X(0..U), where U=Nbuf-1
  uint8_t   Uhalf;    //= U/2; Index of the median
  uint8_t   Iold;     //Index of the oldest value in buffer X(0..U)
  uint8_t   CntUp;    //Count values above the median; On the upper-side
  uint8_t   CntLo;    //Count values below the median; On the lower-side   
  uint16_t  Isteps;   //Number of steps for calculating the median
  uint16_t  CntO1;    //Count O(1) calculations 
  MEDIANTYP X[NBUF];  //Fixed-size window buffer X(0..U)
} typRunMedianNoSort;

//Initializes the running-median structure for the No-Sort algorithm.
void InitRunMedianNoSort(     //Init. running median based on optimized nonSorting
  typRunMedianNoSort *M,      //Structure for median using a sorting algorithm
  uint8_t            Nbuf )   //ODD; Number of values in buffer X(0..U), where U=N-1
{
  M->Nbuf    = Nbuf;          //Total number of values in buffer X(0..U)
  M->Ubuf    = Nbuf - 1;      //Upper index of buffer X(0..U), where U=Nbuf-1
  M->Uhalf   = M->Ubuf >> 1;  //Index of the median in buffer X(0..U)
  M->Iold    = M->Ubuf;       //Index of oldest value in buffer X(0..U)
  M->CntUp   = M->Uhalf;      //Count values above the median
  M->CntLo   = M->Uhalf;      //Count values below the median
}

//Copyright (c) 2026, M. Møllgaard, Open-source MIT License
//Running Median of a Fixed-Size Window of a Data Stream
//Uses an optimized nonsorting algorithm.
//A buffer array X() stores the values in the fixed-size window.
//For each new stream value, Xnew replaces the oldest value Xold.
//The buffer remains unsorted during median calculation.
//The algorithm balances the number of values on each side of the median.
//Optimized: Duplicates of the median are tracked separately on both sides.
//Particularly efficient when the window contains repeated median values.
//Time complexity:   best case O(1), worst case O(N).
//Memory complexity: O(N); only one buffer is required.
//Cases for calculating the median:
// CASE1: O(1): The median remains unchanged. RETURN.
// CASE2: O(N): Search for the smallest value above the median.
// CASE3: O(N): Search for the largest value below the median.
// The duplicate counters can avoid a buffer scan when the median is unchanged.

void RunMedianNoSortOpt(    //Optimized Nonsorting Running Median
  MEDIANTYP          *Xnew, //New value from the stream
  typRunMedianNoSort *M   ) //Structure for median state and buffer X(0..N)
{
  uint8_t i;                //Buffer index: X(0..i..N)
  MEDIANTYP Xm, X;          //Temporary values used during the scan
  MEDIANTYP Xnext, Xold;    //Temporary values used during the scan
  M->Isteps = 0;            //*TEST* Reset buffer traversal counter
  ++M->CntO1;               //*TEST* Count this as an O(1) calculation
  if (++M->Iold > M->Ubuf)  //If the next buffer index exceeds upper boundary
    {M->Iold = 0;}          //¦ Wrap to the first value in the CIRCULAR buffer
  Xold = M->X[M->Iold];     //Read the oldest value being replaced
  M->X[M->Iold] = *Xnew;    //Store the new value in the buffer
  Xm = M->Xmedian;          //Cache the current median
  if (*Xnew == Xold)        //If the new and replaced values are equal
    return;                 //¦ CASE1: RETURN; The median remains unchanged
  if (*Xnew == Xm)          //If Xnew equals the current median
    if (Xold > Xm) {        //¦ If the replaced value is above the median
      ++M->CntUp;           //¦ ¦ Count another upper-side median duplicate
      return;               //¦ ¦ CASE1: RETURN; The median remains unchanged
    }                       //¦ ¦ 
    else { //(Xold <= Xm)   //¦ Else the replaced value is at or below the median
      ++M->CntLo;           //¦ ¦ Count another lower-side median duplicate
      return;               //¦ ¦ CASE1: RETURN; The median remains unchanged
    }                       //¦  
  else if (*Xnew > Xm) {    //Else if the new value is above the median
    if (Xold > Xm)          //¦ If the replaced value is also above the median
      return;               //¦ ¦ CASE1: RETURN; The median remains unchanged
    else if (Xold == Xm) {  //¦ Else if the replaced value equals the median
      if (M->CntUp > 0) {   //¦ ¦ If upper-side duplicates are tracked
        --M->CntUp;         //¦ ¦ ¦ Decrease the upper-side duplicate count
        return;             //¦ ¦ ¦ CASE1: RETURN; The median remains unchanged
      }                     //¦ ¦
    }                       //¦ ¦
    else { //(Xold < Xm)    //¦ Else the replaced value is below the median
      if (M->CntUp > 0) {   //¦ ¦ If upper-side duplicates are tracked
        --M->CntUp;         //¦ ¦ ¦ Decrease the upper-side duplicate count
        ++M->CntLo;         //¦ ¦ ¦ Increase the lower-side duplicate count
        return;             //¦ ¦ ¦ CASE1: RETURN; The median remains unchanged
      }                     //¦
    }                       //¦ CASE2: Search for the smallest value above edian
    M->CntLo = 0;           //¦ ¦ Reset the lower-side duplicate counter
    --M->CntO1;             //¦ ¦ *TEST* Remove the O(1) count
    i = M->Nbuf;            //¦ ¦ Initialize the scan index to Nbuf
    Xnext = XTYPMAX;        //¦ ¦ Initialize candidate to maximum data value
    do {                    //¦ ¦ Loop Scan the buffer from last element to first
      ++M->Isteps;          //¦ ¦ ¦ *TEST* Count this buffer access
      X = M->X[--i];        //¦ ¦ ¦ Read the next buffer value
      if (X > Xm) {         //¦ ¦ ¦ If the value is above the median
        if (X <= Xnext) {   //¦ ¦ ¦ ¦ If it is no greater than the candidate
          if (X == Xnext)   //¦ ¦ ¦ ¦ ¦ If another occurrence of candidate is found
            {++M->CntUp;}   //¦ ¦ ¦ ¦ ¦ ¦ Count another candidate duplicate
          else              //¦ ¦ ¦ ¦ ¦ Else a closer candidate was found
            {M->CntUp = 0;} //¦ ¦ ¦ ¦ ¦ ¦ Reset the candidate-duplicate counter
          Xnext = X;        //¦ ¦ ¦ ¦ Store the new candidate
        }                   //¦ ¦ ¦
      }                     //¦ ¦ ¦
    } while (i > 0);        //¦ ¦ End Loop Continue until entire buffer is scanned
  }                         //¦ End CASE2
  else { //(Xnew < Xm)      //Else the new value is below the median
    if (Xold < Xm)          //¦ If the replaced value is also below the median
      return;               //¦ ¦ CASE1: RETURN; The median remains unchanged
    else if (Xold == Xm) {  //¦ Else if the replaced value equals the median
      if (M->CntLo > 0) {   //¦ ¦ If lower-side duplicates are tracked
        --M->CntLo;         //¦ ¦ ¦ Decrease the lower-side duplicate count
        return;             //¦ ¦ ¦ CASE1: RETURN; The median remains unchanged
      }                     //¦ ¦
    }                       //¦ ¦
    else { //(Xold > Xm)    //¦ Else the replaced value is above the median
      if (M->CntLo > 0) {   //¦ ¦ If lower-side duplicates are tracked
        --M->CntLo;         //¦ ¦ ¦ Decrease the lower-side duplicate count
        ++M->CntUp;         //¦ ¦ ¦ Increase the upper-side duplicate count
        return;             //¦ ¦ ¦ CASE1: RETURN; The median remains unchanged
      }                     //¦
    }                       //¦ CASE3: Search for the largest value below median
    M->CntUp = 0;           //¦ ¦ Reset the upper-side duplicate counter
    --M->CntO1;             //¦ ¦ *TEST* Remove the O(1) count
    i = M->Nbuf;            //¦ ¦ Initialize the scan index to Nbuf
    Xnext = XTYPMIN;        //¦ ¦ Initialize candidate to minimum data value
    do {                    //¦ ¦ Loop Scan the buffer from last element to first
      ++M->Isteps;          //¦ ¦ ¦ *TEST* Count this buffer access
      X = M->X[--i];        //¦ ¦ ¦ Read the next buffer value
      if (X < Xm) {         //¦ ¦ ¦ If the value is below the median
        if (X >= Xnext) {   //¦ ¦ ¦ ¦ If it is no less than the candidate
          if (X == Xnext)   //¦ ¦ ¦ ¦ ¦ If another occurrence of candidate is found
            {++M->CntLo;}   //¦ ¦ ¦ ¦ ¦ ¦ Count another candidate duplicate
          else              //¦ ¦ ¦ ¦ ¦ Else a closer candidate was found
            {M->CntLo = 0;} //¦ ¦ ¦ ¦ ¦ ¦ Reset the candidate-duplicate counter
          Xnext = X;        //¦ ¦ ¦ ¦ Store the new candidate
        }                   //¦ ¦ ¦
      }                     //¦ ¦ ¦
    } while (i > 0);        //¦ ¦ End Loop Continue until entire buffer is scanned
  }                         //¦ End CASE3
  M->Xmedian = Xnext;       //Update the median to the selected candidate
}                           //

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
  typRunMedianNoSort  M = {};        //Init. to 0: Running median using No-Sort
  InitRunMedianNoSort( &M, NBUF );   //Init. running median using No-Sort
  for ( i=0; i<NSTREAM; i++ ) {      //Loop through stream values
    Xnew = Xrandom( &Seed );         //¦ Generate pseudo random stream value
    RunMedianNoSortOpt( &Xnew, &M);  //¦ Calc. running median using optimized No-Sort
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
  printf("i=id. Stream, e=id. oldest value, n=Steps median calc., Xnew=Newest stream data\n");
  printf("X=..:Unsorted buffer, Xm=Median");
}
//END PROGRAM CODE******************************************************************** 



