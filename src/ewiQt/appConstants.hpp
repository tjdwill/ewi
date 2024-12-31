// appConstants.hpp
// Defining useful constants that the app uses such as
// directories or numeric values.

#ifndef INCLUDED_EWIQT_APPCONSTANTS
#define INCLUDED_EWIQT_APPCONSTANTS

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
#endif 

#ifndef INCLUDED_QT_QTCORE
#include <QtCore>
#define INCLUDED_QT_QTCORE
#endif

namespace ewiQt
{
    struct AppConstants
    {
        static QString const FILE_EXT;
        // Internal App Directories
        static QString const USR_DIR; // Stores user profiles
        static QString const TMP_DIR; // For internal operations
        static QString const JOB_DIR; // Stores job profiles
        static QStringList const APP_DIRS;
        // File name (stem + extension) for saved image tmp file. 
        static QString const PLOT_FILE;
        
        /// Where the executable is located
        static auto getExeDir() -> QString; 
        /// Get the path to the user profile.
        static auto getUserPath(QString const& userID) -> QString;
        static auto getUserPath(std::string const& user_id) -> QString;
        /// Get path to temporary directory
        static auto getTmpDir() -> QString;
        /// Get path to job directory
        static auto getJobDir() -> QString;
        /// Defines path to store the generated plot for display.
        static auto getPlotFile() -> QString;
    };
}

#endif
