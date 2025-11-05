#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/LocationWrapper.h"

int LocationWrapper::compare(IComparable* other) const
{
	LocationWrapper* otherLocation = static_cast<LocationWrapper*>(other);

	if (m_locationId > otherLocation->locationId())
	{
		return -1;
	}
	else if (m_locationId < otherLocation->locationId())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}