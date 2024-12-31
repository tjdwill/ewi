// views.hpp
// A QStackedWidget subclass made specifically for the EWI app.
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
#ifndef INCLUDED_EWIQT_VIEWS
#define INCLUDED_EWIQT_VIEWS

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
#endif //INCLUDED_EWIQT_VIEWS
