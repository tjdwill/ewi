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

#ifndef INCLUDED_STD_CASSERT
#define INCLUDED_STD_CASSERT
#include <cassert>
#endif

namespace ewi
{
    using std::chrono::year_month_day;
    using std::string;   
    using std::vector;

    class ParsedProfile
    {
        private:
            int d_metrics_cnt {};
            string d_role_id;
            vector<string> d_questions;
    };
    /// Parses the data provided by the role file
    /// (*.role?)
    ParsedProfile load_profile(const string& file_path);

    /// A class that stores the answers to a given
    /// survey. At the time of writing, the results
    /// will be a vector of strings. The first
    /// element will be a string capturing the entry
    /// date. Entries 1->m are the answers to the
    /// numeric-based role questions. Finally, the
    /// last entry is extra information the user
    /// provides. 
    class SurveyResults
    {
        public:
            SurveyResults(const vector<string>& responses, int metrics_cnt)
                : d_metrics_cnt{ metrics_cnt }, d_responses{ responses } {
                    // 3 is the minimum length of a valid survey result vector.
                    // It would be [date, metric_questions, extra_info]
                    assert((int) responses.size() > metrics_cnt && metrics_cnt > 0); 
                }

            inline const vector<string>& get_responses() const { return d_responses; }
            inline const int metrics_cnt() const { return d_metrics_cnt; }

        private:
            int d_metrics_cnt;            // How many questions were number-based?
            vector<string> d_responses;   // All of the form responses
    };

    /// Represents the results of a human-factored survey that attempts to ascertain
    /// emotional components that influence perceived workload.
    /// Values range from 1 to 5.
    /// Assumes the median (normal) value of each question is three.
    class EmotionSurvey
    {
        public:
            EmotionSurvey()
                : d_leadership_score(DEFAULT_VALUE),
                d_mission_interest(DEFAULT_VALUE),
                d_challenge_score(DEFAULT_VALUE),
                d_peacefulness_score(DEFAULT_VALUE) 
            {}  // Internal Use Only; Intended to instantiate the
                // static average instance

            explicit EmotionSurvey(const SurveyResults&);

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

            static constexpr double DEFAULT_VALUE {3.};
    };


    /// EEO-specific metrics to evaluate workload.
    class EEOCaseManagerSurvey
    {
        // Job Code: 0260
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
            const SurveyResults& technical_survey,  
            const EEOCaseManagerSurvey& technical_median,
            const SurveyResults& emotion_survey,
            const EmotionSurvey& emotion_median
    );
    // template<typename T, typename U>
    // double calc_ewi<T, U>(T parsed_survey, )

} // namespace ewi
#endif // INCLUDED_EXPLORE
