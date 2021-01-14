#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

pthread_mutex_t gameStart_mutex;
pthread_cond_t gameStart_cv;
pthread_mutex_t gameOver_mutex;
pthread_cond_t gameOver_cv;
pthread_mutex_t p1_mutex;
pthread_cond_t p1_cv;
pthread_mutex_t p2_mutex;
pthread_cond_t p2_cv;
pthread_mutex_t p3_mutex;
pthread_cond_t p3_cv;
pthread_mutex_t p4_mutex;
pthread_cond_t p4_cv;
pthread_mutex_t print_mutex;

FILE *logFile;
int teamAlastThrow = 0;
int teamBlastThrow = 0;
int winCondition = 0;

void* dealerFunction()
{
    pthread_mutex_lock(&gameStart_mutex);
    pthread_cond_wait(&gameStart_cv, &gameStart_mutex);
    pthread_mutex_unlock(&gameStart_mutex);

    int firstPlayer = rand() % 4;

    switch(firstPlayer)
    {
        case 0:
	    pthread_mutex_lock(&p1_mutex);
	    pthread_cond_signal(&p1_cv);
	    pthread_mutex_unlock(&p1_mutex);
	    break;
	case 1:
            pthread_mutex_lock(&p2_mutex);
            pthread_cond_signal(&p2_cv);
            pthread_mutex_unlock(&p2_mutex);
            break;
	case 2:
            pthread_mutex_lock(&p3_mutex);
            pthread_cond_signal(&p3_cv);
            pthread_mutex_unlock(&p3_mutex);
            break;
	case 3:
            pthread_mutex_lock(&p4_mutex);
            pthread_cond_signal(&p4_cv);
            pthread_mutex_unlock(&p4_mutex);
            break;
    	default:
	    printf("Error\n");
	    break;
    }

    pthread_mutex_lock(&gameOver_mutex);
    pthread_cond_wait(&gameOver_cv, &gameOver_mutex);
    pthread_mutex_unlock(&gameOver_mutex);

    pthread_mutex_lock(&p1_mutex);
    pthread_cond_signal(&p1_cv);
    pthread_mutex_unlock(&p1_mutex);

    pthread_mutex_lock(&p2_mutex);
    pthread_cond_signal(&p2_cv);
    pthread_mutex_unlock(&p2_mutex);

    pthread_mutex_lock(&p3_mutex);
    pthread_cond_signal(&p3_cv);
    pthread_mutex_unlock(&p3_mutex);

    pthread_mutex_lock(&p4_mutex);
    pthread_cond_signal(&p4_cv);
    pthread_mutex_unlock(&p4_mutex);

    if(winCondition == 1)
    {
	pthread_mutex_lock(&print_mutex);
        printf("DEALER: The winning team is A and C\n");
	fprintf(logFile, "DEALER: The winning team is A and C\n");
	pthread_mutex_unlock(&print_mutex);
    }else
    {
	pthread_mutex_lock(&print_mutex);
        printf("DEALER: The winning team is B and D\n");
	fprintf(logFile, "DEALER: The winning team is B and D\n");
        pthread_mutex_unlock(&print_mutex);	
    }

    return NULL;
}

void* player1Function()
{
    while(!winCondition)
    {
        pthread_mutex_lock(&p1_mutex);
        pthread_cond_wait(&p1_cv, &p1_mutex);
        pthread_mutex_unlock(&p1_mutex);
	if(winCondition) {pthread_exit(NULL);}

        int dice1 = rand() % 6 + 1;
	int dice2 = rand() % 6 + 1;
	int throw = dice1 + dice2;

	pthread_mutex_lock(&print_mutex);
        printf("PLAYER A: gets %d and %d with a sum %d\n", dice1, dice2, throw);
	fprintf(logFile, "PLAYER A: gets %d and %d with a sum %d\n", dice1, dice2, throw);
        pthread_mutex_unlock(&print_mutex);

	if(throw == teamAlastThrow)
	{
	    winCondition = 1;
	}else
	{
	    teamAlastThrow = throw;
	}

	if(!winCondition)
        {
            pthread_mutex_lock(&p2_mutex);
            pthread_cond_signal(&p2_cv);
            pthread_mutex_unlock(&p2_mutex);
        }else
        {
            pthread_mutex_lock(&gameOver_mutex);
            pthread_cond_signal(&gameOver_cv);
            pthread_mutex_unlock(&gameOver_mutex);
        }

    }

    return NULL;
}

void* player2Function()
{
    while(!winCondition)
    {
        pthread_mutex_lock(&p2_mutex);
        pthread_cond_wait(&p2_cv, &p2_mutex);
        pthread_mutex_unlock(&p2_mutex);
	if(winCondition) {pthread_exit(NULL);}

	int dice1 = rand() % 6 + 1;
        int dice2 = rand() % 6 + 1;
        int throw = dice1 + dice2;

        pthread_mutex_lock(&print_mutex);
        printf("PLAYER B: gets %d and %d with a sum %d\n", dice1, dice2, throw);
	fprintf(logFile, "PLAYER B: gets %d and %d with a sum %d\n", dice1, dice2, throw);
        pthread_mutex_unlock(&print_mutex);

        if(throw == teamBlastThrow)
        {
            winCondition = 2;
        }else
        {
            teamBlastThrow = throw;
        }
	
	if(!winCondition)
        {
            pthread_mutex_lock(&p3_mutex);
            pthread_cond_signal(&p3_cv);
            pthread_mutex_unlock(&p3_mutex);
        }else
        {
            pthread_mutex_lock(&gameOver_mutex);
            pthread_cond_signal(&gameOver_cv);
            pthread_mutex_unlock(&gameOver_mutex);
        }

    }

    return NULL;
}

