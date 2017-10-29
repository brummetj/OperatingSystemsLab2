//
// Created by jbrummet on 10/22/17.
//
#include "process.h"

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
    num_context_switches = 0;
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

double process::getResponse_time() const {
    return response_time;
}

void process::setResponse_time(double response_time) {
    process::response_time = response_time;
}

double process::getTurnaround_time() const {
    return turnaround_time;
}

void process::setTurnaround_time(double turnaround_time) {
    process::turnaround_time = turnaround_time;
}
