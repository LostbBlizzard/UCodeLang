#pragma once
#include "LangTypes.hpp"

#include <thread>
#include <functional>
#include "DataType/UnorderedMap.hpp"
#include "DataType/PackageTask.hpp"
#include <mutex>
UCodeLangStart

using ThreadID = int;
static constexpr ThreadID RunOnAnyThread = 0;

struct TaskManger;

ThreadID GetSyncThreadID(TaskManger* manger);

template<typename T>
struct WeakMutex
{
	WeakMutex(T&& item, ThreadID vaildthread, TaskManger* manger)
		:_Item(item), vaildthread(vaildthread), manger(manger)
	{}

	WeakMutex(T&& item, TaskManger* manger)
		:_Item(item), vaildthread(GetSyncThreadID(manger)), manger(manger)
	{}

	WeakMutex(TaskManger* manger, ThreadID vaildthread)
		:vaildthread(vaildthread), manger(manger)
	{}

	WeakMutex(TaskManger* manger)
		:_Item(), vaildthread(GetSyncThreadID(manger)), manger(manger)
	{}

	template<typename R>
	UCodeLangForceinline R Lock_r(std::function<R(T& val)> call)
	{
		auto r = call(_Item);
		return r;
	}

	template<typename R>
	UCodeLangForceinline R Lock_r(std::function<R(const T& val)> call) const
	{
		auto r = call(_Item);
		return r;
	}

	UCodeLangForceinline void Lock(std::function<void(T& val)> call)
	{
		call(_Item);
	}

	UCodeLangForceinline void Lock(std::function<void(const T& val)> call) const
	{
		call(_Item);
	}


	~WeakMutex()
	{

	}
private:
	T _Item;
	ThreadID vaildthread;
	TaskManger* manger;
};

template<typename T>
struct Mutex
{
public:
	Mutex()
	{

	}
	Mutex(T item)
		:_Item(item)
	{

	}
	Mutex(const Mutex<T>& tocopy) = delete;
	Mutex& operator=(const Mutex<T>& tocopy) = delete;

	Mutex& operator=(Mutex<T>&& tocopy) = default;
	Mutex(Mutex<T>&& tocopy) = default;



	template<typename R>
	UCodeLangForceinline R Lock_r(std::function<R(T& val)> call)
	{
		_mutex.lock();
		UCodeLangDefer(_mutex.unlock());
		return call(_Item);
	}

	template<typename R>
	UCodeLangForceinline R Lock_r(std::function<R(const T& val)> call)
	{
		_mutex.lock();
		UCodeLangDefer(_mutex.unlock());
		return call(_Item);
	}

	UCodeLangForceinline void Lock(std::function<void(T& val)> call)
	{
		_mutex.lock();
		UCodeLangDefer(_mutex.unlock());
		call(_Item);
	}

	UCodeLangForceinline void Lock(std::function<void(const T& val)> call)
	{
		_mutex.lock();
		UCodeLangDefer(_mutex.unlock());
		call(_Item);
	}

	UCodeLangForceinline bool TryLock(std::function<void(T& val)> call)
	{
		if (_mutex.try_lock()) {
			UCodeLangDefer(_mutex.unlock());
			call(_Item);
			return true;
		}
		return false;
	}
	UCodeLangForceinline bool TryLock(std::function<void(const T& val)> call)
	{
		if (_mutex.try_lock()) {
			UCodeLangDefer(_mutex.unlock());
			call(_Item);
			return true;
		}
		return false;
	}

	template<typename R>
	UCodeLangForceinline Optional<R> TryLock_r(std::function<R(T& val)> call)
	{
		if (_mutex.try_lock())
		{
			UCodeLangDefer(_mutex.unlock());
			return call(_Item);
		}
		return Optional<R>();
	}

	template<typename R>
	UCodeLangForceinline Optional<R> TryLock_r(std::function<R(const T& val)> call)
	{
		if (_mutex.try_lock()) {
			UCodeLangDefer(_mutex.unlock());
			return call(_Item);
		}
		return Optional<R>();
	}
	std::mutex& GetMutex()
	{
		return _mutex;
	}
private:
	T _Item;
	std::mutex _mutex;
};


struct TaskManger
{
public:
	using TaskID = uintptr_t;
private:
	Vector<std::thread> threads;

	struct WorkerData
	{
		Vector<TaskID> TaskToID;
		Unique_ptr<std::condition_variable> UpdateWorker;
		WorkerData()
		{
			UpdateWorker = std::make_unique<std::condition_variable>();
		}
		WorkerData(const WorkerData&) = delete;
		WorkerData& operator=(const WorkerData&) = delete;

