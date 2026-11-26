#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 8
#define MAXCYL 199
static int req[N]={98,183,37,122,14,124,65,67};
static int cmp(const void*a,const void*b){int x=*(const int*)a,y=*(const int*)b;return (x>y)-(x<y);}
static int sum(int p,const int*a,int n){int t=0;for(int i=0;i<n;i++){t+=abs(a[i]-p);p=a[i];}return t;}
static void show(const char*name,int p,const int*a,int n,int extra){printf("%-7s %d",name,p);for(int i=0;i<n;i++)printf(" %d",a[i]);printf(" -> %d cylinders\n",sum(p,a,n)+extra);}
static void sstf(int p,int*out){int done[N]={0};for(int k=0;k<N;k++){int best=-1,dmin=MAXCYL+1;for(int i=0;i<N;i++)if(!done[i]){int d=abs(req[i]-p);if(d<dmin){best=i;dmin=d;}}out[k]=req[best];done[best]=1;p=req[best];}}
int main(void){int a[N+2],s[N],n=0;memcpy(s,req,sizeof s);qsort(s,N,sizeof s[0],cmp);show("FCFS",53,req,N,0);sstf(53,a);show("SSTF",53,a,N,0);
for(int i=N-1;i>=0;i--)if(s[i]<53)a[n++]=s[i];a[n++]=0;for(int i=0;i<N;i++)if(s[i]>53)a[n++]=s[i];show("SCAN",53,a,n,0);
n=0;for(int i=N-1;i>=0;i--)if(s[i]<53)a[n++]=s[i];for(int i=0;i<N;i++)if(s[i]>53)a[n++]=s[i];show("LOOK",53,a,n,0);
n=0;for(int i=0;i<N;i++)if(s[i]>53)a[n++]=s[i];a[n++]=MAXCYL;int total=sum(53,a,n);int low[N],m=0;for(int i=0;i<N;i++)if(s[i]<53)low[m++]=s[i];if(m){total+=MAXCYL;for(int i=0;i<m;i++)total+=(i==0?low[i]:low[i]-low[i-1]);}
printf("C-SCAN  53");for(int i=0;i<n;i++)printf(" %d",a[i]);printf(" -> wrap to 0");for(int i=0;i<m;i++)printf(" %d",low[i]);printf(" -> %d cylinders (wrap %d included)\n",total,MAXCYL);
n=0;for(int i=N-1;i>=0;i--)if(s[i]<53)a[n++]=s[i];for(int i=N-1;i>=0;i--)if(s[i]>53)a[n++]=s[i];show("C-LOOK",53,a,n,0);return 0;}