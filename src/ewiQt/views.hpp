// views.hpp
// A QStackedWidget subclass made specifically for the EWI app.
// The important aspect that warrants this creation is the signals and slots.
// I don't want the top-level interface to communicate with inner-widgets of a given page,
// so this class serves to emit signals that pass data up the chain. At the time of
// writing, I'm unaware of the more idiomatic way to do this.
#ifndef INCLUDED_VIEWS
#define INCLUDED_VIEWS

#ifndef INCLUDED_QT_QTCORE
#include <QtCore>
#define INCLUDED_QT_QTCORE
#endif

#ifndef INCLUDED_QT_QWIDGET
#include <QWidget>
#define INCLUDED_QT_QWIDGET
#endif 

class QStackedWidget;
class QPushButton;
namespace ewiQt
{

class UserOpsWidget;
class ProfileLoader;
/// This class forms the bulk of the clickable UI for the app. 
/// It houses multiple pages and coordinates the switching between them.
/// In order to allow necessary connections with higher-level components, it exposes
/// pointers to important child widgets. This way, potentially-unwieldy signal propagation
/// is precluded.
class Views : public QWidget
{
    Q_OBJECT;

public:
    Views(QWidget* parent=nullptr);
    inline auto getProfileLoader() const -> ProfileLoader* { return d_profileLoaderPage; }
    inline auto getUserOps() const -> UserOpsWidget* { return d_userOpsPage; }

    // public data members (for access)
    QPushButton* d_homeButton {};
    QPushButton* d_aboutButton {};
    QPushButton* d_helpButton {};
    QPushButton* d_profileLoaderButton {};
    QPushButton* d_userOpsButton {};
    QPushButton* d_exitButton {};

private slots:
    /// Switch to showing the specified page.
    void changePage(QString const& pageName);

signals:
    // Define necessary signals to enable data propagation.
    void changePageReq(QString const& pageName);

private:
    void createConnections();
    void createNavBar();
    void createPages();
    void validatePtrs();

    // Data Members
    QWidget* d_navBar {};

    QMap<QString, int> d_pageIdxs {};
    QStackedWidget* d_pages {};
    QWidget* d_homePage {};
    ProfileLoader* d_profileLoaderPage {};
    UserOpsWidget* d_userOpsPage {};

    // Define Keys for page Map
    QString const HOME_KEY { "Home" };
    QString const PROFILE_LOADER_KEY { "Profile Loader" };
    QString const USER_OPS_KEY { "User Operations" };
};

} // namespace ewiQt
#endif //INCLUDED_VIEWS
