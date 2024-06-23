#ifndef ProcessScheduler_h
#define ProcessScheduler_h 
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <sstream>
#include <list>
#include <iterator>
using namespace std;

struct Block
{
	int time = 0;
	string command = "";
};

struct Process
{
	string name = "";
	int index = 0;
	string state = "New";
	int arrival = 0;
	string instructions = "";
	string Current_Que = "NULL";

	int burst = 0;
	int i = 0;
	queue<Block> blocks;
};

void readInstructions(Process* p, int size)
{
	//Reading instructions for each process
	for (int i = 0; i < size; i++)
	{
		ifstream f;
		Block b;
		f.open(p[i].name + ".txt", ios::in); // each file is opened based on the process name

		if (!f)
			cout << "file not found";

		string temp;
		//while (!f.eof())
		while(getline(f, temp))
		{
			//getline(f, temp);
			if (temp == "COMPUTE")	//+1 in burst
				p[i].burst++;

			if (temp == "READ SCREEN")
			{
				b.command = "READ SCREEN";
				b.time = p[i].burst;
				p[i].blocks.push(b);
				p[i].burst += 2;
			}

			if (temp == "WRITE NETWORK")
			{
				b.command = "WRITE NETWORK";
				b.time = p[i].burst;
				p[i].blocks.push(b);

				p[i].burst += 3;
			}

			if (temp == "WRITE DISK")
			{
				b.command = "WRITE DISK";
				b.time = p[i].burst;
				p[i].blocks.push(b);
				p[i].burst += 2;
			}

			p[i].instructions += temp + "\n";
		}

		//displaying the code
		cout << "\nInstructions of " << p[i].name << "\nExpected Burst time : " << p[i].burst << endl << p[i].instructions;
		f.close();
	}
}
//-------------------------------------------------
void processFile(ofstream& f, Process p, int time)
{
	f << "\n--------------------------------------- Time == " << time << "---------------------------------------";
	f << "\nName : " << p.name;
	f << "\nArrival time : " << p.arrival;
	f << "\n Burst time : " << p.burst;
	f << "\nState : " << p.state;
	f << "\nQueue : " << p.Current_Que << endl;

}

void cpuFile(ofstream& f, Process p, int time)
{
	f << "\n--------------------------------------- Time == " << time << "---------------------------------------";
	f << "\nName : " << p.name;

	if (p.Current_Que == "DEVICE")// || (p.blocks.front().command == "READ SCREEN" && p.blocks.front().time == time))
		f << "\nCommand Running = " << "READ SCREEN";

	else if (p.Current_Que == "NETWORK")// || (p.blocks.front().command == "WRITE NETWORK" && p.blocks.front().time == time))
		f << "\nCommand Running = " << "WRITE NETWORK";

	else if (p.Current_Que == "HardDisk")// ||(p.blocks.front().command == "WRITE DISK" && p.blocks.front().time == time))
		f << "\nCommand Running = " << "WRITE DISK";
	//if (p.state == "Running" || p.Current_Que == "CPU")
	else
		f << "\nCommand Running = " << "COMPUTE";


}

