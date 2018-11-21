#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

struct NODE 
{  
    struct NODE *next;   
    int index; // 몇번째 process인지 index
    int my_cnt; // 해당 process가 몇번 실행되었는지 check
    int my_nice; // 몇번째 nice 값을 가지는지 
    float vtime; // vruntime 계산
};

struct NODE *head;
struct NODE *temp;

int cnt_ts = 0; // time slice 수
pid_t pid[27]; // fork 한 pid
int input[6]; // argv
char *alpha[27] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
float nice_val[5] = {0.64, 0.8, 1, 1.25, 1.5625}; // nice값 저장
int i,j,total;

void push(int index, int my_cnt, int my_nice, float vtime); // queue 의 맨앞에 원소 넣기
void printList(); // 모든 원소 출력 (확인용)
void freeList(); // malloc free
void sortList(); // vruntime 순으로 정렬
void swap(struct NODE *a, struct NODE *b); // sort 위한 함수
int getIdx(); // queue 의 맨 앞의 index 확인
void updateVtime(); // timeslice 마다 vruntime update
void my_handler(int signum); // signalhandler
void killAll(); // process들 정리
void updateMycnt();
