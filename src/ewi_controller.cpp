// ewi_controller.cpp
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
#include "ewi_controller.hpp"
//- STL
#include <cassert>
#include <chrono>
#include <optional>
#include <thread>  // for sleeping
#include <vector>
//- Third-party
#include <cpperrors>
#include <Eigen/Eigen>
#include <QtWidgets>
//- In-house
#include <ewiQt/appConstants.hpp>
#include <ewiQt/ewiUI.hpp>
#include <ewiQt/QtConverter.hpp>
#include <ewi/employee_record.hpp>
#include <ewi/metrics.hpp>
#include <ewi/survey.hpp>


/* Imports */
using cpperrors::Exception;
using ewiQt::EWIUi;
using QtC = ewiQt::QtConverter;
using AC = ewiQt::AppConstants;

//---------- Helper Function(s) -------------------
/// A deletion function for when recursiveRemove fails.
static void attemptRemove(QString const& dirName)
{
    QString SELF { "." };
    QString PARENT{ ".." };
    QTextStream qout { stdout };
    bool permCheck = QFile::setPermissions(dirName, QFile::ReadOther | QFile::WriteOther);
    assert(permCheck);
    QDir dir { dirName };

    // Assumes NON-RECURSIVE structure
    for (auto const& entryName : dir.entryList())
    {
        /*
            It was discovered on Windows tests that `gnuplot.exe` prevents the plot temp file from
            being closed if the user generated a plot during the session. Therefore, the file will
            need to be skipped. However, it *can* be deleted on startup, so I will likely include
            a special case for that in the relevant section of the code.
        */
        if (entryName == SELF || entryName == PARENT || entryName == AC::PLOT_FILE)
            continue;
        QString entry { dir.absoluteFilePath(entryName) };
        QDir checkDir { entry };
        if (checkDir.exists() && checkDir.absolutePath() != dirName)
            attemptRemove(checkDir.absolutePath());
        else
        {
            QFile f { entry };
            bool permissionChange = f.setPermissions(QFile::ReadOther | QFile::WriteOther);
            assert(permissionChange);
            bool removeCheck = QFile::remove(entry);
            assert(removeCheck);
        }
    }
    return;
}

//-------------------------------------------------

/* Definitions */
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
    emit d_app->setPersonalQuestionsSig(QtC::toQt(ewi::PersonalSurvey::questions()));
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
    QDir appRoot { AC::getExeDir() }; 

    // TODO: Handle temp folder check here. If the tmp folder exists, that means the app did
    // not shut down properly, meaning user changes may not have been written to their data
    // file.
    
    // 2 January 2025:
    // Check if the plot tmp file still exists and delete if so. 
    // This is to workaroud the Windows behavior where gnuplot.exe keeps the file hostage.
    QFile plotFile { AC::getPlotFile() };
    if (plotFile.exists())
    {
        bool rmTest = plotFile.remove();
        assert(rmTest);
    }

    // Handle subdirectories
    for (auto const& d : AC::APP_DIRS)
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
void EWIController::appShutdown()
{
    if (d_profile_loaded)
        exportUser(AC::getUserPath(d_user_profile->who().id.formal()));
    // Delete tmp 
    QDir tmpDir { AC::getTmpDir() };
    bool test = tmpDir.removeRecursively();
    if (!test)
        // The plot file likely couldn't be removed on Windows due to gnuplot.exe.
        // Try deleting everything else.
        attemptRemove(AC::getTmpDir());
    close();
}

void EWIController::createUser(QStringList userData)
{
    /*
        Check if employee exists already
    */
    QString userFile { AC::getUserPath(userData[0]) };
    if (QFile::exists(userFile))
    {
        QString err_msg { 
            "User \"" + userData[0] + '"' + " already exists. Delete\n`" + userFile
            + "`\nif you really want to recreate the user."
        };
        sendError(QtC::to_stl(err_msg));
        return;
    }

    // Save the user current loaded
    if (d_user_profile)
        exportUser(AC::getUserPath(d_user_profile->who().id.formal()));

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
    assert(d_user_profile);
    ewi::EmployeeRecordIOUtils::export_record(*d_user_profile, QtC::to_stl(pathName));
}

