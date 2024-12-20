// profileLoader.t.cpp
#include "profileLoader.hpp"
#include <QApplication>

using ewiQt::ProfileLoader;
int main(int argc, char* argv[])
{
    QApplication app { argc, argv };
    ProfileLoader prof {};
    prof.show();

    return app.exec();
}