void queueFile(ofstream& f, list<Process>p, int time, queue<int>FCFS1, queue<int>FCFS2, priority_queue<int, vector<int>, greater<int>> SJF, queue<int>block1, queue<int>block2, queue<int>block3)
{

	queue<int> F1 = FCFS1;
	queue<int> F2 = FCFS2;
	priority_queue<int, vector<int>, greater<int>> F3 = SJF;


	queue<int> B1 = block1;
	queue<int> B2 = block2;
	queue<int> B3 = block3;

	list<Process>::iterator it;

	f << "\n--------------------------------------- Time == " << time << "---------------------------------------";

	if (F1.empty())
		f << "\nFCFS1 is empty";

	else
	{
		f << "\nProcesses in FCFS1 : ";
		while (!F1.empty())
		{
			for (it = p.begin(); it != p.end(); it++)
			{
				if (it->index == F1.front())
					f << it->name << "\t";
			}
			F1.pop();
		}
	}


	if (F2.empty())
		f << "\nFCFS2 is empty";

	else
	{
		f << "\nProcesses in FCFS2 : ";
		while (!F2.empty())
		{
			for (it = p.begin(); it != p.end(); it++)
			{
				if (it->index == F2.front())
					f << it->name << "\t";
			}
			F2.pop();
		}
	}


	if (F3.empty())
		f << "\nSJF is empty";

	else
	{
		f << "\nProcesses in SJF : ";
		while (!F3.empty())
		{
			for (it = p.begin(); it != p.end(); it++)
			{
				if (it->index == F3.top())
					f << it->name << "\t";
			}
			F3.pop();
		}
	}
	//-------------------------------------
	//blocked queues
	if (B1.empty())
		f << "\nHardDisk Queue is empty";

	else
	{
		f << "\nProcesses in HardDisk : ";
		while (!B1.empty())
		{
			for (it = p.begin(); it != p.end(); it++)
			{
				if (it->index == B1.front())
					f << it->name << "\t";
			}
			B1.pop();
		}
	}

	if (B2.empty())
		f << "\nNetwork Queue is empty";

	else
	{
		f << "\nProcesses in Network : ";
		while (!B2.empty())
		{
			for (it = p.begin(); it != p.end(); it++)
			{
				if (it->index == B2.front())
					f << it->name << "\t";
			}
			B2.pop();
		}
	}

	if (B3.empty())
		f << "\nDevice Queue is empty";

	else
	{
		f << "\nProcesses in Device : ";
		while (!B3.empty())
		{
			for (it = p.begin(); it != p.end(); it++)
			{
				if (it->index == B3.front())
					f << it->name << "\t";
			}
			B3.pop();
		}
	}


}
//------------------------------------------------
//used for writing in file


