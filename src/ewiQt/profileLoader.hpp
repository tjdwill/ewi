// profileLoader.hpp
// A widget for gathering the information needed to start an EWI session.
#ifndef INCLUDED_EWIQT_PROFILELOADER
#define INCLUDED_EWIQT_PROFILELOADER

#ifndef INCLUDED_QT_QWIDGET
#include <QWidget>
#define INCLUDED_QT_QWIDGET
#endif

#ifndef INCLUDED_QT_QDIALOG
#include <QDialog>
#define INCLUDED_QT_QDIALOG
#endif

#ifndef INCLUDED_QT_QTCORE
#include <QtCore>
#define INCLUDED_QT_QTCORE
#endif


class QPushButton;
namespace ewiQt
{

/// A Widget that allows the user to load/create a user profile and/or load a job profile.
/// TODO: Add ability to create a job survey.  For now, however, the job of this class
/// is to present a QFileDialog and retrieve the path to the job.
class ProfileLoader : public QWidget
{
    Q_OBJECT;

public:
    ProfileLoader(QWidget* parent=nullptr);
    
    /// Get the user to provide a path to a job profile definition file.  If the
    /// QString returned is Null, the user cancelled the operation.  Qt functions
    /// treat nulled strings as empty strings (exception is the QString::isNull
    /// method), so just check if the received string is empty.
    auto loadJob() -> QString;
    /// A convenience function to create the user-data-gathering dialog.
    /// If the bool parameter is set to `true`, the function adapts the presented
    /// form accordingly to collect additional data for user creation.
    auto loadUser(bool createMode=false) -> QStringList;

    // Data members
    QPushButton* d_jobLoadButton {};
    QPushButton* d_userLoadButton {};
    QPushButton* d_userCreateButton {};

private:
    /// Create all sub-components and organize them.
    void createLayout();

};

} // namespace ewiQt

#endif // INCLUDED_EWIQT_PROFILELOADER
