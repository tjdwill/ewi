#include "explore.hpp"

#include <string>
#include <vector>
#include <cpperrors>

namespace ewi {
    using std::string;   
    using std::vector;
    using err::Exception;

    // EmotionSurvey
    EmotionSurvey::EmotionSurvey(const SurveyResults&) {
        // TODO: Unimplemented
        throw Exception("Unimplemented Method");
    }

    const EmotionSurvey& EmotionSurvey::average() {
        static EmotionSurvey avg {};
        return avg;
    }

    double calc_ewi(
            const EEOCaseManagerSurvey& technical_survey,
            const EEOCaseManagerSurvey& technical_median,
            const EmotionSurvey& emotion_survey,
            const EmotionSurvey& emotion_median
    ) {
        // TODO: Complete the function
        double ewi {};
        // Get the technical portion of the score
        vector<double> technical_vals = technical_survey.values();
        vector<double> tech_avg = technical_median.values();
        

        vector<double> emotion_results = emotion_survey.values();
        vector<double> emotion_avg = emotion_median.values();
    
        return ewi;
    }
} // namespace ewi
