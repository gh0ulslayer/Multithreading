INPUT : n (no of pharma companies) , m (no of vaccination zones) , s (no of students) , followed by the n success probabilities of pharma companies.

All the pharma companies , zones , students are considered as threads and are intitialised in main().

Structs used are "Pharmacompany" , "Student" , "VZone" which all contains thread_ID mutexes along with initialisation of some variables which are either inputs or defined later in the different functions.

Each pharmacompany's id is denoted by "company_no" variable , and it's state , that wether it is making vaccines or not is denoted by the variable named "state", the number of batches produced by a pharmacompany is denoted by "no_of_batches" , and the capacity to vaccinate the students is given by "stud_vaccined", success probability is denoted by "efficiency"

Each vaccination zone's id is denoted by "Vzone_no" , and it's state , that wether is is waiting for vaccines to arrive/started vaccinating is denoted by the variable named "state" , number of students vaccinated by a zone at any time is given  by the variable "vaccined" ,  nuumber of vaccine's on a zone is denoted by "capacity" , the number of students which can be vaccinated at a time is given by "slots" , success probability of the vaccine batch arrived from the company is given by "cur_efficiency" , mutex "lockbatch" is defined to lock the program in certain places such as while updating number of students vaccine and slots remaining

Each student's id is denoted by "student_num" , and his/her state ,  that wether he/she is waiting for vaccination/assigned to a vaccination zone is denoted by the variable named "state" ,  "zone_no" denotes the id of the zone on which student is assigned a slot for vaccination , "round_no" denoted his/her round of vaccination.



Vaccine batches are prepared in 2-5 sec ,to create a single batch by a individual pharmacompany using the function "preparing_vaccines" , vaccines are not created if students remaining for vaccination are 0 , after preparing vaccine batches it calls a function for delivering batches named "vaccine_ready" , in this function batches from pharma companies are delivered to those zones which don't have any batch. Slots are made ready for vaccinations in the function "zone_wait" , one slots are ready , a zone is ready to vaccinate , and if capacity of that zone is 0 then it zone's state changes to 0 which means that it's waiting for a new batch of vaccines.Now students send signal that wether they're available for vaccination or not , and once they give signal that they're ready for vaccination , "student_wait" is called in which his credibilities are checked that wether it's his first round of vaccination or not and if he's not vaccinated properly/even once , he's sent for vaccination , the function "studdd_vaccine" is called for his antibody check , in that function , if he tested positive in antibody test which means vaccine worked on him ,  and then "student_rem" is decreased under a mutex lock "student_gone" , and if he's tested negative it means that vaccine didn't worked on him ,and he's again send to another round of vaccination depending upon how many times he's already vaccinated , if the count is greater than 3 , then he's not sent to another round of vaccination.

Simulation over!!

