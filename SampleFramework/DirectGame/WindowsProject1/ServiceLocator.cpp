#include "ServiceLocator.h"

void ServiceLocator::AddService(Service* service)
{
	services.push_back(service);
}
