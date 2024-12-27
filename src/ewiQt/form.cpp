// survey.cpp
#include "form.hpp"
//- Third-party
#include <QtGlobal>
#include <QtWidgets>


namespace ewiQt
{
Form::Form(
        QStringList const& questions,
        QString const& title,
        QWidget* parent
)
    : QDialog(parent)
{
    bool test = createForm(questions);
    Q_ASSERT(test);
          
    // connect slots and signals
    setConnections();

    // final configs
    setWindowTitle(title);
    //setMinimumSize(sizeHint());
}

Form::Form(
        QStringList const& questions,
        QRegularExpression const& inputFilter,
        QString const& title,
        QWidget* parent
)
    : QDialog(parent), d_regex(inputFilter)
{
    bool test = createForm(questions);
    Q_ASSERT(test);
          
    // connect slots and signals
    setConnections();

    // final configs
    setWindowTitle(title);
    //setMinimumSize(sizeHint());
}


QStringList Form::getResponses()
{
    return d_results;
}

void Form::accept()
{
    // Retrieve Data
    Q_ASSERT(d_results.isEmpty());
    auto date = d_date->date().toString("yyyy-MM-dd");
    d_results.push_back(date);
    for (auto const& answer : d_answers)
        d_results.push_back(answer->text());
    d_results.push_back(d_notes->toPlainText());
    
    emit responsesReady(d_results);
    // Call parent method
    QDialog::accept();
}

void Form::enableSubmitButton()
{
    d_submit->button(QDialogButtonBox::Ok)
        ->setEnabled(this->isValid());
}

auto Form::createForm(QStringList const& questions) -> bool
{

    QFormLayout* mainLayout { new QFormLayout };
    d_notes = new QTextEdit;
    d_date = new QDateEdit;
    d_submit = new QDialogButtonBox;

    // Create form rows consisting of the question and the answer
    // field.
    d_date->setDisplayFormat("dd-MM-yyyy");
    d_date->setDate(QDate::currentDate());
    mainLayout->addRow(tr("Date (dd-mm-yyyy):"), d_date);
    // Create question and answers.
    // Each line must accept only numeric input.
    QValidator* validator { new QRegularExpressionValidator(
            QRegularExpression(d_regex),
            this
    )};
    for (auto const& question : questions)
    {
        QLineEdit* field { new QLineEdit };
        field->setValidator(validator);
        d_answers.push_back(field);
        mainLayout->addRow(question, field);
    }
    
    mainLayout->addRow(tr("Additional Notes"), d_notes); // anonymous label
    // Customize Submit button
    d_submit->addButton(QDialogButtonBox::Cancel);
    d_submit->addButton(QDialogButtonBox::Ok)
        ->setText(tr("Submit"));
    d_submit->button(QDialogButtonBox::Ok)
        ->setEnabled(false);
    mainLayout->addWidget(d_submit);
    setLayout(mainLayout);

    return ptrsValid();
}

auto Form::isValid() -> bool
{
    for (auto const& field : d_answers)
    {
        if (!(field->hasAcceptableInput()))
            return false;
    }
    return true;
}

auto Form::ptrsValid() -> bool
{
    if (!d_date || !d_notes || !d_submit)
        return false;
    for (auto const& ptr: d_answers)
        if (!ptr)
            return false;
    return true;
}

void Form::setConnections()
{
   connect(d_submit, &QDialogButtonBox::accepted, this, &Form::accept);
   connect(d_submit, &QDialogButtonBox::rejected, this, &Form::reject);

   for (auto const& field: d_answers)
       connect(
           field, &QLineEdit::textChanged,
           this, &Form::enableSubmitButton
       );
}
} // namespace ewiQt
