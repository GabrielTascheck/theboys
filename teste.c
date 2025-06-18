#include <stdio.h>

int qParticiona(int v[], int a, int b, int x)
{
  int m = a - 1;
  for (int i = a; i <= b; i++)
  {
    if (v[i] <= x)
    {
      m++;
      int aux = v[m];
      v[m] = v[i];
      v[i] = aux;
    }
  }
  return m;
}

void qSort(int v[], int a, int b)
{
  if (a >= b)
    return;
  int m = qParticiona(v, a, b, v[b]);
  qSort(v, a, m - 1);
  qSort(v, m + 1, b);
}

#include <stdio.h>

int main()
{

  return 0;
}