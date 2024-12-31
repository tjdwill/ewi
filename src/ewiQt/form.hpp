// form.hpp
// A prototype for a survey widget in Qt.
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
#ifndef INCLUDED_EWIQT_FORM
#define INCLUDED_EWIQT_FORM

#include <qlist.h>
#ifndef INCLUDED_QT_QDIALOG
#include <QDialog>
#define INCLUDED_QT_QDIALOG
#endif

#ifndef INCLUDED_QT_QTCORE
#include <QtCore>  // for Q_OBJECT macro
#define INCLUDED_QT_QTCORE
#endif


class QDateEdit;
class QLabel;
class QLineEdit;
class QDialogButtonBox;
class QTextEdit;

namespace ewiQt
{
class Form : public QDialog
{
    Q_OBJECT;

public:
    Form() = delete;
    Form(
            QStringList const& questions,
            QString const& title="Survey Form",
            QWidget* parent=nullptr
    );
    Form(
            QStringList const& questions,
            QRegularExpression const& inputFilter,
            QString const& title="Survey Form",
            QWidget* parent=nullptr
    );

signals:
    void responsesReady(QStringList);

public slots:
    /// Retrieve all form values in a form that is
    /// compatible with the lower-level infrastructure.
    QStringList getResponses();

private slots:
    /// Perform actions associated with a valid form
    /// submission.  Specifically, update this object's
    /// `d_results` QVector, hide this widget, and set
    /// the result code to `Accepted`.
    void accept();
    /// Only enable interactivity with Submit button when
    /// all input is valid.
    void enableSubmitButton();
    /// Reject the form; discarding all input.
    //void reject();

private:
    /// Handles the creation of the form in terms of widget placement.
    auto createForm(QStringList const& questions) -> bool;
    /// Determine if all form entries have valid input
    auto isValid() -> bool;
    /// Call this on setup to ensure all pointers are non-null.
    auto ptrsValid() -> bool;
    /// Establish all connections in one place
    void setConnections();

    QStringList d_results {};
    QRegularExpression d_regex {};
    QVector<QLineEdit*> d_answers {};
    QDateEdit* d_date;
    QTextEdit* d_notes;
    QDialogButtonBox* d_submit;
};
} // namespace ewiQt
#endif // INCLUDED_EWIQT_FORM
