#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
int k, a, e, c, t1, t2;
double t;
pthread_t per[1000];
pthread_t atics[1000];

int min(int a, int b)
{
	if ( b > a )
	return a;
	else
	return b;	
}
int genrandom(int lower, int upper)
{
	int num = rand() % (upper + 1 - lower) ;
	num +=  lower ;
	return num;
}

//all structs start here
struct coordinator
{
	pthread_mutex_t lock;
	int available;
} coordinators[1000];

typedef struct performer 
{
	int arrivaltime;
	char name[10] , ic;
} performer;

struct ele
{
	sem_t mutex;
	char prev , name[10];
} elec[1000];

struct acou
{
	sem_t mutex;
	char prev , name[10];
} acous[1000];


performer performers[1000];

int eleccheck(char* name, char ch, int time);
int acouscheck(char* name, char ch, int time);
void collectTshirt(char* name);
void* performersfun(void* x);

void collectTshirt(char* name)
{
	int terminate = 1;
	while (terminate)
	{
		for(int i = 0;i < c; i++)
		{
			pthread_mutex_lock(&coordinators[i].lock);
			if (coordinators[i].available == 1)
			{
				coordinators[i].available = 0;
				printf("%s collecting T-shirt\n", name);
				sleep(2);
				terminate = 0;
				coordinators[i].available = 1;
				printf("T-shirt collected\n");
				pthread_mutex_unlock(&coordinators[i].lock);
				break;
			}
			pthread_mutex_unlock(&coordinators[i].lock);
		}
	}
}

int acouscheck(char* name, char ch, int time)
{
	int index = -1;
	for(int i = 0;i < a; i++)
	{
		if (acous[i].prev == '0')
		{
			sem_wait(&acous[i].mutex);
			acous[i].prev = ch;
			index = i;
			strcpy(acous[i].name, name);
			break;
		}
	}
	return index;
}
int eleccheck(char* name, char ch, int time)
{
	int index = -1;
	for(int i = 0;i < e; i++)
	{
		if (elec[i].prev == '0')
		{
			sem_wait(&elec[i].mutex);
			elec[i].prev = ch;
			index = i;
			strcpy(elec[i].name, name);
			break;
		}
	}
	return index;
}

