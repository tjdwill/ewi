#ifndef INCLUDED_EXPLORE
#define INCLUDED_EXPLORE

#ifndef INCLUDED_STD_CASSERT
#include <cassert>
#define INCLUDED_STD_CASSERT
#endif

#ifndef INCLUDED_STD_CHRONO
#include <chrono>
#define INCLUDED_STD_CHRONO
#endif

#ifndef INCLUDED_STD_MAP
#include <map>
#define INCLUDED_STD_MAP
#endif

#ifndef INCLUDED_STD_OPTIONAL
#include <optional>
#define INCLUDED_STD_OPTIONAL
#endif

#ifndef INCLUDED_STD_STRING
#include <string>
#define INCLUDED_STD_STRING
#endif

#ifndef INCLUDED_STD_VECTOR
#include <vector>
#define INCLUDED_STD_VECTOR
#endif

#ifndef INCLUDED_EIGEN
#include <Eigen/Eigen>
#define INCLUDED_EIGEN
#endif

namespace ewi
{
    /// Stores statistics for a given set of metrics.
    struct MetricStats
    {
        public:
            Eigen::VectorXd d_averages;  // mean value for each metric
            std::optional<Eigen::VectorXd> d_deviations {};

            // CONSTRUCTORS
            // TODO: Check semantics of Eigen objects. Pass-by-value, reference, or move?
            MetricStats(Eigen::VectorXd averages, Eigen::VectorXd deviations)
                : d_averages { std::move(averages) }, d_deviations { std::move(deviations) }
            {
                assert(d_deviations);
                assert(d_averages.size() == d_deviations->size()); 
            }

            MetricStats(Eigen::VectorXd averages)
                : d_averages { std::move(averages) } {}

            // ACCESSORS
            inline auto averages() const -> Eigen::VectorXd const& { return d_averages; }
            inline auto deviations() const -> std::optional<Eigen::VectorXd> const& { return d_deviations; }
        private:
            MetricStats() = delete; 
    };

    /// A class for representing identification.
    /// Includes unambiguous data member (d_id)
    /// and a more "human-readable" member (d_name).
    struct AbstractID
    {
        std::string d_id;
        std::string d_name;
    };
    struct JobID : AbstractID {};
    struct EmployeeID : AbstractID {};

    /// A given data entry. Can be for either 
    /// job-related surveys or emotion surveys.
    struct Entry 
    {
        public:
            // CONSTRUCTORS
            Entry() = delete;
            Entry(
                    std::chrono::year_month_day date,
                    std::string const& notes,
                    Eigen::VectorXd const& entry_data
            ) noexcept
                : d_date(date), d_notes(std::move(notes)), d_data(std::move(entry_data)) {}
                
            Entry(
                    std::chrono::year_month_day date,
                    std::string const& notes,
                    std::vector<double> const& entry_data
            );


            inline auto date() const noexcept -> std::chrono::year_month_day const& { return d_date; }
            inline auto notes() const noexcept -> std::string const& { return d_notes; }
            inline auto data() const noexcept -> Eigen::VectorXd const& { return d_data; }

            friend auto operator<=>(Entry const& a, Entry const& b) noexcept { return a.d_date <=> b.d_date; }; // The dates are equal.
            friend auto operator==(Entry const& a, Entry const& b) noexcept -> bool = default;  // all data members are equal.
        private:
            std::chrono::year_month_day d_date;
            std::string d_notes;
            Eigen::VectorXd d_data;      
    };
                                                      

    /// A collection of entries
    class Record 
    {
        public:
            // CONSTRUCTORS
            Record(std::vector<Entry>& entries) noexcept
                : d_entries{ std::move(entries) } {
                    assert(!d_entries.empty());
                }

            // ACCESSORS and METHODS
            
            // returns the total entry count
            inline auto num_entries() const noexcept -> int { return (int) d_entries.size(); }
            inline auto entries() const noexcept -> std::vector<Entry> const& { return d_entries; }
            // Replace exisiting entry with a new one.
            void update_entry(Entry const&);
            // Inserts an entry to the record.
            void add_entry(Entry const&);
            // Sorts entries by date (least to most recent).
            void sort() noexcept;

        private:
            std::vector<Entry> d_entries {};
    };
    // Generate statistics for the given date
    // range [from, until)
    auto get_stats(
            Record const& record,
            std::chrono::year_month_day const& from,
            std::chrono::year_month_day const& until
    ) -> MetricStats;

