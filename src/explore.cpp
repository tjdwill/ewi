#include "explore.hpp"

#include <string>
#include <vector>

namespace ewi {
    using std::string;   
    using std::vector;

    const EmotionSurvey& EmotionSurvey::average() {
        static EmotionSurvey avg {3., 3., 3., 3.};
        return avg;
    }

    double calc_ewi(
            const EEOCaseManagerSurvey& technical_survey,
            const EEOCaseManagerSurvey& technical_median,
            const EmotionSurvey& emotion_survey,
            const EmotionSurvey& emotion_median
    ) {
        // Get the technical portion of the score
        vector<double> technical_vals = technical_survey.values();
        vector<double> tech_avg = technical_median.values();
        

        vector<double> emotion_results = emotion_survey.values();
        vector<double> emotion_avg = emotion_median.values();
    

    }
} // namespace ewi
