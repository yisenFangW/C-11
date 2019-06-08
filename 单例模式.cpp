//单例模式：只有一个实例（static实例），线程安全，禁止拷贝赋值，用户需要通过接口获取实例（static成员函数）
//1.自己能想到的实现方法（普通懒汉模式）（通过加锁与智能指针，实现线程安全与析构对象指针）：
class Singleton {
public:
	typedef std::shared_ptr<Singleton> sptr;
	~Singleton(){
		cout << "discontruct called!" << endl;
	}
	Singleton(const Singleton &) = delete;
	Singleton& operator = (const Singleton &) = delete;
	static sptr getInstance() {
		if (instance_ptr == nullptr) {
			lock_guard<mutex> instance_guard(instance_mutex);
			if (instance_ptr == nullptr) {
        //这个地方的实现需要注意一下，没有用make_shared<Singleton>
				instance_ptr = std::shared_ptr<Singleton>(new Singleton());
			}
		}
		return instance_ptr;
	}
private:
	static sptr instance_ptr;
	static mutex instance_mutex;
	Singleton() {
		cout << "construct called!" << endl;
	}
};

//2.局部静态变量懒汉模式
class Singleton2 {
public:
	~Singleton2() {
		cout << "discontruct called!" << endl;
	}
	Singleton2(const Singleton2 &) = delete;
	Singleton2& operator = (const Singleton2 &) = delete;
	//必要要申明为引用，以为拷贝构造函数及赋值构造函数已经删除，所以只能够用引用传输。
	//如果当变量在初始化的时候，并发同时进入声明语句，并发线程将会阻塞等待初始化结束。（effective C++）
	//上述这句话保证了并发线程在获取静态局部变量的时候一定是初始化过的，所以具有线程安全性。
	//静态变量的生存期从程序开始到结束,局部变量在结束时，会由析构函数进行销毁
	static Singleton2& getInstance() {
		static Singleton2 instance;
		return instance;
	}
private:
	static mutex instance_mutex;
	Singleton2() {
		cout << "construct called!" << endl;
	}
};

//3.单例模式模板(需要将模板类申明为子类的友元)
//申明为友元的原因，是因为父类要调用子类的构造函数（而子类构造函数是私有的）；
template<typename T>
class Singleton3 {
public:
	virtual ~Singleton3() = default;
	Singleton3(const Singleton3&) = delete;
	Singleton3& operator = (const Singleton3&) = delete;
	static T& getInstance() {
		static T instance;
		return instance;
	}
protected:
	Singleton3() {
		cout << "construct called!" << endl;
	}
};

class DerivedSingle :public Singleton3<DerivedSingle> {
public:
	friend class Singleton3<DerivedSingle>;
	~DerivedSingle() {
		cout << "discontruct called!" << endl;
	}
	DerivedSingle(const DerivedSingle&) = delete;
	DerivedSingle& operator = (const DerivedSingle&) = delete;
private:
	DerivedSingle() = default;
};

//4.单例模式模板(不需要将模板类申明为子类的友元)
//这里使用了一个辅助的类token，可以将子类的构造函数申明为共有的，因为子类不能够自己调用构造函数
template<typename T>
class Singleton4 {
public:
	virtual ~Singleton4() = default;
	Singleton4(const Singleton4&) = delete;
	Singleton4& operator = (const Singleton4&) = delete;
	static T& getInstance() {
		//注意这个地方的实现
		static T instance{ token() };
		return instance;
	}
protected:
	Singleton4() {
		cout << "construct called!" << endl;
	}
	struct token{};
};

class DerivedSingle2 :public Singleton4<DerivedSingle2> {
public:
	DerivedSingle2(token) {}
	~DerivedSingle2() {
		cout << "discontruct called!" << endl;
	}
	DerivedSingle2(const DerivedSingle2&) = delete;
	DerivedSingle2& operator = (const DerivedSingle2&) = delete;
};