		WorkerData(WorkerData&&) noexcept = default;
		WorkerData& operator=(WorkerData&&) noexcept = default;
	};
	struct WorkerGroup
	{
		size_t NextWorker = 0;
		Vector<WorkerData> WorkerDatas;
	};
	Mutex<WorkerGroup> WorkerDatas;


	template<typename T>
	using OnDoneFunc = std::function<void(T&& val)>;

	struct RuningTaskInfo
	{
		Vector<TaskID> dependencies;

		struct AnyOnDone
		{

			virtual ~AnyOnDone()
			{

			}
		};

		template<typename T>
		struct AnyOnDone_t : AnyOnDone
		{
			OnDoneFunc<T> func;
			AnyOnDone_t(OnDoneFunc<T> value)
				:func(std::move(value))
			{

			}
		};
		struct AnyTaskReturn
		{

			virtual ~AnyTaskReturn()
			{

			}
		};

		template<typename T>
		struct AnyTaskReturn_t : AnyTaskReturn
		{
			T val;
			AnyTaskReturn_t(T&& value)
				:val(std::move(value))
			{

			}
		};
		Unique_ptr<AnyOnDone> OnDone;
		std::shared_ptr<std::function<void()>> _Func;
		Unique_ptr<AnyTaskReturn> _TaskReturn;
		bool TaskCanRun = false;
		bool Taskcompleted = false;
		size_t refscount = 0;

		template<typename T>
		void SetOnDone(OnDoneFunc<T> done)
		{
			OnDone = Unique_ptr<AnyOnDone>((AnyOnDone*)new AnyOnDone_t<T>(done));
		}

		template<typename T>
		void CallOnDone(T&& val)
		{
			UCodeLangAssert(OnDone.get());
			AnyOnDone_t<T>* done = (AnyOnDone_t<T>*)OnDone.get();
			return done->func(val);
		}

		template<typename T>
		void SetReturn(T&& val)
		{
			_TaskReturn = Unique_ptr<AnyTaskReturn>((AnyTaskReturn*)new AnyTaskReturn_t<T>(std::move(val)));
		}

		bool HasTaskReturn()
		{
			return _TaskReturn.get();
		}
		bool TaskCompleted()
		{
			return Taskcompleted;
		}

		template<typename T>
		T GetReturn()
		{
			UCodeLangAssert(_TaskReturn.get())
				T r = std::move(((AnyTaskReturn_t<T>*)_TaskReturn.get())->val);
			_TaskReturn = {};
			return r;
		}

		template<typename T>
		Optional<T> TryGetReturn()
		{
			if (HasTaskReturn())
			{
				return GetReturn<T>();
			}
			else
			{
				return {};
			}
		}

	};

	Mutex<UnorderedMap<TaskID, Shared_ptr<RuningTaskInfo>>> tasks;
	static constexpr TaskID NullTaskID = 0;
public:
	struct Void {};

	template<typename T>
	struct Task
	{
		friend TaskManger;
	public:
		using OnDoneFunc = std::function<void(T&& val)>;
		TaskID GetID() const
		{
			return _id;
		}
		Task& OnDone(OnDoneFunc done)
		{
			_manger->OnDone(_id, done);
			return *this;
		}

		Task() :_id(NullTaskID), _manger(nullptr) {}
		Task(const Task& tocopy) = delete;
		Task& operator=(const Task& tocopy) = delete;
		~Task()
		{
			if (_id != NullTaskID)
			{
				_manger->tasks.Lock([_id = this->_id](UnorderedMap<TaskID, Shared_ptr<RuningTaskInfo>>& val)
				{
					auto& taskinfo = val.GetValue(_id);
					taskinfo->refscount--;
					if (taskinfo->refscount == 0)
					{
						val.erase(_id);
					}
				});
			}
		}
		Task(Task&& tocopy)
			:_id(tocopy._id), _manger(tocopy._manger)
		{
			tocopy._id = NullTaskID;
		}
		Task& operator=(Task&& tocopy)
		{
			_id = tocopy._id;
			_manger = tocopy._manger;

			tocopy._id = NullTaskID;
		}


		auto Wait()
		{
			return _manger->WaitFor(*this);
		}

		void Run()
		{
			return _manger->Run(*this);
		}

	private:
		Task(TaskID id, TaskManger* manger) :_id(id), _manger(manger) {}

