// profileLoader.cpp
// A widget for gathering the information needed to start an EWI session.
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
#include "profileLoader.hpp"
//- Third-party
#include <QtWidgets>


namespace ewiQt
{
    ProfileLoader::ProfileLoader(QWidget* parent)
        : QWidget(parent)
    {
        createLayout();
    }
    
    void ProfileLoader::createLayout()
    {
        d_jobLoadButton = new QPushButton(tr("Select Job..."));
        d_userLoadButton = new QPushButton(tr("Load..."));
        d_userCreateButton = new QPushButton(tr("Create"));

        QGroupBox* userGroup { new QGroupBox(tr("User Profile")) };
        QHBoxLayout* userButtons { new QHBoxLayout };
        userButtons->addWidget(d_userLoadButton);
        userButtons->addWidget(d_userCreateButton);
        // userButtons->addStretch(1);
        userGroup->setLayout(userButtons);

        QGroupBox* jobGroup { new QGroupBox(tr("Job Profile")) };
        QVBoxLayout* jobButtons { new QVBoxLayout };
        jobButtons->addWidget(d_jobLoadButton);
        //jobButtons->addStretch(0);
        jobGroup->setLayout(jobButtons);

        QVBoxLayout* mainLayout { new QVBoxLayout };
        mainLayout->addStretch();
        mainLayout->addWidget(userGroup);
        mainLayout->addWidget(jobGroup);
        mainLayout->addStretch();
        setLayout(mainLayout);

        setWindowTitle(tr("Profile Loader"));
    }

    auto ProfileLoader::loadJob(QString initialDir) -> QString
    {
       QString job_file = QFileDialog::getOpenFileName(
                this, 
                tr("Select a Job Definition File"),
                initialDir, 
                "Text files (*.txt)"
       );
       return job_file;
    }

    auto ProfileLoader::loadUser(bool createMode) -> QStringList
    {
        QStringList userData {};
        QVector<QLineEdit*> fields {};

        // create dialog
        QDialog* userDialog { new QDialog(this) };

        QDialogButtonBox* submitButtons { new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel) };
        auto okButton = submitButtons->button(QDialogButtonBox::Ok);
        okButton->setEnabled(false);

        QFormLayout* layout { new QFormLayout };
        QLineEdit* userIDEdit { new QLineEdit };
        fields.push_back(userIDEdit);
        layout->addRow(tr("User ID:"), userIDEdit);


        if (createMode) {
            // Add Additional Field(s)
            QLineEdit* userNameEdit { new QLineEdit };
            fields.push_back(userNameEdit);
            layout->addRow(tr("Name:"), userNameEdit);
        }
        layout->addWidget(submitButtons);
        userDialog->setLayout(layout);

        // Define Lambdas for connections
        auto isFilled = [&fields]() {
            for (auto const& field: fields)
                if (field->text().isEmpty())
                    return false;
            return !fields.isEmpty();
        };
        auto enableSubmitButton = [okButton, &isFilled](){
            okButton->setEnabled(isFilled());
        };

        // Set connections
        for (auto const& field: fields)
            connect(field, &QLineEdit::textChanged, okButton, enableSubmitButton);
        connect(
                userDialog, 
                &QDialog::accepted, 
                [&userData, &fields]() mutable {
                    for (auto const& field : fields)
                        userData.push_back(field->text());
                }
        );
        connect(okButton, &QPushButton::clicked, userDialog, &QDialog::accept);
        connect(
                submitButtons->button(QDialogButtonBox::Cancel), &QPushButton::clicked,
                userDialog, &QDialog::reject
        );

        // Block until user submits dialog
        userDialog->exec();
        userDialog->deleteLater();
        return userData;
    }
} // namespace ewiQt

