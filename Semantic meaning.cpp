//
// Created by 李若昊 on 12/19/20.
//

#include<stdio.h>
#include<ctype.h>
//#include<conio.h>
#include<string.h>
#define maxvartablep 500
int TESTparse();
int program();
int compound_stat();
int statement();
int expression_stat();
int expression();
int bool_expr();
int additive_expr();
int term();
int factor();
int if_stat();
int while_stat();
int for_stat();
int write_stat();
int read_stat();
int declaration_stat();
int declaration_list();
int statement_list();
int lookup(char *name,int *paddress);
int name_def(char *name);
//char token[20], token1[40];
char Scanout[300], Codeout[300];
FILE *fp, *fout;
struct var{
    char name[8];
    int address;
}vartable[maxvartablep];
int vartablep=0,labelp=0,datap=0;


int name_def(char* name)
{
    int i,es=0;
    if(vartablep >=maxvartablep) return(21);
    for(i=vartablep-1;i==0;i--)
    {
        if(strcmp(vartable[i].name,name)==0)
        {
            es=22;
            break;
        }
    }
    if(es>0) return(es);
    strcpy(vartable[vartablep].name,name);
    vartable[vartablep].address=datap;
    datap++;
    vartablep++;
    return(es);
}


int lookup(char *name,int *paddress)
{
    int i=0,es=0;
    for(i=0;i<vartablep;i++)
    {
        if(strcmp(vartable[i].name,name)==0)
        {
            *paddress=vartable[i].address;
            return(es);
        }
    }
    es=23;
    return(es);
}
int TESTparse()
{
    int es=0;
    printf("请输入要打开的文件名：");
    scanf("%s",Scanout);
    if((fp=fopen(Scanout,"r"))==NULL)
    {
        printf("\n打开%s错误\n",Scanout);
        es=10;
        return(es);
    }
    printf("请输入目标文件名：");
    scanf("%s",Codeout);
    if((fout=fopen(Codeout,"w"))==NULL)
    {
        printf("\n创建%s错误\n",Codeout);
        es=10;
        return(es);
    }
    if(es==0) es=program();
    printf("语法分析结果：\n");
    switch(es)
    {
        case 0:printf("语法分析成功\n"); break;
        case 1:printf("缺少‘{’\n"); break;
        case 2:printf("缺少‘}’\n"); break;
        case 3:printf("缺少标识符\n"); break;
        case 4:printf("缺少分号\n"); break;
        case 5:printf("缺少‘（’\n"); break;
        case 6:printf("缺少‘）’\n"); break;
        case 7:printf("缺少操作数\n"); break;
        case 10:printf("打开文件%s失败\n",Scanout); break;
        case 21:printf("符号表溢出\n"); break;
        case 22:printf("变量重复定义\n"); break;
        case 23:printf("变量未声明\n");	 break;
    }
    fclose(fp);
    fclose(fout);
    return(es);
}
int program()
{
    int es=0,i;
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"{"))
    {
        es=1;
        return(es);
    }
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    es=declaration_list();
    if(es>0) return(es);
/*	printf("	符号表\n");
	printf("名字	地址\n");
	for(i=0;i<vartablep+1;i++)
		printf("	%s	%d\n",vartable[i].name,vartable[i].address);*/
    es=statement_list();
    if(es>0) return(es);
    if(strcmp(token,"}"))
    {
        es=2;
        return(es);
    }
}

int declaration_list()
{
    int es=0;
    while(strcmp(token,"int")==0)
    {
        es=declaration_stat();
        if(es>0) return(es);
    }
    return(es);
}

int declaration_stat()
{
    int es=0;
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"ID"))
    {
        es=3;
        return(es);
    }
    es=name_def(token1);
    if(es>0)  return(es);
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,";"))
    {
        es=4;
        return(es);
    }
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    return(es);
}

int statement_list()
{
    int es=0;
    while(strcmp(token,"}"))
    {
        es=statement();
        if(es>0) return(es);
    }
    return(es);
}

