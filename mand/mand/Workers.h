#pragma once

#include <atomic>
#include <condition_variable>
#include <thread>
#include <queue>
#include <array>
#include <functional>

class Workers
{
public:
	Workers(int n);
	~Workers();

	void AddJob(std::function<void()> job);
	void Start();
	void WaitForEnd();
	void End();
private:
	void work_();

	std::atomic<int> i_workers_{ 0 };
	std::thread* th_workers_;

	std::condition_variable cv_job_;
	std::condition_variable cv_done_;

	std::mutex mut_jobs_;
	std::queue<std::function<void()>> q_jobs_;
	std::atomic<int> i_jobs_awaiting_{ 0 };
	std::atomic<int> i_jobs_unfinished_{ 0 };

	std::atomic<bool> b_end_work_{ false };
};
