//Creates Process class
//
// Created by jbrummet on 10/22/17.
//
class process {

    private:
        double arrival_time;
        int burst_time;
        int premp_time;
        int burst_left;
        bool been_seen;
        double initial_wait;
        double total_wait;
        int priority;
        int pid;
        double finish_time;
        bool context_switch;
        double response_time;
        double turnaround_time;
        int num_context_switches;

    public:

        process() = default;;

        process(int _id, int time, int burst, int _priority);

        void set_arrival(double time);

        double get_arrival();

        void set_pid(int num);

        int get_pid();

        int get_burst();

        void dec_burst();

        int get_priority();

        int get_burst_left();

        double get_initial_wait();

        double get_total_wait();

        void set_premp_time(int time);

        void add_wait(double time);

        bool get_seen();

        void seen(double time);

        void ready(){ been_seen = true; }

        double getInitial_wait() const;

        void setInitial_wait(double initial_wait);

        bool isContext_switch() const;

        void setContext_switch(bool context_switch);

        double getFinish_time() const;

        void setFinish_time(double finish_time);

        double getResponse_time() const;

        void setResponse_time(double response_time);

        double getTurnaround_time() const;

        void setTurnaround_time(double turnaround_time);

        void inc_num_context(){
            num_context_switches++;
        }

        int get_num_context(){
            return num_context_switches;
        }
};


