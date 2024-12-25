// imgViewer.cpp
#include "imgViewer.hpp"
//- Third-party
#include <QtWidgets>
#include <QtGui>



namespace ewiQt
{
    ImgViewer::ImgViewer(QPixmap img, QWidget* parent)
        : QDialog(parent), d_img(img)
    {
        d_exportbutton = new QPushButton(tr("Export..."));
        d_okButton = new QPushButton(tr("Ok"));

        QHBoxLayout* btnLayout { new QHBoxLayout };
        btnLayout->addWidget(d_exportbutton);
        btnLayout->addStretch();
        btnLayout->addWidget(d_okButton);

        QLabel* picture { new QLabel };
        picture->setPixmap(d_img);
        picture->setScaledContents(true);

        QVBoxLayout* mainLayout { new QVBoxLayout };
        mainLayout->addWidget(picture);
        mainLayout->addLayout(btnLayout);
        setLayout(mainLayout);
        // Add connection(s)
        connect(d_exportbutton, &QPushButton::clicked, this, &ImgViewer::exportImg);
        connect(d_okButton, &QPushButton::clicked, this, &ImgViewer::accept);
    }

    void ImgViewer::exportImg()
    {
        // Get file path
        QString savePath { QFileDialog::getSaveFileName(
                this, 
                tr("Save Image"),
                "./img.png",
                "Image files (*.jpg, *.png)"
        )};
        if (savePath.isEmpty())
            return;

        // Save the Image
        if(!d_img.save(savePath))
        {
            QMessageBox err { this };
            err.setText(tr("File could not be saved due to unknown error."));
            err.setInformativeText(tr("One potential cause is invalid file type (use either *.jpg or *.png)."));
            err.exec();
        }
        else 
        {
            QMessageBox success { this };
            success.setText(tr("Success."));
            success.exec();
        }
    }
}
