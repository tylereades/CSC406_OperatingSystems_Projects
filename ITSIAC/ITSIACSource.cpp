//************************************  PROGRAM IDENTIFICATION  *************************************** 
//*                                                                                                   * 
//*   PROGRAM FILE NAME:  ITSIACSource.cpp      ASSIGNMENT #:  1            Grade: _________          * 
//*                                                                                                   *
//*   PROGRAM AUTHOR:                   ______________                                                *
//*                                      David Clapp                                                  * 
//*                                                                                                   * 
//*   COURSE #:  CSC 40600 21                              DUE DATE:  March 20, 2018                  * 
//*                                                                                                   * 
//*****************************************************************************************************

//***********************************  PROGRAM DESCRIPTION  ******************************************* 
//*                                                                                                   *
//*   PROCESS: This program is designed to simulate the operations of a theoretical machine           *
//*            called ITSIAC. The program is executed by interpreting 16-bit binary numbers           *
//*            and performing actions within the program's "internal memory" based on these           *
//*            binary numbers.                                                                        *
//*                                                                                                   *
//*   USER DEFINED                                                                                    *
//*    MODULES     : initializeControlStorage - Read the data from the micro code input file          *
//*                            and place it into the controlStorage array.                            *
//*                  initializePrimaryMemory - Initialize every index of primaryMemory to -1.         *
//*                  binToDec - Convert the binary bitset number into a decimal integer.              *
//*                  readData - Read the data from the input file, echo print it to the               *
//*                            output file, and place it into primaryMemory.                          *
//*                  printData - Prints the contents of primaryMemory to the output file.             *
//*                  runMicroCode - Run the machine-level and micro-level code as specified           *
//*                            in controlStorage.                                                     *
//*                                                                                                   *
//*****************************************************************************************************


#include<iomanip>
#include<iostream>
#include<string>
#include<fstream>
#include<bitset>
using namespace std;

void initializeControlStorage(ifstream&);
void initializePrimaryMemory();
int binToDec(bitset<16>);
void readData(ifstream&, ofstream&);
void printData(ofstream&);
void runMicroCode(int[2][256], ofstream&);
void Header(ofstream&);
void Footer(ofstream&);

	// Registers.
int ACC1 = 0;
int ACC2 = 0;
int SDR1 = 0;
int SDR2 = 0;
int TMPR1 = 0;
int TMPR2 = 0;
int PSIAR = 0;
int SAR = 0;
int CSIAR = 0;
int MIR = 0;

	// Counters.
int microCounter = 0;
int machineCounter = 0;

	// primaryMemory and controlStorage arrays.
int primaryMemory[2][256];
int controlStorage[100];

//*****************************************************************************************************
int main()
{
		// Open the microcode text file.
	ifstream inFile;
	inFile.open("ITSIACmicro.txt", ios::in);

		// Open the output file.
	ofstream outFile;
	outFile.open("ITSIACoutput.txt", ios::out);

		// Initialize both controlStorage and primaryMemory.
	initializeControlStorage(inFile);
	initializePrimaryMemory();

		// Close the microcode text file and open the data text file.
	inFile.close();
	inFile.open("ITSIACdata.txt", ios::in);

		// Print the header to the output file.
	Header(outFile);

		// Echo print the orignial data in binary and load it into primaryMemory.
	outFile << "Echo Print of Original Data" << endl;
	outFile << "___________________________" << endl;
	readData(inFile, outFile);
	outFile << endl;

		// Print out the contents of primaryMemory in hexidecimal before any
		//          micro code instructions have been run.
	outFile << "Primary Memory Contents" << endl;
	outFile << "_______________________" << endl;
	printData(outFile);

		// Print the contents of the registers to the output file.
	outFile << "Accumulator: " << hex << uppercase << setw(2) << setfill('0') << ACC1;
	outFile << hex << uppercase << setw(2) << setfill('0') << ACC2 << endl;
	outFile << "PSIAR: " << hex << uppercase << setw(4) << setfill('0') << PSIAR << endl;
	outFile << "SAR: " << hex << uppercase << setw(4) << setfill('0') << SAR << endl;
	outFile << "SDR: " << hex << uppercase << setw(2) << setfill('0') << SDR1;
	outFile << hex << uppercase << setw(2) << setfill('0') << SDR2 << endl;
	outFile << "TMPR: " << hex << uppercase << setw(2) << setfill('0') << TMPR1;
	outFile << hex << uppercase << setw(2) << setfill('0') << TMPR2 << endl;
	outFile << "CSIAR: " << hex << uppercase << setw(4) << setfill('0') << CSIAR << endl;
	outFile << "MIR: " << hex << uppercase << setw(4) << setfill('0') << MIR << endl;
	outFile << endl;

		// Run the microcode.
	runMicroCode(primaryMemory, outFile);

		// Print out the contents of primaryMemory in hexidecimal after the
		//          micro code instructions have been completed.
	outFile << "Primary Memory Contents" << endl;
	outFile << "_______________________" << endl;
	printData(outFile);

		// Print the contents of the registers and counters to the output file.
	outFile << "Accumulator: " << hex << uppercase << setw(2) << setfill('0') << ACC1;
	outFile << hex << uppercase << setw(2) << setfill('0') << ACC2 << endl;
	outFile << "PSIAR: " << hex << uppercase << setw(4) << setfill('0') << PSIAR << endl;
	outFile << "SAR: " << hex << uppercase << setw(4) << setfill('0') << SAR << endl;
	outFile << "SDR: " << hex << uppercase << setw(2) << setfill('0') << SDR1;
	outFile << hex << uppercase << setw(2) << setfill('0') << SDR2 << endl;
	outFile << "TMPR: " << hex << uppercase << setw(2) << setfill('0') << TMPR1;
	outFile << hex << uppercase << setw(2) << setfill('0') << TMPR2 << endl;
	outFile << "CSIAR: " << hex << uppercase << setw(4) << setfill('0') << CSIAR << endl;
	outFile << "MIR: " << hex << uppercase << setw(4) << setfill('0') << MIR << endl;
	outFile << "The total number of Machine Level Instructions: " << dec << machineCounter << endl;
	outFile << "The total number of Micro Instructions: " << dec << microCounter << endl;

		// Print the footer to the output file.
	Footer(outFile);
	
	return 0;
}
//*****************************************************************************************************



