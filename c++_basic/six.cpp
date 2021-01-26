#include<iostream>

int main()
{
    int a = 10,b = 20,c = 30;
    b = c = ++a;
    std::cout<<a<<std::endl;
    std::cout<<b<<std::endl;
    std::cout<<c<<std::endl;
    if("c++"=="C++")
        std::cout<<"is equal"<<std::endl;
    else
        std::cout<<"is not equal"<<std::endl;
    
    return 0;
}