// userOpsWidget.t.cpp
#include "userOpsWidget.hpp"
#include <QApplication>


using ewiQt::UserOpsWidget;
int main(int argc, char* argv[])
{
    QApplication app { argc, argv };
    UserOpsWidget widget {};
    widget.show();

    return app.exec();
}
