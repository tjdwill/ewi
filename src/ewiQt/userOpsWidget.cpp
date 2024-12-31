// userOpsWidget.cpp
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
#include "userOpsWidget.hpp"
//- Third-party
#include <QtWidgets>


namespace ewiQt
{
    UserOpsWidget::UserOpsWidget(QWidget* parent)
        : QWidget(parent)
    {
        d_personalSurveyButton = new QPushButton(tr("&Personal Survey"));
        d_technicalSurveyButton = new QPushButton(tr("&Technical Survey"));
        d_exportRecordButton = new QPushButton(tr("&Export User Record..."));
        d_metricsButton = new QPushButton(tr("&View Workload..."));

        QVBoxLayout* mainLayout { new QVBoxLayout };
        mainLayout->addStretch();

        QGroupBox* surveyBox { new QGroupBox(tr("Make an Entry")) };
        QHBoxLayout* SBLayout { new QHBoxLayout };
        SBLayout->addWidget(d_technicalSurveyButton);
        SBLayout->addWidget(d_personalSurveyButton);
        surveyBox->setLayout(SBLayout);
        mainLayout->addWidget(surveyBox);

        QGroupBox* metricsBox { new QGroupBox(tr("Metrics")) };
        QHBoxLayout* MBLayout { new QHBoxLayout };
        MBLayout->addWidget(d_metricsButton);
        metricsBox->setLayout(MBLayout);
        mainLayout->addWidget(metricsBox);

        
        // TODO: 30 December 2024 - disabling the export
        // metrics button in order to prevent it being
        // used. The export/load feature will need to be
        // refactored.
        /*
        QGroupBox* exportBox { new QGroupBox(tr("Export")) };
        QHBoxLayout* EBLayout { new QHBoxLayout };
        EBLayout->addWidget(d_exportRecordButton);
        exportBox->setLayout(EBLayout);
        mainLayout->addWidget(exportBox);
        */
        mainLayout->addStretch();
        setLayout(mainLayout);
        
        validatePtrs();
    }

    void UserOpsWidget::validatePtrs()
    {
        Q_ASSERT(d_exportRecordButton);
        Q_ASSERT(d_metricsButton);
        Q_ASSERT(d_personalSurveyButton);
        Q_ASSERT(d_technicalSurveyButton);
    }
} // namespace ewiQt

