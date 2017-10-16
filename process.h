//Creates Process class 
class process {
private:
    double arrival_time;
    int burst_time;
    int premp_time;
    int burst_left;
    bool been_seen;
    double initial_wait;
public:
    double getInitial_wait() const;

    void setInitial_wait(double initial_wait);

private:
    double total_wait;
    int priority;
    int pid;
    double finish_time;
    bool context_switch;
public:
    bool isContext_switch() const;

    void setContext_switch(bool context_switch);

public:
    double getFinish_time() const;

    void setFinish_time(double finish_time);

public:

    process() = default;;

    process(int _id, int time, int burst, int _priority);

    void set_arrival(double time);

    void set_pid(int num);

    double get_arrival();

    int get_burst();

    int get_priority();

    int get_burst_left();

    void dec_burst();

    double get_initial_wait();

    double get_total_wait();

    void set_premp_time(int time);

    void add_wait(double time);

    bool get_seen();

    void ready(){
        been_seen = true;
    }

    void seen(double time);

    int get_pid();
};

process::process(int _id, int time, int burst, int _priority) {
    pid = _id;
    burst_time = burst;
    burst_left = burst;
    arrival_time = time;
    been_seen = false;
    priority = _priority;
    initial_wait = 0;
    total_wait = 0;
    context_switch = false;
}

void process::set_pid(int num) {
    pid = num;
}

bool process::get_seen() {
    return been_seen;
}

int process::get_priority() {
    return priority;
}

int process::get_burst() {
    return burst_time;
}

int process::get_pid() {
    return pid;
}

void process::set_arrival(double time) {
    arrival_time = time;
}

double process::get_arrival() {
    return arrival_time;
}

int process::get_burst_left() {
    return burst_left;
}

void process::dec_burst() {
    burst_left = burst_left - 1;
}

double process::get_initial_wait() {
    return initial_wait;
}

double process::get_total_wait() {
    return total_wait;
}

void process::set_premp_time(int time) {
    premp_time = time;
}

void process::add_wait(double time) {
    initial_wait += time;
    total_wait += time;
}

void process::seen(double time) {
    been_seen = true;
    initial_wait = time;
    total_wait = time;
}

double process::getFinish_time() const {
    return finish_time;
}

void process::setFinish_time(double finish_time) {
    process::finish_time = finish_time;
}

bool process::isContext_switch() const {
    return context_switch;
}

void process::setContext_switch(bool context_switch) {
    process::context_switch = context_switch;
}

double process::getInitial_wait() const {
    return initial_wait;
}

void process::setInitial_wait(double initial_wait) {
    process::initial_wait = initial_wait;
}
