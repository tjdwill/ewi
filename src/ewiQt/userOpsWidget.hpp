// userOperations.hpp
#ifndef INCLUDED_EWI_USEROPSWIDGET
#define INCLUDED_EWI_USEROPSWIDGET

#ifndef INCLUDED_QT_QTCORE
#include <QtCore>
#define INCLUDED_QT_QTCORE
#endif

#ifndef INCLUDED_QT_QWIDGET
#include <QWidget>
#define INCLUDED_QT_QWIDGET
#endif


class QPushButton;
namespace ewiQt
{

/// Houses options for user interaction.
class UserOpsWidget : public QWidget
{
    Q_OBJECT;

public:
    UserOpsWidget(QWidget* parent=nullptr);

    QPushButton* d_technicalSurveyButton {};
    QPushButton* d_personalSurveyButton {};
    QPushButton* d_metricsButton {};
    QPushButton* d_exportRecordButton {};
private:
    void validatePtrs();
};
} // namespace ewiQt

#endif // INCLUDED_EWI_USEROPSWIDGET
