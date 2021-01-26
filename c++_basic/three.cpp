#include<iostream>

int main()
{
    int x = 10;
    int &y = x;
    int r = 20;
    std::cout<<"x "<<x<<std::endl;
    std::cout<<"y "<<y<<std::endl;
    y = r;
    std::cout<<"x "<<x<<std::endl;
    std::cout<<"y "<<y<<std::endl;
    return 0;
}