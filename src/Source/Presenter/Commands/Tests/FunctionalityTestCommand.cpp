#include "../../../../Headers/Presenter/Commands/Tests/FunctionalityTestCommand.h"


void FunctionalityTestCommand::execute()
{
	try
	{
		FunctionalityTester tester(m_pregeneratedDataCount);
		tester.runTests();
		m_output = tester.outputToString();
	}
	catch (const std::runtime_error& e)
	{
		m_output = "Test wasn't successful";
	}
}
