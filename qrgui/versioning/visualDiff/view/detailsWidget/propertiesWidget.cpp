#include "propertiesWidget.h"

using namespace versioning;
using namespace versioning::details;

PropertiesWidget::PropertiesWidget(DiffModel *diffModel
		, bool isGraphical, QWidget *parent)
	: QWidget(parent), mDiffModel(diffModel)
	, mIsGraphical(isGraphical), mPropertyWidgets()
{
	mLayout = new QGridLayout(this);
	mLayout->setMargin(3);
}

void PropertiesWidget::setId(const qReal::Id &graphicalId, const qReal::Id &logicalId)
{
	reset();
	mGraphicalId = graphicalId;
	mLogicalId = logicalId;
	QListIterator<QString> i = difference()->propertyNamesIterator();
	int index = -1;
	while (i.hasNext())
	{
		++index;
		QString propertyName = i.next();
		addPropertyWidget(propertyName, index);
	}
}

void PropertiesWidget::reset()
{
	clearPropertyWidgets();
}

CommonDifference *PropertiesWidget::difference() const
{
	CommonDifference *result;
	if (mIsGraphical)
		result = mDiffModel->difference(mGraphicalId)->graphicalDifference();
	else
		result = mDiffModel->difference(mLogicalId)->logicalDifference();
	return result;
}

void PropertiesWidget::clearPropertyWidgets()
{
	foreach (PropertyWidget *widget, mPropertyWidgets)
	{
		mLayout->removeWidget(widget);
		delete widget;
	}
	mPropertyWidgets.clear();
}

void PropertiesWidget::addPropertyWidget(QString const &propertyName, int index)
{
	PropertyWidget *widget = new PropertyWidget(mDiffModel, mIsGraphical, this);
	widget->setProperty(mGraphicalId, mLogicalId, propertyName);
	mPropertyWidgets.append(widget);
	connect(widget, SIGNAL(mouseEntered(qReal::Id)), this, SLOT(onMouseEnteredIdWidget(qReal::Id)));
	connect(widget, SIGNAL(mouseLeaved(qReal::Id)), this, SLOT(onMouseLeavedIdWidget(qReal::Id)));
	connect(widget, SIGNAL(idListButtonClicked(QString)), this, SLOT(onIdListButtonClicked(QString)));
	mLayout->addWidget(widget, index, 0);
}

void PropertiesWidget::onMouseEnteredIdWidget(const qReal::Id &id)
{
	emit mouseEnteredIdWidget(id);
}

void PropertiesWidget::onMouseLeavedIdWidget(const qReal::Id &id)
{
	emit mouseLeavedIdWidget(id);
}

void PropertiesWidget::onIdListButtonClicked(const QString &propertyName)
{
	emit idListButtonClicked(mIsGraphical, propertyName);
}