#pragma once
#include "Service.h"
#include "Singleton.h"
#include <vector>
#include <typeinfo>

using namespace std;

class ServiceLocator
{
public:
	template <typename T> T* GetService();
	void AddService(Service* service);

private:
	vector<Service*> services;

};

template<typename T>
inline T* ServiceLocator::GetService()
{
	// auto targetType = typeid(T);
	for (auto service : services)
	{
		// if (typeid(service) == targetType)
		T* result = dynamic_cast<T*>(service);
		if (result)
			return result;
	}

	return nullptr;
}
