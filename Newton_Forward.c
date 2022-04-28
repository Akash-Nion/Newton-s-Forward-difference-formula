#include<stdio.h>
#include<math.h>
main()
{
float x[10],y[10],d[10][10],a,u,h,tsum,sum=0;
int fact(int);
float f(float,int);
int i,j,n;
printf("Enter the number of pair of values\t");
scanf("%d",&n);
printf("Enter the initial value\t\t");
scanf("%f",&x[0]);
printf("Enter the common difference\t");
scanf("%f",&h);
for(i=0;i<n;i++)
x[i+1]=x[i]+h;
printf("Enter the value of y\n");
for(i=0;i<n;i++)
scanf("%f",&y[i]);
for(i=0;i<n;i++)
d[0][i]=y[i];
printf("The difference table is\n");
for(i=1;i<n;i++)
{
for(j=0;j<n-i;j++)
{
    d[i][j]=d[i-1][j+1]-d[i-1][j];
printf("d[%d][%d]  %f",i,j,d[i][j]);
}
printf("\n");
}
printf("Enter the point we want to interpolate:\t");
scanf("%f",&a);
u=(a-x[0])/h;
for(i=1;i<n;i++)
{
    sum=sum+(f(u,i)*d[i][0])/fact(i);
    printf("%f\n",sum);
}
tsum=sum+y[0];
printf("The value at %f is %f",a,tsum);
}
float f(float x,int i)
{
    int l;
    float prod=x;
    for(l=1;l<i;l++)
        prod=prod*(x-l);
    return (prod);
}
int fact(int k)
{
    int prod=1,i;
    for(i=1;i<=k;i++)
        prod=prod*i;
    return (prod);
}
