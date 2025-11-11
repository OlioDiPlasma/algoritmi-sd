#include<stdlib.h>
#include<string.h>
#include<stdio.h>

int count(int *a, int n, int l, int r, int val);
void majority( int *a, int n);
int majorityR( int *a, int n, int l, int r);

int main(int argc, char **argv) {
  int i, n, *a;
  printf("input n:  ");
  scanf("%d", &n);

  a = malloc (sizeof(int)*n);

  printf("input elements\n");
  for(i=0;i<n;i++)
    scanf("%d", &a[i]);

  printf("O(NlogN) recursive algorithm\n");
  majority(a, n);

  return 0;
}

int count(int *a, int n, int l, int r, int val){
  int i, cnt=0;
  for (i=l; i<=r; i++)
    if (a[i]==val)
      cnt++;
  return cnt;
}

void majority( int *a, int n) {
  int l=0, r=n-1, res;
  res = majorityR(a, n, l, r);
  if (res==-1)
    printf("No majority element\n");
  else
    printf("Majority element = %d\n", res);
}

int majorityR( int *a, int n, int l, int r) {
  int resLeft, resRight, resLeftCnt, resRightCnt;
  int m = (l + r)/2 ;
  if (l == r)
    return a[l] ;

  resLeft = majorityR(a, n, l, m);
  resRight = majorityR(a, n, m+1, r);

  if (resLeft==resRight)
    return resLeft;

  resLeftCnt = count(a, n, l, r, resLeft);
  resRightCnt = count(a, n, l, r, resRight);

  if (resLeftCnt > (r-l+1)/2)
    return resLeft;

  if (resRightCnt > (r-l+1)/2)
    return resRight;

  return -1;
}
