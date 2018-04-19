#define STQ_H
using namespace std;

void manageIOQ();
void manageLTQ();
void manageSTQ();
void addToQueue(int, int);
int removeFromQueue(int);



//*****************************************************************************************************
void manageIOQ()
{
		// File Name: Queues.h
		// Primary Author: David Clapp
		// Date Last Revised: 4/18/2017
		// Description: Manage the I/O queue.

		// If the ioqueue isn't empty...
	if (!ioq_empty)
	{
			// ...increment wait counters for all processes in the queue.
		for (int i = 0; i < jobcount; i++)
		{
			for (int j = 0; j < numEleIOQ; j++)
			{
				if (ioQueue[j] == jList[i].number)
				{
					jList[i].ioWait++;
				}
			}
		}
	}
		// If the cpu_complete_flag is true...
	if (cpu_complete_flag)
	{
			// ...and the ioqueue isn't full...
		if (!ioq_full)
		{
				// ...add the process that is in the cpu into the ioqueue.
			addToQueue(1, cpu);

				// Set the cpu to 0.
			cpu = 0;

				// set ioq_empty to false.
			ioq_empty = false;

				// Set the cpu_ready_flag to true.
			cpu_ready_flag = true;
			
				// If the ioqueue is full...
			if (numEleIOQ == 29)
			{
					// ...set the ioq_full flag to true.
				ioq_full = true;
			}

				// Set the cpu_complete_flag to false.
			cpu_complete_flag = false;
		}
	}
}
//*****************************************************************************************************



//*****************************************************************************************************
void manageLTQ()
{
		// File Name: Queues.h
		// Primary Author: David Clapp
		// Date Last Revised: 4/18/2017
		// Description: Manage the long term queue.

		// If the LTQ isn't empty...
	if (!ltq_empty)
	{
			// ...increment wait counters for all processes in the queue.
		for (int i = 0; i < jobcount; i++)
		{
			for (int j = 0; j < numEleLTQ; j++)
			{
				if (longTermQueue[j] == jList[i].number)
				{
					jList[i].ltqWait++;
				}
			}
		}
	}

		// If a job has arrived and the LTQ isn't full...
	if ((job_flag) && (!ltq_full))
	{
			// ...put the incoming job in the queue.
		addToQueue(2, jList[procStarted].number);

			// Increment the number of processes started.
		procStarted++;

			// Set job_flag to false.
		job_flag = false;

			// Set ltq_empty to false.
		ltq_empty = false;
	}

		// If the LTQ is full...
	if (numEleLTQ == 59)
	{
			// ...set ltq_full to true.
		ltq_full = true;
	}
}
//*****************************************************************************************************



//*****************************************************************************************************
void manageSTQ()
{
		// File Name: Queues.h
		// Primary Author: David Clapp
		// Date Last Revised: 4/18/2017
		// Description: Manage the short term queue.

		// If the STQ isn't empty...
	if (!stq_empty)
	{
			// ...increment wait counters for all processes in the queue.
		for (int i = 0; i < jobcount; i++)
		{
			for (int j = 0; j < numEleSTQ; j++)
			{
				if (shortTermQueue[j] == jList[i].number)
				{
					jList[i].stqWait++;
				}
			}
		}
	}

		// If the io_complete_flag is true...
	if (io_complete_flag)
	{
			// ...set the io_complete_flag to false.
		io_complete_flag = false;

			// Set the io_device_flag to true.
		io_device_flag = true;

			// If the finished_flag is true...
		if (finished_flag)
		{
				// ...decrement more_jobs.
			more_jobs--;

				// Set finished_flag to false.
			finished_flag = false;

				// Collect the data.
			// Still need to figure this one out.****************************************************************************************************
		}
			// If the finished_flag is false...
		else
		{
				// ...and stq_full is false...
			if (!stq_full)
			{
					// ...add the process that is in the I/O device to the short term queue.
				addToQueue(3, device);

					// Set device back to 0.
				device = 0;

					// If the short term queue is now full...
				if (numEleSTQ == 29)
				{
						// ...set the stq_full flag to true.
					stq_full = true;
				}
			}
		}
	}

		// If the short term queue is not full and the long term queue is not empty...
	if ((!stq_full) && (!ltq_empty))
	{
			// ...add a process from the long term queue to the short term queue.
		int transfer = removeFromQueue(2);
		addToQueue(3, transfer);

			// Set stq_empty to false.
		stq_empty = false;
		
			// If the long term queue is now empty...
		if (numEleLTQ == 0)
		{
				// ...Set the ltq_empty flag to true.
			ltq_empty = true;

				// Set the ltq_full flag to false.
			ltq_full - false;
		}

			// If the short term queue is now full...
		if (numEleSTQ == 29)
		{
				// ...set the stq_full flag to true.
			stq_full = true;
		}
	}
}
//*****************************************************************************************************



//*****************************************************************************************************
void addToQueue(int queue, int value)
{
		// File Name: Queues.h
		// Primary Author: David Clapp
		// Date Last Revised: 4/18/2017
		// Description: Adds an element, the value integer passed in, to the end of
		//          a queue, determined by the queue integer passed in.

	switch (queue)
	{
			// Insert into the I/O queue.
		case 1:
		{
			ioQueue[numEleIOQ] = value;
			numEleIOQ++;
			break;
		}
			// Insert into the long term queue.
		case 2:
		{
			longTermQueue[numEleLTQ] = value;
			numEleLTQ++;
			break;
		}
			// Insert into the short term queue.
		case 3:
		{
			shortTermQueue[numEleSTQ] = value;
			numEleSTQ++;
			break;
		}
	}
}
//*****************************************************************************************************



//*****************************************************************************************************
int removeFromQueue(int queue)
{
		// File Name: Queues.h
		// Primary Author: David Clapp
		// Date Last Revised: 4/18/2017
		// Description: Removes the first element in a queue, determined by the integer
		//          that is passed in, and returns the value of the removed element.

	int removedValue;

	switch (queue)
	{
			// Remove from the I/O queue.
		case 1:
		{
				// Set the removed value to the first element in the I/O queue.
			removedValue = ioQueue[0];
			
				// Resort the I/O queue.
			for (int i = 0; i < numEleIOQ; i++)
			{
				ioQueue[i] = ioQueue[i + 1];
			}
				// Set the last element to -1.
			ioQueue[numEleIOQ-1] = -1;

				// Decrement the number of elements in the I/O queue.
			numEleIOQ--;
			break;
		}
			// Remove from the long term queue.
		case 2:
		{
				// Set the removed value to the first elemnet in the long term queue.
			removedValue = longTermQueue[0];

				// Resort the long term queue.
			for (int i = 0; i < numEleLTQ; i++)
			{
				longTermQueue[i] = longTermQueue[i + 1];
			}

				// Set the last element to -1.
			longTermQueue[numEleLTQ-1] = -1;

				// Decrement the number of elements in the long term queue.
			numEleLTQ--;
			break;
		}
			// Remove from the short term queue.
		case 3:
		{
				// Set the removed value to the first elements in the short term queue.
			removedValue = shortTermQueue[0];

				// Resort the short term queue.
			for (int i = 0; i < numEleSTQ; i++)
			{
				shortTermQueue[i] = shortTermQueue[i + 1];
			}

				// Set the last element to -1.
			shortTermQueue[numEleSTQ-1] = -1;

				// Decrement the number of elements in the short term queue.
			numEleSTQ--;
			break;
		}
	}

		// Return the removed value.
	return removedValue;
}
//*****************************************************************************************************