//*****************************************************************************************************
void initializeControlStorage(ifstream& inFile)
{
				// Receives - The micro code input file.
				// Task - Read the data from the micro code input file and place it into
				//          the controlStorage array.
				// Returns - controlStorage array with the read data in it.

	int temp;

	for (int i = 0; i < 100; i++)
	{
			// Place each line of the micro code file into the control
			//          storage array.
		inFile >> temp;
		controlStorage[i] = temp;
	}
}
//*****************************************************************************************************


//*****************************************************************************************************
void initializePrimaryMemory()
{
				// Receives - Nothing.
				// Task - Initialize every index of primaryMemory to -1.
				// Returns - The initialized primaryMemory array.

		// Initialize each element of primaryMemory to -1.
	for (int i = 0; i < 256; i++)
	{
		primaryMemory[0][i] = -1;
		primaryMemory[1][i] = -1;
	}
}
//*****************************************************************************************************



//*****************************************************************************************************
int binToDec(bitset<16> code)
{
				// Receives - A bitset number.
				// Task - Convert the binary bitset number into a decimal integer.
				// Returns - An integer in decimal notation.
	
	int decimal = 0;
	
		// Initially set the negative flag to false.
	bool negative = false;
		
		// Bitset's are stored in the opposite direction from a normal array.
		// To check if negative, which would usually be the first, check the last number.
		// If the number is negative...
	if (code[15] == 1)
	{
			// ...Set the negative flag to true.
		negative = true;
	}
	
		// Calculate the decimal value of the binary number.
	for (int i = 15; i >= 0; i--)
	{
			// If the entire number was neagive, flip each bit.
		if (negative)
			code[i].flip();

			// Add the current bit, multiplied by its base, to the decimal number.
		decimal += (int)(pow(2, i)*code[i]);
	}

		// If the number is negative...
	if (negative)
	{
			// ...Add one and flip the sign.
		decimal += 1;
		decimal *= -1;
	}

	return decimal;
}
//*****************************************************************************************************



