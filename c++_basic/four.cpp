#include<iostream>

int main()
{
    int number[] = {1,2,3,4,5,6,7,8,9,10};
    std::cout<<4[number]<<std::endl;
    int matrix[2][3] = {{7,8,3},{4,54,12}};
    for(auto i:matrix)
            for(int j=0;j<3;j++)
                std::cout<<i[j]<<",";
            
    return 0;
}