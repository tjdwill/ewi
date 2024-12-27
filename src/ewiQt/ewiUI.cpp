// ewiUI.cpp
#include "ewiUI.hpp"
//- Third-party
#include <QtWidgets>
#include <QPixmap>
#include <qglobal.h>
#include <qmessagebox.h>
#include <qnamespace.h>
//- In-house
#include "form.hpp"
#include "imgViewer.hpp"
#include "profileLoader.hpp"
#include "userOpsWidget.hpp"
#include "views.hpp"


namespace ewiQt
{
    EWIUi::EWIUi(QWidget* parent)
        : QMainWindow(parent)
    {
        setup();
        setCentralWidget(d_appPages);
        createActions();
        validatePtrs();
        createConnections();
        setWindowTitle(tr("EWI App"));
    }

    void EWIUi::createActions()
    {
        aboutAction = new QAction(tr("About")); 

        aboutQtAction = new QAction(tr("About Qt"));
        
        createUserAction = new QAction(tr("Create User"));
        
        exitAction = new QAction(tr("Exit"));
        
        exportUserAction = new QAction(tr("Export User Profile..."));
        
        getMetricsAction = new QAction(tr("View Workload"));
        
        helpAction = new QAction(tr("Help"));
        
        loadJobAction = new QAction(tr("Load Job Definition"));
        
        loadUserAction = new QAction(tr("Load User"));
        
        servePersonalSurveyAction = new QAction(tr("Create Personal Entry"));
        
        serveTechnicalSurveyAction = new QAction(tr("Create Technical Entry"));
    }

    void EWIUi::createConnections()
    {
        // Connect Actions
        /// Connect buttons to action triggers
        connect(d_appPages->d_aboutButton, &QPushButton::clicked, aboutAction, &QAction::trigger);
        connect(d_appPages->d_exitButton, &QPushButton::clicked, exitAction, &QAction::trigger);
        connect(d_appPages->d_helpButton, &QPushButton::clicked, helpAction, &QAction::trigger);
        connect(
                d_appPages->getProfileLoader()->d_jobLoadButton, &QPushButton::clicked,
                loadJobAction, &QAction::trigger
    );
        connect(
                d_appPages->getProfileLoader()->d_userCreateButton, &QPushButton::clicked,
                createUserAction, &QAction::trigger
        );
        connect(
                d_appPages->getProfileLoader()->d_userLoadButton, &QPushButton::clicked,
                loadUserAction, &QAction::trigger
        );
        connect(
                d_appPages->getUserOps()->d_technicalSurveyButton, &QPushButton::clicked, 
                serveTechnicalSurveyAction, &QAction::trigger
        );
        connect(
                d_appPages->getUserOps()->d_personalSurveyButton, &QPushButton::clicked,
                servePersonalSurveyAction, &QAction::trigger
        );
        connect(
                d_appPages->getUserOps()->d_metricsButton, &QPushButton::clicked,
                getMetricsAction, &QAction::trigger
        );
        connect(
                d_appPages->getUserOps()->d_exportRecordButton, &QPushButton::clicked,
                exportUserAction, &QAction::trigger
        );
        
        /// connect action triggers to slots
        connect(aboutAction, &QAction::triggered, this, &EWIUi::about); 
        connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt); 
        connect(createUserAction, &QAction::triggered, this, &EWIUi::fireCreateUser);
        connect(exitAction, &QAction::triggered, this, &EWIUi::fireAppShutdown);
        connect(exportUserAction, &QAction::triggered, this, &EWIUi::fireExportUser);
        connect(getMetricsAction, &QAction::triggered, this, &EWIUi::fireGetMetrics);
        connect(helpAction, &QAction::triggered, this, &EWIUi::viewHelp);
        connect(loadJobAction, &QAction::triggered, this, &EWIUi::fireLoadJob);
        connect(loadUserAction, &QAction::triggered, this, &EWIUi::fireLoadUser);
        // TODO: Revise survey action sequence.
        connect(servePersonalSurveyAction, &QAction::triggered, this, [this]() { serveSurvey(PERSONAL_SURVEY); });
        connect(serveTechnicalSurveyAction, &QAction::triggered, this, [this]() { serveSurvey(TECHNICAL_SURVEY); });

