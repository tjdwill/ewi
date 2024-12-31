// appConstants.cpp
/*
* Copyright (C) 2024 Terrance Williams
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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
