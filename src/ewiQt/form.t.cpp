// form.t.cpp
// Given a successful form submission, print the data to the screen
#include "form.hpp"

#include <QApplication>
#include <QtCore>
#include <QtWidgets>


using ewiQt::Form;
class FormResponses : public QObject
{
    Q_OBJECT;

public:
    FormResponses(Form* form, QObject* parent=nullptr)
        : QObject(parent)
    {
        connect(
                form, &Form::responsesReady,
                this, &FormResponses::printResponses
        );
    }

public slots:
   void printResponses(QStringList const& responses)
   {
        QTextStream out {stdout};
        out << "\nForm Responses\n------------------\n";
        for (auto const& response : responses)
            out << response << "\n";
   };
};

auto get_questions() -> QStringList const&
{
    static QStringList questions {
    /*
        "Who",
        "What?",
        "When?",
        "Where is my super suit?",
        "Why?",
    */
        "How many?"
    };
    return questions;
}

// Include the generated MOC file here so there are no CMake build errors with AUTOMOC. 
#include "form.t.moc"

int main(int argc, char* argv[])
{
    QApplication app { argc, argv };
    Form form { get_questions() };
    FormResponses responses { &form };
    form.show();
    return app.exec();
}

