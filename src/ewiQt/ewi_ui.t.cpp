#include "ewi_ui.hpp"
#include <QApplication>
#include <QtWidgets>
#include <qlist.h>


using ewiQt::EWIApp;

QStringList PERSONAL_Qs { "How many personal questions?" };
QStringList TECHNICAL_Qs { "What technical questions?" };
class EWIAppTestController : public QWidget
{
    Q_OBJECT;
public:
    EWIAppTestController(QWidget* parent=nullptr);
    void show();
private:
    void createConnections();

    EWIApp* d_app {};
    QTextStream qout { stdout };
};

int main(int argc, char* argv[])
{
    QApplication app { argc, argv };
    EWIAppTestController myApp {};
    myApp.show();

    return app.exec();
}

//-----------------------------TestController Implementation-----------------------------------
EWIAppTestController::EWIAppTestController(QWidget* parent)
    : QWidget(parent)
{
    d_app = new EWIApp(this);
    createConnections();

    emit d_app->setPersonalQuestionsSig(PERSONAL_Qs);
    emit d_app->jobChangedSig(TECHNICAL_Qs);
    emit d_app->profileLoadedSig();
}

void EWIAppTestController::createConnections()
{
    connect(d_app, &EWIApp::appShutdownSig, this, &EWIAppTestController::close);
    connect(
            d_app, &EWIApp::createUserSig,
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
            d_app, &EWIApp::exportUserSig,
            this, [this](QString pathName)
            {
                this->qout << "Exporting User Data to: " << pathName << "\n";            
                this->qout.flush();
            }
    );
    connect(
            d_app, &EWIApp::loadJobSig,
            this, [this](QString jobDefPath)
            {
                this->qout << "Load Job Definition from: " << jobDefPath << "\n";
                this->qout.flush();
            }
    );
    connect(
            d_app, &EWIApp::loadUserSig,
            this, [this](QString userID)
            {
                this->qout << "Load User w/ ID: " << userID << "\n";
                this->qout.flush();
            }
    );
    connect(d_app, &EWIApp::surveyResponsesSig,
            this, [this](QStringList responses)
            {
                this->qout << "Responses\n---------" << "\n";
                for (auto const& response : responses)
                    this->qout << response << "\n";
                this->qout.flush();
            }
    );
    connect(
            d_app, &EWIApp::getMetricsSig,
            this, [this](QVector<QDate> dates)
            {
                this->qout << "Get metrics from: " << dates[0].toString()
                << " to " << dates[1].toString() << "\n";
                this->qout.flush();
            }
    );
}

void EWIAppTestController::show()
{
    d_app->show();
}
#include "ewi_ui.t.moc"