void CPU(list<Process>process, int globalTick)
{
	queue<int> FCFS1;
	queue<int> FCFS2;
	//priority_queue<int> SJF;

	priority_queue<int, vector<int>, greater<int>> SJF;
	queue<int> HardDisk;
	queue<int> Network;
	queue<int> Device;

	int localtime = 0;
	int cputime = 0;
	int count = 1;
	int processSize = process.size();

	ofstream file1("Process.txt", ios::app);
	ofstream file2("CPU.txt", ios::app);
	ofstream file3("Queue.txt", ios::app);

	if (!file1)
		cout << "file 1 not created";
	if (!file2)
		cout << "file 2 not created";
	if (!file3)
		cout << "file 3 not created";




	cout << "\nExecution Started\n\n\n";
	list<Process>::iterator p;


	for (p = process.begin(); p != process.end() && localtime <= globalTick;)
	{
		if (localtime == p->arrival)
		{
			cout << "\nprocess : [" << p->name << "] is inserted in FCFS1 with time slice 3 at time : " << localtime << endl;
			FCFS1.push(p->index); //the process is pushed into the highest priority queue
			p->Current_Que = "FCFS1";
			p->state = "Ready";

			if (count != processSize)
			{
				p++;
				count++;
			}

			//-------------------------------------------------------
		label1:

			list<Process>* it = &process;
			Process* temp = new Process;
			bool search = 0;
			queue<Process> del;
			while (!search)
			{
				Process pa = it->front();
				if (pa.index == FCFS1.front())						//searches for the process with the index stored in front
				{
					temp = &it->front();
					search = 1;
				}
				else
				{
					del.push(it->front());
					it->pop_front();
				}
			}

			while (!del.empty())
			{
				process.push_front(del.front());
				del.pop();
			}
			//upper lines are editied
			search = 0;

			if (temp->burst == 0)
				goto exit1;

			temp->state = "Ready";
			temp->Current_Que = "FCFS1";
			processFile(file1, *temp, localtime);												//writing to process.txt
			queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
			cpuFile(file2, *temp, localtime);													//writing to cpu.txt
			//--------------------------------------------------------
			cout << "\nFCFS1 would run for process : [" << temp->name << "]\n";
			cputime = 0;
			while (cputime < 3 && temp->burst > 0)
			{
				temp->state = "Running";
				temp->Current_Que = "CPU";
				processFile(file1, *temp, localtime);											//writing to process.txt
				queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
				cpuFile(file2, *temp, localtime);													//writing to cpu.txt

				if (localtime >= globalTick)	//if global tick finishes while in CPU 
				{
					cout << "\n\n\nGlobal Tick Finished";
					return;
				}

				if (localtime == p->arrival && p->Current_Que == "NULL")
				{
					cout << "\nprocess : [" << p->name << "] is inserted in FCFS1 with time slice 3 at time : " << localtime << endl;
					FCFS1.push(p->index);
					//-------------------------------------------------------

					if (count != processSize)
					{
						p++;
						count++;
					}

				}
				if (temp->blocks.empty() == 0)
				{
					if (temp->blocks.front().time == temp->i)
					{
						cout << "\nProcess [" << temp->name << "] encountered " << temp->blocks.front().command;
						cout << "\nadded to Blocked queue\n\n";

						if (temp->blocks.front().command == "READ SCREEN")
						{
							Device.push(temp->index);	//added to Device queue
							localtime += 2;				// 2 seconds read time
							temp->burst -= 2;
							temp->Current_Que = "DEVICE";
							temp->state = "Blocked";
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							FCFS1.push(Device.front());	// added to highest priority queue
							Device.pop();

						}

						if (temp->blocks.front().command == "WRITE NETWORK")
						{
							Network.push(temp->index);	//added to Device queue
							localtime += 3;				// 2 seconds read time
							temp->burst -= 3;
							temp->Current_Que = "NETWORK";
							temp->state = "Blocked";
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							FCFS1.push(Network.front());	// added to highest priority queue
							Network.pop();
						}

						if (temp->blocks.front().command == "WRITE DISK")
						{
							HardDisk.push(temp->index);	//added to Device queue
							localtime += 2;				// 2 seconds read time
							temp->burst -= 2;
							temp->Current_Que = "HardDisk";
							temp->state = "Blocked";
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							FCFS1.push(HardDisk.front());	// added to highest priority queue
							HardDisk.pop();
						}
						processFile(file1, *temp, localtime);											//writing to process.txt
						queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
						cpuFile(file2, *temp, localtime);													//writing to cpu.txt
						temp->blocks.pop();	//the current block is removed from process
					}
				}
				temp->burst--;
				localtime++;
				cputime++;
				temp->i++;
			}

			//the process is added to the next FCFS with 6 seconds
			temp->Current_Que = "FCFS2";
			temp->state = "Ready";
			processFile(file1, *temp, localtime);											//writing to process.txt
			queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
			cpuFile(file2, *temp, localtime);													//writing to cpu.txt

																								//the process is added to the next FCFS with 6 seconds
			FCFS2.push(FCFS1.front());
			cout << "\nprocess : [" << temp->name << "] is inserted in FCFS2 with time slice 6 at time : " << localtime << endl;
		exit1:
			FCFS1.pop();
			if (!FCFS1.empty()) //highest priority is given to FCFS1 and cannot go to next till this is empty
				goto label1;
		}

		cout << "\n\n-----------------------------------------------------------\nGlobal time Remaining : " << globalTick - localtime << endl;
		list<Process>::iterator it;
		for (it = process.begin(); it != process.end(); ++it)
		{
			cout << it->name << "\t" << it->burst << endl;
		}
		cout << "\n-----------------------------------------------------------\n";
		//--------------------------------------------------------------------------------------------------------------------------------------------

		if (localtime <= globalTick && (!FCFS2.empty() && FCFS1.empty())) //condition for a process entering for the second queue
		{
			cout << "\n\n\n\nFCFS2 has started running";
			if (count != processSize)
			{
				p++;
				count++;
			}
		label2:

			list<Process>* it = &process;
			Process* temp = new Process;
			bool search = 0;
			queue<Process> del;
			while (!search)
			{
				Process pa = it->front();
				if (pa.index == FCFS2.front())						//searches for the process with the index stored in front
				{
					temp = &it->front();
					search = 1;
				}
				else
				{
					del.push(it->front());
					it->pop_front();
				}
			}

			while (!del.empty())
			{
				process.push_front(del.front());
				del.pop();
			}
			//upper lines are editied
			search = 0;
			if (temp->burst == 0)
				goto exit2;

			temp->state = "Ready";
			temp->Current_Que = "FCFS2";
			processFile(file1, *temp, localtime);												//writing to process.txt
			queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
			cpuFile(file2, *temp, localtime);													//writing to cpu.txt

			cout << "\nFCFS2 would run for process : [" << temp->name << "]\n";
			cputime = 0;
			while (cputime < 6 && temp->burst > 0)
			{
				temp->state = "Running";
				temp->Current_Que = "CPU";
				processFile(file1, *temp, localtime);												//writing to process.txt
				queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
				cpuFile(file2, *temp, localtime);													//writing to cpu.txt

				if (localtime >= globalTick)	//if global tick finishes while in CPU 
				{
					cout << "\n\n\nGlobal Tick Finished";
					return;
				}

				if (localtime == p->arrival && p->Current_Que == "NULL")	//if a new process enters it goes to FCFS1
				{
					cout << "\nprocess : [" << p->name << "] is inserted in FCFS2 with time slice 6 at time : " << localtime << endl;
					FCFS1.push(p->index);
					//-------------------------------------------------------

					if (count != processSize)
					{
						p++;
						count++;
					}
				}
				if (temp->blocks.empty() == 0)
				{
					if (temp->blocks.front().time == temp->i)
					{
						cout << "\nProcess [" << temp->name << "] encountered " << temp->blocks.front().command;
						cout << "\nadded to Blocked queue\n\n";

						if (temp->blocks.front().command == "READ SCREEN")
						{
							Device.push(temp->index);	//added to Device queue
							localtime += 2;				// 2 seconds read time
							temp->burst -= 2;
							temp->i += 2;
							temp->Current_Que = "DEVICE";
							temp->state = "Blocked";
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							FCFS1.push(Device.front());	// added to highest priority queue
							Device.pop();
						}

						if (temp->blocks.front().command == "WRITE NETWORK")
						{
							Network.push(temp->index);	//added to Device queue
							localtime += 3;				// 2 seconds read time
							temp->burst -= 3;
							temp->i += 3;
							temp->Current_Que = "NETWORK";
							temp->state = "Blocked";
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							FCFS1.push(Network.front());	// added to highest priority queue
							Network.pop();
						}

						if (temp->blocks.front().command == "WRITE DISK")
						{
							HardDisk.push(temp->index);	//added to Device queue
							localtime += 2;				// 2 seconds read time
							temp->burst -= 2;
							temp->i += 2;
							temp->Current_Que = "HardDisk";
							temp->state = "Blocked";
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							FCFS1.push(HardDisk.front());	// added to highest priority queue
							HardDisk.pop();
						}
						processFile(file1, *temp, localtime);												//writing to process.txt
						cpuFile(file2, *temp, localtime);													//writing to cpu.txt
						queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt

						temp->blocks.pop();	//the current block is removed from process
					}
				}

				temp->i++;
				temp->burst--;
				localtime++;
				cputime++;
			}

			processFile(file1, *temp, localtime);												//writing to process.txt
			cpuFile(file2, *temp, localtime);													//writing to cpu.txt
			queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt

			temp->Current_Que = "SJF";
			temp->Current_Que = "Ready";

			SJF.push(FCFS2.front());
		exit2:
			FCFS2.pop();

			if (!FCFS1.empty())
			{
				cout << "\nFCFS1 is not empty yet going back\n\n\n";
				goto bloc;
			}
			else if (!FCFS2.empty())
				goto label2;



		}
		for (it = process.begin(); it != process.end(); ++it)
		{
			cout << it->name << "\t" << it->burst << endl;
		}
		cout << "\n-----------------------------------------------------------\n";
		//------------------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------------------

		if (localtime <= globalTick && (!SJF.empty() && FCFS2.empty())) //condition for a process entering for the second queue/*
		{
			cout << "\n\n\n\nSJF has started running";
			if (count != processSize)
			{
				p++;
				count++;
			}
		label3:

			list<Process>* it = &process;
			Process* temp = new Process;
			bool search = 0;
			queue<Process> del;
			while (!search)
			{
				Process pa = it->front();
				if (pa.index == SJF.top())						//searches for the process with the index stored in front
				{
					temp = &it->front();
					search = 1;
				}
				else
				{
					del.push(it->front());
					it->pop_front();
				}
			}

			while (!del.empty())
			{
				process.push_front(del.front());
				del.pop();
			}
			//upper lines are editied
			search = 0;

			if (temp->burst == 0)
				goto exit3;

			temp->state = "Ready";
			temp->Current_Que = "SJF";
			processFile(file1, *temp, localtime);												//writing to process.txt
			queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
			cpuFile(file2, *temp, localtime);													//writing to cpu.txt

			cout << "\nSJF would run for process : [" << temp->name << "]\n";
			cputime = 0;

			while (temp->burst > 0)
			{
				temp->state = "Running";
				temp->Current_Que = "CPU";
				processFile(file1, *temp, localtime);												//writing to process.txt
				queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
				cpuFile(file2, *temp, localtime);													//writing to cpu.txt

				if (localtime >= globalTick)	//if global tick finishes while in CPU 
				{
					cout << "\n\n\nGlobal Tick Finished";
					return;
				}

				if (localtime == p->arrival && p->Current_Que == "NULL")	//if a new process enters it goes to FCFS2
				{
					cout << "\nprocess : [" << p->name << "] is inserted in FCFS2 with time slice 6 at time : " << localtime << endl;
					FCFS1.push(p->index);
					//-------------------------------------------------------

					if (count != processSize)
					{
						p++;
						count++;
					}
				}
				if (temp->blocks.empty() == 0)
				{
					if (temp->blocks.front().time == temp->i)
					{
						cout << "\nProcess [" << temp->name << "] encountered " << temp->blocks.front().command;
						cout << "\nadded to Blocked queue\n\n";

						if (temp->blocks.front().command == "READ SCREEN")
						{
							Device.push(temp->index);	//added to Device queue
							localtime += 2;				// 2 seconds read time
							temp->burst -= 2;
							temp->i += 2;
							temp->Current_Que = "DEVICE";
							temp->state = "Blocked";
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							FCFS1.push(Device.front());	// added to highest priority queue
							Device.pop();
						}

						if (temp->blocks.front().command == "WRITE NETWORK")
						{
							Network.push(temp->index);	//added to Device queue
							localtime += 3;				// 2 seconds read time
							temp->burst -= 3;
							temp->i += 3;
							temp->Current_Que = "NETWORK";
							temp->state = "Blocked";
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							FCFS1.push(Network.front());	// added to highest priority queue
							Network.pop();
						}

						if (temp->blocks.front().command == "WRITE DISK")
						{
							HardDisk.push(temp->index);	//added to Device queue
							localtime += 2;				// 2 seconds read time
							temp->burst -= 2;
							temp->i += 2;
							temp->Current_Que = "HardDisk";
							temp->state = "Blocked";
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							FCFS1.push(HardDisk.front());	// added to highest priority queue
							HardDisk.pop();
						}
						cpuFile(file2, *temp, localtime);													//writing to cpu.txt
						processFile(file1, *temp, localtime);												//writing to process.txt
						queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
						temp->blocks.pop();	//the current block is removed from process

					}
				}

				temp->i++;
				temp->burst--;
				localtime++;
				cputime++;
			}

			temp->Current_Que = "SJF";
			temp->Current_Que = "Ready";
			cpuFile(file2, *temp, localtime);													//writing to cpu.txt
			processFile(file1, *temp, localtime);												//writing to process.txt
			queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt

		exit3:
			SJF.pop();


			if (!FCFS1.empty())
			{
				cout << "\nFCFS1 is not empty yet going back\n\n\n";
				goto bloc;
			}

			if (!SJF.empty())
				goto label3;

		}


		for (it = process.begin(); it != process.end(); ++it)
		{
			cout << it->name << "\t" << it->burst << endl;
		}
		cout << "\n-----------------------------------------------------------\n";

		//------------------------------------------------------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------------------------------------------------------
	bloc:
		if (localtime <= globalTick && (!FCFS1.empty())) //this would run FCFS1 for blocked queus again
		{
			cout << "\n\n\n\nFCFS1 has started again running for blocked queue";
			if (count != processSize)
			{
				p++;
				count++;
			}
		label11:
			list<Process>* it = &process;
			Process* temp = new Process;
			bool search = 0;
			queue<Process> del;
			while (!search)
			{
				Process pa = it->front();
				if (pa.index == FCFS1.front())						//searches for the process with the index stored in front
				{
					temp = &it->front();
					search = 1;
				}
				else
				{
					del.push(it->front());
					it->pop_front();
				}
			}

			while (!del.empty())
			{
				process.push_front(del.front());
				del.pop();
			}
			//upper lines are editied
			search = 0;

			//if (temp->burst == 0)
				//goto exit7;

			temp->state = "Ready";
			temp->Current_Que = "SJF";
			cpuFile(file2, *temp, localtime);													//writing to cpu.txt
			processFile(file1, *temp, localtime);												//writing to process.txt
			queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt




			cout << "\nFCFS1 would run for blocked process : [" << temp->name << "]\n";
			cputime = 0;
			while (cputime < 3 && temp->burst > 0)
			{
				temp->state = "Running";
				temp->Current_Que = "CPU";
				cpuFile(file2, *temp, localtime);													//writing to cpu.txt
				processFile(file1, *temp, localtime);												//writing to process.txt
				queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt


				if (localtime >= globalTick)	//if global tick finishes while in CPU 
				{
					cout << "\n\n\nGlobal Tick Finished";
					return;
				}

				if (localtime == p->arrival && p->Current_Que == "NULL")	//if a new process enters it goes to FCFS2
				{
					cout << "\nprocess : [" << p->name << "] is inserted in FCFS1 with time slice 3 at time : " << localtime << endl;
					FCFS1.push(p->index);
					//-------------------------------------------------------

					if (count != processSize)
					{
						p++;
						count++;
					}
				}

				if (temp->blocks.empty() == 0)
				{
					if (temp->blocks.front().time == temp->i)
					{
						cout << "\nProcess [" << temp->name << "] encountered " << temp->blocks.front().command;
						cout << "\nadded to Blocked queue\n\n";

						if (temp->blocks.front().command == "READ SCREEN")
						{
							Device.push(temp->index);	//added to Device queue
							localtime += 2;				// 2 seconds read time
							temp->burst -= 2;
							temp->i += 2;
							temp->Current_Que = "DEVICE";
							temp->state = "Blocked";
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							FCFS1.push(Device.front());	// added to highest priority queue
							Device.pop();
						}

						if (temp->blocks.front().command == "WRITE NETWORK")
						{
							Network.push(temp->index);	//added to Device queue
							localtime += 3;				// 2 seconds read time
							temp->burst -= 3;
							temp->i += 3;
							temp->Current_Que = "NETWORK";
							temp->state = "Blocked";
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							FCFS1.push(Network.front());	// added to highest priority queue
							Network.pop();
						}

						if (temp->blocks.front().command == "WRITE DISK")
						{
							HardDisk.push(temp->index);	//added to Device queue
							localtime += 2;				// 2 seconds read time
							temp->burst -= 2;
							temp->i += 2;
							temp->Current_Que = "HardDisk";
							temp->state = "Blocked";
							cpuFile(file2, *temp, localtime);													//writing to cpu.txt
							queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt
							FCFS1.push(HardDisk.front());	// added to highest priority queue
							HardDisk.pop();
						}
						cpuFile(file2, *temp, localtime);													//writing to cpu.txt
						processFile(file1, *temp, localtime);												//writing to process.txt
						queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt

						temp->blocks.pop();	//the current block is removed from process
					}
				}

				temp->i++;
				temp->burst--;
				localtime++;
				cputime++;
			}

			//the process is added to the next FCFS with 6 seconds
			temp->Current_Que = "FCFS2";
			temp->Current_Que = "Ready";
			cpuFile(file2, *temp, localtime);													//writing to cpu.txt
			processFile(file1, *temp, localtime);												//writing to process.txt
			queueFile(file3, process, localtime, FCFS1, FCFS2, SJF, HardDisk, Network, Device);	//writing to queue.txt


			//the process is added to the next FCFS with 6 seconds
			FCFS2.push(FCFS1.front());
			cout << "\nprocess : [" << temp->name << "] is inserted in FCFS2 with time slice 6 at time : " << localtime << endl;
			//exit7:
			FCFS1.pop();
			if (!FCFS1.empty()) //highest priority is given to FCFS1 and cannot go to next till this is empty
				goto label11;


		}

		//if all processes are finished
		bool flag = 1;
		for (it = process.begin(); it != process.end(); ++it)
		{
			if (it->burst > 0)
				flag = 0;
		}
		if (flag)
		{
			++localtime;
			cout << "\n\n\n\nProgram finished :)\nGlobal time remaining : " << globalTick - localtime;
			cout << "\nLocal time used : " << localtime << endl;
			return;

		}

		localtime++;
	}

}



