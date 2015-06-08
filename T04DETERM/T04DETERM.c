/* FILE NAME: T04DETERM.C
 * PROGRAMMER: AS3
 * DATE: 08.06.2015
 * PURPOSE: to calculate determinator.
*/

#include <stdio.h>

#define MAX 30


double A[MAX][MAX], sum = 0;
int p[MAX];
int Parity = 0, N = 3;
   

void Swap( int *A, int *B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
}

void Load( char *FileName )
{
  FILE *File;
  int i, j;

  if ((File = fopen(FileName, "r")) != NULL)
  {
    fscanf(File, "%d", &N);
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        fscanf(File, "%lf", &A[i][j]);

    fclose(File);
  }
} /* End of 'Load' function */

void Go( int Pos )
{
  int i, x, save;

  if (Pos == N)
  {
    double prod = 1;

    for (i = 0; i < N; i++)
    {
      prod *= A[i][p[i]];

      if (Parity == 0)
        sum += prod;
      else
        sum -= prod;
    }

    return;
  }

  save = Parity;
  Go(Pos + 1);
  for (i = Pos + 1; i < N; i++)
  {
    Parity = !Parity;
    Swap(&p[Pos], &p[i]);
    Go(Pos + 1);
  }

  Parity = save;
  x = p[Pos];

  for (i = Pos + 1; i < N; i++)
    p[i - 1] = p[i];
  p[N - 1] = x;
} /* End of 'Go' function */

void main( void )
{
  int i;
                                                                          
  Load("m.txt");
  
  for (i = 0; i < N; i++)
    p[i] = i + 1;
	
  Load("m.txt");      
  Go(0);
  printf("The determinator of this matrix is %lf.\n", sum);

}