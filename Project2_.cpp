/*Class: 351
Secction: 01
Authors: Stephen Lastimosa, Dayna Anderson, Ian Alvarez
Date: 6/28/2018
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <queue> 
#include <string>
#include <math.h>
#include <iomanip>


struct process {
	int pId, arrivalT, lifeT, spaceNum, memReq, pagesReq;
	std::vector<int> addVector;
};
struct page {
	int start, end, piD, num, arrT, endT;
};

void printMemMap(std::vector<page>, int);
void printInputQueue(std::queue<process>);

int main() {
	int memSize, pSize, availablePages;
	std::queue<process> waitQueue, inQueue;
	std::string fiName;

	std::cout << "Memory Size: ";
	std::cin >> memSize;

	std::cout << "Page Size(1:100, 2:200, 3:400): ";
	std::cin >> pSize;


	while ((pSize != 1) && (pSize != 2) && (pSize != 3)) {
		std::cout << "Invalid value!\nPlease enter the number corresponding with the Page Size you would like to choose!\nPage Size(1:100, 2:200, 3:400): ";
		std::cin >> pSize;
	}
	switch (pSize) {
	case 1:
		pSize = 100;
		break;
	case 2:
		pSize = 200;
		break;
	case 3:
		pSize = 400;
	}
	availablePages = memSize / pSize;
	std::cin.ignore();
	std::cout << "Please enter the name of a workload file: ";
	std::getline(std::cin, fiName);
	std::fstream fin(fiName, std::fstream::in);

	int totProc;


	/*This section reads in information from input 
	file and stores all processes in the waitQueue*/

	if (fin.is_open()) {
		fin >> totProc;
		//read in process information to temp process and then push to inQueue
		for (int i = 0; i < totProc; i++) {
			process temp;
			temp.memReq = 0;
			fin >> temp.pId >> temp.arrivalT >> temp.lifeT >> temp.spaceNum;
			//read address spaces into process address vector and store their total sum in memReq.
			for (int j = 0; j < temp.spaceNum; j++) {
				int addr;
				fin >> addr;
				temp.memReq = temp.memReq + addr;
				temp.addVector.push_back(addr);
			}
			int wholep = (temp.memReq / pSize);
			double partialp = temp.memReq%pSize;
			int add = 0;
			if (partialp > 0) {
				add = 1;
			}
			temp.pagesReq = wholep + add;
			if (temp.memReq <= memSize) { //here we ignore any processes that require memory
				waitQueue.push(temp);	// larger than the total memory entered.(per project doc instructions)
			}

		}//close for
		fin.close();
	}

	/*in this section we can start the clock and move the processes now in our 
	waitqueue into the input queue at their arrival times. The processes are then moved
	into memory when there is enough space available and their status is tracked.*/
	long t = 0;
	int pages = memSize / pSize;
	std::vector<page> memMap;
	std::vector<int>turnaroundTimes;
	page init;
	int lastProcID = waitQueue.back().pId;

	//initalize needed pages in memory map with start & end addressees
	for (int i = 0; i < pages; i++) {
		init.start = i*pSize;
		init.end = ((i + 1)*pSize) - 1;
		init.num = 0;
		init.piD = -1;
		init.endT = -1;
		memMap.push_back(init);
	}

	int maxTime = 99999;

	while (t != maxTime+1) {
		bool print = true;
		bool printMap = false;
		bool waitCheck = false;
		//prints out arrival of processes at time t, adds them to the input queue and then prints the updated input queue
		if (!waitQueue.empty()) {
			waitCheck =(waitQueue.front().arrivalT == t);
		}
			while (waitCheck){
				std::cout << "t = " << t<<std::endl;
				std::cout << "\t\tProcess " << waitQueue.front().pId << " arrives\n";	
				inQueue.push(waitQueue.front());				
				waitQueue.pop();								
				printInputQueue(inQueue);
				print = false;
				printMap = true;

				if (!waitQueue.empty()) {
					waitCheck = (waitQueue.front().arrivalT == t);
				}
				else {
					waitCheck = false;
				}
			}
		
			bool pprint = true;

		//clean out pages with finished processes
		for (int i = 0; i < pages; i++) {
			int p = 0;
			if (memMap[i].endT == t) {
				p = memMap[i].piD;
				memMap[i].piD = -1;
				memMap[i].num = 0;
				memMap[i].endT = 0;
				availablePages++;
			}
			if ((p != 0)&&(pprint)) {
				if (print) {
					std::cout << "t = " << t << std::endl;
					print = false;
				}
				std::cout << "\t\tProcess " << p << " completes\n";
				printMap = true;
				pprint = false;
			}
		}


		//compares available memory to items in input queue and places them in available pages of main memory.

		while ((!inQueue.empty()) && (availablePages >= inQueue.front().pagesReq)) {
			if (print) {
				std::cout << "t = " << t<<std::endl;
				print = false;
			}
			std::cout << "\t\tMM moves Process " << inQueue.front().pId << " to memory\n";
			printMap = true;
			int endTime = t + inQueue.front().lifeT;
			int pnum = 1;
			int i = 0;
			if (memMap[i].piD == lastProcID) {
				maxTime = endTime;
			}
			memMap[i].arrT = inQueue.front().arrivalT;
			turnaroundTimes.push_back(memMap[i].endT - memMap[i].arrT);
			
			while ((inQueue.front().pagesReq != 0) && (i < pages)) {
				if (memMap[i].piD == -1) {
					memMap[i].piD = inQueue.front().pId;
					memMap[i].num = pnum;
					memMap[i].endT = endTime;
					pnum++;
					availablePages--;
					inQueue.front().pagesReq--;

				}
				i++;
			}
			inQueue.pop();
		}//end front of queue to memorywhile

		//print memory map.
		if (printMap) {
			printMemMap(memMap, pages);
		}
		t++;
	}//time frame ends

	//this section calculates the average turnaround time from the turnaround time vector and prints it to the console.
	int sum=0;
	for (int i = 0; i < turnaroundTimes.size(); i++) {
		sum += turnaroundTimes[i];
	}
	double turnaround = sum / turnaroundTimes.size();
	std::cout <<"\nAverage Turnaround Time: " <<std::fixed << std::setprecision(2) << turnaround <<"\n"<< std::endl;
	return 0;
}