void* player3Function()
{
    while(!winCondition)
    {
        pthread_mutex_lock(&p3_mutex);
        pthread_cond_wait(&p3_cv, &p3_mutex);
	pthread_mutex_unlock(&p3_mutex);
	if(winCondition) {pthread_exit(NULL);}   

        int dice1 = rand() % 6 + 1;
        int dice2 = rand() % 6 + 1;
        int throw = dice1 + dice2;

        pthread_mutex_lock(&print_mutex);
        printf("PLAYER C: gets %d and %d with a sum %d\n", dice1, dice2, throw);
	fprintf(logFile, "PLAYER C: gets %d and %d with a sum %d\n", dice1, dice2, throw);
        pthread_mutex_unlock(&print_mutex);

        if(throw == teamAlastThrow)
        {
            winCondition = 1;
        }else
        {
            teamAlastThrow = throw;
        }        

	if(!winCondition)
        {
            pthread_mutex_lock(&p4_mutex);
            pthread_cond_signal(&p4_cv);
            pthread_mutex_unlock(&p4_mutex);
        }else
        {
            pthread_mutex_lock(&gameOver_mutex);
            pthread_cond_signal(&gameOver_cv);
            pthread_mutex_unlock(&gameOver_mutex);
        }

    }

    return NULL;
}

void* player4Function()
{
    while(!winCondition)
    {
        pthread_mutex_lock(&p4_mutex);
        pthread_cond_wait(&p4_cv, &p4_mutex);
        pthread_mutex_unlock(&p4_mutex);
	if(winCondition) {pthread_exit(NULL);}

        int dice1 = rand() % 6 + 1;
        int dice2 = rand() % 6 + 1;
        int throw = dice1 + dice2;

        pthread_mutex_lock(&print_mutex);
        printf("PLAYER D: gets %d and %d with a sum %d\n", dice1, dice2, throw);
	fprintf(logFile, "PLAYER D: gets %d and %d with a sum %d\n", dice1, dice2, throw);
        pthread_mutex_unlock(&print_mutex);

        if(throw == teamBlastThrow)
        {
            winCondition = 2;
        }else
        {
            teamBlastThrow = throw;
        }

	if(!winCondition)
	{
	    pthread_mutex_lock(&p1_mutex);
            pthread_cond_signal(&p1_cv);
	    pthread_mutex_unlock(&p1_mutex);
	}else
	{
	    pthread_mutex_lock(&gameOver_mutex);
	    pthread_cond_signal(&gameOver_cv);
	    pthread_mutex_unlock(&gameOver_mutex);
	}
    }

    return NULL;
}


int main(int argc, char* argv[])
{
    if (argc != 2) {fprintf(stderr, "USAGE: %s seed\n", argv[0]); exit(-1);}
    int seed = atoi(argv[1]);
    srand(seed);

    logFile = fopen("log.txt", "w");

    pthread_t dealer;
    pthread_t player1;
    pthread_t player2;
    pthread_t player3;
    pthread_t player4;

    pthread_mutex_init(&gameStart_mutex, NULL);
    pthread_cond_init(&gameStart_cv, NULL);
    pthread_mutex_init(&gameOver_mutex, NULL);
    pthread_cond_init(&gameOver_cv, NULL);
    pthread_mutex_init(&p1_mutex, NULL);
    pthread_cond_init(&p1_cv, NULL);
    pthread_mutex_init(&p2_mutex, NULL);
    pthread_cond_init(&p2_cv, NULL);
    pthread_mutex_init(&p3_mutex, NULL);
    pthread_cond_init(&p3_cv, NULL);
    pthread_mutex_init(&p4_mutex, NULL);
    pthread_cond_init(&p4_cv, NULL);
    pthread_mutex_init(&print_mutex, NULL);
    
    pthread_create(&dealer, NULL, dealerFunction, NULL);
    pthread_create(&player1, NULL, player1Function, NULL);
    pthread_create(&player2, NULL, player2Function, NULL);
    pthread_create(&player3, NULL, player3Function, NULL);
    pthread_create(&player4, NULL, player4Function, NULL);

    pthread_mutex_lock(&gameStart_mutex);
    pthread_cond_signal(&gameStart_cv);
    pthread_mutex_unlock(&gameStart_mutex);

    pthread_join(dealer, NULL);
    pthread_join(player1, NULL);
    pthread_join(player2, NULL);
    pthread_join(player3, NULL);
    pthread_join(player4, NULL);

    fclose(logFile);

    pthread_mutex_destroy(&gameStart_mutex);
    pthread_cond_destroy(&gameStart_cv);
    pthread_mutex_destroy(&gameOver_mutex);
    pthread_cond_destroy(&gameOver_cv);
    pthread_mutex_destroy(&p1_mutex);
    pthread_cond_destroy(&p1_cv);
    pthread_mutex_destroy(&p2_mutex);
    pthread_cond_destroy(&p2_cv);
    pthread_mutex_destroy(&p3_mutex);
    pthread_cond_destroy(&p3_cv);
    pthread_mutex_destroy(&p4_mutex);
    pthread_cond_destroy(&p4_cv);
    pthread_mutex_destroy(&print_mutex);

    return 0;
}
