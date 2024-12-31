// imgViewer.t.cpp
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
#include "imgViewer.hpp"
//- Third-party
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
