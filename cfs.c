#include "ku_cfs.h"

int main(int argc, char * argv[])
{

    int sum = 0; // alphabet 의 범위를 넘어가는지 check
 	for(i = 1 ; i <= 5 ; i++)
    {
 		input[i-1] = atoi(argv[i]);	
        sum+=input[i-1];
 	}

    total = atoi(argv[6]);

    printf("%d\n",total);
    if(sum > 27) printf("잘못된 프로세스의 수 입니다.");

    head = malloc(sizeof(struct NODE)); // linkedlist 생성

 	// linkedlist 에 nice 값 별 process 등록
    int cnt = 0;
    for(i = 0 ; i < 5 ; i++)
    {
	for(j = 0 ; j < input[i] ; j++)
    	{
		push(cnt++, 0, i, 0); // 어떤 nice 값에 속하는지 넣음
	}
    }		

    printList();
    puts("=======================");
    sortList();
    puts("=======================");
    printList();

    // fork & execl
    for(i = 0 ; i < sum ; i++)
    {   
        pid[i] = fork();
        if(pid[i] == 0) execl("./ku_app","ku_app",(const char*)alpha[i],NULL);
    }

    // handler 등록
    struct sigaction handler;
    handler.sa_handler = my_handler;
    sigaction(SIGALRM, &handler, NULL);

    // timer 설정
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    sleep(5);

    // timer 시작
    setitimer(ITIMER_REAL, &timer, NULL);

    while(1)
    {
        pause();
    }

    freeList();
}


void my_handler(int signum)
{
    int prev_idx;

    // 첫번째 timeslice 인 경우
    if(cnt_ts == 0)
    {
        killAll(); // process 상태 체크
        prev_idx = getIdx();
        cnt_ts++;
        updateVtime();
	    printList();
        kill(pid[prev_idx],SIGCONT);
    }

    // 나머지 경우
    else
    {
        kill(pid[prev_idx],SIGSTOP); // 전에 실행중이던 process 를 STOP
        killAll(); 
        updateMycnt(); // my_cnt update
        updateVtime(); // vtime update

        sortList(); // sort
        printList();

        prev_idx = getIdx(); // 맨 앞 process의 index 를 얻어
        cnt_ts++;
        kill(pid[prev_idx],SIGCONT); // 실행시킨다
    }
}


void sortList()
{
    int swapped, i;
    struct NODE *a;
    struct NODE *b = NULL;
 
    if (a == NULL) return;

    do
    {
        swapped = 0;
        a = head;

        while (a->next != b)
        {
            if (a->vtime > a->next->vtime) // vtime 에 대해 정렬
            { 
                swap(a, a->next);
                swapped = 1;
            }
            else if(a->vtime == a->next->vtime) // vtime 이 같은 경우 index와 nice에 대해 정렬
            {
                if(a->index > a->next->index || a->my_nice > a->next->my_nice)
		{
                    swap(a, a->next);
                    swapped = 1;
                }
            }
            a = a->next;
       }
        b = a;
    }
    while (swapped);
}
 
void swap(struct NODE *a, struct NODE *b)
{
    float temp = a->vtime;
    a->vtime = b->vtime;
    b->vtime = temp;

    int temp_index = a->index;
    a->index = b->index;
    b->index = temp_index;

    int temp_cnt = a->my_cnt;
    a->my_cnt = b->my_cnt;
    b->my_cnt = temp_cnt;

    int temp_my_nice = a->my_nice;
    a->my_nice = b->my_nice;
    b->my_nice = temp_my_nice;


}


void freeList()
{
    temp = head->next;      
	while (temp != NULL)    
	{
	    struct NODE *next = temp->next;    
	    free(temp);                        
	    temp = next;                      
	}
}

void printList(){
    temp = head->next;   
    while (temp != NULL)               
    {
        printf("%d %d %d %f\n", temp->index, temp->my_cnt, temp-> my_nice, temp->vtime);   
        temp = temp->next;            
    }
}

void push(int index, int my_cnt, int my_nice, float vtime)   
{
    struct NODE *newNODE = malloc(sizeof(struct NODE));   
    newNODE->next = head->next;    
    newNODE->index = index;
    newNODE->vtime = vtime;      
    newNODE->my_cnt = my_cnt;  
    newNODE->my_nice = my_nice;    

    head->next = newNODE;   
}

void updateMycnt(){ // 해당 process 가 실행된 횟수 check
    temp = head->next;
    temp->my_cnt = temp->my_cnt+1;
}

void updateVtime() // 실행된 process의 vruntime update
{
    temp = head->next;   
	temp->vtime = nice_val[temp->my_nice] * temp->my_cnt;
}

int getIdx()
{
    temp = head->next;
    return temp->index;
}

void killAll()
{    
    if(cnt_ts >= total)
    {
        for(i = 0; i < total; i++)
        {
            kill(pid[i],SIGKILL);
        }
        kill(getpid(),SIGKILL);
    }
}
