#ifndef TERRARIUM_SEMAPHORE_H
#define TERRARIUM_SEMAPHORE_H

#include <atomic>
#include <mutex>
#include <condition_variable>

// Kernel Semaphore
struct semaphore {
	std::mutex				lock;
	std::condition_variable	mWaitVariable;
	std::atomic<int>		mCount { 0 };
};

Semaphore();
~Semaphore();

void					Release(uint inNumber = 1);

void					Acquire(uint inNumber = 1);

inline int				GetValue() const								{ return mCount.load(std::memory_order_relaxed); }

#endif // TERRARIUM_SEMAPHORE_H