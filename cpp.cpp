#include<iostream>
#include<fstream>
#include<time.h>
#include<math.h>
#include<stdlib.h> 
using namespace std;
#define STACK_INIT_SIZE 100;
#define STACKINCREMENT 10;

double Operate(double a,char theta,double b); 
//产生随机数
int createOperateNum(int bit);
char createOperateChar(int i);
void getInputData();
void createOperate();
double EvaluateExpression(double &p);
void Savefile();
int Loadfile();

typedef struct
{
	char *base;
	char *top;
	int stacksize;
}SqStack;  //运算符栈
typedef struct
{
	double *base;
	double *top;
	int stacksize;
}SqStack1;   //操作数栈

//全局变量
char operateChar[4];//运算符
double  stackNum[100];     //操作数数组
char stackOpChar[100];  //操作符字符数组
int stackNumNo=0;       //操作数个数
int stackOpCharNo=0;    //运算符个数
int opType;           //操作数种类
bool error=false;
int m;

//用户输入结构
struct input
{
	int no;//习题数；
	int operateNo;           //操作数数目；
	int operateBit;          //操作数数位；
	int isFraction;          //能否为小数；
	int isNegative;          //能否为负数；
	int scopemax;               //允许出现的最大值；
	int scopemin;               //允许出现的最小值
//	char yourOperateChar[4];//可以出现的运算符；
}in;
void InitStack(SqStack &S)
{//构造一个操作符空栈S
	S.base=new char[100];
	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE;
}
void InitStack(SqStack1 &S)
{   //构造一个操作数空栈S
	S.base=new double[100];
	S.top=S.base;
	S.stacksize=STACK_INIT_SIZE;
}

char GetTop(SqStack S)
{//若栈不空，则用e返回S的栈顶元素
    char e;
	if(S.top==S.base)return -1;
	e=*(S.top-1);
	return e;
}
double GetTop(SqStack1 S)
{//若栈不空，则用e返回S的栈顶元素
    double e;
	if(S.top==S.base)return -1;
	e=*(S.top-1);
	return e;
}

template<class T,class T1>
int Push(T &S,T1 e)
{//插入元素e为新的栈顶元素
	*S.top++=e;
	return 0;
}
template<class T,class T1>
int Pop(T &S,T1 &e)
{    //若栈不空，则删除S的栈顶元素，用e返回其值，并返回0，否则返回-1；
	if(S.top==S.base)return -1;
	e=*--S.top;
	return 0;
}

