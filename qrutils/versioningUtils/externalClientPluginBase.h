#pragma once

#include "../utilsDeclSpec.h"
#include "../../qrgui/versioning/versioningPluginInterface.h"

#include <QtCore/QProcess>

namespace qReal
{
namespace versioning
{

/// Base class for all VCS clients that implement their functionality
/// in some external process
class QRUTILS_EXPORT ExternalClientPluginBase : public QObject, public VersioningPluginInterface
{
public:
	void setWorkingCopyManager(qrRepo::versioning::WorkingCopyManagementInterface *workingCopyManager);

	virtual void init(PluginConfigurator const &configurator);

	/// Starts process which executable`s path specified in by setPathToClient() method
	/// @param args Startup arguments
	/// @param needPreparation Specifies if working copy must be fetchced from current project
	/// @param needPreparation Specifies if changes in working copy must be registered in current project
	/// @param targetProject A path to project whicth will recieve chages in working copy after processing.
	///                      If empty value speified, target project will be working one
	bool invokeOperation(QStringList const &args, bool needPreparation = true
			, bool needProcessing = true, QString const &targetProject = QString()
			, bool reportErrors = true);

protected:
	ExternalClientPluginBase();
	virtual ~ExternalClientPluginBase() {}

	QString pathToClient() const;
	void setPathToClient(QString const &pathToClient);

	QString standartOutput() const;

	/// Must be called every time when occured error
	void onErrorOccured(QString const &errorMessage);
	/// Must be called every time when occured some errors
	void onErrorsOccured(QStringList const &errorMessages);

	/// Virtual, must be implemented in each plugin. Returns
	/// process communication timeout in ms
	virtual int timeout() const = 0;
	/// Virtual, must be implemented in each plugin. Returns
	/// path to folder where all temp operations must occure
	virtual QString tempFolder() const = 0;

private:
	bool startProcess(QStringList const &args, bool reportErrors = true);
	bool checkClientPath(bool reportErrors = true);
	bool processErrors(bool reportErrors = true);
	bool waitForClient(bool reportErrors = true);

	qrRepo::versioning::WorkingCopyManagementInterface *mWorkingCopyManager;
	ErrorReporterInterface *mErrorReporter;
	QString mPathToClient;
	QProcess *mClientProcess;
};

}
}
