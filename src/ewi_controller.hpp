// ewi_controller.hpp
// The driver for the app, this class serves as the liasion between the GUI and the
// backend.
#ifndef INCLUDED_EWI_CONTROLLER
#define INCLUDED_EWI_CONTROLLER

#include "ewi/survey.hpp"
#include "ewiQt/ewiUI.hpp"
#include "ewiQt/views.hpp"
#ifndef INCLUDED_STD_OPTIONAL
#include <optional>
#define INCLUDED_STD_OPTIONAL
#endif

#ifndef INCLUDED_QT_QWIDGET
#include <QWidget>
#define INCLUDED_QT_QWIDGET
#endif

#ifndef INCLUDED_EWI_EMPLOYEE_RECORD
#include <ewi/employee_record.hpp>
#endif

namespace ewiQt
{
    class EWIUi;
}

class EWIController : public QWidget
{
    Q_OBJECT;

public:
    EWIController(QWidget* parent=nullptr);

    /// Overload the show 

public slots:

private:
    /// Check if user and job are both loaded.
    bool d_profile_loaded { false };
    std::optional<ewi::EmployeeRecord> d_user_profile {};
    std::optional<ewi::ParsedProfile> d_job_profile {};

    ewiQt::EWIUi* d_app {};
};

#endif // INCLUDED_EWI_CONTROLLER
