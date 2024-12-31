// views.cpp
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
#include "views.hpp"
//- Third-party
#include <QtWidgets>
//- In-house
#include "profileLoader.hpp"
#include "userOpsWidget.hpp"

namespace ewiQt
{
    Views::Views(QWidget* parent)
        : QWidget(parent)
    {
        d_pages = new QStackedWidget;

        createNavBar();
        createPages();
        // validatePtrs();
        createConnections();

        QHBoxLayout* layout { new QHBoxLayout };
        layout->addWidget(d_navBar);
        layout->addStretch();
        layout->addWidget(d_pages);
        layout->addStretch();
        setLayout(layout);
    }

    void Views::changePage(QString const& pageName)
    {
        if (d_pages->currentIndex() != d_pageIdxs[pageName])
            d_pages->setCurrentIndex(d_pageIdxs[pageName]);
    }

    void Views::createConnections()
    {
        connect(
                d_homeButton, &QPushButton::clicked,
                this, [this](){ emit changePageReq(HOME_KEY);  }
        );
        connect(
                d_profileLoaderButton, &QPushButton::clicked,
                this, [this]() { emit changePageReq(PROFILE_LOADER_KEY); }
        );
        connect(this, &Views::changePageReq, this, &Views::changePage);
        
        connect(
                d_userOpsButton, &QPushButton::clicked,
                this, [this](){ d_pages->setCurrentIndex(d_pageIdxs[USER_OPS_KEY]);  }
        );
        // connect(d_exitButton, &QPushButton::clicked, this, &Views::close);
    }

    void Views::createNavBar()
    {
        d_navBar = new QWidget;
        
        d_homeButton = new QPushButton(tr("Home"));
        d_aboutButton = new QPushButton(tr("About"));
        d_helpButton = new QPushButton(tr("Help"));
        d_profileLoaderButton = new QPushButton(tr("Profile"));
        d_userOpsButton = new QPushButton(tr("Actions"));
        d_exitButton = new QPushButton(tr("Exit"));

        QVBoxLayout* navLayout { new QVBoxLayout };
        navLayout->addWidget(d_homeButton);
        navLayout->addWidget(d_profileLoaderButton);
        navLayout->addWidget(d_userOpsButton);
        navLayout->addWidget(d_helpButton);
        navLayout->addWidget(d_aboutButton);
        navLayout->addWidget(d_exitButton);

        d_navBar->setLayout(navLayout);
    }

    void Views::createPages()
    {
        // All widgets are reparented upon setting the
        // layout of this class.


        d_pages = new QStackedWidget;
        // Create Home Page
        d_homePage = new QWidget;
        QLabel* homeTitle { new QLabel(tr("Home Page")) };
        QHBoxLayout* homeLayout { new QHBoxLayout };
        homeLayout->addWidget(homeTitle);
        d_homePage->setLayout(homeLayout);
        
        // ProfileLoader
        d_profileLoaderPage = new ProfileLoader(this);

        // User Operations Page
        d_userOpsPage = new UserOpsWidget(this);

        // Add Pages to StackedWidget
        d_pageIdxs.insert(HOME_KEY, d_pages->addWidget(d_homePage));
        d_pageIdxs.insert(PROFILE_LOADER_KEY, d_pages->addWidget(d_profileLoaderPage));
        d_pageIdxs.insert(USER_OPS_KEY, d_pages->addWidget(d_userOpsPage));
    }
}

