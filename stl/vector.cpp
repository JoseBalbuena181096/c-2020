#include<iostream>
#include<vector>
using namespace std;

//Vectors
//1. Alternative for arrays
//2. They are sequence containers
//3. They can have dynamic size

void change(int *array)
{
    for(int i=0;i<5;i++)
        array[i]*=100;
}

int main()
{
    //vector<int> a;//empty
    /*
    //vector<int> b(5);//create  vector of size
    vector<int> b(5,2);//create  vector of the size and initial value
    vector<int> c(5);
    a.push_back(1);
    a.push_back(2);
    for(int i=0;i<2;i++)
        cout<<a[i]<<" ";
    cout<<endl;
    for(int x:a)
        cout<<x<<" ";
    cout<<endl;    
    for(auto x:a)
        cout<<x<<" ";
    cout<<endl;
    //vector<int>::iterator it;
    for(auto it=a.begin();it!=a.end();it++)
        cout<<*it<<" ";
    cout<<endl;
    for(auto x:b)
        cout<<x<<" ";
    cout<<endl;
    if(a.empty())
        cout<<"A is empty"<<endl;
    else
        cout<<"A is not empty"<<endl;
    c.clear();
    for(auto x:c)
        cout<<x<<" ";
    if(c.empty())
        cout<<"c is empty"<<endl;
    else
        cout<<"c is not empty"<<endl;
    */
   /*
    for(int i=0;i<10;i++)
        a.push_back(i);
    for(auto x:a)
        cout<<x<<" ";
    cout<<"\n";
    a.erase(a.begin()+2);
    for(auto x:a)
        cout<<x<<" ";
    cout<<"\n";
    a.erase(a.begin()+1,a.begin()+4);
    for(auto x:a)
        cout<<x<<" ";
    cout<<"\n";
    a.insert(a.begin()+1,2);
    for(auto x:a)
        cout<<x<<" ";
    cout<<"\n";
    //The last item
    cout<<a[a.size()-1]<<"\n";
    cout<<a.front()<<"\n";
    cout<<a.back()<<"\n";
    int t = 0xAAAAAAA;
    cout<<t;
 
 */
    int a[5] = {1,2,3,4,5};
    change(a);
    for(int i=0;i<5;i++)
        cout<<a[i]<<" ";
    return 0;
}