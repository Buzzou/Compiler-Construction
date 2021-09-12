//
// Created by 李若昊 on 11/14/20.
//

#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
using namespace std;


// 返回值类别：-3为整数以0开头了，-2为打开文件出错，-1为词语错误
// 1为保留字，2为一般标识符，3为无符号整数，4为单目符，5为双目符,6为注释

extern int TESTscan()
{
    string keyword[7]={"if","else","for","while","int","write","read"};
    char singleword[50]={'(',')',';','{','}','+','-','*'};  //没有'/','<','>','='
    char doubleword[10]= {'>','<','!','='}; //双目符的首字符

    FILE *file_pointer;

    string token=""; //识别出的单词
    //-----------------------------------------------------传入文件-----------------------------------------------------//
    if(!(file_pointer=fopen("/Users/ruohaoli/CLionProjects/Compiler Construction/Untitled.txt","r")))
    {
        cout<<"打开文件出错"<<endl;
        return -2;
    }
    //---------------------------------------------------读取一个字符---------------------------------------------------//
    char buffer;    //每次读入的单字符
    S1: buffer=getc(file_pointer);
    //-----------------------------------------------对每一个字符分情况判断-----------------------------------------------//
    while(buffer!=EOF)  //读到的不是文件尾
    {
        //--------------------------------------------------吃掉空格等--------------------------------------------------//
        while(buffer==' '||buffer=='\n'||buffer=='\t')
            buffer=getc(file_pointer);  //吃掉，往后读
        //------------------------------------------初读为字母，只可能是标识符---------------------------------------------//
        //标识符：字母打头，后接任意字母或数字。
        if(isalpha(buffer))
        {
            token+=buffer;  //先预存进token
            buffer=getc(file_pointer);  //预读一个
            while(isalnum(buffer))  //如果后接字母或数字则拼在一起
            {
                token+=buffer;   //连起来放在token里
                buffer=getc(file_pointer);   //再读，直到不是字母或数字
            }
            fseek(file_pointer, -1, SEEK_CUR);//指针回退一格,把标识符后一个字符吐出来
            //---------------------------------------------查保留字---------------------------------------------//
            //保留字为标识符的子集，包括：if, else, for, while, int, write, read。
            for(int i=0;i<7;i++)
            {
                if(token==keyword[i])
                {
                    cout<<"保留字"<<' '<<token<<endl;
                    token.clear();
                    goto S1;
                }
            }
            cout<<"一般标识符"<<' '<<token<<endl;
            token.clear();
            goto S1;
        }
            //----------------------------------------初读为数字，只可能是无符号整数--------------------------------------------//
            //无符号整数：由数字组成，但最高位不能为 0，允许一位的 0。
        else if (isdigit(buffer))
        {
            token+=buffer;  //先存token
            buffer=getc(file_pointer);   //预读
            while(isdigit(buffer))  //只要下一位还是数字
            {
                token+=buffer;   //就拼接，拼起来的部分存在token里
                buffer=getc(file_pointer);   //再读
            }
            fseek(file_pointer, -1, SEEK_CUR);//回退一格
            if(token.length()>1&&token.at(0)=='0')
            {
                cout<<"*** An integer cannot begin with zero."<<' '<<"Detected input: "<<token<<" ***"<<endl;
                token.clear();    //应该要跳过以0开头的这串数字
                goto S1;
            }
            else
                cout<<"无符号整数 "<<token<<endl;//输出无符号整数
            token.clear();
            goto S1;
        }
            //--------------------------------------------------单目符识别--------------------------------------------------//
            /* 分界符：(、)、;、{、}
            *  运算符：+、-、*、/、=、<、>
            */
        else if(strchr(singleword,buffer)!=NULL)    //在单目符集合中有
        {
            token+=buffer;  //存token
            cout<<"单目符 "<<token<<endl;
            token.clear();
            goto S1;
        }
            //--------------------------------------------------双目符识别--------------------------------------------------//
            //运算符：>=、<=、!=、==、=
        else if(strchr(doubleword,buffer)!=NULL)
        {
            token+=buffer;  //先存token
            buffer=getc(file_pointer);   //超前搜索以判断是不是双目符
            //----------------------------------如果下一位是'='则连起来拼接成'?='----------------------------------//
            if(buffer=='=')
            {
                token+=buffer;  //俩连起来
                cout<<"双目符 "<<token<<endl;
                token.clear();
                goto S1;
            }
                //-------------------------------------如果下一位不是'='就是单目符-------------------------------------//
            else
                fseek(file_pointer, -1, SEEK_CUR);//回退一格
            cout<<"单目符 "<<token<<endl;
            token.clear();
            goto S1;
        }
            //-----------------------------------------------------注释----------------------------------------------------//
        else if(buffer=='/')    //初读为'/'
        {
            buffer=getc(file_pointer);   //超前搜索
            //----------------------------------------下一个字符是*才是注释---------------------------------------//
            //注释符：/*       */
            if(buffer=='*') //就是注释！
            {
                char after_buffer;
                after_buffer = getc(file_pointer);    //注释体，比如/*This*/，after_buffer='T'
                do {
                    buffer = after_buffer;   //buffer前移从*变成T
                    after_buffer = getc(file_pointer);    //after_asterisk前移变成h，现在 buffer 和 after_buffer 全部进入注释体
                }
                while ((buffer != '*' || after_buffer != '/') && after_buffer != EOF);   //直到读到注释结尾*/或文件尾
                buffer = getc(file_pointer); //读掉/
            }
                //--------------------------------------下一个字符不是*那就是除号--------------------------------------//
            else
            {
                token+='/';
                cout<<"单目符 "<<token<<endl;
                token.clear();
                goto S1;
            }
        }
            //---------------------------------------------------错误处理---------------------------------------------------//
        else
        {
            token+=buffer;
            cout<<"*** Detected unrecognized character: "<<token<<" ***"<<endl;
            token.clear();
            goto S1;
        }
    }
    fclose(file_pointer);    //关闭输入文件
}
