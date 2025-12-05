#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/PersonHashWrapper.h"

std::string PersonHashWrapper::toString()
{
	return m_person->toString();
}

IRecord* PersonHashWrapper::clone()
{
	PersonHashWrapper* person = new PersonHashWrapper(new Person(m_person->birthNumber(), m_person->firstName(), m_person->lastName(), m_person->birthDay()));
	
	for (int i{}; i < m_tests.size(); ++i)
	{
		person->tests().push_back(m_tests[i]);
	}

	return person;
}

bool PersonHashWrapper::equals(IRecord* other)
{
	PersonHashWrapper* otherPersonHashWrapper = dynamic_cast<PersonHashWrapper*>(other);
	Person* otherPerson = otherPersonHashWrapper->getData();

	if (m_person->firstName() != otherPerson->firstName() || m_person->lastName() != otherPerson->lastName() ||
		m_person->birthNumber() != otherPerson->birthNumber() || m_person->birthDay() != otherPerson->birthDay())
	{
		return false;
	}
	return true;
}

bool PersonHashWrapper::is(IRecord* other)
{
	return m_person->birthNumber() == static_cast<PersonHashWrapper*>(other)->getData()->birthNumber();
}

int PersonHashWrapper::getSize()
{
	return m_person->binBirthNumber().size() + m_person->binFirstName().size() + m_person->binLastName().size() +
		sizeof(int) + (2 * sizeof(unsigned int)) + sizeof(int) + (MAX_TEST_COUNT * sizeof(int));
}

bool PersonHashWrapper::toBytes(uint8_t* bytesOutput)
{
	if (m_person->firstName().size() > m_person->binFirstName().m_capacity ||
		m_person->lastName().size() > m_person->binLastName().m_capacity ||
		m_person->birthNumber().size() > m_person->binBirthNumber().m_capacity)
	{
		return false;
	}

	int y = static_cast<int>(m_person->birthDay().year());
	unsigned int m = static_cast<unsigned>(m_person->birthDay().month());
	unsigned int d = static_cast<unsigned>(m_person->birthDay().day());

	uint8_t* index = bytesOutput;
	index = ByteConverter::toByteFromString(m_person->binBirthNumber(), index);
	index = ByteConverter::toByteFromString(m_person->binFirstName(), index);
	index = ByteConverter::toByteFromString(m_person->binLastName(), index);
	index = ByteConverter::toByteFromPrimitive(y, index);
	index = ByteConverter::toByteFromPrimitive(m, index);
	index = ByteConverter::toByteFromPrimitive(d, index);
	index = ByteConverter::toByteFromPrimitive(m_tests.size(), index);

	for (int i{}; i < m_tests.size(); ++i)
	{
		index = ByteConverter::toByteFromPrimitive(m_tests[i], index);
	}

	return true;
}

IRecord* PersonHashWrapper::fromBytes(uint8_t* byteBuffer)
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
	year y = year{ ByteConverter::fromByteToPrimitive<int>(index) };
	index += sizeof(int);
	month m = month{ ByteConverter::fromByteToPrimitive<unsigned>(index) };
	index += sizeof(unsigned int);
	day d = day{ ByteConverter::fromByteToPrimitive<unsigned>(index) };
	index += sizeof(unsigned int);

	PersonHashWrapper* person = new PersonHashWrapper(new Person(
		birthNumber,
		firstName,
		lastName,
		year_month_day{ y, m, d }
	));

	int testCount = ByteConverter::fromByteToPrimitive<int>(index);
	index += sizeof(unsigned int);
	for (int i{}; i < testCount; ++i)
	{
		person->tests().push_back(ByteConverter::fromByteToPrimitive<unsigned int>(index));
		index += sizeof(unsigned int);
	}

	return person;
}

size_t PersonHashWrapper::hash()
{
	return std::hash<std::string>{}(m_person->birthNumber());
}

PersonHashWrapper* PersonHashWrapper::dummyInstance()
{
	std::string dummyString = "";
	std::chrono::year_month_day dummyDate{};

	return new PersonHashWrapper(new Person(
		dummyString,
		dummyString,
		dummyString,
		dummyDate
	));
}

PersonHashWrapper* PersonHashWrapper::createInstance(uint8_t* byteBuffer)
{
	PersonHashWrapper* dummy = PersonHashWrapper::dummyInstance();
	PersonHashWrapper* instance = dynamic_cast<PersonHashWrapper*>(dummy->fromBytes(byteBuffer));
	delete dummy;

	return instance;
}

PersonHashWrapper::~PersonHashWrapper()
{
	delete m_person;
}
