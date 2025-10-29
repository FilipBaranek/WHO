#include "../../../../Headers/Presenter/Commands/Tests/SpeedTestCommand.h"
#include "../../../../Headers/Model/Tests/SpeedTester.h"

void SpeedTestCommand::execute()
{
	SpeedTester tester(m_dataInOrder);
	tester.testInsertion();
	tester.testRemoval();
	tester.testPointSearch();
	tester.testIntervalSearch();
	tester.testFindMinKey();
	tester.testFindMaxKey();

	m_output = tester.outputToString();
}
