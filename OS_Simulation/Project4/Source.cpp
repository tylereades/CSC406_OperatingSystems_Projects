

#include <fstream>
#include <iostream>
#include <iomanip>
#include <ostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "Variables.h"
#include "Queues.h"

using namespace std;


void getData() {
	
	int i = 0, newJob = 0, burstLength = 0;
	dataIN >> ws >> newJob;
	while (newJob > 0) { //Read in jobs until the last negative value is reached
		jList[i].number = newJob;
		dataIN >> jList[i].length;
		dataIN >> jList[i].interArrival;
		dataIN >> jList[i].IOBurstLength;
		dataIN >> burstLength; 
		int j = 0;
		while (burstLength > 0) {
			jList[i].CPUBurst[j] = burstLength;
			dataIN >> burstLength; 
			j++; //Increment the burst count
		}
		jList[i].IOburstCount = j; // Set the number of bursts to the number of loops

		i++; // Increment the job counter
		if (burstLength < 0) { //Burst length < 0 is sentinel
							   //Copy the job list into a list from which values will not be deleted
			for (int z = 0; z < i; z++) {
				statList[z] = jList[z];
			}
			jobcount = i;
			return; //Exit the function if a burst length of -1 is reached
		}
		dataIN >> ws >> newJob;
	}
}

	// Print the jList structure array for testing.
void printData()
{
	for (int i = 0; i < jobcount; i++)
	{
		cout << jList[i].number << " " << jList[i].length << " " << jList[i].interArrival;
		cout << " " << jList[i].IOBurstLength;
		for (int j = 0; j < jList[i].IOburstCount; j++)
		{
			cout << " " << jList[i].CPUBurst[j];
		}
		cout << endl;
	}
}


int main()
{
	getData();
	//printData(); //Test for printing the structure array.


	for (int i = 0; i < 30; i++)
	{
		addToQueue(3, jList[i].number);
	}

	cout << "Short Term Queue:" << endl;
	for (int i = 0; i < 30; i++)
	{
		cout << shortTermQueue[i] << " ";
		if ((i + 1) % 10 == 0)
		{
			cout << endl;
		}
	}
	/*
	for (int i = 0; i < 30; i++)
	{
		int transfer = removeFromQueue(3);
		addToQueue(1, transfer);
	}

	cout << "Short Term Queue:" << endl;
	for (int i = 0; i < 30; i++)
	{
		cout << shortTermQueue[i] << " ";
		if ((i + 1) % 10 == 0)
		{
			cout << endl;
		}
	}

	cout << "I/O Queue:" << endl;
	for (int i = 0; i < 30; i++)
	{
		cout << ioQueue[i] << " ";
		if ((i + 1) % 10 == 0)
		{
			cout << endl;
		}
	}
	*/
	cout << "Short Term Queue Wait Times:" << endl;
	for (int i = 0; i < 30; i++)
	{
		cout << jList[i].stqWait << " ";
		if ((i + 1) % 10 == 0)
		{
			cout << endl;
		}
	}
	
	stq_empty = false;
	for (int i = 0; i < 50; i++)
	{
		manageSTQ();
	}

	cout << "Short Term Queue Wait Times:" << endl;
	for (int i = 0; i < 30; i++)
	{
		cout << jList[i].stqWait << " ";
		if ((i + 1) % 10 == 0)
		{
			cout << endl;
		}
	}
	cin.ignore();
	return 0;
}