		TaskID _id;
		TaskManger* _manger;
	};
	using VoidTask = Task<Void>;
	TaskManger()
	{

	}
	TaskManger& operator=(TaskManger&& tocopy) = default;
	TaskManger(TaskManger&& tocopy) = default;

	TaskManger(const TaskManger& tocopy) = delete;
	TaskManger& operator=(const TaskManger& tocopy) = delete;

	static void Threads(size_t Count, TaskManger& r)
	{
		r.threads.reserve(Count);

		for (size_t i = 0; i < Count; i++)
		{
			r.threads.push_back(std::thread(ThreadStart, &r, i + 1));
		}
	}
	static void Threads(TaskManger& r)
	{
		return Threads(std::thread::hardware_concurrency(), r);
	}

	template<typename T, typename... Pars>
	Task<T> AddTask(std::function<T(Pars...)> funcion, Vector<TaskID> dependencies, Pars... pars)
	{
		return AddTask(funcion, RunOnAnyThread, dependencies, pars...);
	}

	template<typename... Pars>
	Task<Void> AddTask(std::function<void(Pars...)> funcion, Vector<TaskID> dependencies, Pars... pars)
	{
		std::function<Void(Pars...)> func = [f = std::move(funcion)](Pars... p)
			{
				f(p...);
				return Void();
			};
		return AddTask(func, dependencies, pars...);
	}


	template<typename T, typename... Pars>
	Task<T> AddTask(std::function<T(Pars...)> funcion, ThreadID threadtorunon, Vector<TaskID> dependencies, Pars... pars)
	{
		auto task_promise = std::make_shared<Packaged_Task<T(Pars...)>>(std::move(funcion));
		auto newid = NewTaskID();

		RuningTaskInfo info;
		info.dependencies = std::move(dependencies);

		std::function<void()> func = [this, newid, task_promise, pars = std::make_tuple(std::forward<Pars>(pars)...)]()
			{
				std::apply([task_promise](auto&& ... args) {
					(*task_promise)(args...);
				}, std::move(pars));

				T r = (*task_promise).get_future().get();
				tasks.Lock([newid, r = std::move(r)](UnorderedMap<TaskID, std::shared_ptr<RuningTaskInfo>>& val)
				{
					auto& runtaskinfo = val.GetValue(newid);
					runtaskinfo->SetReturn(std::move(r));

					runtaskinfo->Taskcompleted = true;
					runtaskinfo->refscount--;
					if (runtaskinfo->refscount == 0)
					{
						val.erase(newid);
					}
				});
			};

		info._Func = std::make_shared<std::function<void()>>(std::move(func));
		info.refscount = 2;

		tasks.Lock([newid, info = std::make_shared<RuningTaskInfo>(std::move(info))](UnorderedMap<TaskID, std::shared_ptr<RuningTaskInfo>>& val)
		{
			val.AddValue(newid, std::move(info));
		});

		auto func2 = [this, newid](WorkerGroup& val)
			{
				auto worker = val.NextWorker;
				val.NextWorker++;
				if (val.NextWorker >= WorkerCount())
				{
					val.NextWorker = 0;
				}


				auto& workerdata = val.WorkerDatas[worker];

				workerdata.TaskToID.push_back(newid);
				workerdata.UpdateWorker->notify_one();
			};
		WorkerDatas.Lock(func2);

		return Task<T>(newid, this);
	}

	template<typename... Pars>
	Task<Void> AddTask(std::function<Void(Pars...)> funcion, ThreadID threadtorunon, Vector<TaskID> dependencies, Pars... pars)
	{
		std::function<Void(Pars...)> func = [f = std::move(funcion)](Pars... p)
			{
				f(p...);
				return Void();
			};
		return AddTask(func, threadtorunon, dependencies, pars...);
	}
	ThreadID GetSyncThreadID()
	{
		return 0;
	}
	size_t WorkerCount()
	{
		return threads.size() + 1;// and this thread
	}

