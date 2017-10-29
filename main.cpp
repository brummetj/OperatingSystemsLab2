#include <iostream>
#include <iomanip>
#include <fstream>
#include "process.h"
#include "container.h"

using namespace std;

void sort_burst(list processes);
void sort_priority(list processes);
void sort_pid(list processes);
void statistics(list statistics, string title);
void cpu_scheduling_menu();
list get_processes(string filename);

void first_come_first_serve(const string &input_file, const string &output_file){

    list processes = get_processes(input_file);
    string title = "First Come First Serve";
    int num_completed = 0;
    int clock = 0;
    list queue;
    list stats;
    int index;
    int num_processes = processes.size();
    while(num_completed != num_processes){

        index = processes.size();
        for(int i = 0; i < index; i++){

            if(processes.index(i).get_arrival() <= clock){
                cout << "[time " << processes.index(i).get_arrival() << "ms] PID - " << processes.index(i).get_pid() <<
                          " created (requires " << processes.index(i).get_burst() << "ms CPU time)";
                cout << endl;
                queue.insert_end(processes.index(i));
                processes.delete_first();
                index--;
                i--;
            }
        }
        if(queue.size() != 0){

            if(!queue.index(0).get_seen()){

                double arrival = clock - queue.index(0).get_arrival();
                process temp = queue.index(0);
                temp.seen(arrival);
                queue.edit(0,temp);
                cout << "[time " << clock << "ms] Process " << queue.index(0).get_pid() <<
                     " accessed CPU for the first time (initial wait time " << queue.index(0).get_initial_wait() << "ms)" << endl;
            }

            process temp = queue.index(0);
            temp.dec_burst();
            queue.edit(0, temp);
            if(queue.index(0).get_burst_left() == 0){

                double wait = ((clock +1 ) - queue.index(0).get_burst() - queue.index(0).get_arrival());
                double finish = clock + 1;
                double turnAround = ((clock +1) - queue.index(0).get_arrival());
                double response = wait;

                process insert = queue.index(0);
                insert.setFinish_time(finish);
                insert.setResponse_time(response);
                insert.setTurnaround_time(turnAround);
                queue.edit(0, insert);
                cout << "[time " << clock + 1 << "ms] Process " << queue.index(0).get_pid() <<
                     " completed its CPU burst (turnaround time " << ((clock +1) - queue.index(0).get_arrival()) << "ms, initial wait time "
                     << queue.index(0).get_initial_wait() << "ms, response time " << wait << "ms)" << endl;
                stats.insert_start(queue.index(0));
                queue.delete_first();
                num_completed++;
            }

        }
        clock++;
    }
    statistics(stats, title);
}

void shortest_remaining_job_first(const string &input_file, const string &output_file) {

    list processes = get_processes(input_file);

    string title = "Shortest Remaining Job First";
    int num_completed = 0;
    double clock = 0;
    list queue;
    list stats;
    int index;
    int num_processes = processes.size();

    while (num_completed != num_processes) {
        index = processes.size();
        for(int i = 0; i < index; i++){

            if(processes.index(i).get_arrival() <= clock){
                cout << "[time " << clock  << "ms] PID - " << processes.index(i).get_pid() <<
                          " created (requires " << processes.index(i).get_burst() << "ms CPU time)";
                cout << endl;
                if(queue.size() !=0 &&
                        processes.index(i).get_burst() < queue.index(0).get_burst() &&
                        queue.index(0).get_burst_left() != queue.index(0).get_burst()){

                    //Context Switch on preemption
                    cout << "[time " << clock<< "ms] Context switch (swapped out process "
                         << queue.index(0).get_pid() << " for process " << processes.index(i).get_pid() << ")" << endl;
                    clock += 0.5;
                    process setContext = queue.index(0);
                    setContext.setContext_switch(true);
                    setContext.inc_num_context();
                    queue.edit(0, setContext);
                }
                queue.insert_end(processes.index(i));
                sort_burst(queue);

                //Arrival First!
                processes.delete_first();
                index--;
                i--;
            }
        }
        if (queue.size() != 0) {

            if (!queue.index(0).get_seen()) {

                if(queue.size() > 1 && queue.index(0).get_burst_left() == queue.index(1).get_burst_left()){
                    if(queue.index(0).get_arrival() > queue.index(1).get_arrival()){
                        process t1 = queue.index(0);
                        process t2 = queue.index(1);
                        queue.edit(0, t2);
                        queue.edit(1, t1);
                    }
                }
                if(!queue.index(0).isContext_switch()){
                    double arrival = clock - queue.index(0).get_arrival();
                    process edit = queue.index(0);
                    edit.seen(arrival);
                    queue.edit(0, edit);
                    cout << "[time " << clock << "ms] Process " << queue.index(0).get_pid() <<
                         " accessed CPU for the first time (initial wait time " << queue.index(0).get_initial_wait()
                         << "ms)" << endl;
                }
            }


            process t = queue.index(0);
            t.dec_burst();
            queue.edit(0, t);


            if (queue.index(0).get_burst_left() == 0) {

                double wait = ((clock + 1) - queue.index(0).get_burst() - queue.index(0).get_arrival());

                if(queue.index(0).isContext_switch()){
                    process initial = queue.index(0);
                    initial.setInitial_wait(wait);
                    queue.edit(0, initial);
                    wait = queue.index(0).get_arrival();
                }

                double finish = clock + 1;
                double turnAround = ((clock +1) - queue.index(0).get_arrival());
                double response = wait;

                process insert = queue.index(0);
                insert.setFinish_time(finish);
                insert.setResponse_time(response);
                insert.setTurnaround_time(turnAround);
                queue.edit(0, insert);

                cout << "[time " << clock + 1  << "ms] Process " << queue.index(0).get_pid() <<
                     " completed its CPU burst (turnaround time " << ((clock + 1) - queue.index(0).get_arrival())
                     << "ms, initial wait time "
                     << queue.index(0).get_initial_wait() << "ms, response time " << wait << "ms)" << endl;

                stats.insert_start(queue.index(0));
                queue.delete_first();
                num_completed++;
            }
        }
        clock++;
    }

    statistics(stats, title);
}