void* performersfun(void* x)
{
	performer* inp = (performer*) x;
	volatile double elapsed;
	int terminate = 1 , flag = 0;
	int sleepyy = inp->arrivaltime , time1;
	sleep(sleepyy);
	printf("%s %c Arrived\n", inp->name, inp->ic);
	srand(time(0));
	time1 = genrandom(t1, t2);
	time_t begin , now;
	time(&begin);
	while (terminate)
	{
		if (inp->ic == 'g' || inp->ic == 'p')
		{
			fflush(stdout);
			int x = 1;//genrandom(1, 2);
			if (x == 1)
			{
				int index = acouscheck(inp->name, inp->ic, time1);
				if ( ! (index + 1) )
				{
					index = eleccheck(inp->name, inp->ic, time1);
					if (index + 1)
					{
						flag = 1;
						printf("%s performing %c at electric stage for %d sec\n", inp->name, inp->ic, time1);
						sleep(time1);
						if (elec[index].prev == 's')
							sleep(2);
						printf("%s performance at electric stage finished\n", inp->name);
						elec[index].prev = '0';	
						sem_post(&elec[index].mutex);
						collectTshirt(inp->name);
						pthread_exit(NULL);
						break;
					}
				}
				else
				{
					flag = 1;
					printf("%s performing %c at acoustic stage for %d sec\n", inp->name, inp->ic, time1);
					sleep(time1);
					if (acous[index].prev == 's')
						sleep(2);
					printf("%s performance at acoustic stage finished\n", inp->name);
					acous[index].prev = '0';
					sem_post(&acous[index].mutex);
					collectTshirt(inp->name);
					pthread_exit(NULL);
					break;
				}
			}
			else
			{
				int index = eleccheck(inp->name, inp->ic, time1);
				if (!(index+1))
				{
					index = acouscheck(inp->name, inp->ic, time1);
					if (index+1)
					{
						flag = 1;
						printf("%s performing %c at acoustic stage for %d sec\n", inp->name, inp->ic, time1);
						sleep(time1);
						if (acous[index].prev == 's')
							sleep(2);
						printf("%s performance at acoustic stage finished\n", inp->name);
						acous[index].prev = '0';
						sem_post(&acous[index].mutex);
						collectTshirt(inp->name);
						pthread_exit(NULL);
						break;
					}
				}
				else
				{
					flag = 1;
					printf("%s performing %c at electric stage for %d sec\n", inp->name, inp->ic, time1);
					sleep(time1);
					if (elec[index].prev == 's')
						sleep(2);
					printf("%s performance at electric stage finished\n", inp->name);
					elec[index].prev = '0';
					collectTshirt(inp->name);
					sem_post(&elec[index].mutex);
					pthread_exit(NULL);
				}
				if (index+1)
					break;
			}
		}
		if (inp->ic == 'v')
		{
			int index;
			index = acouscheck(inp->name, inp->ic, time1);
			if (index != -1)
			{
				flag = 1;
				printf("%s performing %c at acoustic stage for %d sec\n", inp->name, inp->ic, time1);
				sleep(time1);
				if (acous[index].prev == 's')
					sleep(2);
				printf("%s performance at acoustic stage finished\n", inp->name);
				acous[index].prev = '0';
				collectTshirt(inp->name);
				sem_post(&acous[index].mutex);
				pthread_exit(NULL);
				break;
			}
		}
		if (inp->ic == 'b')
		{
			int index;
			index = eleccheck(inp->name, inp->ic, time1);
			if (index != -1)
			{
				flag = 1;
				printf("%s performing %c at electric stage for %d sec\n", inp->name, inp->ic, time1);
				sleep(time1);
				if (elec[index].prev == 's')
					sleep(2);
				sem_post(&elec[index].mutex);
				elec[index].prev = '0';
				collectTshirt(inp->name);
				pthread_exit(NULL);
				break;
			}
		}
		if (inp->ic == 's')
		{
			int chose1,chose2;
			chose1 = genrandom(1, 2);
			if (e == 0)
				chose1 = 1;
			if (a == 0)
				chose1 = 2;
			if (chose1 == 1)
			{
				chose2 = genrandom(1, 2);
				if (chose2 == 1)
				{
					int index = acouscheck(inp->name, inp->ic, time1);
					if (index == -1)
					{
						for(int i = 0;i < a; i++)
						{
							if (acous[i].prev != 's')
							{
								sem_wait(&acous[i].mutex);
								index = i;
								acous[i].prev = 's';
								flag = 1;
								printf("\033[1;31m%s joined %s's performance, performance extended by 2 secs\n\033[0m", inp->name, acous[i].name);
								while (acous[i].prev - '0')
								{}
								sem_post(&acous[i].mutex);
								pthread_exit(NULL);
								break;
							}
							else
								sem_post(&acous[i].mutex);
						}
					}
					else
					{
						flag = 1;
						printf("%s performing %c at acoustic stage for %d sec\n", inp->name, inp->ic, time1);
						sleep(time1);
						printf("%s performance at acoustic stage finished\n", inp->name);
						acous[index].prev = '0';
						sem_post(&acous[index].mutex);
						pthread_exit(NULL);
						break;
					}
				}
				else
				{
					int index = -1;
					for(int i = 0;i < a; i++)
					{
						if (acous[i].prev - 's' && acous[i].prev - '0')
						{
							sem_wait(&acous[i].mutex);
							index = i;
							acous[i].prev = 's';
							flag = 1;
							printf("\033[1;31m%s joined %s's performance, performance extended by 2 secs\n\033[0m", inp->name, acous[i].name);
							while (acous[i].prev - '0')
							{}
							sem_post(&acous[i].mutex);
							pthread_exit(NULL);
							break;
						}
						else
							sem_post(&acous[index].mutex);
					}
					if (!(index+1))
					{
						index = acouscheck(inp->name, inp->ic, time1);
						if (index+1)
						{
							flag = 1;
							printf("%s performing %c at acoustic stage for %d sec\n", inp->name, inp->ic, time1);
							sleep(time1);
							printf("%s performance at acoustic stage finished\n", inp->name);
							acous[index].prev = '0';
							sem_post(&acous[index].mutex);
							pthread_exit(NULL);
							break;
						}
					}
				}
			}
			else
			{
				chose2 = genrandom(1, 2);
				if (chose2 == 1)
				{
					int index = eleccheck(inp->name, inp->ic, time1);
					if (index == -1)
					{
						for(int i = 0;i < e; i++)
						{
							if (elec[i].prev != 's')
							{
								sem_wait(&elec[i].mutex);
								index = i;
								elec[i].prev = 's';
								flag = 1;
								printf("\033[1;31m%s joined %s's performance, performance extended by 2 secs\n\033[0m", inp->name, elec[i].name);
								while (elec[i].prev - '0')
								{
									//do nothing
								}
								sem_post(&elec[i].mutex);
								pthread_exit(NULL);
								break;
							}
							else
								sem_post(&elec[i].mutex);
						}
					}
					else
					{
						flag = 1;
						printf("%s performing %c at electric stage for %d sec\n", inp->name, inp->ic, time1);
						sleep(time1);
						printf("%s performance at electric stage finished\n", inp->name);
						elec[index].prev = '0';
						sem_post(&elec[index].mutex);
						pthread_exit(NULL);
						break;
					}
				}
				else
				{
					int index = -1;
					for(int i = 0;i < e; i++)
					{
						if (elec[i].prev != 's' && elec[i].prev != '0')
						{
							elec[i].prev = 's';
							sem_wait(&elec[i].mutex);
							index = i;
							flag = 1;
							printf("\033[1;31m%s joined %s's performance, performance extended by 2 secs\n\033[0m", inp->name, elec[i].name);
							while (elec[i].prev - '0')
								{
									//do nothing
								}
							sem_post(&elec[i].mutex);
							pthread_exit(NULL);
							break;
						}
						else
							sem_post(&elec[i].mutex);
					}
					if (!(index+1))
					{
						index = eleccheck(inp->name, inp->ic, time1);
						if (index +1)
						{
							flag = 1;
							printf("%s performing %c at electric stage for %d sec\n", inp->name, inp->ic, time1);
							sleep(time1);
							printf("%s performance at electric stage finished\n", inp->name);
							elec[index].prev = '0';
							sem_post(&elec[index].mutex);
							pthread_exit(NULL);
							break;
						}
					}
				}
			}
		}
		time(&now);
		elapsed = difftime(now, begin);
		if (elapsed > t)
		{
			if (flag == 0)
			{
				terminate = 0;
				printf("%s %c left because of impatience\n", inp->name, inp->ic);
				pthread_exit(NULL);
				break;
			}
		}
	}
	pthread_exit(NULL);
}
int main(void)
{
	scanf("%d %d %d %d %d %d %lf", &k, &a, &e, &c, &t1, &t2, &t);

	for(int i = 0;i < k; i++)
		scanf("%s %c %d", performers[i].name, &performers[i].ic, &performers[i].arrivaltime);
	
	for(int i = 0;i < a; i++)
	{
		acous[i].prev = '0';
		sem_init(&acous[i].mutex, 0, 2);
	}
	
	for(int i = 0;i < e; i++)
	{
		elec[i].prev = '0';
		sem_init(&elec[i].mutex, 0, 2);
	}
	
	for(int i = 0;i < c; i++)
	{
		coordinators[i].available = 1;
		pthread_mutex_init(&coordinators[i].lock, NULL);
	}
	
	for(int i = 0;i < k; i++)
		pthread_create(&per[i], NULL, performersfun, (void*)(&performers[i]));
	
	for(int i = 0;i < k; i++)
		pthread_join(per[i], NULL);
	
	for(int i = 0;i < a; i++)
		sem_destroy(&acous[i].mutex);
	
	for(int i = 0;i < e; i++)
		sem_destroy(&elec[i].mutex);
	
	printf("Event Over\n");
}
