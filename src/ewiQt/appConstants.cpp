// appConstants.cpp
#include "appConstants.hpp"
//-- STL
#include <qchar.h>
#include <qcoreapplication.h>
#include <string>
//- Third-Party
#include <QtCore>
//- In-house
#include <ewiQt/QtConverter.hpp>


using QtC = ewiQt::QtConverter;
namespace ewiQt
{
    QString const AppConstants::FILE_EXT { ".txt" };
    QString const AppConstants::USR_DIR { ".usr" };
    QString const AppConstants::TMP_DIR { ".tmp" };
    QString const AppConstants::JOB_DIR { ".jobs" };
    QStringList const AppConstants::APP_DIRS {
        AppConstants::USR_DIR,
        AppConstants::JOB_DIR,
        AppConstants::TMP_DIR
    };
    QString const AppConstants::PLOT_FILE { "plot.png" };

    auto AppConstants::getExeDir()-> QString 
    {
        return QCoreApplication::applicationDirPath();
    }
    auto AppConstants::getUserPath(QString const& userID) -> QString
    {
        return getExeDir() + '/' + USR_DIR + '/' + userID + FILE_EXT;
    }
    auto AppConstants::getUserPath(std::string const& user_id) -> QString
    {
        return getUserPath(QtC::from_stl(user_id));
    }
    auto AppConstants::getTmpDir() -> QString
    {
        return getExeDir() + '/' + TMP_DIR;
    }
    auto AppConstants::getJobDir() -> QString
    {
        return getExeDir() + '/' + JOB_DIR;
    }
    auto AppConstants::getPlotFile() -> QString
    {
        return getTmpDir() + '/' + PLOT_FILE;
    }
}
