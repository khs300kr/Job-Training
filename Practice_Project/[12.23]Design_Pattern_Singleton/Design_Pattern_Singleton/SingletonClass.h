class Singleton
{
private:
	Singleton() {}
	static Singleton* instance; // Holds an instance of the class

public:
	static Singleton* getInstance(); // Ensures that it is the only one
									 // public grants access to the instance from anywhere
};


class FileSystem
{
private:
	FileSystem() {}
	static FileSystem* instance_;

public:
	static FileSystem& instance()
	{
		// Lazy initialize.
		if (instance_ == nullptr) instance_ = new FileSystem();
		return *instance_;
	}

};

// C++11 은 이니셜라이저에게 local static 변수가 한번 동작 하게한다.
// This code is Thread-Safe!

class FileSystem
{
private:
	FileSystem() {}
public:
	static FileSystem& instance()
	{
		static FileSystem *instance = new FileSystem();
		return *instance;
	}

};