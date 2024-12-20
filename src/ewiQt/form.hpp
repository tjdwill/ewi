// form.hpp
// A prototype for a survey widget in Qt.
#ifndef INCLUDED_QT_SURVEY
#define INCLUDED_QT_SURVEY

#ifndef INCLUDED_QT_QDIALOG
#include <QDialog>
#define INCLUDED_QT_QDIALOG
#endif

#ifndef INCLUDED_QT_CORE
#include <QtCore>  // for Q_OBJECT macro
#define INCLUDED_QT_CORE
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
    QVector<QLineEdit*> d_answers {};
    QDateEdit* d_date;
    QTextEdit* d_notes;
    QDialogButtonBox* d_submit;
};
} // namespace ewiQt
#endif // INCLUDED_QT_SURVEY