int statement()
{
    int es=0;
    if(es==0&&strcmp(token,"if")==0) es=if_stat();
    if(es==0&&strcmp(token,"while")==0) es=while_stat();
    if(es==0&&strcmp(token,"for")==0) es=for_stat();
    if(es==0&&strcmp(token,"write")==0) es=write_stat();
    if(es==0&&strcmp(token,"read")==0) es=read_stat();
    if(es==0&&strcmp(token,"{")==0) es=compound_stat();
    if(es==0&&strcmp(token,"ID")==0||strcmp(token,"NUM")==0||strcmp(token,"(")==0)  es=expression_stat();
    return(es);
}

int if_stat()
{
    int es=0,label1,label2;
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"{"))  return(es=5);
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    es=expression();
    if(es>0) return(es);
    if(strcmp(token,"}"))  return(es=6);
    label1=labelp++;
    fprintf(fout,"	BR LABEL%d\n",label1);
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    es=statement();
    if(es>0) return(es);
    label2=labelp++;
    fprintf(fout,"	BR LABEL%d\n",label2);
    fprintf(fout,"LABEL%d\n",label1);
    if(strcmp(token,"else")==0)
    {
        fscanf(fp,"%s %s\n",token,token1);
        printf("%s %s\n",token,token1);
        es=statement();
        if(es>0)  return(es);
    }
    fprintf(fout,"LABEL%d\n",label2);
    return(es);

}
int while_stat()
{
    int es=0,label1,label2;
    label1=labelp++;
    fprintf(fout,"LABEL%d\n",label1);
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"("))  return(es=5);
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    es=statement();
    if(es>0) return(es);
    if(strcmp(token,")"))  return(es=6);
    label2=labelp++;
    fprintf(fout,"	BRF LABEL%d\n",label2);
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    es=statement();
    if(es>0) return(es);
    fprintf(fout,"	BR LABEL%d\n",label1);
    fprintf(fout,"LABEL%d\n",label2);
    return(es);

}
int for_stat()
{
    int es=0,label1,label2,label3,label4;
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"("))  return(es=5);
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    es=expression();
    if(es>0) return(es);
    fprintf(fout,"	POP\n");
    if(strcmp(token,";"))  return(es=4);
    label1=labelp++;
    fprintf(fout,"LABEL%d\n",label1);
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    es=expression();
    if(es>0) return(es);
    label2=labelp++;
    fprintf(fout,"	BRF LABEL%d\n",label2);
    label3=labelp++;
    fprintf(fout,"	BR LABEL%d\n",label3);
    if(strcmp(token,";"))  return(es=4);
    label4=labelp++;
    fprintf(fout,"LABEL%d\n",label4);
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    es=expression();
    if(es>0)  return(es);
    fprintf(fout,"	POP\n");
    fprintf(fout,"	BR LABEL%d\n",label1);
    if(strcmp(token,")"))  return(es=6);
    fprintf(fout,"LABEL%d\n",label3);
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    es=statement();
    if(es>0) return(es);
    fprintf(fout,"	BR LABEL%d\n",label4);
    fprintf(fout,"LABEL%d\n",label2);
    return(es);
}

int write_stat()
{
    int es=0;
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    es=expression();
    if(es>0) return(es);
    if(strcmp(token,";"))  return(es=4);
    fprintf(fout,"OUT\n");
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    return(es);
}

int read_stat()
{
    int es=0,address;
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,"ID"))  return(es=3);
    es=lookup(token1,&address);
    if(es>0) return(es);
    fprintf(fout,"  IN\n");
    fprintf(fout,"	STO %d\n",address);
    fprintf(fout,"	POP\n");
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    if(strcmp(token,";"))  return(es=4);
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    return(es);
}

int compound_stat()
{
    int es=0;
    fscanf(fp,"%s %s\n",token,token1);
    printf("%s %s\n",token,token1);
    es=statement_list();
    return(es);
}

int expression_stat()
{
    int es=0;
    if(strcmp(token,";")==0)
    {
        fscanf(fp,"%s %s\n",token,token1);
        printf("%s %s\n",token,token1);
        return(es);
    }
    es=expression();
    if(es>0) return(es);
    fprintf(fout,"	POP\n");
    if(strcmp(token,";")==0)
    {
        fscanf(fp,"%s %s\n",token,token1);
        printf("%s %s\n",token,token1);
        return(es);
    }
    else{
        return(es=4);
    }
}


