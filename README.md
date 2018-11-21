# Operating System



> 2018 건국대 운영체제 과제


<br>

#### CFS (Completly Fair Scheduler) 구현

- ArrayList 구현

- Signal Handler 구현

  - 이전에 실행하던 프로세스를 stop

  - vruntime 기준으로 정렬 후 프로세스를 실행

  - 남은 프로세스들을 kill

<br>

#### Malloc & Free 구현

Nextfit 방식으로 Freelist 를 구현

- malloc

  - freelist 의 공간을 고려해 요청한 크기 만큼을 할당

  - freelist 내의 공간을 nextfit 방식으로 찾기

  - freelist 내의 공간을 split 해 할당

- free

  - magic num 을 검사 후 요청한 크기 만큼을 해제
  - 연속된 freelist 의 경우 coleasing 
