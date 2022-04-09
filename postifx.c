#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10 //MAX_STACK_SIZE 10으로 지정
#define MAX_EXPRESSION_SIZE 20 //MAX_EXPRESSION_SIZE 20으로 지정

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum{ //precedence구조체 선언
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE]; //MAX_EXPRESSION_SIZE크기의 char형 infixExp변수정의
char postfixExp[MAX_EXPRESSION_SIZE]; //MAX_EXPRESSION_SIZE크기의 char형 postfixExp변수정의
char postfixStack[MAX_STACK_SIZE]; //MAX_STACK_SIZE크기의 char형 postfixStack변수정의
int evalStack[MAX_STACK_SIZE]; //MAX_STACK_SIZE크기의 int형 evalStack변수정의

int postfixStackTop = -1; //int형 postfixStackTop변수정의 -1값 넣어줌
int evalStackTop = -1; //int형 evalStackTop변수정의 -1값 넣어줌

int evalResult = 0; //int형 evalResult변수정의 0값 넣어줌

void postfixPush(char x); //void형 postfixPush함수정의 char x매개변수로 받아옴
char postfixPop(); //char형 postfixPop함수정의
void evalPush(int x); //void형 evalPush함수정의 int x매개변수로 받아옴
int evalPop(); //int형 evalPop함수정의
void getInfix(); //void형 getInfix함수정의
precedence getToken(char symbol); //precedence구조체 getToken정의 char symbol매개변수로 받아옴
precedence getPriority(char x); //precedence구조체 getPriority정의 char x매개변수로 받아옴
void charCat(char* c); //void형 charCat함수정의 char* c매개변수로 받아옴
void toPostfix(); //void형 toPostfix함수정의
void debug(); //void형 debug함수정의
void reset(); //void형 reset함수정의
void evaluation(); //void형 evaluation함수정의

int main()
{
	printf("[----- [Seok Won Park] [2017018003] -----]");
	char command; //command를 받아올 char변수 정의
	
	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //command값 받아와 저장

		switch(command) {
		case 'i': case 'I': //command가 i,I일경우
			getInfix(); //getInfix함수호출
			break; //호출끝나면정지
		case 'p': case 'P': //command가 p,P일경우
			toPostfix(); //toPostfix함수호출
			break; //호출끝나면정지
		case 'e': case 'E': //command가 e,E일경우
			evaluation(); //evaluation함수호출
			break; //호출끝나면정지
		case 'd': case 'D': //command가 d,D일경우
			debug(); //debug함수호출
			break; //호출끝나면정지
		case 'r': case 'R': //command가 r,R일경우
			reset(); //reset함수호출
			break; //호출끝나면정지
		case 'q': case 'Q': //command가 q,Q일경우
			break; //호출끝나면정지
		default: //위 case제외한 다른값
			printf("\n       >>>>>   Concentration!!   <<<<<     \n"); //잘못입력했다는 출력
			break; //호출끝나면정지
		}

	}while(command != 'q' && command != 'Q'); //만약 q,Q면 정지

	return 1;//실행 잘됐으면 1 리턴


}

void postfixPush(char x) //값 넣는 함수
{
    postfixStack[++postfixStackTop] = x; //postfixStackTop를 증가시켜 postfixStack에 넣고 x저장
}

char postfixPop() //값 빼는 함수
{
	char x; //char형 x정의
    if(postfixStackTop == -1) //만약 아무것도 들어있지 않다면
        return '\0'; //\0리턴
    else {
    	x = postfixStack[postfixStackTop--]; //x에 postfixStackTop를 postfixStack에 넣은 값 저장후 postfixStackTop감소
    }
    return x;//x 리턴
}

void evalPush(int x) //값 넣는 함수
{
    evalStack[++evalStackTop] = x; //evalStackTop를 증가시켜 evalStack에 넣고 x저장
}

int evalPop() //값 빼는 함수
{
    if(evalStackTop == -1) //만약 아무것도 들어있지 않다면
        return -1; //-1리턴
    else
        return evalStack[evalStackTop--]; //evalStack[evalStackTop]리턴 후evalStackTop 감소시킴
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */

void getInfix() //입력 수식 받는 함수
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp); //indixExp에다가 저장
}

precedence getToken(char symbol) //입력수식을 관련 정보를 저장한 구조체
{
	switch(symbol) { //symbol로 받아온값이랑 case를 비교해서 값을 반환해줌
	case '(' : return lparen; 
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand; 
	}
}

