// imgViewer.hpp
#ifndef INCLUDED_EWIQT_IMGVIEWER
#define INCLUDED_EWIQT_IMGVIEWER

#include <qpixmap.h>
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
