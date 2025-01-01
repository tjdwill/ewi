# Employee Workload Index (EWI) Tracker

EWI Tracker is an application developed to aid users in tracking and visualizing the workload for
their given job.

## Background

Communicating one's workload is a difficult task due to the large number of variables and external
factors influencing it. Typically, companies attempt to estimate workload through performance
metrics, meaning output is typically the sole determining factor in the perception of an employee's
workload. However, it is possible (and likely common) that non-performance-based metrics contribute
to one's workload as well. For example, the number of emails, phone calls, and meetings one fields
can take up a substantial amount of one's workday, even if no tangible performance metric results
from the effort. Another factor in one's workload, though rarely discussed, is how much time one
spends on tasks outside of his or her official role. It is common for employees to help peers,
juniors, and even superiors with their work, especially if the employee in question is a high
performer. This volume of work may not be reflected in official performance metrics (ex. helping a
peer revise a presentation, troubleshoot an everyday task, or advise in an urgent,
department-related matter).

Another factor affecting the perception of workload is one's emotional state. While this is
sometimes acknowledged in passing (ex. "company morale"), it is difficult to establish a standard
method to evaluate such a thing. This difficulty is exacerbated by:

1. Varying levels of emotional intelligence/empathy among leaders.
2. Trepidation on part of the employee due to fear of indirect reprisal.

However, it is important to explicitly acknowledge this component. Someone who is emotionally
fulfilled/invested in their job would likely perceive a high technical workload in a more positive
light than if they were dissatisfied or outright unhappy. In an especially positive state, the
employee may even enjoy the challenge associated with such a load. On the other hand, for an
employee who is in a less fulfilled state, it is possible that even a *normal* workload would be
perceived as too much. 


With the above in mind, this application attempts to provide a means for employees to track both
aspects of workload perception. Though it's difficult to objectively quantify the concept of
workload, this program uses a two-part index that the author calls the *Employee Workload Index
(EWI)* to help estimate workload intensity. The output workload visualization can then be used to
give an imprecise-but-useful indication of how a given employee likely perceives their current
workload.

### Employee Workload Index (EWI)

EWI is a simple model the author created to aid in workload visualization. The idea is to have two
aspects of workload contribution—both technical (vocational) and personal (emotional) aspects—and
use them to visualize the given employee's workload. It is important to note that this application
does *not* provide a mathematically-proven heuristic for evaluating workload. Instead, it uses
information provided by the user to provide a helpful approximation.

The technical aspect of EWI is the more objective of the two. When people evaluate workload, they do
so by implicitly comparing the workload being considered (their own, an employee, a peer, etc.) with
some ground truth or "normal" workload. In other words, technical workload is viewed in this
application as a relative concept. The program allows an entity to define a series of metrics
associated with a job and specify an estimated global average for each metric. Then, the user tracks
their values for these metrics over time. Upon request, the program then compares the user's local
average with the global average.

As an example, let's consider a role in which daily phone call duration contributes to technical
workload. Say the global average for the role is 30 minutes per day. If Jane Doe averages 2 hours of
calls per day for a given calendar period, the program will report that Janes has 4x the average
workload for this given metric. Similarly, if Jane averages 0 minutes, the program will report the
workload for this metric as 0. Since the author is not prescient enough to know what metrics a given
industry may want to define, negatives are also allowed. However, the program was written with
non-negative numbers in mind.

The personal aspect is a little simpler. The user is asked a number of pre-determined questions that
aim to assess emotional satisfaction with their current role. These questions evaluate things like
confidence in leadership and investment in company/department mission. Each question requires a
response ranging from 1 to 5 (inclusive). The responses are averaged and normalized to produce a
single value ranging from -1 to 1 (incl.). Whether the calculated average falls on the positive or
negative end of zero determines how satisfied the user is. This value can then be used to help
interpret how the user may perceive the results found in the technical portion. Unlike the technical
aspect in which indexes are defined on a per-metric basis, the personal aspect is one number. The
author made this decision in order to help maintain employee privacy. In other words, even if the
personal aspect index is negative, a given employer or superior wouldn't be able to know from the
visualization *which* questions were answered negatively. It is also intended to encourage leaders
to communicate with those they lead.