	template<typename T>
	T WaitFor(Task<T>& item)
	{

		auto id = item.GetID();
		UCodeLangAssert(id != NullTaskID)

			Optional<T> r;

		while (r.has_value() == false)
		{
			auto func = [&r, this, id](UnorderedMap<TaskID, Shared_ptr<RuningTaskInfo>>& val)
				{
					auto& runingtask = *val.GetValue(id);

					runingtask.TaskCanRun = true;
					r = runingtask.template TryGetReturn<T>();
				};

			tasks.Lock(func);


			if (!r.has_value())
			{
				//do Work for a bit

				auto threadidx = ThreadIndex();

				std::function<WorkerData* (WorkerGroup&)> func2 = [threadidx, this](WorkerGroup& val) -> WorkerData*
					{
						return &val.WorkerDatas[threadidx];
					};

				auto& WorkData = *(WorkerDatas.Lock_r<WorkerData*>(func2));


				auto v = ThreadTick(this, WorkData);
				if (!v.has_value())
				{
					continue;
				}
				else
				{
					v.value()();
				}
			}

		}
		return r.value();
	}

	template<typename T>
	void Run(Task<T>& item)
	{
		auto id = item.GetID();

		auto func = [this, id](UnorderedMap<TaskID, Shared_ptr<RuningTaskInfo>>& val)
			{
				auto& runingtask = *val.GetValue(id);
				runingtask.TaskCanRun = true;
			};

		tasks.Lock(func);
	}

	template<typename T>
	WeakMutex<T> NewWeakMutex()
	{
		WeakMutex<T> r(this);

		return r;
	}
	template<typename T>
	WeakMutex<T> NewWeakMutex(T&& Val)
	{
		WeakMutex<T> r(this, Val);

		return r;
	}
	void Init()
	{
		auto func = [this](WorkerGroup& val)
			{
				val.WorkerDatas.resize(1 + threads.size());
				canstart = true;
			};
		WorkerDatas.Lock(func);
	}
private:
	Mutex<TaskID> _NextID = NullTaskID;
	Mutex<size_t> NextThreadTask = 0;
	bool canstart = false;

	TaskID NewTaskID()
	{
		return _NextID.Lock_r<TaskID>([](TaskID& id)
		{
			id++;
			if (id == NullTaskID)
			{
				id++;
			}
			return id;
		});

	}

	template<typename T>
	void OnDone(TaskID id, OnDoneFunc<T> done)
	{
		tasks.Lock([id, done](UnorderedMap<TaskID, RuningTaskInfo>& val)
		{
			auto& taskinfo = val.GetValue(id);
			taskinfo.template SetOnDone<T>(done);
		});
	}

	static void ThreadStart(TaskManger* This, size_t I)
	{
		ThreadInd = I;
		while (This->canstart == false);

		std::function<WorkerData& (WorkerGroup&)> func = [](WorkerGroup& val) ->WorkerData&
			{
				return val.WorkerDatas[ThreadInd];
			};
		auto& WorkData = This->WorkerDatas.Lock_r(func);

		while (true)
		{
			std::function<void()> func;
			{
				std::unique_lock<std::mutex> lock(This->WorkerDatas.GetMutex());

				WorkData.UpdateWorker->wait(lock, [&WorkData, &v = This->canstart]()
					{
						return WorkData.TaskToID.size() || v == false;
					});

				if (This->canstart == false) { break; }

				auto v = ThreadTick(This, WorkData);
				if (!v.has_value())
				{
					continue;
				}
				else
				{
					func = std::move(v.value());
				}
			}

			func();
		}
	}
	static Optional<std::function<void()>> ThreadTick(TaskManger* This, WorkerData& WorkData)
	{
		auto& tasklist = WorkData.TaskToID;
		Optional<std::function<void()>> r;
		auto func = [tasklist, &r, This](UnorderedMap<TaskID, Shared_ptr<RuningTaskInfo>>& val) mutable
			{
				for (size_t i = 0; i < tasklist.size(); i++)
				{
					auto& Item = tasklist[i];

					if (!val.HasValue(Item)) { continue; }

					auto& tinfo = *val.GetValue(Item);
					if (tinfo.TaskCanRun == false)
					{
						continue;
					}

					bool randependencies = true;

					for (auto& dep : tinfo.dependencies)
					{
						if (val.HasValue(dep))
						{
							auto& depinfo = *val.GetValue(dep);
							if (!depinfo.TaskCompleted())
							{
								randependencies = false;
								break;
							}
						}
					}

					if (randependencies == false) { continue; }

					tasklist.erase(tasklist.begin() + i);
					r = std::move(*tinfo._Func);
					break;
				}
			};

		This->tasks.TryLock(func);

		return r;
	}

	inline static thread_local size_t ThreadInd = 0;
	static size_t ThreadIndex()
	{
		return ThreadInd;
	}
};
inline ThreadID GetSyncThreadID(TaskManger* manger)
{
	return manger->GetSyncThreadID();
}


UCodeLangEnd