double Operate(double a,char theta,double b)
{
	switch(theta)
	{
	case '+':return a+b;
	case '-':return a-b;
	case '*':return a*b;
	case '/':return a/b;
	}
	return 0;
}
char Precede(char t1,char t2)
{
	if((t1=='+'||t1=='-')&&(t2=='*'||t2=='/'))return '<';
	else return '>';
}
//产生随机数
int createOperateNum(int bit)
{ 
	int i=1,randomNumber,randomdata;
	while(bit>0)
	{
		i=10*i;
		bit--;
	}	
    randomNumber = rand();//生成随机数,rand（） 生成的是0～32767之间的一个随机数。 
    randomdata =  (randomNumber % i); //获得不大于bit位的随机数
		
	return randomdata;
}
//产生运算符
char createOperateChar(int i)
{
    int randomNumber,randomdata;
	randomNumber = rand();
	randomdata = (randomNumber % i);

	return operateChar[randomdata]; 
}
//用户输入
void getInputData()
{
	cout<<"请输入习题数（习题数规定至少不小于5）："<<endl;
	cin>>in.no;
	while(in.no<5)         //如果输入题数in.no小于5，则请重新输入
	{
		cout<<"您的输入题数不够，请重新输入"<<endl;
		cin>>in.no;
	}
	cout<<"请输入操作数个数:"<<endl;
	cin>>in.operateNo;
	cout<<"请输入操作数位数"<<endl;
	cin>>in.operateBit;
	cout<<"运算中间结果是否允许为小数，1代表能，0代表不能"<<endl;
	cin>>in.isFraction;
	if(in.isFraction==1)
	{
		cout<<"请输入您想保留的小数位数m（m≥0且m≤4）:"<<endl;
		cin>>m;
	}
	cout<<"运算中间结果是否允许为负数，1代表能，0代表不能"<<endl;
	cin>>in.isNegative;
	cout<<"请输入结果允许出现的最大值："<<endl;
	cin>>in.scopemax;
	cout<<"请输入结果允许出现的最小值："<<endl;
	cin>>in.scopemin;
	cout<<"请输入您想要使用的操作符,并以'#'结束输入:"<<endl;
	char c[6];
	opType=0;
	int i=0;
	cin>>c;
	while(c[i]!='#')
	{
		if(c[i]=='+'||c[i]=='-'||c[i]=='*'||c[i]=='/')
			if(opType<4)operateChar[opType++]=c[i];
		i++;
	}
}
//生成运算式
void createOperate()
{
	int i=in.operateNo;
	srand((unsigned)time(NULL));//根据当前时间生成随机数生成器种子
	while(i>0)
	{
		stackNum[stackNumNo++]=createOperateNum(in.operateBit);
		stackOpChar[stackOpCharNo++]=createOperateChar(opType);
		i--;
	}
	stackNumNo--;
	stackOpCharNo--;//将多产生的一个操作符删除；   
	stackOpChar[stackOpCharNo]='#';
	stackOpCharNo--;
}
/////////////////////////
void reset()
{
	stackNumNo=0;
	stackOpCharNo=0;
	error=false;
}
double EvaluateExpression(double &p)
{	
	SqStack OPTR;	SqStack1 OPND;
	int i=1,j=1;   char c;
	double q;
	InitStack(OPTR);	InitStack(OPND);
    Push(OPND,stackNum[0]);  Push(OPTR,stackOpChar[0]);  Push(OPND,stackNum[1]); 
	while(i<stackNumNo)
	{
		if(stackOpChar[i]!='#')
			switch(Precede(GetTop(OPTR),stackOpChar[i]))
		{
			case '<':
				Pop(OPND,p);
				p=Operate(p,stackOpChar[i],stackNum[j+1]);
				if((in.isNegative==0&&p<0)||(in.isFraction==0&&p!=int(p)))error=true;
				Push(OPND,p);
				break;
			case '>':
				Pop(OPTR,c);
                Pop(OPND,p);   Pop(OPND,q);
                p=Operate(q,c,p);
                if((in.isNegative==0&&p<0)||(in.isFraction==0&&p!=int(p)))error=true;
				Push(OPND,p);
				Push(OPND,stackNum[j+1]);
				Push(OPTR,stackOpChar[i]);
				break;
		}
		i++;
		j++;
	}
	Pop(OPTR,c);
    Pop(OPND,p);   Pop(OPND,q);
    p=Operate(q,c,p);
	if(p<in.scopemin||p>in.scopemax||(in.isNegative==0&&p<0)||(in.isFraction==0&&p!=int(p))) //当p被指定为不能为负数时，p<0
		error=true;  //p不能小于指定的最小值，不能大于指定的最大值
	return 0;                                                             
}
//-------------------------------------------------文件的保存------------------------------------//
void Savefile()
{
	ofstream outfile("1.txt");
	if(!outfile)
	{
		cout<<"Open 1.txt error!"<<endl;
		return;
	}
	outfile<<in.isFraction<<" "<<in.isNegative<<" "<<in.no<<" "<<in.operateBit<<" "<<
		in.operateNo<<" "<<in.scopemax<<" "<<in.scopemin<<" "<<opType<<" ";
	for(int i=0;i<opType;i++)outfile<<operateChar[i];		
}
int Loadfile()
{
	ifstream infile("1.txt");
	if(!infile)
	{
		cout<<"Open 1.txt error!"<<endl;
		return 0;
	}
	infile>>in.isFraction>>in.isNegative>>in.no>>in.operateBit>>in.operateNo>>in.scopemax>>in.scopemin>>opType;
	for(int i=0;i<opType;i++)infile>>operateChar[i];
	return 1;
}
int main()
{
//	Loadfile();
    //用户输入数据
	double p,result;
	char t;
    int correct=0,fail=0,j=Loadfile();
	if(!j)getInputData();
	else
	{
		cout<<"是否需要重新输入数据(y/n)"<<endl;
		cin>>t;
		if(t=='y')getInputData();
	}

	Savefile();
	cout<<in.no<<endl;

	
	//产生相应的运算式
	while(in.no>0)
	{
		createOperate();
		EvaluateExpression(p);
		if(!error)
		{
			int i=0;
			while(i<=stackOpCharNo)
			{
				cout<<stackNum[i]<<stackOpChar[i];
				i++;
			}
			cout<<stackNum[i]<<"="<<endl;
			///////////////////////////////////
			cout<<"请输入您的运算结果："<<endl;
			cin>>result;
			if(in.isFraction==1)
			{
				if((int)(p*pow(10,m+1))%10>=5)
					p=(int)(p*pow(10,m)+1)/pow(10,m);
				else p=(int)(p*pow(10,m))/pow(10,m);
			}

			if(p==result)
			{
				cout<<"正确"<<endl;
				correct++;
			}
			else
			{
				cout<<"错误"<<endl;
				cout<<"正确答案应为："<<p<<endl;
				fail++;
			}
			in.no--;
		}
			reset();
    }
	cout<<"您一共做对了"<<correct<<"题，做错了"<<fail<<"题"<<endl;
	return 0;
}
