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

int Person::getSize()
{
	return m_firstNameBin.size() + m_lastNameBin.size() + m_birthNumber.size() + sizeof(int) + (2 * sizeof(unsigned int));
}

bool Person::toBytes(std::vector<uint8_t>& bytesOutput)
{
	if (m_firstName.size() > MAX_FIRST_NAME_SIZE || m_lastName.size() > MAX_LAST_NAME_SIZE || m_birthNumber.size() > MAX_BIRTHNUMBER_SIZE)
	{
		return false;
	}

	int y = static_cast<int>(m_birthDay.year());
	int m = static_cast<unsigned>(m_birthDay.month());
	int d = static_cast<unsigned>(m_birthDay.day());

	ByteConverter::toByteFromString(m_birthNumberBin, bytesOutput);
	ByteConverter::toByteFromString(m_firstNameBin, bytesOutput);
	ByteConverter::toByteFromString(m_lastNameBin, bytesOutput);
	ByteConverter::toByteFromPrimitive(y, bytesOutput);
	ByteConverter::toByteFromPrimitive(m, bytesOutput);
	ByteConverter::toByteFromPrimitive(d, bytesOutput);

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

Person* Person::createInstance(uint8_t* buffer)
{
	std::string dummyString = "";
	std::chrono::year_month_day dummyDate{};

	Person dummy(
		dummyString,
		dummyString,
		dummyString,
		dummyDate
	);

	return dynamic_cast<Person*>(dummy.fromBytes(buffer));
}
