#include<iostream>

int main()
{
    char a = 'a';
    char arr[] = {'a','b','c','d','e'};
    int *p = new int(10);
    float *d = new float(1.22);
    std::cout<<sizeof(a)<<std::endl;
    std::cout<<sizeof(arr)<<std::endl;
    std::cout<<sizeof(p)<<std::endl;
    std::cout<<sizeof(d)<<std::endl; 
    return 0;
}