void round_robin(const string &input_file, const string &output_file, int quantum_time){


    list processes = get_processes(input_file);
    string title = "Round Robin";
    int num_completed = 0;
    int index;
    double clock = 0;
    list queue;
    list stats;
    int num_processes = processes.size();
    int time_slice = 0;
    while(num_completed != num_processes){
        index = processes.size();
        for(int i = 0; i < index; i++){

            if(processes.index(i).get_arrival() <= clock){
                cout << "[time " << processes.index(i).get_arrival() << "ms] PID - " << processes.index(i).get_pid() <<
                          " created (requires " << processes.index(i).get_burst() << "ms CPU time)";
                cout << endl;
                queue.insert_end(processes.index(i));
                processes.delete_first();
                index--;
                i--;
            }
        }
        if(queue.size() != 0){

            if(!queue.index(0).get_seen()){

                if(!queue.index(0).isContext_switch()){
                    double arrival = clock - queue.index(0).get_arrival();
                    process edit = queue.index(0);
                    edit.seen(arrival);
                    queue.edit(0, edit);
                    cout << "[time " << clock << "ms] Process " << queue.index(0).get_pid() <<
                         " accessed CPU for the first time (initial wait time " << queue.index(0).get_initial_wait()
                         << "ms)" << endl;
                }
            }

            process temp = queue.index(0);
            temp.dec_burst();
            queue.edit(0, temp);
            time_slice++;
            if(queue.index(0).get_burst_left() == 0){

                double wait = ((clock +1 ) - queue.index(0).get_burst() - queue.index(0).get_arrival());
                double finish = clock + 1;
                double turnAround = ((clock +1) - queue.index(0).get_arrival());

                process insert = queue.index(0);
                insert.setFinish_time(finish);
                insert.setResponse_time(queue.index(0).get_initial_wait());
                insert.setTurnaround_time(turnAround);
                insert.setInitial_wait(wait);
                queue.edit(0, insert);

                cout << "[time " << clock + 1  << "ms] Process " << queue.index(0).get_pid() <<
                     " completed its CPU burst (turnaround time " << ((clock + 1) - queue.index(0).get_arrival())
                     << "ms, initial wait time "
                     << wait << "ms, response time " << queue.index(0).getResponse_time() << "ms)" << endl;

                stats.insert_start(queue.index(0));
                queue.delete_first();
                time_slice = 0;
                num_completed++;
            }
            else if(time_slice == quantum_time){
                int last_pid = queue.index(0).get_pid();
                process setContext = queue.index(0);
                clock = clock + 0.5;
                setContext.setContext_switch(true);
                setContext.inc_num_context();
                queue.edit(0, setContext);
                queue.insert_end(queue.index(0));
                queue.delete_first();
                cout << "[time " << clock + 1 << "ms] Context switch (swapped out process " << last_pid << " for process " << queue.index(0).get_pid() << ")" << endl;
                time_slice = 0;
            }
        }
        clock++;
    }

    statistics(stats, title);
}

