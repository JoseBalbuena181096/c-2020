#include<iostream>
#include<stdio.h>
int main()
{
    int a = 1;
    int b = 10 ,c = 20;

    int k = a?b:c;
    std::cout<<k<<std::endl;
    (a==10) ? (printf("YES")):(printf("NO\n"));
    (a==1) ? (b == 10 ? printf("YES\n"):printf("NO")):(printf("a is no equal to 1"));
    return 0;
}