void runProcessScheduling(const char* PathofJobFile, int globalTick)
{
	ifstream file;
	int numberOfProcesses = 0;

	file.open(PathofJobFile, ios::in);
	if (!file)
		cout << "File not created";

	//Reading job.txt to get number of processes
	string temp;

//	while (!file.eof())
	while(getline(file, temp))
	{
		//getline(file, temp);
		numberOfProcesses++;
	}

	// using the same file object to read job.txt
	file.clear();
	file.seekg(0, ios::beg);

	Process* p = new Process[numberOfProcesses]; //data is stored in array first

	for (int i = 0; i < numberOfProcesses && (!file.eof()); i++) 	//job.txt is read to get name and arrival of processes
		file >> p[i].name >> p[i].arrival;

	file.close();

	cout << "Displaying job.txt\n"; //Displaying job.txt
	for (int i = 0; i < numberOfProcesses; i++)
		cout << p[i].name << " " << p[i].arrival << endl;

	for (int i = 0; i < numberOfProcesses; i++) 	//sorting the processes in terms of arrival time
		for (int j = 0; j < numberOfProcesses; j++)
			if (p[j].arrival > p[i].arrival)
				swap(p[j], p[i]);

	for (int i = 0; i < numberOfProcesses; i++) 	//allocating indexes after bubble sort
		p[i].index = i;

	readInstructions(p, numberOfProcesses); 	//Reading instructions for each process

	list<Process> processes; 	//storing all this in process linked list
	for (int i = 0; i < numberOfProcesses; i++)
		processes.push_back(p[i]);


	//writing the starting processes in process.txt
	ofstream Pfile;
	ofstream Pfil2("Queue.txt", ios::out);
	Pfil2.close();
	ofstream Pfil3("CPU.txt", ios::out);
	Pfil3.close();

	Pfile.open("Process.txt", ios::out);
	if (!file)
		cout << "\nFile not created";

	for (int i = 0; i < numberOfProcesses; i++)
	{
		Pfile << "\n--------------------------------------- Time == " << "0" << "---------------------------------------";
		Pfile << "\nName : " << p[i].name;
		Pfile << "\nArrival time : " << p[i].arrival;
		Pfile << "\nBurst time : " << p[i].burst;
		Pfile << "\nState : " << p[i].state;
		Pfile << "\nQueue : " << p[i].Current_Que << endl;

	}
	Pfile << "\n\nProcesses will be written Before CPU Allocation,During CPU,Blocked and After CPU for all queues\n-----------------------------------------Timer started-----------------------------------------------------------\n\n\n";
	Pfile.close();

	CPU(processes, globalTick);

}

#endif

