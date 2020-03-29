#include "Workers.h"

Workers::Workers(int n) {
	th_workers_ = new std::thread[n];
	for (int i(n - 1); i >= 0; i--)
		th_workers_[i] = std::thread([this]() { this->work_(); });

	while (i_workers_ != n)
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

Workers::~Workers() {
	delete[] th_workers_;
}

void Workers::AddJob(std::function<void()> job) {
	std::lock_guard<std::mutex> gd_lock(mut_jobs_);
	q_jobs_.push(job);
	i_jobs_awaiting_++;
	i_jobs_unfinished_++;
}

void Workers::Start() {
	cv_job_.notify_all();
}

void Workers::WaitForEnd() {
	std::mutex mut;
	std::unique_lock<std::mutex> uq_lock(mut);
	cv_done_.wait(uq_lock, [this] {return i_jobs_unfinished_ == 0; });
}

void Workers::End() {
	b_end_work_ = true;
	cv_job_.notify_all();
	for (int i(i_workers_ - 1); i >= 0; i--)
		th_workers_[i].join();
}

void Workers::work_() {
	std::mutex mut;
	std::unique_lock<std::mutex> uq_lock(mut);

	std::function<void()> job = nullptr;

	i_workers_++;
	while (true) {
		while (true) {
			{
				std::lock_guard<std::mutex> gd_lock(mut_jobs_);

				if (q_jobs_.empty())
					break;

				job = q_jobs_.front();
				q_jobs_.pop();
				i_jobs_awaiting_--;
			}
			job();
			i_jobs_unfinished_--;

			job = nullptr;

			if (i_jobs_unfinished_ == 0) {
				cv_done_.notify_one();
			}
		}
		cv_job_.wait(uq_lock, [=] {return ((i_jobs_awaiting_ != 0) || b_end_work_); });
		if (b_end_work_)
			break;
	}
}