# Design Journal: Employee Workload Index

## Quick Navigation

- [Journal](#journal)
- [Living Questions List](#living-questions) 
- [To-Do List](#ewi-todo)

---

## Journal

## 1 November 2024

So, I was able to achieve the vast majority of my goals. Most importantly, I learned how to
run tests using CMake, which will greatly improve my workflow. I also learned more about
the system in general in terms of best practices, but CMake is definitely something that
requires time and experimentation to learn properly.

In terms of design, I made some small adjustments to the classes. `Entry` has had its Eigen
dependency removed. It now only concerns itself with std::vector<double>. For `Record`, I
removed the dependency on `MetricStats`. Now, I simply have it return an Eigen matrix that
can then be used to construct a MetricStats object. I did it this way, however, so that
neither `Record` nor `MetricStats` need know about the other. finally, I added some more
convenience methods and moved the greater record types to the draft space for now; I still
need to determine how to organize them into components.

## 27 October 2024

Lord willing, the next two weeks will be research and skill improvement focused. I
especially plan to concentrate on CMake to make the build process easier to complete.
Testing needs careful attention, so investing time up front to learn will have great payoff
down the road.

To summarize, here are my goals:

- [x] Read ~~two~~ eight chapters of the CMake book.
- [x] Write notes on const correctness and CMake
- ~~[ ] Skim Ch. 4 of Qt book.~~
- [x] Read various blog articles in my backlog.

## 26 October 2024

- Fixed `Record::find_entries` and passed all tests.
- Implemented many methods for `Record`.
    - `update_entry`, `add_entry`, `remove_entry`, `find_entry`
- Wrote `operator<<` overloads for `Entry` and `Record`.
- Learned that we can't return references from a `std::optional<T>`

## 25 October 2024

Worked on `Record::find_entries`. Found a bug. Need to find a way to organize my data such
that I can think of edge cases as normal cases.

## 24 October 2024

Worked to implement `Record::find_entries()` method.

## 23 October 2024

EEO Job code: 0260


## 22 October 2024

Ok, let's rethink the goal here. Ultimately, I want to write a piece of software that
allows employees to more adequately track and represent their workloads. Every mental
iteration of this idea has some visual component that allows employers to "see" the
employee's workload. I wanted to create a numeric index, Employee Workload Index (EWI),
that communicates in one number how intense the workload is. This number would depend on
two factors: an objective job-related component and a subjective emotional component. Both
are important because the latter colors the perception of the former. 

However, the job-related component also requires statistical data in order to calculate
its portion of the EWI. The idea is that the further the employee's workload metrics
deviate from the mean, the "stronger" the effect of the workload (or lack thereof). I have
no such data, though, nor do I know how to get it. This is because the metrics associated
with a given role aren't determined. Industries would need to converge on a uniform profile
for their respective fields, and then *that* collected data could be used as a mean.

Since I can't access that data, my method of reporting will need to change. As of now, I
cannot calculate EWI. That means the idea is on backburner. It'll still exist in the
codebase as a declaration (maybe I'll comment it out), but I won't use it until I can implement it.

For now, I think I should concentrate on a more visual means of communication. One thing I
could do is create double-bar graphs that compare the mean for a given metric to the user's
mean value for a given date range. The problem with this, however, is that the units
between metrics may vary, so the graph could be skewed by metrics with high default values.

Alternatively, I could produce one graph per metric, but this is also undesirable for cases
with a large quantity of metrics. What if I produced a graph that showed how each metric
deviates from the mean? For each metric, I could take the average for the user and subtract
it from the average of the metric. Then, I could divide this difference by the metric
average to determine how large the deviation is? This would enable a more uniform plot (and
also would be centered around zero as desired).

## 21 October 2024

### Calculating EWI

The *Employee Workload Index* (EWI) is an estimate of how a given employee likely perceives
their workload. Given data regarding the employee's current workload, the role's average
workload, and information on the employee's emotional state, a score is calculated to
determine how burdensome the workload is.

The method of calculation is up to me. I need to create a formula that:

- Acknowledges that emotional state colors perception of workload intensity.
    - Inversely proportional
- Allows significant deviations from the norm to have great impact on the resulting score.

Ideally, I would have information regarding the standard deviation for a given metric, but,
as of now, I have no such data, and can't base the calculation off of that. However, this
shouldn't be a major issue because the implementation of the EWI calc can always be
adjusted.

So, let's create a prototype.

```
EWI = <Role Impact Score> + <Emotion Impact Score>
```

#### Emotional Impact

This aspect is tricky to handle properly. On one hand, a subpar emotional index should
amplify the role index in both directions.

Scratch that. I think I'll report the two elements separately. This would offload the
interpretation onto the viewer.

That being said, here's one way to calculate the emotional index.

Currently, the accepted values range from one to five with the baseline value being three
(normal) and an exceptionally positive emotional state being five. Regardless of the range,
however, it is imperative that there be an odd number of possible values. This makes the
average center around a whole number in a monotonoically-increasing sequence of values.

I want a scale that communicates how widely an employee's emotional state deviates from the
norm:

```
 1  2 3 4 5
      _     // average

-2 -1 0 1 2
      _     // centered around the average

-1 -0.5 0 0.5 1 
        _   // normalized
```

So, a given procedure could be:

0. Subtract all scores by the ideal average.
1. Sum all scores.
2. Divide by (Max\_Value * # of responses)

This results in a normalized score whose center is 0 (neutral).

### Data Formats

- Entry
    - Date
    - Metric Data 
    - Extra Notes
- Record
    - Collection of entries
- CompleteRecord
    - Job-based Data Record
    - Emotion-based Data Record
- EmployeeRecord
    - Identifier
    - Collection of CompleteRecords mapped to JobCodes
- JobCode
    - Identifier for a given job position

Note: I don't really need to worry about the size of a given person's record. Assuming 365
entries a year for 50 years, the person would accumulate 18,250 entries. Most people don't
work for that long at one company, and they certainly don't work every day for that period.

### File Formats

**Employee Record**

Vision: text file with identification information + space-delimited records. Must be easy
to append to.

```
# Employee Record file
## Identification Region
Name: <HERE>


---
## Records
<role_id> <SurveyType> <Date> <Metrics> <Notes String>
<role_id> <SurveyType> <Date> <Metrics> <Notes String>
...
<role_id> <SurveyType> <Date> <Metrics> <Notes String>
```

The Notes string must be enclosed within quotation marks and MUST live on a single line. To
do so, newline sequences will be converted to escape sequences (i.e. `U+0020` -> `\\n`)

**Survey Profile**

Vision: text file allowing the job-specific questions to be specified.

See the format in [yesterday's post](#20-october-2024)

### Statistical Data

I'm going to need to be able to compare the user's answers with the average values for each
question. However, I need the averages to do so. Should I make a class that represents the
statistics for a role? The problem is that, for a newly created job profile, there's no
data. I *could* allow estimates for a given role, but I would lack information on how the
data varies among the set.

I only need this data when generating the EWI.

## 20 October 2024

### Dynamic Questions

Let's say we want to define a role. We can do so via a text file of the following format:

```
# <ROLE TITLE>: <some unique ID>

<Question/Metric Here> | <average value>
<Question/Metric Here> | <average value>
<Question/Metric Here> | <average value>
...
<Question/Metric Here> | <average value>
//empty line or EoF
```

Notes: 

- Ignore leading and trailing whitespace
- `|` is the delimiter between the metric and the average value
- All entries are assumed to have associated numeric answers.
- Each non-empty line corresponds to a metric.
- Duplicates are not checked.


In theory, I could then parse this file (should I create a file extension? `.role`) and extract the
questions and averages.

**Question**: How could I update averages? Figure it out later.

### Flow

With this method of allowing profiles to be created, I can envision the following flow:

- Load user and role profiles; parse role file
- Prompt user to create a new entry; Generate form from parsed questions
- Get survey results; convert to numeric data.
- Store entry data
- Give Emotion survey if necessary (based on time since last completion)


## 19 October 2024

I assume a given employee's records are only valid for the company they are currently employed by.
Knowing that the questions used to evaluate workload will vary according to the role the employee
occupies, there must be a way to contain records of survey responses over time for each role. One
method of tracking the survey responses over time is to use a map of key-value pairs. The keys would
be the date of a given entry (valid because dates YYYY-MM-DD are unique on a per-person, per-role
basis), and the values would be the remaining entry data. 

I could take this map even further out, using this map of entries as values in an outer hash map whose keys
are job role IDs. Typically, a job will have some unambiguous code associated with it within a company, so
a given employee could have:

```
Michael = {
    "role_id_0": {
        "2024-01-01": <all of the data for this entry>,
        "2024-01-02": <all of the data for this entry>,
        ...
    },
    "manager_position": {
        "2024-09-04": <all of the data for this entry>,
    }
}
```

However, does a given data entry look like? 
- How do I allow the entry questions to change as a function of the role?
- How do I keep the averages in sync with the entry questions?

### Barebones Prototype

- Get data through survey forms
    - Role-related survey ("daily")
    - Emotional component survey (~weekly-ish)
- Role-related surveys can be overwritten in case of a mistake.
- EWI is week-based.
- Static averages
- Only one profile

## 18 October 2024

I'm not going to get the design right on the first try, so let's just try something that can work.

I know I need:

- A Job profile
    - What are the common, measurable metrics associated with a given field or role?
    - What are the average statistics for these metrics?
- A way of recording and updating an employee's workload data over time.
- A report system

Rather than re-inventing the wheel as the first option, can I repurpose existing technology?

## 14 October 2024

### Evaluating Employee Workload

In order to get the data I need, I want the following:

**Technical**

- Date (look into std::chrono)
- Time Spent: Job-related correspondence 
- Time Spent: Meetings/Trainings
- Quantity: Interviews observed
- Quantity: Extrapositional requests fielded
- Quantity: Active cases; (Weekly recording?) ~~- Quantity: Contacts made (Weekly recording?)~~

**Human/Emotional Component**

The following questions are rated from one to five, and the assumed median value is, of course,
three.

- Confidence in leadership
- Investment/Interest in department mission
- How challenging do you find your work?
- How peaceful is your life?


I'm honestly not sure how to structure this data. Should a given entry be a type? What is the domain
language for this application's business logic?

---

## Living Questions

Here are some questions that have come up as I work this project.

### Language

- [ ] How do I compare two Eigen objects?
- [ ] When do I specify `noexcept`?
- [x] East-style const?
- [ ] `class` vs. `struct`: semantic meanings?
- [x] How are boolean comparision operator overloads implemented? What is the spaceship operator `<=>`?
- [ ] Difference: Initialization vs. Assignment

### Software Design/Project Management

- [ ] Is it worth it to test *every* class (ex. a class of basic getters and setters?)
- [ ] When do I need to create a physical dependency vs. use a forward declaration?
- [ ] Is it wise to use a namespace-wide enum class that serves as an error type?

---

## EWI ToDo

- Eigen
    - [ ] Eigen semantics
    - [ ] Instantiation, comparison, arithmethmetic operations 
    - [ ] Manipulation, basic and advanced access
- Qt
    - [ ] Ch. 4 of Book
    - [ ] Read fundamentals in docs
- CMake Book
    - [x] Learn to write tests
    - [x] How to structure a project
    - [x] Compiling components
- Notes Updates
    - [x] East-style const (basics)
    - [ ] Function declarations using `auto` and `->`
    - [ ] Important keywords (`explicit`, `noexcept`, `protected`, `friend`, `private/public`, `inline`, `static`, `constexpr`, `const`)
    - [ ] Making a third-party library visible