void preemptive_priority(const string &input_file, const string &output_file) {

    list processes = get_processes(input_file);
    string title = "Preemptive Priority";
    int num_completed = 0;
    double clock = 0;
    double total_wait;
    int last_pid = 0;
    list queue;
    list stats;
    bool cswitch = false;
    int index;
    int num_processes = processes.size();

    while (num_completed != num_processes) {
        index = processes.size();
        for(int i = 0; i < index; i++){

            if(processes.index(i).get_arrival() <= clock){
                cout << "[time " << clock  << "ms] PID - " << processes.index(i).get_pid() <<
                          " created (requires " << processes.index(i).get_burst() << "ms CPU time)";
                cout << endl;
                if(queue.size() !=0 &&
                   processes.index(i).get_priority() < queue.index(0).get_priority() &&
                   queue.index(0).get_burst_left() != queue.index(0).get_burst()){

                    //Context Switch on preemption
                    cout << "[time " << clock<< "ms] Context switch (swapped out process "
                         << queue.index(0).get_pid() << " for process " << processes.index(i).get_pid() << ")" << endl;
                    clock += 0.5;
                    process setContext = queue.index(0);
                    setContext.inc_num_context();
                    setContext.setContext_switch(true);
                    queue.edit(0, setContext);
                }
                queue.insert_end(processes.index(i));
                sort_priority(queue);

                //Arrival First!
                processes.delete_first();
                index--;
                i--;
            }
        }
        if (queue.size() != 0) {

            if (!queue.index(0).get_seen()) {

                if(queue.size() > 1 && queue.index(0).get_burst_left() == queue.index(1).get_burst_left()){
                    if(queue.index(0).get_arrival() > queue.index(1).get_arrival()){
                        process t1 = queue.index(0);
                        process t2 = queue.index(1);
                        queue.edit(0, t2);
                        queue.edit(1, t1);
                    }
                }
                if(!queue.index(0).isContext_switch()){
                    double arrival = clock - queue.index(0).get_arrival();
                    process edit = queue.index(0);
                    edit.seen(arrival);
                    queue.edit(0, edit);
                    cout << "[time " << clock << "ms] Process " << queue.index(0).get_pid() <<
                         " accessed CPU for the first time (initial wait time " << queue.index(0).get_initial_wait()
                         << "ms)" << endl;
                }
            }


            process t = queue.index(0);
            t.dec_burst();
            queue.edit(0, t);


            if (queue.index(0).get_burst_left() == 0) {

                double wait = ((clock + 1) - queue.index(0).get_burst() - queue.index(0).get_arrival());

                if(queue.index(0).isContext_switch()){
                    process initial = queue.index(0);
                    initial.setInitial_wait(wait);
                    queue.edit(0, initial);
                    wait = queue.index(0).get_arrival();
                }

                double finish = clock + 1;
                double turnAround = ((clock +1) - queue.index(0).get_arrival());
                double response = wait;

                process insert = queue.index(0);
                insert.setFinish_time(finish);
                insert.setResponse_time(response);
                insert.setTurnaround_time(turnAround);
                queue.edit(0, insert);

                cout << "[time " << clock + 1  << "ms] Process " << queue.index(0).get_pid() <<
                     " completed its CPU burst (turnaround time " << ((clock + 1) - queue.index(0).get_arrival())
                     << "ms, initial wait time "
                     << queue.index(0).get_initial_wait() << "ms, response time " << wait << "ms)" << endl;
                stats.insert_start(queue.index(0));
                last_pid = queue.index(0).get_pid();
                double turn = ((clock + 1) - queue.index(0).get_arrival());
                queue.delete_first();
                num_completed++;
            }
        }
        clock++;
    }

    statistics(stats, title);

}

int main()
{
    cpu_scheduling_menu();

	return 0;
}


void cpu_scheduling_menu(){

    bool menu = true;
    do {
        string input_file, output_file; int algorithm;
        cout << "-------CPU Scheduling Simulation-------\n\n";
        cout << "Main Menu: \n";
        cout << "Please enter your the name of the test file -> ";
        cin >> input_file;
        cout << "\nPlease enter the name of the output file -> ";
        cin >> output_file;
        cout << "\nAlgorithms:";
        cout << "\nFirst Come First Serve - 0";
        cout << "\nShortest Remaining Time First - 1";
        cout << "\nRound Robin - 2";
        cout << "\nPriority - 3";
        cout << "\n\nPlease enter the number of the algorithm to simulate, any other # will quit the program: ";
        cin >> algorithm;

        switch(algorithm){

            case 0: {
                first_come_first_serve(input_file, output_file);
                break;
            }
            case 1: {
                shortest_remaining_job_first(input_file, output_file);
                break;
            }
            case 2: {
                int time_slice;
                cout << "\n Enter the quantum time: ";
                cin >> time_slice;
                round_robin(input_file, output_file, time_slice);
                break;
            }
            case 3: {
                preemptive_priority(input_file, output_file);
                break;
            }
            default: menu = false;
        }
    } while (!menu);

}

