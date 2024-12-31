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
2. Trepedation on part of the employee due to fear of indirect reprisal.

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

## Profile Menu

### Defining a Job Profile

### Creating a User Profile

**Note:** In order to save your newly-created profile, ensure you shut down the program via the
navigation bar's `Exit` button. If this is not done, the program will not be able to locate the user
on the next run. In general, make it a habit to use the `Exit` button.

### Loading a User Profile

### Changing Profiles

## Exporting Data 