// userOperations.hpp
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
#ifndef INCLUDED_EWIQT_USEROPSWIDGET
#define INCLUDED_EWIQT_USEROPSWIDGET

#ifndef INCLUDED_QT_QTCORE
#include <QtCore>
#define INCLUDED_QT_QTCORE
#endif

#ifndef INCLUDED_QT_QWIDGET
#include <QWidget>
#define INCLUDED_QT_QWIDGET
#endif


class QPushButton;
namespace ewiQt
{

/// Houses options for user interaction.
class UserOpsWidget : public QWidget
{
    Q_OBJECT;

public:
    UserOpsWidget(QWidget* parent=nullptr);

    QPushButton* d_technicalSurveyButton {};
    QPushButton* d_personalSurveyButton {};
    QPushButton* d_metricsButton {};
    QPushButton* d_exportRecordButton {};
private:
    void validatePtrs();
};
} // namespace ewiQt

#endif // INCLUDED_EWIQT_USEROPSWIDGET
