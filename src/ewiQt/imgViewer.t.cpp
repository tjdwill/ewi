// imgViewer.t.cpp
#include "imgViewer.hpp"
#include <QApplication>
#include <QtWidgets>
#include <QPixmap>


using ewiQt::ImgViewer;
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        QTextStream qerr { stderr };
        qerr <<  "Usage: <appName> <path-to-source-image>" << "\n";
        qerr.flush();
        std::exit(1);
    }
    QApplication app { argc, argv };
    QPixmap img { argv[1] };
    ImgViewer viewer (img);
    
    viewer.show();

    return app.exec();
}
