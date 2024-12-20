// views.t.cpp
#include "views.hpp"
#include <QApplication>


using ewiQt::Views;
int main(int argc, char* argv[])
{
    QApplication app { argc, argv };
    Views view_test {};
    view_test.show();
    return app.exec();
}
