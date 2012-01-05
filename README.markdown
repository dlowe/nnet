# Usage:
    $ gunzip ioccc.tar.gz
    $ ./prog -400 ./ioccc-1/ ./ioccc-0/ < /dev/null > ioccc.judge
    ...
    $ ./prog prog.c < ioccc.judge
    prog.c 0.981931
    $ ./prog squeaky_clean.c < ioccc.judge
    squeaky_clean.c 0.013124

# Synopsis:
This is an artificially intelligent judging tool to help the IOCCC judges.
Here's to shorter, more frequent contests!

# Description:
This is a multilayer perceptron (a feedforward artificial neural network)
which can be trained, using on-line backpropagation, to classify input files.

It has a fixed topology of 2^16 input neurons, 6 hidden neurons and 1 output
neuron.

The neurons' activation function is the logistic function 1 / (1 + e ^ -x).

## Classifying

    ./prog files < trained.net

To classify files, one specifies a trained network (on stdin) and one or more
files to classify. The program will output one line per successfully-classified
file, with the filename and the classification, a number between 0 and 1.

The interpretation of the classification depends on how the network was
trained, but is geared toward interpretation as a *probability*.

## Training

    ./prog -<n_iterations> corpus1/ [...] corpus0/ < start.net > end.net

To train the network, one must specify a starting network (on stdin), two or
more corpora (directories containing training data), and the number of training
iterations to run. The program will write some progress data to stderr and,
when it's done, will serialize the network to stdout.

If no input network is given, a random new network will be generated as a
starting point (i.e. redirect /dev/null to stdin).

The first corpus will be assigned a target value of 1. The last will be
assigned a target value of 0. Intervening directories will be assigned
intermediate target values.

The learning rate is hard-coded as 0.3. No momentum factor is used.

## Examples

I've included four training corpora to play with:

 * ioccc.tar.gz: train the program to identify winning IOCCC entries
 * png.tar.gz: train the program to differentiate .png from .gif images
 * english.tar.gz: train the program to differentiate english from french text
 * xor.tar.gz: train the program to perform "xor"

### ioccc corpora

The ioccc-1 corpus was obtained by taking all files matching /[a-z]+\.c/ from
the all.tar.gz download from www.ioccc.org.

The ioccc-0 corpus was obtained by searching github.com for "language:c",
and using some sed+curl scripting, doing "raw" downloads of the files on the
first 10 pages of results whose filenames matched /.*\.c/.

Results:

 * using these corpora as a training set
 * using pre- and post-obfuscation versions of its own source code as a test
   set
 * training for 1000 iterations
 * reports that the pre-obfuscation version has 0.NNN probability of being an
   ioccc winning entry
 * reports that the post-obfuscation version has 0.NNN probability of being an
   ioccc winning entry
 * the predictive accuracy is TBD by the outcome of the contest ;)

### png corpora

The png-1 corpus was obtained by manually scraping the first results from an
images.google.com search for "obfuscate filetype:png".

The png-0 corpus was obtained by manually scraping the first results from an
images.google.com search for "obfuscate filetype:gif".

Results:

 * using these corpora as a training set
 * using several hundred random *.gif and *.png files from my home computer as
   a test set
 * using a rough "early stopping" algorithm over 1000-iteration training runs,
   which stopped after 18000 iterations.
 * interpreting output of "> 0.5" as "probably a .png" and "< 0.5" as "probably
   a .gif"
 * produced a network with about 81% accuracy

### english corpora

The english-1 corpus was obtained by manually scraping the first results from
a https://www.google.com/webhp?lr=lang_en search for "paris filetype:txt".

The english-0 corpus was obtained by manually scraping the first results from
a https://www.google.com/webhp?lr=lang_fr search for "paris filetype:txt".

Results:

 * using these corpora as a training set
 * using several dozen additional text files (obtained by the same methods)
   as a test set
 * using a rough "early stopping" algorithm over 1000-iteration training runs,
   which stopped after 2000 iterations.
 * interpreting output of "> 0.5" as "probably english" and "< 0.5" as
   "probably french"
 * produced a network with 100% accuracy

### xor corpora

The xor-1 corpus consists of two files containing '01' and '10' respectively.

The xor-0 corpus consists of two files containing '00' and '11' respectively.

Results:

 * using these corpora as a training set
 * takes about 8000 iterations to learn xor to within a tolerance of <0.01

# Limitations
The program can't tell you anything meaningful about files with less than two
bytes in them (I'm looking at you, smr.c!)

You must include the trailing directory separator on training directories
(this allows the program to be portable without wasting precious bytes on
figuring out how to concatenate directory and file names...)

Serialized network files are only portable between systems with the same
floating-point representation and endianness.

Making sure not to overfit the network to the training data is a bit of a
black art. I used a wrapper script to implement early stopping with a set of
test data separated from the training data.

Bad input (e.g. nonexistent files, non-numeric number of iterations, etc.)
tends to result in empty output.

Given exactly one corpus, the program will crash or produce garbage.

# Obfuscation:

Zombies!