    /// Represents an employee's entire survey information
    /// for a given job. 
    struct JobRecord 
    {
        public:
            Record d_job;
            Record d_emotion;

            JobRecord() = delete;
            // TODO: Assert that the two records aren't
            // equal
            JobRecord(Record& job_record, Record& emotion_record);
    };

    /// Represents an employee's entire workload record for a given company.
    /// This type is meant to enable tracking the employee's workload through
    /// multiple positions in their tenure.
    struct EmployeeRecord 
    {
        public:
            EmployeeID d_employee;
            std::map<JobID, JobRecord> d_records;

            EmployeeRecord() = delete;
    };

    /// Export record data to a file
    void export_employee(EmployeeRecord const&, std::string path="");
    /// load employee from file
    EmployeeRecord load_employee(std::string id_string);

    /// Emotion Survey Related things
    // This could be a class, but I'm making it a namespace
    // because I don't need to create an object.
    namespace Emotion 
    {
        /// Represents the results of a human-factored survey that attempts to ascertain
        /// emotional components that influence perceived workload.
        /// Values range from 1 to 5.
        /// Assumes the median (normal) value of each question is three.
        
        static std::vector<std::string> SURVEY {
            "How much confidence do you have in your direct leadership?",
            "How passionate are you about the department's current mission?",
            "How much do you think your work challenges you?",
            "In general, how peaceful is your life?"
        };
        // Define survey answer extremes
        // If you change the range, ensure there are an odd number of values
        // such that the mean is a whole number.
        int constexpr MIN_VAL = { 1 };
        int constexpr MAX_VAL { 5 };
        int constexpr IDEAL_MEAN { (MIN_VAL + MAX_VAL)/2 };
    } // namespace Emote

    /// The output of the parsed job file.
    /// Contains:
    ///     - Role ID
    ///     - The role-specific questions
    ///     - Estimated Averages for each question
    struct ParsedProfile
    {
        public:
            JobID d_role_id;
            std::vector<std::string> d_questions;
            MetricStats d_averages;  // The average value for each metric.

            // CONSTRUCTORS
            explicit ParsedProfile (
                    JobID role_id,
                    std::vector<std::string> questions,
                    MetricStats averages
            ) noexcept
                : d_role_id{ std::move(role_id) }, d_questions{ std::move(questions) }, d_averages{ std::move(averages) }
            {}
            ParsedProfile() = delete;

            // METHODS

            // Returns amount of quantitative questions
            inline auto metrics_cnt() const -> int { return (int) d_questions.size(); }
    };

    /// Parses the data provided by the role file
    /// (*.role?, *.jrl)
    ParsedProfile load_profile(std::string const& file_path);


    /// Stores the answers to a given survey. At
    /// the time of writing, the results will be a std::vector of
    /// std::strings. The first element will be a std::string capturing
    /// the entry date. Entries 1->m are the answers to the
    /// numeric-based role questions. Finally, the last entry
    /// is extra information the user provides. 
    class SurveyResults
    {
        public:
            // CONSTRUCTORS
            SurveyResults(std::vector<std::string> const& responses, int metrics_cnt)
                : d_metrics_cnt{ metrics_cnt }, d_responses{ std::move(responses) } {
                    // 3 is the minimum length of a valid survey result std::vector.
                    // It would be [date, metric_questions, extra_info]
                    assert((int) d_responses.size() > metrics_cnt && metrics_cnt > 0); 
                }

            // ACCESSORS
            inline auto get_responses() const -> std::vector<std::string> const& { return d_responses; }
            inline auto metrics_cnt() const -> int const { return d_metrics_cnt; }

            // METHODS
            std::vector<double> extract_metrics() const;

        private:
            int d_metrics_cnt;            // How many questions were number-based?
            std::vector<std::string> d_responses;   // All of the form responses
    };


    /*
    /// EEO-specific metrics to evaluate workload.
    /// Job Code: 0260
    class EEOCaseManagerSurvey
    {
        public:
            EEOCaseManagerSurvey();          // UNIMPLEMENTED
            explicit EEOCaseManagerSurvey(SurveyResults const&);

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
*/
/*   EWI CALCULATION; THIS IS POSTPONED
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
*/
} // namespace ewi
#endif // INCLUDED_EXPLORE
