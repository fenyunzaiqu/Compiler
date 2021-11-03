//
//  main.cpp
//  word_scanner
//
//  Created by 周南 on 10/30/21.
//

#include <iostream>
#include <fstream>
#include <cstring>

#define maxKeyword 13//变量标识符的最大长度
char key[9][maxKeyword]={"begin","end","integer","if","then","else","function","read","write"};
//存放单词
char token[maxKeyword];
int line=1;//保存行号
enum errorType {illgealword=1,dontmatch=2,varexceed=3};//1:非法字符 2:冒号不匹配 3:标识符长度溢出
using namespace std;
int isKey(char s[])
{
    for(int i=0;i<9;i++)
    {
        if(strcmp(s,key[i])==0){//比较字符串的函数，如果相同返回0，a<b返回负数，a>b返回正数
            return i+1;//根据begin=1，end=2,integer=3....
        }
    }
    return -1;//返回报错，没有这个关键词
}

void error(int errorType,ofstream &err){
    switch(errorType){
        case illgealword:
        err<<"***LINE:"<<line<<"  "<<"1.非法字符"<<"\n";
        break;
        case dontmatch:
        err<<"***LINE:"<<line<<"  "<<"2.冒号不匹配\n";
        break;
        case varexceed:
        err<<"***LINE:"<<line<<"  "<<"3.标识符长度溢出\n";
        break;
    }
    
}

bool isChar(char ch){
    if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))
        return true;
    else
        return false;
}

bool isNum(char ch){
    if(ch>='0'&&ch<='9')
       return true;
    else
       return false;
}

bool isSymbol(char ch){
    if(ch=='<'||ch=='>'||ch=='='||ch=='-'||ch==';'||ch=='*'||ch==' '||ch=='\n'||ch==':'||ch=='('||ch==')')//合法符号
        return true;
    else
        return false;
}

void scan(int &syn,int &p,char s[],ofstream &err,ofstream &outfile){//p代表着扫描位置，s代表着输入的字符串，syn是单词种别
    int count=0;
    if(s[p]==' ')p++;//代表着ppt图里空白第0步循环，无限跳过空格
    if(s[p]=='\n'){
        p++;
        line++;
        outfile<<" EOLN"<<" 24"<<endl;
    }
    if(isChar(s[p]))
    {
        while(isNum(s[p])||isChar(s[p]))//代表着第1步的循环，终结条件为非数字字母第二步
        {
            token[count++]=s[p];//用来把扫描到的单个字符保存成单词token
            p++;
        }
        if(count>maxKeyword){
            error(varexceed, err);
        }
        token[count]='\0'; //放一个当作一个单词的结束符号
        syn=isKey(token);//看这个单词是不是关键词
        if(syn==-1)//如果不是关键词
        {
            syn=10;//那么这个单词是标识符种别10：形如字母开头的int This1,letter(letter|digital)*
        }
    }
    else if(isNum(s[p])){//如果开头是数字,代表着第三步
        while(isNum(s[p])){//如果是数字，到第四步不是数字终结
            token[count++]=s[p];
            p++;
        }
        token[count]='\0';
        syn=11;//标记为常数种别11
    }
    else if(!isSymbol(s[p])){
        error(illgealword, err);
        p++;
    }
    else{
        //把有分支争议的大于等于，小于等于扔到后面去判断,case下面相当于ppt里的5，6，7，8
        switch (s[p]) {
            case '=':
                syn=12;
                token[0]=s[p];
                token[1]='\0';
                break;
            case '-':
                syn=18;
                token[0]=s[p];
                token[1]='\0';
                break;
            case '*':
                syn=19;
                token[0]=s[p];
                token[1]='\0';
                break;
            case '(':
                syn=21;
                token[0]=s[p];
                token[1]='\0';
                break;
            case ')':
                syn=22;
                token[0]=s[p];
                token[1]='\0';
                break;
            case ';':
                syn=23;
                token[0]=s[p];
                token[1]='\0';
                break;
            default:
                break;
        }
        if(s[p]=='<'){ //13,14,15的<> <= <三种情况
            token[count++]=s[p];
            if(s[p+1]=='>'){
                p++;
                token[count++]=s[p];
                syn=13;
            }
            else if(s[p+1]=='='){
                p++;
                token[count++]=s[p];
                syn=14;
            }
            else{
                syn=15;
            }
            token[count]='\0';
        }
        if(s[p]=='>'){
            token[count++]=s[p];
            if(s[p+1]=='='){
                p++;
                token[count++]=s[p];
                syn=16;
            }
            else{
                syn=17;
            }
            token[count]='\0';
        }
        if(s[p]==':'){
            token[count++]=s[p];
            if(s[p+1]=='='){
                p++;
                token[count++]=s[p];
                syn=20;
            }
            else{
                error(dontmatch, err);
            }
        }
        p++;
    }
}


int main(int argc, const char * argv[]) {
    ifstream readfile("/Users/zhounan/Downloads/test2.txt");
    ofstream outfile("/Users/zhounan/Downloads/out.txt");
    ofstream err("/Users/zhounan/Downloads/error.txt");
    readfile.seekg(0,ios::end);
    int length=readfile.tellg();
    readfile.seekg(0,ios::beg);
    char* s=new char[length];
    readfile.read(s,length);
    int p=0;
    int syn;
    while(p<strlen(s)){
        scan(syn, p, s,err,outfile);
        if(*token!='\0')
        outfile<<token<<" "<<   syn<<" ";
        memset(token, 0x00, sizeof(char)*strlen(token));
    }
    outfile<<" EOF 25"<<endl;
    return 0;
}
