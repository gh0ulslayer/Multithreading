#include <stdio.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#define pnf(k) printf("%s", k);

int i, n, m, s, waitingggg = 0;
int student_vaccined = 0 , student_rem, yet_to_be ,students_came = 0;
pthread_mutex_t student_gonee;
pthread_mutex_t debug_lock;


//all structs start from ere
typedef struct Pharmacompany
{
	int company_no , state, no_of_batches, second, stud_vaccined;
	float efficiency;
	pthread_t thread_ID; 
} Pharmacompany;

typedef struct VZone
{
	int Vzone_no, slots, vaccined, capacity, state, wait;
	float curr_efficiency;
	pthread_mutex_t lockbatch;
	pthread_t thread_ID; 
} VZone;

typedef struct Student
{
	int state, student_num, zone_no, round_no;
	pthread_t thread_ID;
} stud;

//function declarations
void preparing_vaccines( Pharmacompany *pc );
void vaccine_ready( Pharmacompany *pc );
void zone_wait( VZone *v );
void ready_to_vaccinate( VZone *v );
void student_coming( int ridd );
void studdd_vaccine( stud *st );
void student_wait( stud *st );

Pharmacompany *pharmacompanies;
VZone *zones;
stud *students;

void preparing_vaccines( Pharmacompany *pc )
{
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);
	while(1)
	{
		if ( !student_rem  ) //All students vaccinated
			break;
		pc -> stud_vaccined = 10 + rand() % 11 ;
		pc -> state = 1;
		pc -> no_of_batches = 1 + rand() & 5;
		pc -> second = 2 + rand() % 4;
		if ( pc -> stud_vaccined > student_rem ) 
		{
			pc -> no_of_batches = 1;
			pc -> stud_vaccined = 1 + rand() % student_rem;
		}
		pc -> state = 2;
		printf("Pharma Company %d is preparing  %d batches of vaccine with %d capacity each which have prob %1.2f\n" , pc -> company_no, pc -> no_of_batches, pc -> stud_vaccined , pc -> efficiency);
		sleep( pc -> second );
		printf("Pharma Company %d has prepared %d batches of vaccine with %d capacity each which have prob %1.2f\n" , pc -> company_no, pc -> no_of_batches, pc -> stud_vaccined , pc -> efficiency);
		vaccine_ready(pc);
	}
	pthread_exit(NULL);
} 

void vaccine_ready( Pharmacompany *pc )
{
	int noo = pc -> no_of_batches;
	while (noo)
	{
		if ( !student_rem )
		{
			pthread_mutex_unlock(&zones[i].lockbatch);
			return;
		}
		for(int i = 1 ; i <= m ; i++)
		{
			pthread_mutex_lock(&zones[i].lockbatch);
			if (zones[i].capacity < 1 && zones[i].state == 0 )
			{
				noo--;
				printf("Pharmaceutical Company %d is delivering a vaccine batch to Vaccination Zone %d which has success probability %1.2f\n", pc -> company_no ,i, pc -> efficiency);
				zones[i].state = 2; //filled
				zones[i].capacity = pc -> stud_vaccined;
				zones[i].curr_efficiency = pc -> efficiency;
			}
			if ( noo == 0 )
			{
				printf("Pharmaceutical Company %d again started preparing batches\n", pc -> company_no );
				pthread_mutex_unlock(&zones[i].lockbatch);
				return;
			}
			if ( ! student_rem )
			{
				pthread_mutex_unlock(&zones[i].lockbatch);
				return;
			}
			pthread_mutex_unlock(&zones[i].lockbatch);

		}
	}
}

void zone_wait( VZone *v)
{
	struct timeval t2;
	gettimeofday(&t2 , NULL);
	srand(t2.tv_usec * t2.tv_sec);
	while(1)
	{
		while( v -> state == 0 )
		{
			//do nothing
		}
		if ( v -> capacity >= 1 )
		{
			v -> slots = 1 + rand() % v -> capacity;
			printf("Vaccination Zone %d with capacity = %d and available slots = %d is ready to vaccinate\n", v -> Vzone_no , v -> capacity , v -> slots);
			v -> vaccined = 0;
			ready_to_vaccinate(v);
		}
		if ( ! student_rem ) break;
		if ( v -> capacity == 0 )
		{
			printf("Vaccination Zone %d is waiting for the next batch of vaccines\n", v -> Vzone_no );
			v -> state = 0;
		}
	}
	pthread_exit(NULL);
}

void ready_to_vaccinate( VZone *v )
{
	while(1)
	{
		if ( ! student_rem ) return;
		int tslot = v -> slots;
		v -> state = 1;
		if (tslot == 0)
		{
			printf("Vaccination Zone %d with capacity = %d has filled all the slots.\n",v -> Vzone_no , v -> capacity );
			v -> state = 2;
			return;
		}
	}
}

void student_coming(int ridd)
{
	while( s > students_came )
	{
		int p , q; 
		sleep( rand() % 4 );
		p = 1 + rand() % 6;
		while( ( s - students_came ) / p == 0 )
			p = 1 + rand() % 6;

		q = 1 + rand() % ( ( s - students_came ) / p ) ;
		if (q)
		{
			for(int  i = students_came + 1 ; i < students_came + q + 1; i++ )
				pthread_create(&students[i].thread_ID , NULL , (void *)student_wait , &students[i]);
		}
		students_came += q;
	}
	pthread_exit(NULL);
}

