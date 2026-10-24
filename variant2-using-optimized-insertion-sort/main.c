//Tested in https://www.onlinegdb.com
//Published on zenodo.org: 10.5281/zenodo.23626176
//BEGIN PROGRAM CODE****************************************************************** 
#include <stdio.h>
#include <stdint.h> //Used for fixed-width integer types

#define NBUF            5  //[<251] ODD! Number of values in buffer X(0..U); NBUF=U+1
#define NSTREAM        10  //*TEST* Number of values from the stream
#define MEDIANTYP int16_t  //Datatype for median buffer

typedef struct {      //Data type for the median using sorting
  MEDIANTYP Xmedian;  //Current median value
  uint8_t   Nbuf;     //ODD; Total number of values in buffer X(0..U)
  uint8_t   Ubuf;     //EVEN; Upper index of buffer X(0..U), where U=Nbuf-1
  uint8_t   Uhalf;    //= U/2; Index of the median
  uint8_t   Iold;     //Index of the oldest value in buffer X(0..U)
  uint8_t   e[NBUF];  //Auxiliary index array used for sorting.
  uint16_t  Isteps;   //Number of steps for calculating the median
  uint16_t  CntO1;    //Count O(1) calculations 
  MEDIANTYP X[NBUF];  //Fixed-size window buffer X(0..U)
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
  uint8_t i = Nbuf;         //Help variable used to init. the auxiliary index array
  S->Nbuf   = Nbuf;         //Total number of values in buffer X(0..U)
  S->Ubuf   = Nbuf - 1;     //Upper index of buffer X(0..U), where U=Nbuf-1
  S->Uhalf  = S->Ubuf >> 1; //Index of the median in buffer X(0..U)
  S->Iold   = S->Ubuf;      //Index of oldest value in buffer X(0..U)
  while (--i!=0) S->e[i]=i; //Init. auxiliary index array
}


//Lean Algorithm: (c) 2026, M. Møllgaard, Open-source MIT License
//Calculates the running median for a fixed-size window of a data stream.
//Uses an optimized, adaptive Insertion Sort algorithm.
//Sorts an auxiliary index array; the original data buffer remains unchanged.
//For each new stream value, Xnew replaces the oldest value Xold in the buffer.
//Optimization: Reuses the existing order of the index array to reduce sorting effort.
//Improvement:  Skips sorting when Xnew and Xold are on the same side of the median.
//Beware:       Data are only partially sorted after the sorting has been skiped.
//Returns the middle value in the sorted index array as the running median.
//Time complexity:   best case O(1), worst case O(N).
//Memory complexity:           O(N) for the auxiliary index array.
//Insertion Sort is a well-known sorting method.
//Beware: sroting

void RunMedianInsSortOpt(       //Optimized Insertion-Sort Running Median
  MEDIANTYP        *Xnew,       //Xnew= new value from the stream
  typRunMedianSort *S    )      //Structure for median incl. buffer X(0..N)
{ 
  uint16_t  ek, j, k, ej;       //Indices for insertion sort
  uint16_t  i = 0;              //Index for insertion sort and buffer copying.
  MEDIANTYP Xinsert, Xold;      //Value to be inserted and old value in buffer
  if (++S->Iold > S->Ubuf)      //if next index in buffer exceeds upper boundary
    {S->Iold = 0;}              //¦ the set index to first value in CIRCULAR buffer 
  Xold = S->X[S->Iold];         //Old value from buffer
  S->X[S->Iold] = *Xnew;        //Replace oldest value with new stream value
  S->Isteps = 1;                //*TEST" Count 0 steps for calc. median
  ++S->CntO1;                   //Count O(1) calculations
  if (*Xnew > S->Xmedian) {     //if Xnew on upper side
    if (Xold > S->Xmedian)      //¦ if Xold on upper side
      return;                   //¦ ¦ CASE1: both on same side: no change; RETURN 
  } //(Xnew < Xm)               //else //Xnew on lower side          
  else if (Xold < S->Xmedian)   //¦ if Xold on lower side
    return;                     //¦ ¦ CASE1: both on same side: no change; RETURN  
//else                          //else 
  {                             //¦ CASE2: sort array and find median   
    --S->CntO1;                 //¦ Undo Count O(1) calculations
    while (i < S->Ubuf) {       //¦ Loop for i=0..Ubuf 
      j  = i;                   //¦ ¦ Index of preceding sorted value
      k  = ++i;                 //¦ ¦ Index of value to insert
      ek = S->e[k];             //¦ ¦ Index for value to insert into sorted order
      Xinsert = S->X[ek];       //¦ ¦ Value to insert into sorted order
      ej = S->e[j];             //¦ ¦ Index for preceding value
      ++S->Isteps;              //¦ ¦ *TEST" Count steps for calc. median 
      while (Xinsert<S->X[ej]){ //¦ ¦ Loop: Shift larger values one position right
        ++S->Isteps;            //¦ ¦ ¦ *TEST" Count steps for calc. median 
        S->e[k] = ej;           //¦ ¦ ¦ Move index for preceding value to the right
        k = j;                  //¦ ¦ ¦ Update insertion position
        if (k == 0) break;      //¦ ¦ ¦ if at start of buffer then RETURN
        --j;                    //¦ ¦ ¦ Move index for insertion position left
        ej = S->e[j];           //¦ ¦ ¦ Index for preceding value
      }                         //¦ ¦ End loop
      S->e[k] = ek;             //¦ ¦ Index for insert value at the correct position
    }                           //¦ End loop
    S->Xmedian = S->X[S->e[S->Uhalf]]; //Median in the mittle of the sorted buffer
  }
}


int main(void) {                     //Test program
  uint16_t   Seed = 1119;            //*TEST* Seed for random generator
  uint16_t   i, j;                   //Indices for stream traversal and printing
  int16_t    Xnew;                   //New simulated value from the stream
  typRunMedianSort M = {};           //Init. to 0: Running median using insertion sort
  InitRunMedianSort (&M, NBUF );     //Init. running median using optimized ins. sort
  for ( i=0; i<NSTREAM; i++ ) {      //Loop through stream values
    Xnew = Xrandom( &Seed );         //¦ Generate pseudo random stream value
    RunMedianInsSortOpt( &Xnew, &M); //¦ Calculate running median using ins. sort
    printf("i=%04d",      i       ); //¦ Print stream-value index
    printf(" e=%02d",     M.Iold  ); //¦ Print index of oldest value in buffer
    printf(" n=%04d",     M.Isteps); //¦ Print steps for median calculation
    printf(" Xnew=%+03d", Xnew    ); //¦ Print new stream value
    printf(" X="                  ); //¦ Print "X=" before buffer values 
    for ( j=0; j<=M.Ubuf; ++j )      //¦ Loop through window buffer
      printf("%+03d,",  M.X[j]    ); //¦ ¦ Print buffer values X(0..U)
    printf(" Xm=%+03d", M.Xmedian ); //¦ Print median
    printf(" S=");                   //¦ Print "S=" before sorted buffer values
    for ( j=0; j<=M.Ubuf; ++j )      //¦ Loop through sorted buffer
      printf("%+03d,", M.X[M.e[j]]); //¦ ¦ Print sorted values S(0..U)
    printf("\n");                    //¦ Print new line
  }                                  //End loop over stream
  printf("Q=%3d %% O(1) calc. \n",100*M.CntO1/NSTREAM ); //% O(1) path  
  printf("i=id.Stream, e=id.oldest value, n=Steps for median calculation\n");
  printf("X=..:Unsorted buffer, Xm=Median, S=..:Sorted buffer");
}
//END PROGRAM CODE******************************************************************** 
