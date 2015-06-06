/* FILE NAME: T04DERM.C
 * PROGAMMER: AS3
 * DATE: 04.06.2015
 * PURPOSE: DETERMINANT VALUE
 */
#include <stdio.h>
#include <conio.h>

#define MAX 30
double A[MAX][MAX];
int p[MAX];
int N, Parity, SUM = 0;

void Swap( int *A, int *B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
}
void Load( char *FileName )
{
  FILE *F;
  int i, j;

  if ((F = fopen(FileName, "r")) != NULL)
  {
    fscanf(F, "%d", &N);
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        fscanf(F, "%lf", &A[i][j]);
    fclose(F);
  }
}

void Go( int Pos )
{
  int i, PROD = 1;

  if (Pos == N) 
  {  
    for (i = 0; i < N; i++)
    {  
      PROD *= A[i][p[i]];
      if (Parity == 0)  
        SUM += PROD;
      else
      { 
        SUM -= PROD;
        Swap(&p[Pos], &p[i]);
      }
    }
    return;    
  }
  else
  {
    for (i = Pos; i < N; i++)
    {
      
      if (Pos != i)
      {  
        Parity = !Parity; 
        Swap(&p[Pos], &p[i]);
        Go(Pos + 1);  
        Parity = !Parity;
        Swap(&p[Pos], &p[i]);
      }
    }  
  }
}
void main( void )
{
 int i;

 Load("m.txt");
 for (i = 0; i < N; i++)
   p[i] = i + 1; 
 Go(0);
 printf("%i\n", SUM);
}