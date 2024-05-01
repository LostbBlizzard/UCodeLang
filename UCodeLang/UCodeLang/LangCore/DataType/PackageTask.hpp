#pragma once
#include "../UCodeLangNameSpace.hpp"
#include <functional>
#include <future>
UCodeLangStart
template<typename> struct Packaged_Task;



//Just Packaged Task but does not store exceptions so I can Debug the exceptions
template<class T, class... Args>
class Packaged_Task<T(Args...)>
{
public:
	using func = std::function< T(Args...)>;
	using future = std::future<T>;
	using promise = std::promise<T>;
	Packaged_Task()
	{
		_promise = std::make_shared<promise>();
	}
	Packaged_Task(func&& call):base(std::move(call))
	{
		_promise = std::make_shared<promise>();
	}
	Packaged_Task(const func& call) :base(call)
	{
		_promise = std::make_shared<promise>();
	}
	~Packaged_Task()
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
