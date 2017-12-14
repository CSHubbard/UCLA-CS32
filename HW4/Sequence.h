#include <iostream>
#include <vector>
using namespace std;
class myClass
{
public:
	myClass(int v) : value(v + 1)
	{
		cout << v << " ";
	}
	~myClass()
	{
		cout << -value << " ";
	}
private:
	int value;
};
int main()
{
	vector<myClass*> vec;
	vec.push_back(new myClass(2));
	vec.push_back(new myClass(4));
}