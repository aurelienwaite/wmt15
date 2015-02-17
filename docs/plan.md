WMT'15 Plan
========

This document outlines how we will approach the WMT'15 baselines. The expectation is that the we should leave a significant amount of time to hill climb on the baselines.

Current Status
-----------

An initial experiment rebuilding the WMT'13 RU->EN with normalised input is in progress. We have learnt much from this experiment

### Reasons to be Cheerful
* The tools work well once the correct data is in place
    * MTTK
    * Rule extraction
	* KenLM
	* Giza
	* HiFST
* Google cloud allows decoupling from the stack

### Problems, problems
* The pipeline is a mess
* Many moving parts
* No contact or spec between the steps of the pipeline
* Impedance between steps. Each step usually outputs data in a format that is not readable by the next step and must be converted by a Perl script
* Difficult to split work between people
* Link and redirect hell. Each step involves detective work to find out what settings were used
* No source control
* A lot of historical procedures only really understood by Adrià and Juan

Proposed methodology
------------------
Follow an extreme programming/agile methodology
* Define a set of deliverables
* Set a weekly deadline to produce deliverables
* Chop deliverables, not deadlines
* Weekly meeting at 11am Wednesday to discuss the week's deliverables
* Daily short status update (1 paragraph) to be emailed by Eva, Matic, and Rory 

Key Dates
--------
Test set distributed for the translation task - April 20, 2015

Deliverables for the Baselines
----------
A deliverable is a task that should be achievable in under a week, usually by one person

* A functional spec of the whole pipeline from preprocessing to tuning
* New preprocessing scripts. Note we should review the Stanford preprocessing
* Preprocessed training data for En-De, En-Ru, En-Fr. This includes tokenisation
* CoreNLP morphological analysis for the Russian
* Compound splitting for German
* Indexed training data for En-De, En-Ru, En-Fr
* Interpolated 4-gram English language model for En-De, En-Ru, En-Fr
* Interpolated 4-gram German language model
* Interpolated 4-gram French language model
* MTTK Alignments for En-De
* MTTK Alignments for En-Ru
* MTTK Alignments for En-Fr
* Giza Alignments for En-De
* Giza Alignments for En-Ru
* Giza Alignments for En-Fr
* Rule Extraction for De->En
* Rule Extraction for En->De
* Rule Extraction for Ru->En
* Rule Extraction for Fr->En
* Rule Extraction for En->Fr
* Indexed test data En-De, En-Ru, En-Fr
* BLEU scoring scripts
* Tuning for De->En
* Tuning for En->De
* Tuning for Ru->En
* Tuning for Fr->En
* Tuning for for En->Fr

Design Principles
--------------
* Where possible new procedures should be language and dataset agnostic. E.g. preprocessing that works on DE-EN Europarl should work on RU-EN common crawl with minimal changes
* Impedance should be lowered, the output of one step/deliverable should be the input to another step. Reduce the number of Perl scripts that massage data
* Each script/tool should do one thing well and have fully specified inputs and outputs (a.k.a. the Unix philosophy)
* Avoid the single god Perl script anti-pattern
* We should avoid writing code and use industry tools (Hadoop/Spark)
* Avoid grid engine
* Favour explicit references and centralised config as opposed to link and environment variable hell
* Documentation is good!
* Use git to store scripts and other tools


