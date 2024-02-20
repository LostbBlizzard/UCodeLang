#pragma once
#include "../UCodeLangNameSpace.hpp"
#include <functional>
#include <future>
UCodeLangStart
template<typename> struct PackagedTask;



//Just Packaged Task but does not store exceptions so I can Debug the exceptions
template<class T, class... Args>
class PackagedTask<T(Args...)>
{
public:
	using func = std::function< T(Args...)>;
	using future = std::future<T>;
	using promise = std::promise<T>;
	PackagedTask()
	{
		_promise = std::make_shared<promise>();
	}
	PackagedTask(func&& call):base(std::move(call))
	{
		_promise = std::make_shared<promise>();
	}
	PackagedTask(const func& call) :base(call)
	{
		_promise = std::make_shared<promise>();
	}
	~PackagedTask()
	{

	}
	void operator()(Args... args)
	{
		_promise->set_value(base(args...));
	}
	future get_future()
	{
		return _promise->get_future();
	}
private:
	std::function< T(Args...)> base;
	std::shared_ptr<promise> _promise;
};
UCodeLangEnd
