// form.t.cpp
// Given a successful form submission, print the data to the screen
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