//*****************************************************************************************************
void readData(ifstream& inFile, ofstream& outFile)
{
				// Receives - The data input file, and the output file.
				// Task - Read the data from the input file, echo print it to the
				//          output file, and place it into primaryMemory.
				// Returns - primaryMemory array with the read data in it.
	
	char binOpcode[9], binOperand[9];
	int	i = 0, decOpcode, decOperand, temp;
	

	for(;;)
	{
			// Read in the first part of the data file.
		char *binOpcodePtr = binOpcode, *binOperandPtr = binOperand;
		inFile.get(binOpcodePtr, 9);
		inFile.get(binOperandPtr, 9);
		inFile >> ws;

		string input;
		int temp;
		
			// Combine the two character arrays into a string.
		input += binOpcode;
		input += binOperand;
		
			// If the break in the data file is found...
		if (input == "XXXXXXXXXXXXXXXX")
		{
				// ...Read in the second part of the data file.
			for (int j = 0; j < 50; j++)
			{
				getline(inFile, input);
					// Echo print the second part of the data file.
				outFile << input << endl;

					// Place the number into primary memory.
				bitset<16>code(input);
				temp = binToDec(code);
				primaryMemory[1][j + 50] = temp;
			}
				// Stop the read once the second part has been successfully placed
				//          in primaryMemory.
			return;
		}
		
			// Echo print the first part of the data file.
		outFile << input << endl;

			// Place the opcode number into primaryMemory.
		bitset<16>code1(binOpcode);
		temp = binToDec(code1);
		primaryMemory[0][i] = temp;

			// Place the operand number into primaryMemory.
		bitset<16>code2(binOperand);
		temp = binToDec(code2);
		primaryMemory[1][i] = temp;

		i++;
	}
}
//*****************************************************************************************************



//*****************************************************************************************************
void printData(ofstream& outFile)
{
				// Receives - The output file.
				// Task - Prints the contents of primaryMemory to the output file.
				// Returns - Nothing.

	int numCol = 8;
	int curCol = 0;
	int i = 0;

	while (i < 256)
	{
			// While curCol < 8, print as a row.
		while (curCol < numCol)
		{
			
			if (i < 21)
			{
					// Print the contents of the first part of primaryMemory to the
					//          output file in hexidecimal form.
				outFile << hex << uppercase << setw(2) << setfill('0') << primaryMemory[0][i];
				outFile << hex << uppercase << setw(2) << setfill('0') << primaryMemory[1][i] << " ";
			}
			else
			{
					// Convert the integer into a 16-bit bitset. Trying to just print out the integer lead
					//          to excess FFFF to be printed every time.
				int temp = primaryMemory[1][i];
				bitset<16>bit(temp);

					// Print the contents of the second part of primaryMemory to the
					//          output file in hexidecimal form.
				outFile << hex << uppercase << setw(4) << setfill('0') << bit.to_ulong() << " ";
			}
			

				// Increment the current column.
			curCol++;
				// Increment the current position in primaryMemory.
			i++;
		}
		
			// Reset the current column when curCol = 8.
		curCol = 0;
		outFile << endl;
	}
}
//*****************************************************************************************************



