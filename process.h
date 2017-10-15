//Creates Process class 
class process {
private:
	int arrival_time;
	int burst_time;
	int premp_time;
	int burst_left;
	bool been_seen;
	int initial_wait;
	int total_wait;
	int priority;
	int pid;
public:
	process(int burst, int time, int id, int importance);
	void set_arrival(int time);
	void set_pid(int num);
	int get_arrival();
	int get_burst();
	int get_priority();
	int get_burst_left();
	void dec_burst();
    int get_initial_wait();
	int get_total_wait();
	void set_premp_time(int time);
	void add_wait(int time);
	bool get_seen();
	void seen(int time);
	int get_pid();
};

process::process(int burst, int time, int id, int importance)
{
	pid = id;
	burst_time = burst;
	burst_left = burst;
	arrival_time = time;
	been_seen = false;
	priority = importance;
	initial_wait = 0;
	total_wait = 0;
}

void process::set_pid(int num)
{
	pid = num;
}

bool process::get_seen()
{
	return been_seen;
}

int process::get_priority()
{
	return priority;
}

int process::get_burst()
{
	return burst_time;
}

int process::get_pid()
{
	return pid;
}

void process::set_arrival(int time)
{
	arrival_time = time;
}

int process::get_arrival()
{
	return arrival_time;
}

int process::get_burst_left()
{
	return burst_left;
}

void process::dec_burst()
{
	burst_left = burst_left - 1;
}

int process::get_initial_wait()
{
	return initial_wait;
}

int process::get_total_wait()
{
	return total_wait;
}

void process::set_premp_time(int time)
{
	premp_time = time;
}

void process::add_wait(int time)
{
	total_wait = total_wait + time;
}

void process::seen(int time)
{
	been_seen = true;
	initial_wait = time;
	total_wait = time;
}