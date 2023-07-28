#pragma once
class AppObject
{
public:
	AppObject() {};
	~AppObject() { EndApp(); };

	void Init();
	void OnDraw();

	bool IsAppRuning()
	{
		return _IsAppRuning;
	}
	void EndApp()
	{
		if (_IsAppRuning)
		{
			OnAppEnd();
		}
	}
private:
	void OnAppEnd();
	bool _IsAppRuning = false;
};

