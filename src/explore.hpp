#ifndef INCLUDED_EXPLORE
#define INCLUDED_EXPLORE

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
    /// The output of the parsed role file.
    class ParsedProfile
    {
        public:
            explicit ParsedProfile(std::string role_id, std::vector<std::string> questions, std::vector<double> averages) noexcept
                : d_role_id{ role_id }, d_role_questions{ questions }, d_averages{ averages }
            {}

            /// Returns how many numeric components there are.
            inline int metrics_cnt() const { return (int) d_role_questions.size(); }
            inline const std::string& role_id() { return d_role_id; }
            inline const std::vector<std::string>& questions() { return d_role_questions; }
            inline const std::vector<double> averages() { return d_averages; }
        private:
            std::string d_role_id;
            std::vector<std::string> d_role_questions;
            std::vector<double> d_averages;  // The average value for each metric.

            ParsedProfile(); // UNIMPLEMENTED
    };
    /// Parses the data provided by the role file
    /// (*.role?)
    ParsedProfile load_profile(const std::string& file_path);

    /// A class that stores the answers to a given survey. At
    /// the time of writing, the results will be a std::vector of
    /// std::strings. The first element will be a std::string capturing
    /// the entry date. Entries 1->m are the answers to the
    /// numeric-based role questions. Finally, the last entry
    /// is extra information the user provides. 
    class SurveyResults
    {
        public:
            SurveyResults(const std::vector<std::string>& responses, int metrics_cnt)
                : d_metrics_cnt{ metrics_cnt }, d_responses{ responses } {
                    // 3 is the minimum length of a valid survey result std::vector.
                    // It would be [date, metric_questions, extra_info]
                    assert((int) responses.size() > metrics_cnt && metrics_cnt > 0); 
                }

            inline const std::vector<std::string>& get_responses() const { return d_responses; }
            inline const int metrics_cnt() const { return d_metrics_cnt; }
            std::vector<double> extract_metrics() const;

        private:
            int d_metrics_cnt;            // How many questions were number-based?
            std::vector<std::string> d_responses;   // All of the form responses
    };

    /// Represents the results of a human-factored survey that attempts to ascertain
    /// emotional components that influence perceived workload.
    /// Values range from 1 to 5.
    /// Assumes the median (normal) value of each question is three.
    static std::vector<std::string> EMOTION_SURVEY {
        "How much confidence do you have in your direct leadership?",
        "How passionate are you about the department's current mission?",
        "How much do you think your work challenges you?",
        "In general, how peaceful is your life?"
    };

    /// EEO-specific metrics to evaluate workload.
    /// Job Code: 0260
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
    /// TODO: Is it "proper" to specifically encode the
    /// implementation type of the median values?
    double calc_ewi(
            const SurveyResults& technical_survey,  
            const std::vector<double>& technical_median,
            const SurveyResults& emotion_survey,
            const std::vector<double>& emotion_avgs
    );

} // namespace ewi
#endif // INCLUDED_EXPLORE