//prints memory map status
void printMemMap(std::vector<page> mMap, int ptot) {
	std::cout << "\t\tMemory Map: \n";
	int eStart = 0;
	int eEnd = 0;
	bool emptyStart = true;

	for (int i = 0; i < ptot; i++) {
		page p = mMap[i];

		if (p.piD == -1){
			if (emptyStart){
				eStart = p.start;
				emptyStart = false;
			}
			eEnd = p.end;
			if (i == (ptot - 1)){
				std::cout << "\t\t\t" << eStart << "-" << eEnd << ": ";
				std::cout << "\tFree Frame(s)\n";
			}

		}else {
			if (eEnd != 0) {
				std::cout << "\t\t\t" << eStart << "-" << eEnd << ": ";
				if (i == 1) {
					std::cout << "\t";
				}
				std::cout << "\tFree Frame(s)\n";
				eStart = 0;
				eEnd = 0;
				emptyStart = true;
				std::cout << "\t\t\t" << p.start << "-" << p.end << ": ";
				std::cout << "\tProcess " << p.piD << ", \tPage " << p.num<< std::endl;
			}
			else {
				std::cout << "\t\t\t" << p.start << "-" << p.end << ": ";
				if (i == 0) {
					std::cout << "\t";
				}
				std::cout << "\tProcess " << p.piD << ", \tPage " << p.num << std::endl;
			}
		}



	}


	std::cout << std::endl;
}


//prints input queue status
void printInputQueue(std::queue<process> iQueue) {
	std::cout << "\t\tInput Queue: [ ";
	while (!iQueue.empty()) {
		std::cout << iQueue.front().pId << " ";
		iQueue.pop();
	}
	std::cout << "]\n";
}
