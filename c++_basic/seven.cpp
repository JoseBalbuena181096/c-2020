#include<iostream>

int main()
{
    int a = 10;
    int b = 40;
    if(!(a==10))
        std::cout<<"YES"<<std::endl;
    else 
         std::cout<<"NO"<<std::endl;

    int c = 8;
    std::cout<<~(c)<<std::endl;
    return 0;
}
