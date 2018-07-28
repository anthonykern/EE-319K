// ****************** Lab1.c ***************
// Program written by: put your names here
// Date Created: 1/18/2017 
// Last Modified: 1/18/2017 
// Brief description of the Lab
// An embedded system is capturing temperature data from a
// sensor and performing analysis on the captured data.
// The controller part of the system is periodically capturing N
// readings of the temperature sensor. Your task is to write three
// analysis routines to help the controller perform its function
//   The three analysis subroutines are:
//    1. Calculate the mean of the temperature readings 
//       rounded down to the nearest integer
//    2. Calculate the range of the temperature readings, 
//       defined as the difference between the largest 
//       and smallest reading
//    3. Check if the captured readings are a non-increasing montonic series
//       This simply means that the readings are sorted in non-increasing order.
//       We do not say "increasing" because it is possible for consecutive values
//       to be the same, hence the term "non-increasing". The controller performs 
//       some remedial operation and the desired effect of the operation is to 
//       lower the the temperature of the sensed system. This routine helps 
//       verify whether this has indeed happened
#include <stdint.h>
#define True 1
#define False 0
#define N 21       // Number of temperature readings
uint8_t Readings[N]; // Array of temperature readings to perform analysis on 

// Return the computed Mean 
uint8_t Find_Mean(){
	uint8_t i = 0;			//for-loop counter and array pointer
	uint32_t a = 0;			//mean
	for(i = 0; i < 21; i++)
		a += Readings[i];
	a /= 21;
  return(a);
}

// Return the computed Range
uint8_t Find_Range(){
// Replace ths following line with your solution
	uint8_t max = 0;		//maximum value
	uint8_t min = 0;		//minimum value
	uint8_t c = 0;			//for-loop counter and array pointer
	uint8_t foo = 0;		//comparison value
	uint8_t range = 0;	//final return value of max - min
	max = Readings[0];
	min = Readings[0];
	for(c = 1; c < 21; c++)
	{
		foo = Readings[c];
		if(foo > max)
			max = Readings[c];
		else if(foo < min)
			min = Readings[c];			
	}
	range = max - min;
  return(range);
}

// Return True of False based on whether the readings
// a non-increasing montonic series
uint8_t IsMonotonic(){
	uint8_t aValue = 0;		//first comparison value
	uint8_t bValue = 0;		//second comparison value 
	for(int f = 0; f < 20; f++)
	{
		aValue = Readings[f];
		bValue = Readings[f+1];
		if(aValue < bValue)
			return(False);
	}
  return(True);
}

//Testcase 0:
// Scores[N] = {80,75,73,72,90,95,65,54,89,45,60,75,72,78,90,94,85,100,54,98,75};
// Range=55 Mean=77 IsMonotonic=False
//Testcase 1:
// Scores[N] = {100,98,95,94,90,90,89,85,80,78,75,75,75,73,72,72,65,60,54,54,45};	
// Range=55 Mean=77 IsMonotonic=True
//Testcase 2:
// Scores[N] = {80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80};
// Mean=80 Range=0 IsMonotonic=True
//Testcase 3:
// Scores[N] = {100,80,40,100,80,40,100,80,40,100,80,40,100,80,40,100,80,40,100,80,40};
// Mean=73 Range=60 IsMonotonic=False
//Testcase 4:
// Scores[N] = {100,95,90,85,80,75,70,65,60,55,50,45,40,35,30,25,20,15,10,5,0};
// Range=100 Mean=50 IsMonotonic=True

