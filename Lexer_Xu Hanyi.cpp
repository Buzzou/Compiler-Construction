//
// Created by 徐寒邑 on 12/24/20.
//

#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<string>
#include<cstdlib>

using namespace std;

string word = "";
string reserved_word[11];
char buffer;
int line = 1;
int row = 1;
bool flag; //文件是否结束了
int flag2;


void set_reserve() {
    reserved_word[1] = "procedure";
    reserved_word[2] = "def";
    reserved_word[3] = "if";
    reserved_word[4] = "else";
    reserved_word[5] = "while";
    reserved_word[6] = "call";
    reserved_word[7] = "begin";
    reserved_word[8] = "end";
    reserved_word[9] = "and";
    reserved_word[10] = "or";
}

//字母
bool judge_word(char x) {
    if (x >= 'a' && x <= 'z' || x >= 'A' && x <= 'Z') {

        return true;
    } else return false;
}

//数字
bool judge_number(char x) {
    if (x >= '0' && x <= '9') {
        return true;
    } else return false;
}

//界符
bool judge_jiefu(char x) {
    if (x == '(' || x == ')' || x == ',' || x == ';' || x == '{' || x == '}') {
        return true;
    } else return false;
}

//运算符
bool judge_yunsuanfu1(char x) {
    if (x == '+' || x == '-' || x == '*') {
        return true;
    } else return false;
}

//
bool judge_yunsuannfu2(char x) {
    if (x == '=' || x == '>' || x == '<' || x == '!') {
        return true;
    } else return false;
}


//从文件里读一个单词
int scan(FILE *fp) {
    buffer = fgetc(fp);
    if (feof(fp)) {
        flag = 0;
        return 0;

    } else if (buffer == ' ') {
        row++;
        return 0;
    } else if (buffer == '\n') {
        line++;
        row = 1;
        return 0;
    } else if (judge_word(buffer)) {
        word += buffer;
        row++;
        while ((buffer = fgetc(fp)) && (judge_word(buffer))) {
            word += buffer;
            row++;
        }
        if (feof(fp)) {
            flag = 0;
            return 1;
        }

        string temp = word;
        for (int j = 0; j < temp.length(); j++) {
            if (temp[j] >= 'A' && temp[j] <= 'Z') {
                temp[j] += 32;
            }
        }
        for (int i = 1; i <= 10; i++) {
            if (temp == reserved_word[i]) {
                fseek(fp, -1, SEEK_CUR);////////////////////////
                return 3;

            }

        }

        fseek(fp, -1, SEEK_CUR);///////////////////////////////
        if (temp == "int") return 7;
        if (temp == "read") return 8;
        return 1;

    } else if (judge_yunsuanfu1(buffer)) {
        word += buffer;
        row++;
        return 4;
    } else if (judge_number(buffer)) {
        word += buffer;
        row++;
        while ((buffer = fgetc(fp)) && judge_number(buffer)) {
            word += buffer;
            row++;
        }
        if (feof(fp)) {
            flag = 0;
            return 2;
        }
        fseek(fp, -1, SEEK_CUR);////////////////
        return 2;
    }
        //检验界符
    else if (judge_jiefu(buffer)) {
        word += buffer;
        row++;
        return 6;
    }
        //检验 <=、  >=、  <>、  == =、 <、>
    else if (judge_yunsuannfu2(buffer)) {
        row++;
        word += buffer;
        if (buffer == '<')   //为了检验题目中的<> <=
        {
            buffer = fgetc(fp);
            if (buffer == '>' || buffer == '=') {
                word += buffer;
                row++;
                return 5;
            }
        } else if (buffer == '!') {
            buffer = fgetc(fp);
            if (buffer == '=') {
                word += buffer;
                row++;
                return 5;
            } else return -1;
        }
            //检验  >= ==
        else {
            buffer = fgetc(fp);
            if (buffer == '=') {
                word += buffer;
                row++;
                return 5;
            }
        }
        if (feof(fp)) {
            flag = 0;
        }
        fseek(fp, -1, SEEK_CUR);/////////////
        return 4;
    }
        //除号或注释
    else if (buffer == '/') {
        row++;
        word += buffer;
        buffer = fgetc(fp);
        //除号
        if (buffer != '*' && buffer != '/') {
            fseek(fp, -1, SEEK_CUR);//////////////////
            return 4;
        }
        // 注释
        /*if (buffer == '/') {

            bool m1 = 1;
            while (m1) {
                word.clear();
                buffer = fgetc(fp);
                row++;

                if (buffer != '/') continue;
                else {
                    buffer = fgetc(fp);
                    row++;
                    if (buffer == '\n') {
                        line++;
                        row = 1;
                    }
                    return 9;
                }

            }
            if (feof(fp)) {
                flag = 0;
                return 9;
            }
        }*/
        if (buffer == '*') {
            bool m = 1;
            while (m) {
                word.clear();
                buffer = fgetc(fp);
                row++;
                if (buffer == '\n') {
                    line++;
                    row = 1;
                }
                if (buffer != '*') {
                    if (feof(fp)) {
                        //cout<<"xxx ";
                        flag = 0;
                        return 10;
                    } else
                        continue;

                } else {
                    //cout<<"xxx ";
                    buffer = fgetc(fp);
                    row++;
                    if (buffer == '\n') {
                        line++;
                        row = 1;
                    }
                    if (buffer == '/') {
                        m = 0;
                        return 9;
                    } else
                        /*continue;*/
                        return -1;
                    if (feof(fp)) {
                        flag = 0;
                        return 9;
                    } else return -1;
                }
                if (feof(fp)) {
                    flag = 0;
                    return 9;
                }
            }
        }
    } else {
        word += buffer;
        row++;
        return -1;
    }
}

