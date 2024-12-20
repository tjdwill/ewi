// ewi_app.hpp
// This file defines the interface to the Graphical User Interface for the EWI project.
#ifndef INCLUDED_EWI_APP
#define INCLUDED_EWI_APP

#ifndef INCLUDED_QT_QMAINWINDOW
#include <QMainWindow>
#define INCLUDED_QT_QMAINWINDOW
#endif

#ifndef INCLUDED_QT_QTCORE
#include <QtCore>
#define INCLUDED_QT_QTCORE
#endif


class QAction;
class QStackedWidget;
class QWidget;
namespace ewiQt
{

class Views;
class EWIApp : public QMainWindow
{
    Q_OBJECT;

public:
    /// In practice, it would make sense to make the controller this object's parent.
    EWIApp(QWidget* parent=nullptr);
    
public slots:

signals:
    /// Tell the controller to shutdown the app.
    void appShutdownSig();
    /// Sends the data necessary to create a user. First element is the userID. Second is
    /// userName. I'm making this a QStringList to add flexibility in case more data is
    /// needed in the future.
    void createUserSig(QStringList userData);
    /// Send a path to save the user's data.
    void exportUserSig(QString pathName);
    /// Sends date range for metric retrieval [fromDate, toDate]
    void getMetricsSig(QVector<QDate> dates);
    /// Send the path to the selected job definition file to load.
    void loadJobSig(QString const& jobDefPath);
    /// Send a userID to load the user's data/
    void loadUserSig(QString const& userID);
    /// Send responses to the controller
    void surveyResponsesSig(QStringList responses);

    // Public Signals
    // These are signals intended to be fired by the controller to communicate with this
    // object.

    /// The user has successfully changed the current job profile.
    void jobChangedSig(QStringList jobQuestions);
    /// Both the user and the job profile have been loaded.
    void profileLoadedSig();
    /// The controller provides a set of personal survey questions to this object.
    void setPersonalQuestionsSig(QStringList questions);

    
private slots:
    void about();
    /// This slot is needed in order to instantiate the survey form. 
    /// It's called in response to the controller object emitting the `serveSurveySig`
    /// signal.
    void serveSurvey(QString const& type);
    void viewHelp();
    /// This method helps to ensure that user-related actions aren't available until all relevant
    /// information is provided to the application. This is enforced by the conditions of
    /// the firing signal.
    void enableUserOpsButton();
    /// Signal-firing slots
    void fireAppShutdown();
    void fireCreateUser();
    void fireExportUser();
    void fireGetMetrics();
    void fireLoadJob();
    void fireLoadUser();
    void jobChanged(QStringList jobQuestions);
    void profileLoaded();
    void sendSurveyResponses(QStringList responses);
    void setPersonalQuestions(QStringList questions);

private:
    // Setup related functions
    void createActions();
    void createConnections();
    /// Perform relevant widget setup actions
    void setup();
    /// ensure all pointers are non-NULL
    void validatePtrs();

    // Cnovenience methods

    /// Create a temporary dialog to gather data for user loading or creation.
    auto getUserData(bool create=false) -> QStringList;
    auto getDateRange() -> QVector<QDate>;


    // CONSTANTS
    QString const PERSONAL_SURVEY { "Personal" };
    QString const TECHNICAL_SURVEY { "Technical" };


    // Data Members
    QStringList d_technicalQuestions {};
    QStringList d_personalQuestions {};
    Views* d_appPages {};

    QAction* aboutAction {};
    QAction* aboutQtAction {};
    QAction* createUserAction {};
    QAction* exitAction {};
    QAction* exportUserAction {};
    QAction* getMetricsAction {};
    QAction* helpAction {};
    QAction* loadJobAction {};
    QAction* loadUserAction {};
    QAction* servePersonalSurveyAction {};
    QAction* serveTechnicalSurveyAction {};
};
} // namespace ewiQt
#endif // INCLUDED_EWI_APP
