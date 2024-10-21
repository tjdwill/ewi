#include <cassert>
#include <string>
#include <vector>
//
#include <cpperrors>
//
#include "explore.hpp"

namespace ewi 
{
    using std::string;   
    using std::vector;
    using err::Exception;

    // EmotionSurvey


    double calc_ewi(
            const SurveyResults& technical_survey,  
            const vector<double>& technical_avgs,
            const SurveyResults& emotion_survey,
            const vector<double>& emotion_avgs
    ) {
        // TODO: Complete the function
        double ewi {};
        // Get the technical portion of the score
        vector<double> technical_vals = technical_survey.extract_metrics();
        assert(technical_vals.size() == technical_avgs.size());


        vector<double> emotion_vals = emotion_survey.extract_metrics();
        assert(emotion_vals.size() == emotion_avgs.size());
    
        throw Exception("Unimplemented!");
        return ewi;
    }
} // namespace ewi