void EWIController::loadJob(QString jobDefPath)
{
    std::string path { jobDefPath.toStdString() };
    try 
    {
        d_job_profile = ewi::load_profile(path);
        auto const& questions = d_job_profile.value().questions;
        emit d_app->jobChangedSig(QtC::toQt(questions)); 
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
    // if user already loaded, save any changes
    if (d_user_profile)
        exportUser(AC::getUserPath(d_user_profile->who().id.formal()));
    // Let's assume the data is formed correctly.
    QString userFile { AC::getUserPath(userID) };
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
    constexpr int SLEEP_TIME { 100 }; // ms
    static std::string const plot_path = QtC::to_stl(AC::getPlotFile());

    auto stl_dates = QtC::to_stl(dates);
    // We know from the form that the dates are valid in that the `from` date is less than or
    // equal to the `to` date, so checks can be omitted.
    try 
    {
        auto const& wi_rec = d_user_profile->get(d_job_profile->job_label.id);
        if (!wi_rec.technical.is_empty())
        
        {
            // Plot Configuration
            std::string datetime = QDateTime::currentDateTime()
                .toString("yyyy-MM-dd hh:mm:ss")
                .toStdString();

            std::string start_date { QtC::to_stl(dates[0].toString(QtC::QT_DATE_FORMAT)) };
            std::string end_date { QtC::to_stl(dates[1].toString(QtC::QT_DATE_FORMAT)) };
            std::string fig_title = d_user_profile->who().name + "'s "
                + " EWI Report (" + start_date + " to " + end_date + ')';
            std::string tech_title {
                d_job_profile->job_label.title
                + " (" + d_job_profile->job_label.id.formal() + ')'
            };
            ewi::PlotCustomization opts {
                plot_path,
                fig_title,
                tech_title
            };
            // get metrics 
            auto const rec = wi_rec.technical;
            auto metrics = rec.metrics({ stl_dates[0], stl_dates[1] });
            if (!metrics)
            {
                std::ostringstream oss {};
                oss << "No metrics recorded for specified date range: "
                    << stl_dates[0] << " to " << stl_dates[1];
                sendError(oss.str());
                return;
            }
            // Eigen processing
            Eigen::MatrixXd tech_matrix = ewi::to_eigen(*metrics);
            Eigen::VectorXd tech_means = ewi::get_means(tech_matrix);
            Eigen::VectorXd global_tech_means = ewi::to_eigen(d_job_profile->averages);
            auto temp_twi = ewi::calculate_ewi(tech_means, global_tech_means);

            // Update options (set ylim)
            double ymin = std::floor(temp_twi.minCoeff());
            ymin = (ymin < 0) ? ymin : 0;
            double ymax = std::floor(temp_twi.maxCoeff()) + 1.0;
            ymax = (ymax > 1) ? ymax : 2.0;
            opts.ylim = { ymin, ymax };

            std::vector<double> twi = ewi::to_std_vec(
                   temp_twi 
            );
            // Add Personal Work Index if data is present
            if (!wi_rec.personal.is_empty())
            {
                auto const& p_metrics = wi_rec.personal.metrics( { stl_dates[0], stl_dates[1] } );
                if (p_metrics)
                {
                    double p_mean = ewi::to_eigen(*p_metrics).mean();
                    double pwi = ewi::calculate_ewi(
                            p_mean,
                            ewi::PersonalSurvey::IDEAL_MEAN,
                            ewi::PersonalSurvey::MIN_VAL,
                            ewi::PersonalSurvey::MAX_VAL
                    );
                    bool success = ewi::plot_ewi(twi, opts, pwi);
                    assert(success);
                }
            }
            else
            {
                // Only plot the technical data
                bool success = ewi::plot_ewi(twi, opts );
                assert(success);
            }
            // Send image for display
            // Sleep for a small duration to ensure I/O flushes
            QString plotPath { AC::getPlotFile() };
            QFile testFile{ plotPath };
            while (!testFile.exists() || !(testFile.size() > 0))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
            }
            QPixmap plotImg { plotPath };
            while (plotImg.isNull())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
                plotImg.load(plotPath);
            }
            emit d_app->sendImg(plotImg);
        }
        else {
            std::ostringstream oss {};
            oss << "No metrics recorded for specified date range: "
                << stl_dates[0] << " to " << stl_dates[1];
            sendError(oss.str());
            return;
        }
    }
    catch (Exception const& e)
    {
        sendError("No entries recorded for the current job.");
    }
}

void EWIController::processResponses(QStringList responses, QString const& surveyType)
{
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
