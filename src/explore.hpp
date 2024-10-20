#ifndef INCLUDED_EXPLORE
#define INCLUDED_EXPLORE

#ifndef INCLUDED_CHRONO
#define INCLUDED_CHRONO
#include <chrono>
#endif 

#ifndef INCLUDED_STD_VECTOR
#define INCLUDED_STD_VECTOR
#include <vector>
#endif

#ifndef INCLUDED_STD_STRING
#define INCLUDED_STD_STRING
#include <string>
#endif
namespace ewi
{

    using std::chrono::year_month_day;

    class SurveyResults; // alias to a vector of strings?

    /// Represents the results of a human-factored survey that attempts to ascertain
    /// emotional components that influence perceived workload.
    /// Values range from 1 to 5.
    /// Assumes the median (normal) value of each question is three.
    class EmotionSurvey
    {
        public:
            EmotionSurvey();  // UNIMPLEMENTED
            explicit EmotionSurvey(const SurveyResults&);
            explicit EmotionSurvey(double leadership, double mission, double challenge, double peace)
                : d_leadership_score(leadership),
                d_mission_interest(mission),
                d_challenge_score(challenge),
                d_peacefulness_score(peace) 
            {}


            /// return a reference to the static, idealized average
            const EmotionSurvey& average();

            inline std::vector<double> values() const {
                return std::vector<double> {
                    d_leadership_score,  
                    d_mission_interest,  
                    d_challenge_score,  
                    d_peacefulness_score,
                };
            }
            
        private:
            double d_leadership_score;       // How confident are you in your direct leadership?
            double d_mission_interest;       // How interested are you in the department's mission?
            double d_challenge_score;        // How much do you think your work challenges you?
            double d_peacefulness_score;     // How peaceful is your life in general?
    };


    /// EEO-specific metrics to evaluate workload.
    class EEOCaseManagerSurvey
    {
        public:
            EEOCaseManagerSurvey();          // UNIMPLEMENTED
            explicit EEOCaseManagerSurvey(const SurveyResults&);

            // convenience method that allows the metrics to be compared
            // to another entry (i.e. the average)
            inline std::vector<double> values() const {
                return std::vector<double> {
                    d_correspondence_time,
                    d_meeting_time,
                    d_active_cases,
                    d_contacts_made,
                    d_unrelated_tasks,
                };
            }

        private:
            double d_correspondence_time;    // How long did you spend on communication (email, phone, in-person consultation, etc.)?
            double d_meeting_time;           // How long did you spend in meetings?
            double d_active_cases;           // How many active cases do you have?
            double d_contacts_made;          // Number of contacts made.
            double d_unrelated_tasks;        // How many tasks were you asked to complete that were unrelated to your role?
    };

    /// Calculates an estimate of the employee's workload perception.
    /// Assume that the median and the entries match in terms of element
    /// quantity.
    double calc_ewi(
            const EEOCaseManagerSurvey& technical_survey,  
            const EEOCaseManagerSurvey& technical_median,
            const EmotionSurvey& emotion_survey,
            const EmotionSurvey& emotion_median
    );
    //template<typename T, typename U>
    //`double calc_ewi<T, U>(T parsed_survey, )

} // namespace ewi
#endif // INCLUDED_EXPLORE
