#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 //que싸이즈를 4로 지정해줌

typedef char element; //char형 대신 element사용할수 있게 typedef지정
typedef struct { //QueueType struct 정의
	element queue[MAX_QUEUE_SIZE]; //element형(char형) 4크기자리 문자배열 queue[] 정의
	int front, rear; //front,rear int형으로 정의
}QueueType;


QueueType *createQueue(); //cerateQueue로 QueueType구조체 호출정의
int freeQueue(QueueType *cQ); //freeQueue 함수정의 QueueType,*cQ들이 인자
int isEmpty(QueueType *cQ); //isEmpty 함수정의 QueueType,*cQ들이 인자
int isFull(QueueType *cQ); //isFull 함수정의 QueueType,*cQ들이 인자
void enQueue(QueueType *cQ, element item); //enQueue 함수정의 QueueType,*cQ들이 인자
void deQueue(QueueType *cQ, element* item); //deQueue 함수정의 QueueType,*cQ들이 인자
void printQ(QueueType *cQ); //printQ 함수정의 QueueType,*cQ들이 인자
void debugQ(QueueType *cQ); //debugQ 함수정의 QueueType,*cQ들이 인자
element getElement(); //element형(char형) getElement()정의


int main(void) 
{
	printf("[----- [Seok Won Park] [2017018003] -----]"); 
	QueueType *cQ = createQueue(); //QueueType인 cQ에 createQueue()넣어줌
	element data; //element형(char형) data정의

	char command; //command받아올 char형 지정

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = "); 
		scanf(" %c", &command); //char형 입력값 command주소에 받아오기

		switch(command) {
		case 'i': case 'I': //command가 i혹은 I일떄
			data = getElement(); //data에 getElement() 넣어줌
			enQueue(cQ, data); //enQueue함수호출 cQ, data 매개변수로 넣어줌
			break; //다실행했으면 정지
		case 'd': case 'D': //command가 d혹은 D일떄
			deQueue(cQ,&data); //enQueue함수호출 cQ, data의 주소 매개변수로 넣어줌
			break;
		case 'p': case 'P': //command가 p혹은 P일떄
			printQ(cQ); //printQ함수 호출 cQ를 매개변수로 넣어줌
			break;
		case 'b': case 'B': //command가 b혹은 B일떄
			debugQ(cQ); //debugQ함수 호출 cQ를 매개변수로 넣어줌
			break;
		case 'q': case 'Q': //command가 q혹은 Q일떄
			break; //while문의 조건을 타서 정지
		default: //위의 case를 제외한 다른조건에서 
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); //command가 q혹은 Q이기 전까지 반복


	return 1; //실행이 성공적임에 따른 return1
}

QueueType *createQueue() //QueueType타입 포인터createQueue()함수 
{ 
	QueueType *cQ; //QueueType타입 cQ포인터
	cQ = (QueueType *)malloc(sizeof(QueueType)); //cQ를 QueueType타입 싸이즈에 크기만큼 malloc으로 동적할당 받음
	cQ->front = 0; //cQ포인터 front를 0으로 초기화
	cQ->rear = 0; //cQ포인터 rear를 0으로 초기화
	return cQ; //cQ를 return
}

int freeQueue(QueueType *cQ) //int형 freeQueue함수 QueueType *cQ를 매개변수로 받아온다.
{
    if(cQ == NULL) return 1; //만약 cQ가 비었을경우 아무동작 하지않고 1을 return
    free(cQ); //cQ의 동적할당 해제
    return 1; //동작이 다 잘됏으면 1return
}

element getElement() //element형 getElement함수
{
	element item; //elemnet형 item
	printf("Input element = "); 
	scanf(" %c", &item); //char형 입력값 item주소에 받아오기
	return item; //item return
}


int isEmpty(QueueType *cQ) //int형 isEmpty함수 QueueType,*cQ를 매개변수로 받아온다.
{
	if (cQ->front == cQ->rear){ //cQ의 fornt와 rear의 위치가 같다면 
		printf("Circular Queue is empty!"); //Que에 아무것도 안들었다는 경고
		return 1;//후 return1
	}
	else return 0;//또는 return0
}

int isFull(QueueType *cQ) //int형 isFull함수 QueueType,*cQ를 매개변수로 받아온다.
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { //cQ의 rear를 증가시킨것이 fornt와 같다면 
		printf(" Circular Queue is full!"); //가득찼다는 경고
		return 1;//후 return1
	}
	else return 0;//또는 return0
}

void enQueue(QueueType *cQ, element item)//void형 enQueue함수 QueueType *cQ, element item를 매개변수로 받아온다.
{
	if(isFull(cQ)) return;//isFull함수호출 cQ매겨변수로 넣어준다(가득찼는지 확인)
	else {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; //rear를 증가시키고
		cQ->queue[cQ->rear] = item; //queue[rear]에 item을 넣는다.
	}
}

void deQueue(QueueType *cQ, element *item)//void형 enQueue함수 QueueType *cQ, element item를 매개변수로 받아온다.
{
	if(isEmpty(cQ)) return;//isEmpty함수호출 cQ매겨변수로 넣어준다(큐가 비었는지 확인)
	else {
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE; //front를 증가시키고
		*item = cQ->queue[cQ->front]; //item에 queue[cQ->front]값을 넣어주고 return
		return;
	}
}


void printQ(QueueType *cQ) //printQ함수 QueueType,*cQ를 매개변수로 받아온다.
{
	int i, first, last; //int형 i,first,last정의

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; //first에 증가시킨 front
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; //last에 증가시킨 rear값을 넣어준다

	printf("Circular Queue : [");

	i = first; //i를 first값으로 정의하고
	while(i != last){ //i가 last값이랑 같아질 때까지
		printf("%3c", cQ->queue[i]); //queue[i]값을 출력한다
		i = (i+1)%MAX_QUEUE_SIZE; //i를 증가시킴

	}
	printf(" ]\n"); //que안의 값을 다 출력하는 함수이다
}


void debugQ(QueueType *cQ) //debugQ함수 QueueType,*cQ를 매개변수로 받아온다.
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)//i를 0부터 MAX_QUEUE_SIZE까지 증가시키면서
	{
		if(i == cQ->front) {
			printf("  [%d] = front\n", i); //fornt위치 찾아 출력
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); //i번째에 어떤 값이 들어있는지 출력

	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); //fornt와 rear위치 출력
}