void student_wait( stud *st )
{
	while ( st -> round_no < 4 )
	{
		printf("Student %d has arrived for his %d round of Vaccination.\n", st -> student_num , st -> round_no );
		printf("Student %d is waiting to be allocated a slot on a Vaccination Zone.\n", st -> student_num );
		waitingggg++;
		while( st -> state !=1 )
		{
			if ( ! st -> state )
			{
				for(int i = 1 ; i <= m ; i++)
				{
					pthread_mutex_lock(&zones[i].lockbatch);
					if (  zones[i].state == 1  && zones[i].slots > 0 && zones[i].slots != zones[i].vaccined )
					{
						st -> state = 2;
						printf("Student %d assigned a slot on the Vaccination Zone %d and waiting to be vaccinated.\n", st -> student_num , zones[i].Vzone_no );
						waitingggg--;
						st -> zone_no = i;
						zones[i].vaccined++;
						
						pthread_mutex_unlock(&zones[i].lockbatch);
						break;
					}
					pthread_mutex_unlock(&zones[i].lockbatch);
				}
			}
			if ( st -> state == 2 )
			{
				while ( 1 ) 
				{
					if (zones[ st -> zone_no ].vaccined == zones[ st -> zone_no ].slots ) break;
					if ( waitingggg == 0 ) break;
				}
				studdd_vaccine(st);
			}
		}
		pthread_mutex_lock(&student_gonee);
		student_rem--;
		pthread_mutex_unlock(&student_gonee);
	}
	pthread_exit(NULL);
}

void studdd_vaccine( stud *st )
{
	int p = st -> zone_no;
	printf("Student %d on Vaccination Zone %d has been vaccinated which has success probability %1.2f\n", st -> student_num , p , zones[p].curr_efficiency);
	
	int ss = rand() % 101;
	int prob = zones[p].curr_efficiency * 100;

	if ( ss < prob )
	{
		printf("Student %d has tested 'positive' for antibodies\n", st -> student_num );
		st -> state = 1;
		st -> round_no = 5;
		
	}
	else
	{
		printf("Student %d has tested 'negative' for antibodies\n", st -> student_num );
		if ( st -> round_no > 3 )
		{
			st -> state = 1;
			st -> round_no  = 5;
		}
		else
		{
			st -> round_no++;
			st -> state = 0;
			pthread_mutex_lock(&zones[i].lockbatch);
			zones[p].vaccined--;
			zones[p].capacity--;
			zones[p].slots--;
			pthread_mutex_unlock(&zones[i].lockbatch);
			student_wait(st);
		}
	}
	pthread_mutex_lock(&zones[i].lockbatch);
		zones[p].vaccined--;
		zones[p].capacity--;
		zones[p].slots--;
		pthread_mutex_unlock(&zones[i].lockbatch);
}

int main(void)
{
	pnf("Enter the number of Pharma Companies: ");
	scanf("%d" , &n );
	pnf("Enter the number of Vaccination Zones :");
	scanf("%d" , &m );
	pnf("Enter the number of students :");
	scanf("%d" , &s );

	pharmacompanies = (Pharmacompany *)malloc((n+1) * sizeof(Pharmacompany));
	zones = (VZone *)malloc((m+1) * sizeof(VZone));
	students = (stud *)malloc((s+1) * sizeof(stud));

	student_rem = s;
	yet_to_be = s;
	float pp;
	for(int i = 1 ; i <= n ; i++)
	{
		printf("Enter the probability : ");
		scanf("%f", &pp);
		pharmacompanies[i].efficiency = pp;
		pharmacompanies[i].company_no = i;
		pharmacompanies[i].state = 0;
	}
	for(int i = 1 ; i <= m ; i++)
	{
		zones[i].Vzone_no = i;
		zones[i].state = 0;
		zones[i].slots = 0;
		zones[i].capacity = 0;
		zones[i].vaccined = 0;
		zones[i].wait = 1;
		pthread_mutex_init(&zones[i].lockbatch , NULL);
	}
	for(int i = 1 ; i <= s ; i++)
	{
		students[i].student_num = i;
		students[i].state = 0;
	}

	pnf("Simulation started\n");

	for(int i = 1 ; i <=n ; i++)
		pthread_create(&pharmacompanies[i].thread_ID, NULL, (void *)preparing_vaccines , &pharmacompanies[i]);

	for(int i = 1 ; i <=m ; i++)
		pthread_create(&zones[i].thread_ID, NULL, (void *)zone_wait , &zones[i]);
	
	for(int i = 1 ; i <= s ; i++)
		students[i].round_no = 1;

	pthread_mutex_init(&student_gonee , NULL);
	pthread_mutex_init(&debug_lock , NULL);

	pthread_t student_cam;
	pthread_create(&student_cam , NULL , (void *)student_coming , &students_came );
	while(s > students_came)
		;
	for(int i = 1 ; i <= s ; i++)
		pthread_join(students[i].thread_ID , 0);

	pnf("Simulation Over!!!\n");
	free(zones);
	free(students);
	free(pharmacompanies);
	return 0;
}