#include "ServiceLocator.h"

void ServiceLocator::AddService(Service* service)
{
	services.push_back(service);
}

void ServiceLocator::ClearServices()
{
	for (auto service : services)
		delete service;
	services.clear();
}