list get_processes(string filename){

    ifstream fileIn;

    fileIn.open(filename);    //opening the file
    list processes;

    if (fileIn.fail()) {

        cout << "Error opening file.\n";

        return processes;   //exit if file not found

    }

    while (!fileIn.eof()) {

        int pid, burstTime, arrivalTime, priority;

        fileIn >> pid;

        fileIn >> arrivalTime;

        fileIn >> burstTime;

        fileIn >> priority;

        process obj(pid, arrivalTime, burstTime, priority);
        processes.insert_end(obj);
    }

    fileIn.close();

    return  processes;


}

void sort_burst(list processes){

    int index = processes.size();

    bool sorted = false, _switch = false;
    while(!sorted){
        for(int i = 1; i < index; i++){
            if(processes.index(i - 1).get_burst() >= processes.index(i).get_burst()){
                process temp = processes.index(i);
                process temp1 = processes.index(i-1);
                processes.edit(i - 1, temp);
                processes.edit(i, temp1);
                i--;
                _switch = true;
            }
        }
        if(!_switch){
            sorted = true;
        }
        _switch = false;
    }

}
void sort_priority(list processes){

    int index = processes.size();

    bool sorted = false, _switch = false;
    while(!sorted){
        for(int i = 1; i < index; i++){
            if(processes.index(i - 1).get_priority() >= processes.index(i).get_priority()){
                process temp = processes.index(i);
                process temp1 = processes.index(i-1);
                processes.edit(i - 1, temp);
                processes.edit(i, temp1);
                i--;
                _switch = true;
            }
        }
        if(!_switch){
            sorted = true;
        }
        _switch = false;
    }

}
void sort_pid(list processes){

    int index = processes.size();

    bool sorted = false, _switch = false;
    while(!sorted){
        for(int i = 1; i < index; i++){
            if(processes.index(i - 1).get_pid() >= processes.index(i).get_pid()){
                process temp = processes.index(i);
                process temp1 = processes.index(i-1);
                processes.edit(i - 1, temp);
                processes.edit(i, temp1);
                i--;
                _switch = true;
            }
        }
        if(!_switch){
            sorted = true;
        }
        _switch = false;
    }

}

template<typename T> void printElement(T t, const int& width)
{
    char separator    = ' ';
    cout << left << setw(width) << setfill(separator) << t << setw(2) << setfill(' ') << '|';
}

void statistics(list statistics, string title){

    int name_width = 15;
    double avg_wait = 0;
    double avg_turn = 0;
    double avg_cpu = 0;
    double avg_response = 0;
    double total_context = 0;

    sort_pid(statistics);
    cout << "-----------------------------------------------------------"
         << "-------------------------------------------------------" << endl;
    printElement("|PID" , 4);
    printElement("|Arrival" , 8);
    printElement("|CPU-Burst", 10);
    printElement("|Priority" , 9);
    printElement("|Finish Time" , 12);
    printElement("|Waiting Time", 13);
    printElement("|Turn Around", 12);
    printElement("|Response Time" , 14);
    printElement("|No. Of Context", name_width);
    cout << endl;
    for(int i = 0; i < statistics.size(); i++){

        avg_cpu += statistics.index(i).get_burst();
        avg_wait += statistics.index(i).getInitial_wait();
        avg_turn += statistics.index(i).getTurnaround_time();
        avg_response += statistics.index(i).getResponse_time();
        total_context += statistics.index(i).get_num_context();

        printElement(statistics.index(i).get_pid(), 4);
        printElement(statistics.index(i).get_arrival(), 8);
        printElement(statistics.index(i).get_burst(), 10);
        printElement(statistics.index(i).get_priority(), 9);
        printElement(statistics.index(i).getFinish_time(), 12);
        printElement(statistics.index(i).getInitial_wait(), 13);
        printElement(statistics.index(i).getTurnaround_time(), 12);
        printElement(statistics.index(i).getResponse_time(), 14);
        printElement(statistics.index(i).get_num_context(), 15);
        cout << endl;
    }
    cout << "-----------------------------------------------------------"
         << "-------------------------------------------------------" << endl;

    avg_wait /= statistics.size();
    avg_turn /= statistics.size();
    avg_cpu  /= statistics.size();
    avg_response /= statistics.size();

    printElement("|Average Wait time: ", 30);
    printElement(avg_wait, 5);
    cout << endl;
    printElement("|Average Turn Around time: ", 30);
    printElement(avg_turn, 5);
    cout << endl;
    printElement("|Average Response time: ", 30);
    printElement(avg_response, 5);
    cout << endl;
    printElement("|Average CPU time: ", 30);
    printElement(avg_cpu, 5);
    cout << endl;
    printElement("|Total Context Switches: ", 30);
    printElement(total_context, 5);

}