int main() {
    set_reserve();//设置保留字
    cout << "open " << "C:/Users/lenovo/Desktop/编译原理实验/cifa.txt" << endl;
    cout << "press any key" << endl;
    system("pause");
    flag = 1;
    FILE *fp;
    if (!(fp = fopen("C:/Users/lenovo/Desktop/编译原理实验/cifa.txt", "r"))) {
        cout << "not found the file or other error " << endl;
        flag = 0;
    }

    while (flag == 1) {
        //flag2 返回的类型
        flag2 = scan(fp);

        if (flag2 == 1) {
            cout << "  identifier      " << "line " << line << " row " << row - word.length() << "  " << word << endl;
            if (word.length() > 20)
                cout << "ERROR Identifier length cannot exceed 20 characters" << endl;
            word.clear();
        } else if (flag2 == 3) {
            cout << "  保留字          " << "line " << line << " row " << row - word.length() << "  " << word << endl;
            word.clear();
        } else if (flag2 == 4) {
            cout << "  运算符          " << "line " << line << " row " << row - 1 << "  " << word << endl;
            word.clear();
        } else if (flag2 == 2) {
            cout << "  数字            " << "line " << line << " row " << row - word.length() << "  " << word << endl;
            //if(word[0]=='0')
            //cout<<"ERROR: The first digit cannot be 0!"<<endl;
            word.clear();
        } else if (flag2 == 6) {
            cout << "  界符            " << "line " << line << " row " << row - 1 << "  " << word << endl;
            word.clear();
        } else if (flag2 == 5) {
            cout << "  double_operator " << "line " << line << " row " << row - 2 << "  " << word << endl;
            word.clear();
        } else if (flag2 == 7) {
            cout << "  int             " << "line " << line << " row " << row - 2 << "  " << word << endl;
            word.clear();
        } else if (flag2 == 8) {
            cout << "  read            " << "line " << line << " row " << row - 2 << "  " << word << endl;
            word.clear();
        }
            /////////////////////////////
        else if (flag2 == 9) {
            cout << "  注释            " << "line " << line << " row " << row - 2 << "  " << word << endl;
            word.clear();
        } else if (flag2 == 10) {
            cout << "  注释错误        " << "line " << line << " row " << row - 2 << "  " << word << endl;
            word.clear();
        }
            //非法字符
        else if (flag2 == -1) {
            cout << "  错误               " << "line " << line << " row " << row - 1 << "  " << word << endl;
            word.clear();
        }
    }

    fclose(fp);
    cout << "press e to close" << endl;
    char end;
    while (cin >> end && end != 'e') {
        cout << "只有e可以关闭" << endl;
    }
    return 0;
}