UCam Pipeline Specification
==================

The pipeline is composed of a number of step. Each step performs a transformation of data.

Preprocessing
------------
Raw data is transformed into normalised, tokenised data. Transformations may optionally include:

*   Language detection
*   Length filtering
*   UTF-8 Cleaning
### Prerequisites
None

### Input
* Raw parallel training data

### Output
* Filtered, normalised, and tokenised parallel training data

Morphological Analysis
--------------------
### Prerequisites
* Preprocessing

### Input
* Tokenised monolingual data

###Output
* Morphologically processed data

Indexing
----------
Tokenised and morphologically analysed data is converted into integer encoded data
### Prerequisites
* Preprocessing
* Morphological Analysis

### Input
* Tokenised parallel data
* A previous word map to ensure compatibility

### Output
* Integer encoded parallel data
* Vocabulary
* A new wordmap enlarged with new vocabulary

