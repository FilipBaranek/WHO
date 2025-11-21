#include "../../../../Headers/Model/PCRDatabase/Models/Person.h"

std::string Person::toCsvFormat()
{
	std::ostringstream oss;
	oss << m_birthNumber << ";" << m_firstName << ";" << m_lastName << ";"
		<< static_cast<int>(m_birthDay.year()) << "-"
		<< std::setw(2) << std::setfill('0') << static_cast<unsigned>(m_birthDay.month()) << "-"
		<< std::setw(2) << std::setfill('0') << static_cast<unsigned>(m_birthDay.day()) << "\n";

	return oss.str();
}

std::string Person::toString()
{
	std::ostringstream oss;
	oss << "\n[" << m_birthNumber << "] " << m_firstName << " " << m_lastName
		<< " born " << static_cast<int>(m_birthDay.year()) << "-"
		<< std::setw(2) << std::setfill('0') << static_cast<unsigned>(m_birthDay.month()) << "-"
		<< std::setw(2) << std::setfill('0') << static_cast<unsigned>(m_birthDay.day());

	return oss.str();
}

IRecord* Person::clone()
{
	return new Person(m_birthNumber, m_firstName, m_lastName, m_birthDay);
}

bool Person::equals(IRecord* other)
{
	Person* otherPerson = dynamic_cast<Person*>(other);

	if (m_firstName != otherPerson->firstName() || m_lastName != otherPerson->lastName() ||
		m_birthNumber != otherPerson->birthNumber() || m_birthDay != otherPerson->birthDay())
	{
		return false;
	}
	return true;
}

bool Person::is(IRecord* other)
{
	return m_birthNumber == static_cast<Person*>(other)->birthNumber();
}

int Person::getSize()
{
	return m_birthNumberBin.size() + m_firstNameBin.size() + m_lastNameBin.size() + sizeof(int) + (2 * sizeof(unsigned int));
}

bool Person::toBytes(uint8_t* bytesOutput)
{
	if (m_firstName.size() > MAX_FIRST_NAME_SIZE || m_lastName.size() > MAX_LAST_NAME_SIZE || m_birthNumber.size() > MAX_BIRTHNUMBER_SIZE)
	{
		return false;
	}

	int y = static_cast<int>(m_birthDay.year());
	unsigned int m = static_cast<unsigned>(m_birthDay.month());
	unsigned int d = static_cast<unsigned>(m_birthDay.day());

	uint8_t* index = bytesOutput;
	index = ByteConverter::toByteFromString(m_birthNumberBin, index);
	index = ByteConverter::toByteFromString(m_firstNameBin, index);
	index = ByteConverter::toByteFromString(m_lastNameBin, index);
	index = ByteConverter::toByteFromPrimitive(y, index);
	index = ByteConverter::toByteFromPrimitive(m, index);
	index = ByteConverter::toByteFromPrimitive(d, index);

	return true;
}

IRecord* Person::fromBytes(uint8_t* byteBuffer)
{
	using namespace std::chrono;

	uint8_t* index = byteBuffer;
	std::string birthNumber, firstName, lastName;
	
	BinaryString binBirthNumber = ByteConverter::fromByteToString(birthNumber, index);
	index += binBirthNumber.size();
	BinaryString binFirstName = ByteConverter::fromByteToString(firstName, index);
	index += binFirstName.size();
	BinaryString binLastName = ByteConverter::fromByteToString(lastName, index);
	index += binLastName.size();

	return new Person(
		birthNumber,
		firstName,
		lastName,
		year_month_day{
			year{ByteConverter::fromByteToPrimitive<int>(index)},
			month{ByteConverter::fromByteToPrimitive<unsigned>(index + sizeof(int))},
			day{ByteConverter::fromByteToPrimitive<unsigned>(index + sizeof(int) + sizeof(unsigned int))}
		}
	);
}

Person* Person::dummyInstance()
{
	std::string dummyString = "";
	std::chrono::year_month_day dummyDate{};

	return new Person(
		dummyString,
		dummyString,
		dummyString,
		dummyDate
	);
}

Person* Person::createInstance(uint8_t* buffer)
{
	Person* dummy = dummyInstance();
	Person* instance = dynamic_cast<Person*>(dummy->fromBytes(buffer));
	delete dummy;

	return instance;
}
