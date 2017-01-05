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

// C++11 �� �̴ϼȶ��������� local static ������ �ѹ� ���� �ϰ��Ѵ�.
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