#define VARIABLES_H
using namespace std;

struct jobType {
	int number = -999, length, interArrival, IOBurstLength, CPUBurst[25], IOburstCount;
	int arrivalTime; //Holds the time at which the job entered the system
	int exitTime; //Holds the time at which the job exited the system
	int ltqWait, stqWait, ioWait, cpuWait; //Wait counters for each queue
	int responceTimer = 0;  // a timer to count per process until the cpu has the job
};

ifstream dataIN("SIM_DATA.txt");
ofstream dataOUT5("output.txt"); //Create and open the file to write data to.
int jobcount = 0;
const int MAXJOBNUMBER = 150; //Set the maximum number of jobs to 150
jobType jList[MAXJOBNUMBER]; //Declare and initialize the list of jobs
jobType statList[MAXJOBNUMBER]; //Declare and initialize the list of jobs

int ioQueue[30]; // Declare the I/O queue.
int longTermQueue[60]; // Declare the long term queue.
int shortTermQueue[30]; // Delcare the short term queue.
int numEleIOQ = 0; // Integer to indicate how many jobs are currently in the I/O queue.
int numEleLTQ = 0; // Integer to indicate how many jobs are currently in the long term queue.
int numEleSTQ = 0; // Integer to indicate how many jobs are currently in the short term queue.

int programClock = 0; // The simulator clock to keep track of the total time of the simulation run.
int io_timer = 0; // Indicates the current I/O burst.
int job_timer = 0; // Keeps track of the time between job arrivals.
int more_jobs = 0; // Keeps track of the # of jobs in the system.
int process_timer = 0; // Keeps track of the current CPU burst.
int suspend_timer = 0; // Keeps track of the current interrupt time.
bool cpu_ready_flag = true; // Signals that the CPU is available.
bool cpu_complete_flag = false; // Signals the completion of a CPU burst.
bool finished_flag = false; // Signals that a JOB is finished.
bool interrupt_flag = false; // Signals that an interrupt is in progress.
bool io_complete_flag = false; // Signals the completion of an I/O burst.
bool io_device_flag = true; // Signals that the I/O device is available.
bool job_flag = false; // Signals that a job has arrived.
bool stop_flag = false; // Signals to stop CPU job processing.
bool suspend_flag = false; // Signals context switch to handle interrupt.

int cpu = 0; // Integer to indicate which job has the CPU.
int count = 0; // Keeps track of the number of processes ran.
int device = 0; // Integer to signal which process is in the I/O device.
int process = -1; // Integer to indicate the process entering the CPU.
int ioprocess = -1; // Integer to indicate the process entering the I/O device.
int temp = 0; // Integer to indicate which process is suspended.
bool ioq_empty = true; // Signals that the I/O queue is empty.
bool ioq_full = false; // Signals that the I/O queue is full.
bool ltq_empty = true; // Signals that the long term queue is empty.
bool ltq_full = false; // Signals that the long term queue is full.
bool stq_empty = true; // Signals that the short term queue is empty.
bool stq_full = false; // Signals that the short term queue is full.
int procStarted = 0; // Integer to indicate the job in the jobType array that will
                     //          to the long term queue next.