// ewi_tracker.cpp
// The entire application
#include "ewi_controller.hpp"
//-
#include <QApplication>


int main(int argc, char* argv[])
{
    QApplication app { argc, argv };

    EWIController ewiApp {};
    ewiApp.show();

    return app.exec();
}
