#include <iostream>
#include "process.h"
#include "container.h"

using namespace std;

void sort_burst(list processes);

void first_come_first_serve(list processes){

    int num_completed = 0;
    int clock = 0;
    int last_pid = 0;
    list queue;
    bool c_switch = false;
    int index = processes.size();

    double max_initial = 0;
    double total_initial = 0;
    double min_initial = 0;
    int wait_time = 0;
    int num_processes = processes.size();
    while(num_completed != num_processes){

        index = processes.size();
        for(int i = 0; i < index; i++){

            if(processes.index(i).get_arrival() <= clock){
                std::cout << "[time " << processes.index(i).get_arrival() << "ms] PID - " << processes.index(i).get_pid() <<
                          " created (requires " << processes.index(i).get_burst() << "ms CPU time)";
                std::cout << std::endl;
                queue.insert_end(processes.index(i));
                processes.delete_first();
                index--;
                i--;
            }
        }
        if(queue.size() != 0){

            if(!queue.index(0).get_seen()){

                int arrival = clock - queue.index(0).get_arrival();
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

                int wait = ((clock +1 ) - queue.index(0).get_burst() - queue.index(0).get_arrival());
                cout << "[time " << clock + 1 << "ms] Process " << queue.index(0).get_pid() <<
                     " completed its CPU burst (turnaround time " << ((clock +1) - queue.index(0).get_arrival()) << "ms, initial wait time "
                     << queue.index(0).get_initial_wait() << "ms, response time " << wait << "ms)" << endl;
                queue.delete_first();
                num_completed++;
            }

        }
        clock++;
    }
}

void shortest_remaining_job_first(list processes) {

    int num_completed = 0;
    double clock = 0;
    double total_wait;
    int last_pid = 0;
    list queue;
    list sorted_queue;
    list temp;

    bool cswitch = false;

    int index = processes.size();
    int num_processes = processes.size();

    while (num_completed != num_processes) {
        index = processes.size();
        for(int i = 0; i < index; i++){

            if(processes.index(i).get_arrival() <= clock){
                std::cout << "[time " << clock  << "ms] PID - " << processes.index(i).get_pid() <<
                          " created (requires " << processes.index(i).get_burst() << "ms CPU time)";
                std::cout << std::endl;
                if(queue.size() !=0 &&
                        processes.index(i).get_burst() < queue.index(0).get_burst() &&
                        queue.index(0).get_burst_left() != queue.index(0).get_burst()){

                    //Context Switch on preemption
                    cout << "[time " << clock<< "ms] Context switch (swapped out process "
                         << queue.index(0).get_pid() << " for process " << processes.index(i).get_pid() << ")" << endl;
                    clock += 0.5;
                    process setContext = queue.index(0);
                    setContext.setContext_switch(true);
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
                cout << "[time " << clock + 1  << "ms] Process " << queue.index(0).get_pid() <<
                     " completed its CPU burst (turnaround time " << ((clock + 1) - queue.index(0).get_arrival())
                     << "ms, initial wait time "
                     << queue.index(0).get_initial_wait() << "ms, response time " << wait << "ms)" << endl;

                last_pid = queue.index(0).get_pid();
                double turn = ((clock + 1) - queue.index(0).get_arrival());
                queue.delete_first();
                num_completed++;
            }
        }
        clock++;
    }
}

int main()
{
	list obj;
	process p(1,0,10,1);
    process p1(2,1,2,5);
    process p2(3,2,9,0);
    process p3(4,3,5,10);
    obj.insert_end(p);
    obj.insert_end(p1);
    obj.insert_end(p2);
    obj.insert_end(p3);

    cout << "----------First come first Serve-----------" << endl;
    cout << "-------------------------------------------" << endl;
    first_come_first_serve(obj);
    cout << endl;
    cout << "----------Shortest job first---------------" << endl;
    cout << "-------------------------------------------" << endl;
    list obj2;
    obj2.insert_end(p);
    obj2.insert_end(p1);
    obj2.insert_end(p2);
    obj2.insert_end(p3);
    shortest_remaining_job_first(obj2);

	return 0;
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
        if(_switch != true){
            sorted = true;
        }
        _switch = false;
    }

}