#include "rcSimpleElementGenerator.h"

#include "../russianCRobotGenerator.h"
#include "../rcElementGeneratorFactory.h"
#include "simpleElements/rcAbstractSimpleElementGenerator.h"

using namespace qReal;
using namespace robots::russianC;

SimpleElementGenerator::SimpleElementGenerator(RussianCRobotGenerator *emboxGen
		, qReal::Id const &elementId): AbstractElementGenerator(emboxGen, elementId)
{
}

QList<SmartLine> SimpleElementGenerator::convertBlockIntoCode()
{
	QList<SmartLine> result;

	qReal::Id const logicElementId = mRobotCGenerator->api()->logicalId(mElementId); //TODO

	result = AbstractSimpleElementGenerator::convertedCode(mRobotCGenerator, mElementId, logicElementId);
	return result;
}

bool SimpleElementGenerator::nextElementsGeneration()
{
	IdList const outgoingConnectedElements = mRobotCGenerator->api()->outgoingConnectedElements(mElementId);
	mRobotCGenerator->generatedStringSet() << convertBlockIntoCode();

	if (outgoingConnectedElements.size() == 1) {
		if (outgoingConnectedElements.at(0) == Id::rootId()) {
			mRobotCGenerator->errorReporter().addError("Element " + mElementId.toString() + " has no"\
					" correct next element because its link has no end object."\
					" May be you need to connect it to diagram object.", mElementId);
			return false;
		}

		AbstractElementGenerator* const gen = ElementGeneratorFactory::generator(mRobotCGenerator
				, outgoingConnectedElements.at(0), *mRobotCGenerator->api());
		mRobotCGenerator->previousElement() = mElementId;
		gen->generate();
		delete gen;
		return true;
	} else if ((mElementId.element() == "FinalNode") && (outgoingConnectedElements.size() == 0)) {
		return true;
	} else {
		//case of error end of diagram
		mRobotCGenerator->errorReporter().addError(QObject::tr("There is no outgoing connected element with no final node!")
				, mElementId);
		return false;
	}

	return true;
}

QList<SmartLine> SimpleElementGenerator::addLoopCodeInPrefixForm()
{
	QList<SmartLine> result;
	result << SmartLine(QString::fromUtf8("повторять_бесконечно {"), mElementId, SmartLine::increase);
	return result;
}

QList<SmartLine> SimpleElementGenerator::addLoopCodeInPostfixForm()
{
	QList<SmartLine> result;
	result << SmartLine("}", mElementId, SmartLine::decrease);
	return result;
}

bool SimpleElementGenerator::preGenerationCheck()
{
	IdList const outgoingConnectedElements = mRobotCGenerator->api()->outgoingConnectedElements(mElementId);
	if (outgoingConnectedElements.size() > 1) {
		//case of error in diagram
		mRobotCGenerator->errorReporter().addError(QObject::tr("There are more than 1 outgoing connected elements with simple robot element!"), mElementId);
		return false;
	}

	return true;
}
