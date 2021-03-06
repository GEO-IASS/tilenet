#include "includes.hpp"
#include "utils/thread_pool.hpp"



using boost::asio::io_service;

ThreadPool::ThreadPool(io_service& ioService)
	: mTargetThreadCount(0)
	, mDestroy(false)
	, mService(ioService)
	, log(L"ThreadPool")
{
}

ThreadPool::~ThreadPool()
{
	{
		// Hmmm, a lock will cause an exception on exit cleanup
		// std::lock_guard<std::mutex> lock(mMutex);
		mDestroy = true;
		mWork.reset();
		mService.stop();
	}

	for(auto& p : mThreads)
	{
		tnAssert(p.second.joinable());
		p.second.join();
	}
}

void ThreadPool::setThreadCount(size_t count)
{
	std::lock_guard<std::mutex> lock(mMutex);
	if(mTargetThreadCount == count)
		return;

	bool addThreads = mTargetThreadCount < count;

	mTargetThreadCount = count;

	if(addThreads)
	{
		if(!mWork)
			mWork.reset(new io_service::work(mService));

		while(mThreads.size() < count)
		{
			std::thread thread(std::bind(&ThreadPool::runThread, this));
			mThreads.insert(std::make_pair(thread.get_id(), std::move(thread)));
		}
	}else{
		mWork.reset();
	}
	log.info() << L"thread count is now " << mThreads.size();
}


void ThreadPool::runThread()
{
	try {
		bool need = true;
		while(need)
		{
			mService.reset();
			mService.run();

			std::lock_guard<std::mutex> lock(mMutex);

			if(mDestroy)
				return;

			if(mThreads.size() > mTargetThreadCount)
			{
				need = false;
				auto it = mThreads.find(std::this_thread::get_id());
				tnAssert(it != mThreads.end());
				it->second.detach();
				mThreads.erase(it);
			}else{
				if(!mWork)
				{
					mWork.reset(new io_service::work(mService));
				}
			}
		}
	}catch(...)
	{
		log.error()
			<< L"Exception in worker thread!"
			<< L"\n------------------------\n"
			<< lexical_convert<string>(boost::current_exception_diagnostic_information())
			<< L"\n------------------------";

#ifdef TILENET_RETHROW_THREAD_EXCEPTIONS
		throw;
#endif
	}
}
