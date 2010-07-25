#include "diagram.h"
#include "classes/type.h"
#include "classes/enumType.h"
#include "classes/nodeType.h"
#include "classes/edgeType.h"
#include "editor.h"

#include <QDebug>

using namespace qReal;

Diagram::Diagram(qReal::Id const &id,  qrRepo::RepoApi *api, Editor *editor)
	: mId(id), mApi(api), mEditor(editor)
{
	mDiagramName = mApi->name(id);
	mDiagramDisplayedName = mApi->stringProperty(id, "displayedName");
	mDiagramNodeName  = mApi->stringProperty(id, "nodeName");
}

Diagram::~Diagram()
{
	foreach(Type *type, mTypes.values())
		if (type)
			delete type;
}

bool Diagram::init()
{
	foreach(Id id, mApi->children(mId)) {
		if (id.element() == "MetaEntityNode") {
			Type *nodeType = new NodeType(this, mApi, id);
			if (!nodeType->init(mDiagramName)) {
				delete nodeType;
				qDebug() << "can't load node";
				return false;
			}
			mTypes[nodeType->name()] = nodeType;
		} else if (id.element() == "MetaEntityEdge") {
			Type *edgeType = new EdgeType(this, mApi, id);
			if (!edgeType->init(mDiagramName)) {
				delete edgeType;
				qDebug() << "can't load edge";
				return false;
			}
			mTypes[edgeType->name()] = edgeType;
		} else if (id.element() == "MetaEntityImport") {
			ImportSpecification import = {
				mApi->stringProperty(id, "importedFrom") + "::" + mApi->stringProperty(id, "as"),
				mApi->stringProperty(id, "as"),
				mApi->stringProperty(id, "displayedName"),
			};
			mImports.append(import);
		} else if (id.element() == "MetaEntityEnum") {
			// TODO: load enums
		} else {
			qDebug() << "ERROR: unknown type" << id.element();
			return false;
		}
	}
	return true;
}

bool Diagram::resolve()
{
	foreach (ImportSpecification import, mImports) {
		Type *importedType = mEditor->findType(import.name);
		if (importedType == NULL) {
			qDebug() << "ERROR: imported type" << import.name << "not found, skipping";
			continue;
		}
		Type *copiedType = importedType->clone();
		copiedType->setName(import.as);
		copiedType->setDisplayedName(import.displayedName);
		copiedType->setDiagram(this);
		copiedType->setContext(mDiagramName);
		mTypes.insert(copiedType->qualifiedName(), copiedType);
	}

	foreach(Type *type, mTypes.values())
		if (!type->resolve()) {
			qDebug() << "ERROR: can't resolve type" << type->name();
			return false;
		}

	return true;
}

Editor* Diagram::editor() const
{
	return mEditor;
}

Type* Diagram::findType(QString name)
{
	if (mTypes.contains(name))
		return mTypes[name];

	return mEditor->findType(name);
}

QMap<QString, Type*> Diagram::types() const
{
	return mTypes;
}

QString Diagram::name() const
{
	return mDiagramName;
}

QString Diagram::nodeName() const
{
	return mDiagramNodeName;
}

QString Diagram::displayedName() const
{
	return mDiagramDisplayedName;
}

void Diagram::print()
{
	qDebug() << "elements:" << mTypes.size();
//	foreach(Type *type, mTypes)
//		type->print();
}

QString Diagram::generateNamesMap(QString const& lineTemplate) const
{
	QString result;
	foreach(Type* type, mTypes) {
		QString line = type->generateNames(lineTemplate);
		if (!line.isEmpty())
			result += line + "\n";
	}
	return result;
}

QString Diagram::generateMouseGesturesMap(const QString &lineTemplate) const
{
	QString result;
	foreach(Type* type, mTypes) {
		QString line = type->generateMouseGestures(lineTemplate);
		if (!line.isEmpty())
			result += line + "\n";
	}
	return result;
}

QString Diagram::generatePropertiesMap(const QString &lineTemplate) const
{
	QString result;
	foreach(Type* type, mTypes) {
		QString line = type->generateProperties(lineTemplate);
		if (!line.isEmpty())
			result += line + "\n";
	}
	return result;
}

QString Diagram::generatePropertyDefaultsMap(const QString &lineTemplate) const
{
	QString result;
	foreach(Type* type, mTypes) {
		QString line = type->generatePropertyDefaults(lineTemplate);
		if (!line.isEmpty())
			result += line + "\n";
	}
	return result;
}

QString Diagram::generateFactory(const QString &lineTemplate) const
{
	QString result;
	bool isFirstLine = true;
	foreach(Type* type, mTypes) {
		QString line = type->generateFactory(lineTemplate);
		if (!isFirstLine)
			line.replace("if", "else if");
		isFirstLine = false;
		if (!line.isEmpty())
			result += line + "\n";
	}
	return result;
}

QString Diagram::generateContainers(const QString &lineTemplate) const
{
	QString result;
	bool isFirstLine = true;
	foreach(Type* type, mTypes) {
		QString line = type->generateContainers(lineTemplate);
		if (line.isEmpty())
			continue;
		if (!isFirstLine)
			line.replace("if", "else if");
		isFirstLine = false;
		result += line + "\n";
	}
	return result;
}

QString Diagram::generateConnections(const QString &lineTemplate) const
{
	QString result;
	bool isFirstLine = true;
	foreach(Type* type, mTypes) {
		QString line = type->generateConnections(lineTemplate);
		if (line.isEmpty())
			continue;
		if (!isFirstLine)
			line.replace("if", "else if");
		isFirstLine = false;
		result += line + "\n";
	}
	return result;
}

QString Diagram::generateUsages(const QString &lineTemplate) const
{
	QString result;
	bool isFirstLine = true;
	foreach(Type* type, mTypes) {
		QString line = type->generateUsages(lineTemplate);
		if (line.isEmpty())
			continue;
		if (!isFirstLine)
			line.replace("if", "else if");
		isFirstLine = false;
		result += line + "\n";
	}
	return result;
}