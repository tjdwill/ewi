// ewi_controller.cpp
#include "ewi_controller.hpp"
//- STL
#include <bits/chrono_io.h>
#include <chrono>
#include <optional>
#include <qaction.h>
#include <qchar.h>
#include <string>
#include <unistd.h>
#include <vector>
//- Third-party
#include <cpperrors>
#include <QtWidgets>
//- In-house
#include <ewiQt/ewiUI.hpp>
#include <ewiQt/QtConverter.hpp>
#include <ewi/employee_record.hpp>
#include <ewi/metrics.hpp>
#include <ewi/survey.hpp>


/* Imports */
using cpperrors::Exception;
using ewiQt::EWIUi;
using QtC = ewiQt::QtConverter;

/* Definitions */

//------------- Helpers ---------------
QString const USR_DIR {".usr"};
QString const TMP_DIR { ".tmp"};
QString const JOB_DIR { ".jobs" };
QStringList const APP_DIRS { TMP_DIR, USR_DIR, JOB_DIR };
QString const FILE_EXT { ".txt" };

auto getUserPath(QString const& userID) -> QString
{
    return QCoreApplication::applicationDirPath() + '/' + USR_DIR + '/' + userID + FILE_EXT;
}
auto getUserPath(std::string const& user_id) -> QString
{
    return getUserPath(QtC::from_stl(user_id));
}

//-------------------------------------


EWIController::EWIController(QWidget* parent)
    : QWidget(parent)
{
    // Startup tasks
    validateRuntimeEnv();

    // Layout and App Customization
    d_app = new EWIUi();
    QHBoxLayout* layout { new QHBoxLayout };
    layout->addWidget(d_app);
    setLayout(layout);
    setWindowTitle(tr("EWI Tracker"));
    // TODO: Move app window center to the screen center


    createConnections();
    // provide information to the app
    emit d_app->setPersonalQuestionsSig(QtC::from_stl(ewi::PersonalSurvey::questions()));
}

void EWIController::createConnections()
{
    connect(d_app, &EWIUi::appShutdownSig, this, &EWIController::appShutdown);
    connect(d_app, &EWIUi::createUserSig, this, &EWIController::createUser);
    connect(d_app, &EWIUi::exportUserSig, this, &EWIController::exportUser);
    connect(d_app, &EWIUi::getMetricsSig, this, &EWIController::processMetrics);
    connect(d_app, &EWIUi::loadJobSig, this, &EWIController::loadJob);
    connect(d_app, &EWIUi::loadUserSig, this, &EWIController::loadUser);
    connect(d_app, &EWIUi::surveyResponsesSig, this, &EWIController::processResponses);
}

void EWIController::sendError(std::string const& err_msg)
{
    emit d_app->errorMsgSig(QString::fromStdString(err_msg));
}

void EWIController::validateRuntimeEnv()
{
QDir appRoot { QCoreApplication::applicationDirPath() };

    // TODO: Handle temp folder check here. If the tmp folder exists, that means the app did
    // not shut down properly, meaning user changes may not have been written to their data
    // file.

    // Handle subdirectories
    for (auto const& d : APP_DIRS)
    {
        if(!appRoot.exists(d))
            if(!appRoot.mkdir(d))
            {
                appRoot.setPath(d);
                throw Exception("Could not create required directory: " + appRoot.absolutePath().toStdString());
            }
    }
}

// TODO: Remove after implementation is complete.
QTextStream qout { stdout };
void EWIController::appShutdown()
{
    qout << "Shutdown Triggered" << "\n";
    qout.flush();

    if (d_profile_loaded)
        exportUser(getUserPath(d_user_profile->who().id.formal()));
    close();
}

void EWIController::createUser(QStringList userData)
{
    /*
    qout << "User Creation: [";
    for (auto const& item : userData)
        qout << item << "; ";
    qout << "]\n";
    qout.flush();
    */
    auto data = QtC::to_stl(userData);
    ewi::Employee emp { { data[0] }, data[1] };
    d_user_profile = ewi::EmployeeRecord { emp };
    
    // Send signal that profile is loaded if necessary
    if (!d_profile_loaded && d_job_profile)
    {
        d_profile_loaded = true;
        emit d_app->profileLoadedSig();
    }
}

void EWIController::exportUser(QString pathName)
{
    qout << "Export User to: " << pathName << "\n";
    qout.flush();

    assert(d_user_profile);
    ewi::EmployeeRecordIOUtils::export_record(*d_user_profile, QtC::to_stl(pathName));
}

void EWIController::loadJob(QString jobDefPath)
{
    qout << "Load Job from: " << jobDefPath << "\n";
    qout.flush();

    std::string path { jobDefPath.toStdString() };
    try 
    {
        d_job_profile = ewi::load_profile(path);
        auto const& questions = d_job_profile.value().questions;
        emit d_app->jobChangedSig(QtC::from_stl(questions)); 
        if (!d_profile_loaded && d_user_profile)
        {
            d_profile_loaded = true;
            emit d_app->profileLoadedSig();
        }

    }
    catch (Exception const& e) 
    {
        std::string err_msg { "Job Load Error:\n" + e.what() };
        sendError(err_msg);
        return;
    }
    
}

void EWIController::loadUser(QString userID)
{
    qout << "Load User: " << userID << "\n";
    qout.flush();

    // Let's assume the data is formed correctly.
    QString userFile { getUserPath(userID) };
    try 
    {
       d_user_profile = ewi::EmployeeRecordIOUtils::import_record(QtC::to_stl(userFile));
    }
    catch (Exception const& e) 
    {
       std::string err_msg { "Load User Error:\n" + e.what()  };
       sendError(err_msg);
       return;
    }
    // Send signal that profile is loaded if necessary
    if (!d_profile_loaded && d_job_profile)
    {
        d_profile_loaded = true;
        emit d_app->profileLoadedSig();
    }
    
}

void EWIController::processMetrics(QVector<QDate> dates)
{
    qout << "Get metrics from " << dates[0].toString("yyyy-MM-dd")
        << " to " << dates[1].toString("yyyy-MM-dd") << "\n";
    qout.flush();

    auto stl_dates = QtC::to_stl(dates);
}

void EWIController::processResponses(QStringList responses, QString const& surveyType)
{
    /*
    qout << "Responses\n---------" << "\n";
    qout << "Survey Type: " << surveyType << "\n";
    for (auto const& response : responses)
        qout << response << "\n";
    qout.flush();
    */
    ewi::SurveyResults results { QtC::to_stl(responses), d_job_profile->metric_cnt() };
    
    // Create the entry and update the record
    try
    {
        if (surveyType == d_app->TECHNICAL_SURVEY)
            d_user_profile->add( d_job_profile->job_label.id, ewi::RecordType::Technical, results.to_entry() );
        else
            d_user_profile->add( d_job_profile->job_label.id, ewi::RecordType::Personal, results.to_entry() );

        // TODO: Marking temp file update point.
        // export_entry(results.to_entry); to the relevant file.
    }
    catch (Exception const& e)
    {
        sendError(e.what());
    }
}