//*****************************************************************************************************
void runMicroCode(int primaryMemory[2][256], ofstream& outFile)
{
				// Receives - The two-dimensional integer array representing
				//          primaryMemory, and the output file.
				// Task - Run the machine-level and micro-level code as specified
				//          in controlStorage.
				// Returns - primaryMemory with its contents altered by the
				//          micro code.

	for (;;)
	{
		MIR = controlStorage[CSIAR];
		CSIAR++;

		switch (MIR)
		{
				// Halt.
			case 0:
			{
				return;
			}
				// Move ACC into SDR.
			case 1:
			{
				SDR1 = ACC1;
				SDR2 = ACC2;
				break;
			}
				// Move PSIAR into SDR.
			case 2:
			{
				SDR2 = PSIAR;
				break;
			}
				// Move TMPR into SDR.
			case 3:
			{
				SDR1 = TMPR1;
				SDR2 = TMPR2;
				break;
			}
				// Move SDR into ACC.
			case 4:
			{
				ACC1 = SDR1;
				ACC2 = SDR2;
				break;
			}
				// Move SDR into PSIAR.
			case 5:
			{
				PSIAR = SDR2;
				break;
			}
				// Move SDR into TMPR.
			case 6:
			{
				TMPR1 = SDR1;
				TMPR2 = SDR2;
				break;
			}
				// Move ACC into SAR.
			case 7:
			{
				SAR = ACC2;
				break;
			}
				// Move PSIAR into SAR.
			case 8:
			{
				SAR = PSIAR;
				break;
			}
				// Move TMPR into SAR.
			case 9:
			{
				SAR = TMPR2;
				break;
			}
				// Add ACC to ACC. Store in ACC.
			case 10:
			{
				ACC1 = ACC1 + ACC1;
				ACC2 = ACC2 + ACC2;
				break;
			}
				// Add PSIAR to ACC. Store in ACC.
			case 11:
			{
				ACC2 = ACC2 + PSIAR;
				ACC1 = 0;
				break;
			}
				// Add TMPR to ACC. Store in ACC.
			case 12:
			{
				ACC1 = ACC1 + TMPR1;
				ACC2 = ACC2 + TMPR2;
				break;
			}
				// Subtract ACC from ACC. Store in ACC.
			case 13:
			{
				ACC1 = ACC1 - ACC1;
				ACC2 = ACC2 - ACC2;
				break;
			}
				// Subtract PSIAR from ACC. Store in ACC.
			case 14:
			{
				ACC2 = ACC2 - PSIAR;
				ACC1 = 0;
				break;
			}
				// Subtrac TMPR from ACC. Store in ACC.
			case 15:
			{
				ACC1 = ACC1 - TMPR1;
				ACC2 = ACC2 - TMPR2;
				break;
			}
				// Move ACC into ACC.
			case 16:
			{
				ACC1 = ACC1;
				ACC2 = ACC2;
				break;
			}
				// Move PSIAR into ACC.
			case 17:
			{
				ACC2 = PSIAR;
				ACC1 = 0;
				break;
			}
				// Move TMPR into ACC.
			case 18:
			{
				ACC1 = TMPR1;
				ACC2 = TMPR2;
				break;
			}
				// Move ACC into ACC.
			case 19:
			{
				ACC1 = ACC1;
				ACC2 = ACC2;
				break;
			}
				// Move ACC into PSIAR.
			case 20:
			{
				PSIAR = ACC2;
				break;
			}
				// Move ACC to TMPR.
			case 21:
			{
				TMPR1 = ACC1;
				TMPR2 = ACC2;
				break;
			}
				// Add one to the ACC. Store in ACC.
			case 22:
			{
				ACC2 = ACC2 + 1;
				break;
			}
				// Add one to the PSIAR. Store in ACC.
			case 23:
			{
				ACC2 = PSIAR + 1;
				ACC1 = 0;
				break;
			}
				// Add one to TMPR. Store in ACC.
			case 24:
			{
				ACC1 = TMPR1;
				ACC2 = TMPR2 + 1;
				break;
			}
				// Read from primaryMemory at location SAR. Store in SDR.
			case 25:
			{
				SDR1 = primaryMemory[0][SAR];
				SDR2 = primaryMemory[1][SAR];
				break;
			}
				// Write to primaryMemory at location SAR.
			case 26:
			{
				primaryMemory[0][SAR] = SDR1;
				primaryMemory[1][SAR] = SDR2;
				break;
			}
				// Add one to the CSIAR. Store in CSIAR.
			case 27:
			{
				CSIAR = CSIAR + 1;
				break;
			}
				// Decode SDR. Store in CSIAR.
			case 28:
			{
				CSIAR = SDR1 * 10;
				machineCounter++;
				break;
			}
				// Move 0 into the CSIAR.
			case 29:
			{
				CSIAR = 0;
				break;
			}
				// SKIP. If ACC = 0, add one to CSIAR. Store in CSIAR.
			case 30:
			{
				if ((ACC1 == 0) && (ACC2 == 0))
				{
					CSIAR = CSIAR + 1;
				}
				break;
			}
				// Print dump primaryMemory to output file.
			case 31:
			{
				printData(outFile);
				break;
			}
				// Move CSIAR into MIR.
			case 32:
			{
				MIR = CSIAR;
				break;
			}
		}
		microCounter += 3;
	}
}
//*****************************************************************************************************



//*************************************  FUNCTION HEADER  *********************************************
void Header(ofstream& Outfile)
{
				// Receives - the output file.
				// Task - Prints the output preamble.
				// Returns - Nothing.
	Outfile << setw(30) << "David Clapp";
	Outfile << setw(17) << "CSC 40600";
	Outfile << setw(15) << "Section 21" << endl;
	Outfile << setw(30) << "Spring 2018";
	Outfile << setw(20) << "Assignment #1" << endl;
	Outfile << setw(35) << "-----------------------------------";
	Outfile << setw(35) << "-----------------------------------" << endl << endl;
	return;
}
//************************************* END OF FUNCTION HEADER  ***************************************



//*************************************  FUNCTION FOOTER  *********************************************
void Footer(ofstream& Outfile)
{
				// Receives - the output file.
				// Task - Prints the output salutation.
				// Returns - Nothing.
	Outfile << endl;
	Outfile << setw(35) << " --------------------------------- " << endl;
	Outfile << setw(35) << "|      END OF PROGRAM OUTPUT      |" << endl;
	Outfile << setw(35) << " --------------------------------- " << endl;
	return;
}
//************************************* END OF FUNCTION FOOTER  ***************************************