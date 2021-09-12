//
// Created by 李若昊 on 1/8/21.
//

#include<string>
#include<iostream>
using namespace std;

extern string TESTscan();
extern int TESTparser();

string temp()
{
    return TESTscan();
}
int main()
{
    return TESTparser();
}
