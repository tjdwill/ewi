// imgViewer.hpp
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
#ifndef INCLUDED_EWIQT_IMGVIEWER
#define INCLUDED_EWIQT_IMGVIEWER

#ifndef INCLUDED_QT_QPIXMAP
#include <QPixmap>
#define INCLUDED_QT_QPIXMAP
#endif

#ifndef INCLUDED_QT_QDIALOG
#include <QDialog>
#define INCLUDED_QT_QDIALOG
#endif


class QWidget;
class QPushButton;
namespace ewiQt
{

class ImgViewer : public QDialog
{
    Q_OBJECT;

public:
    ImgViewer(QPixmap img, QWidget* parent=nullptr);
    
    QPushButton* d_exportbutton {};
    QPushButton* d_okButton {};

public slots:
    void exportImg();

private:
    QPixmap d_img;
};

} // namespace ewiQt

#endif // INCLUDED_EWIQT_IMGVIEWER
