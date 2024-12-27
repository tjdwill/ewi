// ewiUI.hpp
// This file defines the interface to the Graphical User Interface for the EWI project.
#ifndef INCLUDED_EWIQT_EWIUI
#define INCLUDED_EWIQT_EWIUI

#ifndef INCLUDED_QT_QMAINWINDOW
#include <QMainWindow>
#define INCLUDED_QT_QMAINWINDOW
#endif

#ifndef INCLUDED_QT_QTCORE
#include <QtCore>
#define INCLUDED_QT_QTCORE
#endif

#ifndef INCLUDED_QT_QPIXMAP
#include <QPixmap>
#define INCLUDED_QT_QPIXMAP
#endif


class QAction;
class QStackedWidget;
class QWidget;
namespace ewiQt
{

class Views;
class EWIUi : public QMainWindow
{
    Q_OBJECT;

public:
    /// In practice, it would make sense to make the controller this object's parent.
    EWIUi(QWidget* parent=nullptr);
    
    // CONSTANTS
    QString const PERSONAL_SURVEY { "Personal" };
    QString const TECHNICAL_SURVEY { "Technical" };

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
    void loadJobSig(QString jobDefPath);
    /// Send a userID to load the user's data/
    void loadUserSig(QString userID);
    /// Send responses to the controller
    void surveyResponsesSig(QStringList responses, QString const& surveyType);

    // Public Signals
    // These are signals intended to be fired by the controller to communicate with this
    // object.

    /// The user has successfully changed the current job profile.
    void jobChangedSig(QStringList jobQuestions);
    /// Both the user and the job profile have been loaded.
    void profileLoadedSig();
    /// The controller provides a set of personal survey questions to this object.
    void setPersonalQuestionsSig(QStringList questions);
    /// Receive an image to display
    void sendImg(QPixmap img);
    
private slots:
    void about();
    /// Displays the metrics to the screen
    void displayImg(QPixmap img);
    /// This method helps to ensure that user-related actions aren't available until all relevant
    /// information is provided to the application. This is enforced by the conditions of
    /// the firing signal.
    void enableUserOpsButton();
    void jobChanged(QStringList jobQuestions);
    void profileLoaded();
    void setPersonalQuestions(QStringList questions);
    /// This slot is needed in order to instantiate the survey form. 
    /// It's called in response to the controller object emitting the `serveSurveySig`
    /// signal.
    void serveSurvey(QString const& type);
    void viewHelp();

    /// Signal-firing slots
    void fireAppShutdown();
    void fireCreateUser();
    void fireExportUser();
    void fireGetMetrics();
    void fireLoadJob();
    void fireLoadUser();
    // void sendSurveyResponses(QStringList responses, QString const& surveyType);

private:
    // SETUP-RELATED FUNCTIONS
    void createActions();
    void createConnections();
    /// Perform relevant widget setup actions
    void setup();
    /// ensure all pointers are non-NULL
    void validatePtrs();

    // CONVENIENCE METHODS

    /// Create a temporary dialog to gather data for user loading or creation.
    auto getUserData(bool create=false) -> QStringList;
    auto getDateRange() -> QVector<QDate>;

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
#endif // INCLUDED_EWIUI
