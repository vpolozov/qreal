#include "rcPlayToneGenerator.h"
#include "../../russianCRobotGenerator.h"

using namespace robots::russianC;

PlayToneGenerator::PlayToneGenerator()
{
}

QList<SmartLine> PlayToneGenerator::convertElementIntoDirectCommand(RussianCRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	result.append(SmartLine(QString::fromUtf8("играть_звук(") + nxtGen->api()->stringProperty(logicElementId, "Frequency") + ", "
			 + nxtGen->api()->stringProperty(logicElementId, "Duration") + ", "
			 + nxtGen->api()->stringProperty(logicElementId, "Volume") + ");", elementId));

	return result;
}
