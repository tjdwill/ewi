// userOpsWidget.cpp
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

        QGroupBox* exportBox { new QGroupBox(tr("Export")) };
        QHBoxLayout* EBLayout { new QHBoxLayout };
        EBLayout->addWidget(d_exportRecordButton);
        exportBox->setLayout(EBLayout);
        mainLayout->addWidget(exportBox);
        
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