        // Communication signsls
        connect(this, &EWIUi::profileLoadedSig, this, &EWIUi::profileLoaded);
        connect(this, &EWIUi::setPersonalQuestionsSig, this, &EWIUi::setPersonalQuestions);
        connect(this, &EWIUi::jobChangedSig, this, &EWIUi::jobChanged);
        connect(this, &EWIUi::sendImg, this, &EWIUi::displayImg);
        connect(this, &EWIUi::errorMsgSig, this, &EWIUi::errorMsg);

    }

    void EWIUi::setup()
    {
        d_appPages = new Views(this);

        // Disable Actions Button until complete profile is
        // loaded.
        d_appPages->d_userOpsButton->setEnabled(false);
    }

    void EWIUi::validatePtrs()
    {
        Q_ASSERT(d_appPages);
        Q_ASSERT(aboutAction);
        Q_ASSERT(aboutQtAction);
        Q_ASSERT(createUserAction);
        Q_ASSERT(exitAction);
        Q_ASSERT(exportUserAction);
        Q_ASSERT(getMetricsAction);
        Q_ASSERT(helpAction);
        Q_ASSERT(loadJobAction);
        Q_ASSERT(loadUserAction);
        Q_ASSERT(servePersonalSurveyAction);
        Q_ASSERT(serveTechnicalSurveyAction);
    }

    auto EWIUi::getUserData(bool createMode) -> QStringList
    {
       return d_appPages->getProfileLoader()
           ->loadUser(createMode); 
    }

    auto EWIUi::getDateRange() -> QVector<QDate>
    {
        QVector<QDate> dates {};
        // Create a simple dialog to gather date information
        QDialog* dateQuery { new QDialog(this) };
        QFormLayout* layout { new QFormLayout };

        QDateEdit* from { new QDateEdit(QDate::currentDate()) };
        from->setDisplayFormat("dd-MM-yyyy");
        layout->addRow(tr("From:"), from);

        QDateEdit* to { new QDateEdit(QDate::currentDate()) };
        to->setDisplayFormat("dd-MM-yyyy");
        layout->addRow(tr("To:"), to);
        dateQuery->setLayout(layout);


        QDialogButtonBox* buttons { new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel) };
        layout->addWidget(buttons);

        // Make relevant connectinos
        auto okButton = buttons->button(QDialogButtonBox::Ok);
        auto cancelButton = buttons->button(QDialogButtonBox::Cancel);
        // Only enable button when date range is valid.
        auto enableOkButton = [from, to, okButton]() -> void { 
            okButton->setEnabled(from->date() <= to->date()); 
        };
        connect(
                okButton, &QPushButton::clicked,
                dateQuery, &QDialog::accept
        );
        connect(
                cancelButton, &QPushButton::clicked,
                dateQuery, &QDialog::reject
        );
        connect(from, &QDateEdit::dateChanged, enableOkButton);
        connect(to, &QDateEdit::dateChanged, enableOkButton);

        if(dateQuery->exec())
        {
            dates.push_back(from->date());
            dates.push_back(to->date());
        }

        dateQuery->deleteLater();
        return dates;
    }

    // SLOTS
   
    // TODO: Remove this QTextStream when implementing real functions.
    QTextStream qerr { stderr };
    void EWIUi::about()
    {
        QString aboutTxt {};
        QTextStream qss { &aboutTxt };
        qss << tr("## Employee Workload Index (EWI) Tracker") << "\n\n"
            << tr("**License**: TODO") << "\n\n"
            << tr("Source Code: ") << "https://github.com/tjdwill/ewi" << "\n\n"
            << "Version " EWI_VERSION << " (Qt " << qVersion() << ')' << "\n\n"
            << "Copyright 2025 &copy; Terrance Williams" << "\n"; 
        qss.flush();

        QMessageBox about { this };
        about.setWindowTitle(tr("About EWI Tracker"));
        about.setTextFormat(Qt::MarkdownText);
        about.setText(aboutTxt);
        about.exec();
    }

    void EWIUi::displayImg(QPixmap img)
    {
        ImgViewer viewer { img, this };
        viewer.exec();
    }

    void EWIUi::enableUserOpsButton()
    {
        d_appPages->d_userOpsButton->setEnabled(true);
    }

    void EWIUi::errorMsg(QString msg)
    {
        QMessageBox::about(this, tr("Error Message"), msg);
    }

    void EWIUi::jobChanged(QStringList job_questions)
    {
        d_technicalQuestions = job_questions;
    }

    void EWIUi::profileLoaded()
    {
        enableUserOpsButton();
    }

    // TODO: The personal survey regex makes it such that this module now implicitly
    // depends on ewi::survey via ewi::PersonalSurvey. I should find a way to remove
    // this dependency. I may have to move form creation to the controller, completely
    // removing the feature from this component.
    //
    // On the other hand, even if I were to change the max and min personal response values
    // in ewi/survey.cpp, I would still need to change this file because the value is
    // encoded in the regex literal. So, moving the logic wouldn't be a net positive in
    // terms of maintenance.
    void EWIUi::serveSurvey(QString const& surveyType)
    {
        Form* survey {};
        if (surveyType == PERSONAL_SURVEY)
        {
            QRegularExpression regex { "[1-4](\\.\\d*)?|5(\\.0)?" };
            survey = { new Form(d_personalQuestions, regex, tr("Personal Survey"), this) };
        }
        else
        {
            QRegularExpression regex {"[+-]?\\d+(\\.\\d*)?"};
            survey = { new Form(d_technicalQuestions, regex, tr("Technical Survey"), this) };
        }
        connect(
                survey, &Form::responsesReady,
                this, [&surveyType, this](QStringList responses)
                {
                     emit surveyResponsesSig(responses, surveyType); 
                }
         );

        survey->exec();
        survey->deleteLater();
    }

    void EWIUi::setPersonalQuestions(QStringList questions)
    {
        d_personalQuestions = questions;
    }

    void EWIUi::viewHelp()
    {
        qerr << "Help Page" << "\n";
        qerr.flush();
    }

    // Signal-firing slots

    void EWIUi::fireAppShutdown()
    {
        // TODO: Who should be responsible for the save action?
        // This object or its controller?
        emit appShutdownSig();
    }

    void EWIUi::fireCreateUser()
    {
        QStringList userData { getUserData(true) };
        if (!userData.isEmpty())
            emit createUserSig(userData);
    }

    void EWIUi::fireExportUser()
    {
        QString pathName = QFileDialog::getSaveFileName(
                this,
                tr("Export User Data"),
                QDir::currentPath(),
                tr("Text file (*.txt)")
        );
        if (!pathName.isEmpty())
            emit exportUserSig(pathName);
    }

    void EWIUi::fireGetMetrics()
    {
        auto dates = getDateRange();
        if (!dates.empty())
            emit getMetricsSig(dates);
    }

    void EWIUi::fireLoadJob()
    {
        QString jobDefPath { d_appPages->getProfileLoader()->loadJob() };
        if (!jobDefPath.isEmpty())
            emit loadJobSig(jobDefPath);
    }

    void EWIUi::fireLoadUser() 
    {
        QStringList userData = getUserData();
        if (!userData.isEmpty())
            emit loadUserSig(userData[0]);
    }

    /*
    void EWIUi::sendSurveyResponses(QStringList responses, QString const& surveyType)
    {
        emit surveyResponsesSig(responses, surveyType);
    }
    */
} // namespace ewiQt

