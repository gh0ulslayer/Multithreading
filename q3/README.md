INPUT : k (instrumentalist) , a , e , c  t1, t2 , t , followed by k lines having info of each instrumentalist

Structs used are "coordinator" , "performer" , "ele" , "acou"

Each coordinator array comprises of a mutex lock which looks after the availability of coordinator , that wether he/she's available or not , along with "available" named variable which does the same work as it's name states , it check that wether a counter is available or not

Each performer's name is denoted by "* name" , performer's instrument id is denoted by "ic" , "arrivaltime" variable records the same thing as its name states , it stores the arrival time of the performer in the event

Each ele  comprises of name , and at the start of the thread execution a prev char is initialised to '0'  , along with name and prev char , ele struct also includes a semaphore , elec array = no of electric stages  

Each acou comprises of name , and at the start of the thread execution a prev char is initialised to '0'  , along with name and prev char , ele struct also includes a semaphore , acous array = no of acoustic stages


In the performersfun function the performers after sleeping for a certain amount of time have arrived and will be ready to perform at any one of the stage after this we have 2 cases

If Performer is a singer .In such a situation we check the preference of the singer whether singer wants to pair up with a musician or he/she wants a solo performance . we are setting this priority using again a randomiser value 1 being a pair-up and 2 being a solo. Using this choice we search for stages and we send the singer to perform if a stage is free or if it already has a musician . After his performance the thread exits or If Performer is a musician .In such situation, we are checking which musician is allowed to perform in what type of stage i.e. electrical or acoustic. After this initial test we set the performer in a loop which checks if there is availability of that particular stage or not. For musicians and guitarists as they have the ability to perform on both types they are required to chose which stage they want this is done by a random binary generator(1 for acoustic and 2 for electric).Now according to their priority a random stage is searched. After performance they collect tshirt in the collectTshirt function. After collecting the t-shirt the thread exits and next performer will perform as a signal is then sent to the semaphore of the stage in which the artist performed. 
 

In a case where performers in their respective threads don't get a stage, the loop is repeated again and the above conditions are checked.
If the waiting time of the performer exceeds t the performer exits the infinite loop and we print a statement saying 'performer left because of impatience'.
After the execution of all threads at the end we join all the threads and the process gets finished followed by a message saying 'Event Over!!!'.

Event Over!!!