## Getting Started

The app navigation bar shows all of the options available to the user:

- `Home`: The Home Page
- `Profile`: load the profiles necessary to use the app
- `Actions`: Actions available to the user once the profiles are loaded.
- `Help`: A "Quick Help" menu to get the user up and running.
- `About`: Information about this application's development.
- `Exit`: Exit the application, saving any updated changes in the process.

<div align="center">

Nav Bar (Action Button Disabled) | Nav Bar (Action Button Enabled)
:-------------------------------:|:--------------------------------:
![Nav Bar](https://github.com/tjdwill/ewi/blob/assets/ewiTrackerImgs/navBar_actionDisabled.png) |  ![Nav Bar (Action Enabled)](https://github.com/tjdwill/ewi/blob/assets/ewiTrackerImgs/navBar_actionEnabled.png)

</div>

## Profile Menu

<p align="center">
    <img src="https://github.com/tjdwill/ewi/blob/assets/ewiTrackerImgs/profileMenu.png" alt="Profile Menu" />
</p>

The `Profile` menu houses options for the app to retrieve information necessary for it to operate.
The user must provide both a user profile and a job profile for the app's actions to become enabled.
The user profile consists of basic information: a user ID and a display name. The ID is intended to
be a unique identifier (ex. your employee ID). The display name can be any name the user chooses,
but be aware that this name appears on the visualization image.

The job profile consists of the identifier (job code + human-readable title) and a list of questions
and metrics for tracking. More on the job profile will be discussed in a later section.

### Creating a User Profile

In order to create a profile for use, click the `Create` button located under the "User Profile"
heading. A window will appear requesting information from the intended user. Once both fields are
occupied with text, the "Ok" button appears. Pressing this button will create the user.

**Note:** In order to save your newly-created profile, ensure you shut down the program via the
navigation bar's `Exit` button once you're done with the session. If this is not done, the program
will not be able to locate the user on the next run. In general, make it a habit to use the `Exit`
button.

### Loading a User Profile

To load a previously-created user, click the `Load...` button under the "User Profile" heading and
input the user's ID.

### Loading a Job Profile

To load a job profile, select the `Select Job...` button under the "Job Profile" heading. This will
open a file selection dialog, allowing the user to select a job profile that's been defined. The
program expects profiles to be stored in its `.jobs` directory, but job files may be loaded from
anywhere.

### Defining a Job Profile

A job file is simply a text file formatted in a specific way. The first line defines the job's
identifier—its formal code and human-readable name. The format is:

```
<job code>: <human-readable name>
```
without the angle brackets. Spaces are allowed in the human-readable name. By "human-readable", I
mean the job's title. For example, one role in a restaurant may have a job code `5555` but the
"human-readable" name would be "Line Cook".

A blank line should follow the identifier line.

The rest of the document defines the metrics to track for the role. One metric is defined per line.
A given line is formatted as:

```
<Metric question> | <estimated average>
```

again, no angled brackets. The `|` character is typed by `SHIFT + \` on a typical QWERTY keyboard.
Each metric has a question that is presented to the user when making an entry (ex. "How many calls
were made?"). The estimated average is the value that will be used to compare the user's tracked
data for that metric. The program expects a number for this value. `-` signs are allowed.

To summarize, a job profile consists of the following:

- an identifier
- blank space
- Metric question-average pairs.

Here is an example of a fully-formed job profile:

```
0260: NETC EEO Informal Process Counselor

Time spent on communication (hr)? | 3
Active cases? | 2
Informal contacts? | 2
```

This results in the following entry survey:

<p align="center">
    <img src="https://github.com/tjdwill/ewi/blob/assets/ewiTrackerImgs/exTechSurvey.png" alt="Example Survey" />
</p>

Finally, job profiles can be loaded from anywhere, but it may be easier to store them with the
program itself. Save the file to `<program location>/.jobs`. If this folder is not visible, ensure
`View Hidden Items` is enabled on whatever file explorer you are using.

**Caution**: Unfortunately, once data for a job profile is recorded, the job profile cannot be
changed. This is due to inexperience on the author's part and will hopefully be changed in a future
iteration.

### Changing Profiles

Profiles can be changed by running through the desired process. Simply load another job and/or user.
If the user is changed by either loading or creating another profile, the previous profile is saved
before the new profile is loaded into the program.


## Action Menu

<p align="center">
    <img src="https://github.com/tjdwill/ewi/blob/assets/ewiTrackerImgs/actionMenu.png" alt="Action Menu" />
</p>

The `Action` menu houses the operations relevant to recording and reporting metrics. The first heading on the page allows the user to take surveys. To record technical metrics, take the technical survey. For personal metrics, take the personal survey. The program will then open a form that the user will fill out. Technical forms will present the questions defined in the job profile. Each field only allows numeric input with the exception of "Additional Notes", a field that allows the user to record any additional information that may be relevant to that entry.

**Note**: In the future, the author plans to implement an action that allows the user to an entry from a specified date. At the time of initial release, the user can always view the desired entries in their file located in the `.usr` directory.

### Taking a Survey

Taking a survey is as easy as clicking the desired survey button. The form is programmed such that it will only allow submission once all *metric* fields are populated. The "Additional Notes" can be empty. It is important to note that the program expects and enforces chronologically-ordered entries. In other words, each technical entry *must* have a date that is later than the previous entry. If the user intends to submit multiple entries (say if they hadn't been able to access their computer for some time), the entries should be made from earliest to latest date. The same rule applies to personal entries. However, technical and personal entries are independent of one another; the program does not enforce chronological ordering between the two. 

How often surveys are taken depends on the user as well as the job profile definition. Some metrics may be written to be recorded daily in some industries; others may have a longer timescale. Personal surveys are intended to be taken weekly, but the program does not enforce this. Frequency is solely up to the user.

### Viewing EWI

To view the EWI report, click the `View Workload...` button under the "Metrics" heading. The program will then ask for the desired date range for the report. It is an inclusive range, so if both the `From` and `To` fields have the same date, the program will generate a report for that day if entries exist for it.

The following is an example report based on contrived metric values for the job defined earlier in this document:

<p align="center">
    <img src="https://github.com/tjdwill/ewi/blob/main/test_resources/ewiQt/test_ewi.png" alt="Example EWI Report">
</p>

Let's discuss how to interpret the plot. If the user only recorded technical data for a given date range, only the technical plot will be displayed. If the user had entries for both technical and personal data within the date range, the program will output a two-part plot. The left will have the technical data, and the right will have the personal data.
If there is no technical data at all for that date range, no plot is shown.

#### Technical Plot

The technical plot consists of a baseline at `y=1` and a series of scatter points. The baseline represents the global average for each metric; since each metric can have varying value scales, the program normalizes each metric such that the global average is 1. 

A given scatter point `(x, y)` represents `(Question num, Technical Index)`, where question number `1` corresponds to question `1` on the job profile. The `Technical Index` (Idx) is the workload approximation. A `y` value of 4 for example means the user has 4x the average expected workload for that metric. Knowing this, we can see visually how much the user's workload deviates from the norm at a glance for each metric.

#### Personal Plot

The right side of the two-part plot displays the Personal Index. This plot is simple to interpret; where does the non-black point fall on the vertical line? If it is above the black point, the user's responses indicated a positive disposition with intensity increasing the closer the index is to 1. If below the black point, the responses indicated a negative disposition. Finally, if the non-black point is on the black point (Idx=0), the responses indicated that the user is exactly neutral. 

The user can save the generated visualization to their file system via the `Export` button.

## Exporting Data 

At the time of writing, there is no functional export feature for this app; the user must do it
manually. 

In short:

1. Locate where the program executable is located.
2. In your file explorer, ensure `View Hidden Items` is enabled.
3. Copy the `.usr` folder to backup users and the `.jobs` folder to save jobs.
