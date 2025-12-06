#include "../../../../Headers/Model/PCRDatabase/ModelWrappers/TestHashWrapper.h"

std::string TestHashWrapper::toString()
{
	return m_pcrTest->toString();
}

IRecord* TestHashWrapper::clone()
{
	return new TestHashWrapper(new ReducedPCRTest(
		m_pcrTest->testId(),
		m_pcrTest->result(),
		m_pcrTest->testValue(),
		m_pcrTest->note(),
		m_pcrTest->testDate()
	), m_person);
}

bool TestHashWrapper::equals(IRecord* other)
{
	TestHashWrapper* testWrapper = dynamic_cast<TestHashWrapper*>(other);
	ReducedPCRTest* otherTest = testWrapper->getData();

	if (m_pcrTest->testId() != otherTest->testId() || m_pcrTest->result() != otherTest->result() ||
		m_pcrTest->testValue() != otherTest->testValue() || m_pcrTest->note() != otherTest->note() ||
		m_pcrTest->testDate() != otherTest->testDate() || m_person != testWrapper->person())
	{
		return false;
	}
	return true;
}

bool TestHashWrapper::is(IRecord* other)
{
	return m_pcrTest->testId() == dynamic_cast<TestHashWrapper*>(other)->getData()->testId();
}

int TestHashWrapper::getSize()
{
	return sizeof(unsigned int) + sizeof(bool) + sizeof(double) + sizeof(int64_t) + m_pcrTest->binNote().size() + m_binPerson.size();
}

bool TestHashWrapper::toBytes(uint8_t* bytesOutput)
{
	if (m_pcrTest->note().size() > m_pcrTest->binNote().m_capacity || m_person.size() > BIRTH_NUMBER_MAX_SIZE)
	{
		return false;
	}

	int64_t timePoint = m_pcrTest->testDate().time_since_epoch().count();

	uint8_t* index = bytesOutput;

	index = ByteConverter::toByteFromPrimitive(m_pcrTest->testId(), index);
	index = ByteConverter::toByteFromPrimitive(m_pcrTest->result(), index);
	index = ByteConverter::toByteFromPrimitive(m_pcrTest->testValue(), index);
	index = ByteConverter::toByteFromPrimitive(timePoint, index);
	index = ByteConverter::toByteFromString(m_pcrTest->binNote(), index);
	index = ByteConverter::toByteFromString(m_binPerson, index);

	return true;
}

IRecord* TestHashWrapper::fromBytes(uint8_t* byteBuffer)
{
	using namespace std::chrono;

	std::string note, person;
	uint8_t* index = byteBuffer;

	unsigned int testId = ByteConverter::fromByteToPrimitive<unsigned int>(index);
	index += sizeof(unsigned int);
	bool result = ByteConverter::fromByteToPrimitive<bool>(index);
	index += sizeof(bool);
	double testValue = ByteConverter::fromByteToPrimitive<double>(index);
	index += sizeof(double);
	int64_t timePoint = ByteConverter::fromByteToPrimitive<int64_t>(index);
	index += sizeof(int64_t);
	BinaryString noteBin = ByteConverter::fromByteToString(note, index);
	index += noteBin.size();
	BinaryString personBin = ByteConverter::fromByteToString(person, index);
	index += personBin.size();

	return new TestHashWrapper(new ReducedPCRTest(
		testId,
		result,
		testValue,
		note,
		time_point<system_clock>{system_clock::duration(timePoint)}
	), person);
}

size_t TestHashWrapper::hash()
{
	return std::hash<unsigned int>{}(m_pcrTest->testId());
}

TestHashWrapper* TestHashWrapper::dummyInstance()
{
	int dummyNum = 0;
	std::string dummyStr = "";

	return new TestHashWrapper(new ReducedPCRTest(
		dummyNum,
		dummyNum,
		dummyNum,
		dummyStr,
		std::chrono::system_clock::now()
	), dummyStr);
}

TestHashWrapper* TestHashWrapper::createInstance(uint8_t* byteBuffer)
{
	TestHashWrapper* dummy = TestHashWrapper::dummyInstance();
	TestHashWrapper* test = dynamic_cast<TestHashWrapper*>(dummy->fromBytes(byteBuffer));
	delete dummy;

	return test;
}

TestHashWrapper::~TestHashWrapper()
{
	delete m_pcrTest;
}
