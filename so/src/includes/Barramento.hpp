#ifndef BARRAMENTO_HPP
#define BARRAMENTO_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <algorithm> 

using namespace std;

class Barramento {

public:
    Barramento(int num_threads) : thread_completed(num_threads, false) {}

    mutex mtx;
    condition_variable cv;
    vector<bool> thread_completed;

    void mark_thread_completed(int thread_id);
    bool all_threads_completed() const;
};

#endif