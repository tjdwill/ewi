#include "ewiUI.hpp"
#include <QApplication>
#include <QtWidgets>
#include <QPixmap>

using ewiQt::EWIUi;

QStringList PERSONAL_Qs { "How many personal questions?" };
QStringList TECHNICAL_Qs { "What technical questions?" };
class EWIUiTestController : public QWidget
{
    Q_OBJECT;

public:
    EWIUiTestController(QWidget* parent=nullptr);

private:
    void createConnections();

    EWIUi* d_app {};
    QTextStream qout { stdout };
};

int main(int argc, char* argv[])
{
    QApplication app { argc, argv };
    EWIUiTestController myApp {};
    myApp.show();

    return app.exec();
}

//-----------------------------TestController Implementation-----------------------------------
EWIUiTestController::EWIUiTestController(QWidget* parent)
    : QWidget(parent)
{
    d_app = new EWIUi();
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(d_app);
    setLayout(layout);
    createConnections();

    emit d_app->setPersonalQuestionsSig(PERSONAL_Qs);
    emit d_app->jobChangedSig(TECHNICAL_Qs);
    emit d_app->profileLoadedSig();

}

void EWIUiTestController::createConnections()
{
    connect(d_app, &EWIUi::appShutdownSig, this, &EWIUiTestController::close);
    connect(
            d_app, &EWIUi::createUserSig,
            this, [this](QStringList data)
            {
                qout << "Create User w/ Data: [";
                for (auto const& item : data)
                    this->qout << item << "; ";
                this->qout << "]" << "\n";
                this->qout.flush();
            }
    );
    connect(
            d_app, &EWIUi::exportUserSig,
            this, [this](QString pathName)
            {
                this->qout << "Exporting User Data to: " << pathName << "\n";            
                this->qout.flush();
            }
    );
    connect(
            d_app, &EWIUi::loadJobSig,
            this, [this](QString jobDefPath)
            {
                this->qout << "Load Job Definition from: " << jobDefPath << "\n";
                this->qout.flush();
            }
    );
    connect(
            d_app, &EWIUi::loadUserSig,
            this, [this](QString userID)
            {
                this->qout << "Load User w/ ID: " << userID << "\n";
                this->qout.flush();
            }
    );
    connect(d_app, &EWIUi::surveyResponsesSig,
            this, [this](QStringList responses, QString const& surveyType)
            {
                this->qout << "Responses\n---------" << "\n";
                this->qout << "Survey Type: " << surveyType << "\n";
                for (auto const& response : responses)
                    this->qout << response << "\n";
                this->qout.flush();
            }
    );
    connect(
            d_app, &EWIUi::getMetricsSig,
            this, [this](QVector<QDate> dates)
            {
                this->qout << "Get metrics from: " << dates[0].toString()
                << " to " << dates[1].toString() << "\n";
                this->qout.flush();

                // Test display function
                //  PROJECT_SOURCE_DIR defined in CMake target
                QPixmap img { PROJECT_SOURCE_DIR "/test_resources/ewiQt/test_ewi.png" };
                emit d_app->sendImg(img);
            }
    );
}

#include "ewiUI.t.moc"
