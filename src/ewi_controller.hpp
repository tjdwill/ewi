// ewi_controller.hpp
// The driver for the app, this class serves as the liasion between the GUI and the backend.
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
#ifndef INCLUDED_EWI_CONTROLLER
#define INCLUDED_EWI_CONTROLLER

#ifndef INCLUDED_STD_OPTIONAL
#include <optional>
#define INCLUDED_STD_OPTIONAL
#endif

#ifndef INCLUDED_QT_QTCORE
#include <QtCore>
#define INCLUDED_QT_QTCORE
#endif

#ifndef INCLUDED_QT_QWIDGET
#include <QWidget>
#define INCLUDED_QT_QWIDGET
#endif

#ifndef INCLUDED_EWI_EMPLOYEE_RECORD
#include <ewi/employee_record.hpp>
#endif

#ifndef INCLUDED_EWI_SURVEY
#include <ewi/survey.hpp>
#endif

/* Forward Declarations */
namespace ewiQt
{
    class EWIUi;
}

/* Definition */
class EWIController : public QWidget
{
    Q_OBJECT;

public:
    EWIController(QWidget* parent=nullptr);


public slots:

private slots:

    /// Perform app shutdown actions (saving, cleanup, etc.)
    void appShutdown();
    void createUser(QStringList userData);
    /// Export the user's data to file.
    void exportUser(QString pathName);
    void loadJob(QString jobDefPath);
    void loadUser(QString userID);
    /// Calculate EWI indexes based on given dates.
    /// Question is, however, what to do when no data exists for those dates?
    void processMetrics(QVector<QDate> dates);
    /// Handle survey results
    void processResponses(QStringList responses, QString const& surveyType);

private:  /* DATA MEMBERS */
    /// Check if user and job are both loaded.
    bool d_profile_loaded { false };
    std::optional<ewi::EmployeeRecord> d_user_profile {};
    std::optional<ewi::ParsedProfile> d_job_profile {};
    ewiQt::EWIUi* d_app {};

private:  /* METHODS */
    void createConnections();
    void sendError(std::string const& err_msg);
    /// Ensure required directories are available to the program.
    /// Assumes this program is self-contained in that critical files are stored within the
    /// same directory structure as the executable.
    ///
    /// Folders:
    ///     - `.jobs`: Stores default job profiles
    ///     - `.usr`: Internal storage of user profiles
    ///     - `.tmp`: Place to store temporary data. (Cleared after each run?)
    void validateRuntimeEnv();
};

#endif // INCLUDED_EWI_CONTROLLER