int expression()
{
    int es=0,fileadd;
    char token2[30],token3[40];
    if(strcmp(token,"ID")==0)
    {
        fileadd=ftell(fp);
        fscanf(fp,"%s %s\n",token2,token3);
        printf("%s %s\n",token2,token3);
        if(strcmp(token2,"=")==0)
        {
            int address;
            es=lookup(token1,&address);
            if(es>0) return(es);
            fscanf(fp,"%s %s\n",token,token1);
            printf("%s %s\n",token,token1);
            es=bool_expr();
            if(es>0)  return(es);
            fprintf(fout,"	STO %d\n",address);
        }
        else{
            fseek(fp,fileadd,0);
            printf("%s %s\n",token,token1);
            es=bool_expr();
            if(es>0) return(es);
        }
    }
    else{
        es=bool_expr();
    }
    return(es);
}
int bool_expr()
{
    int es=0;
    es=additive_expr();
    if(es>0) return(es);
    if(strcmp(token,">")==0||strcmp(token,"<")==0||strcmp(token,">=")==0||strcmp(token,"<=")==0||strcmp(token,"==")==0||strcmp(token,"!=")==0)
    {
        char token2[20];
        strcpy(token2,token);
        fscanf(fp,"%s %s\n",token,token1);
        printf("%s %s\n",token,token1);
        es=additive_expr();
        if(es>0) return(es);
        if(strcmp(token2,">")==0)  fprintf(fout,"	GT\n");
        if(strcmp(token2,"<")==0)  fprintf(fout,"	LES\n");
        if(strcmp(token2,">=")==0)  fprintf(fout,"	GE\n");
        if(strcmp(token2,"<=")==0)  fprintf(fout,"	LE\n");
        if(strcmp(token2,"==")==0)  fprintf(fout,"	EQ\n");
        if(strcmp(token2,"!=")==0)  fprintf(fout,"	NOTEQ\n");
    }
    return(es);
}

int additive_expr()
{
    int es=0;
    es=term();
    if(es>0)  return(es);
    while(strcmp(token,"+")==0||strcmp(token,"-")==0)
    {
        char token2[20];
        strcpy(token2,token);
        fscanf(fp,"%s %s\n",token,token1);
        printf("%s %s\n",token,token1);
        es=term();
        if(es>0) return(es);
        if(strcmp(token2,"+")==0)  fprintf(fout,"	ADD\n");
        if(strcmp(token2,"-")==0)  fprintf(fout,"	SUB\n");
    }
    return(es);
}

int term()
{
    int es=0;
    es=factor();
    if(es>0)  return(es);
    while(strcmp(token,"*")==0||strcmp(token,"/")==0)
    {
        char token2[20];
        strcpy(token2,token);
        fscanf(fp,"%s %s\n",token,token1);
        printf("%s %s\n",token,token1);
        es=factor();
        if(es>0) return(es);
        if(strcmp(token2,"*")==0) fprintf(fout,"	MULT\n");
        if(strcmp(token2,"/")==0) fprintf(fout,"	DIV\n");
    }
    return(es);
}
int factor()
{
    int es=0;
    if(strcmp(token,"(")==0)
    {
        fscanf(fp,"%s %s\n",token,token1);
        printf("%s %s\n",token,token1);
        es=expression();
        if(es>0) return(es);
        if(strcmp(token,")"))  return(es=6);
        fscanf(fp,"%s %s\n",token,token1);
        printf("%s %s\n",token,token1);
    }
    else{
        if(strcmp(token,"ID")==0)
        {
            int address;
            es=lookup(token1,&address);
            if(es>0) return(es);
            fprintf(fout,"	LOAD%d\n",address);
            fscanf(fp,"%s %s\n",token,token1);
            printf("%s %s\n",token,token1);
            return(es);
        }
        if(strcmp(token,"NUM")==0)
        {
            fprintf(fout,"	LOADI%s\n",token1);
            fscanf(fp,"%s %s\n",token,token1);
            printf("%s %s\n",token,token1);
            return(es);
        }
        else{
            return(es=7);
        }
    }
    return(es);
}
int main()
{
    int es=0;
    /*es=TESTscan();
    if(es>0) printf("词法分析有错，编译停止！");
    else printf("词法分析成功");*/
    if(es==0)
    {
        es=TESTparse();
        if(es==0)  printf("语法分析成功");
        else printf("语法分析失败");
    }
}