precedence getPriority(char x) //우선순의를 받아오는 구조체
{
	return getToken(x); //getToken에 x를 넣어서 반환
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c) 
{
	if (postfixExp == '\0') //아무것도 들어있지 않는다면
		strncpy(postfixExp, c, 1); //c를, postfixExp에 복사
	else //그외엔
		strncat(postfixExp, c, 1); //c를, postfixExp에 추가
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0') //exp가 문자열의 끝이아니라면 계속 반복
	{
		if(getPriority(*exp) == operand) //우선순위가 피연산자라면
		{
			x = *exp; //문자를 x에 저장
        	charCat(&x); //charCat함수 호출후 X값 넣어줌
		}
        else if(getPriority(*exp) == lparen) { //우선순위가 왼괄호라면

        	postfixPush(*exp); //postfixPush함수 호출후 *exp매개변수로 넣어줌
        }
        else if(getPriority(*exp) == rparen) //우선순위가 오른괄호라면
        {
        	while((x = postfixPop()) != '(') {  //x가 왼괄호가 아닐 때까지
        		charCat(&x); //charCat함수 호출후 X값 넣어줌
        	}
        }
        else //그외의 것들
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) // 우선순위를 비교해 우선순위가 높다면
            {
            	x = postfixPop(); //postfixPop()으로 x값을 받아오고
            	charCat(&x); //charCat함수 호출후 X주소 넣어줌
            }
            postfixPush(*exp); //postfixPush험수에 *exp 매개변수로 넣어줌
        }
        exp++; //exp증가
	}

    while(postfixStackTop != -1) //postfixStackTo 빌때까지
    {
    	x = postfixPop(); //postfixPop()으로 x값을 받아오고
    	charCat(&x);//charCat함수 호출후 X주소 넣어줌
    }

}

void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp); //infixExp값 출력
	printf("postExp =  %s\n", postfixExp); //postfixExp값 출력
	printf("eval result = %d\n", evalResult); //evalResult값 출력

	printf("postfixStack : "); 
	for(int i = 0; i < MAX_STACK_SIZE; i++) //i를 0부터 MAX_STACK_SIZE보다 작을때까지 1씩증가 루프
		printf("%c  ", postfixStack[i]); //postfixStack[i]스텍에 있는 값 출력

	printf("\n");

}

void reset() //전부 초기화
{
	infixExp[0] = '\0'; //infixExp[0]안의 값 \0으로 변경
	postfixExp[0] = '\0'; //postfixExp[0]안의 값 \0으로 변경

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0'; //postfixStack안의 값 \0으로 변경
         
	postfixStackTop = -1; //스텍의 꼭대기가르키는 인덱스롤 가장 아래도 둔다는 듯(스텍이 비었다는뜻)
	evalStackTop = -1; //위와 동일
	evalResult = 0; //결과값 0으로 초기화
}
void evaluation() //값을 구하는 함수
{
	int opr1, opr2, i; //opr1, opr2, i int형으로 변수 정의

	int length = strlen(postfixExp); //int형 length변수 정의 strlen함수에 postfixExp값 넣기
	char symbol; //char형 symbol변수 정의
	evalStackTop = -1; //evalStackTop를 바닦으로 만듬

	for(i = 0; i < length; i++) //i를 0부터 length보다 작을 때까지만 1씩 증가시킴
	{
		symbol = postfixExp[i]; //symbol에 postfixExp[i]넣어줌
		if(getToken(symbol) == operand) { //getToken 함수에 symbol매개변수로 넣어 호출한것이 operand라면
			evalPush(symbol - '0'); //evalPush함수 호출후 symbol - '0'넣어줌(숫자로 넣는다는뜻)
		}
		else {
			opr2 = evalPop(); //evalPop()함수호출후 값을 opr2넣기
			opr1 = evalPop(); //evalPop()함수호출후 값을 opr1넣기
			switch(getToken(symbol)) { //symbol을 비교해서
			case plus: evalPush(opr1 + opr2); break; //더하기면 더하기연산
			case minus: evalPush(opr1 - opr2); break; //뺴기면 빼기 연산
			case times: evalPush(opr1 * opr2); break; //곱하기면 곱하기연산
			case divide: evalPush(opr1 / opr2); break; //나누기면 나누기연산
			default: break; //다른것이들어오면 정지
			}
		}
	}
	evalResult = evalPop();//결과값은 evalPop()함수로 호출한 값
}