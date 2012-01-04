## Usage:
    $ gunzip ioccc.tar.gz
    $ ./prog -400 ./ioccc-1/ ./ioccc-0/ < /dev/null > ioccc.judge
    ....................
    $ ./prog prog.c < ioccc.judge
    prog.c 0.981931
    $ ./prog squeaky_clean.c < ioccc.judge
    squeaky_clean.c 0.013124

## Synopsis:
This is an artificially intelligent judging tool to help the IOCCC judges.
Here's to shorter, more frequent contests!

## Description:
This is a multilayer perceptron (a feedforward artificial neural network)
which can be trained, using on-line backpropagation, to classify input files. I've
included three training corpora to play with:

 * ioccc.tar.gz: train the program to identify winning IOCCC entries
 * png.tar.gz: train the program to differentiate .png from .gif images
 * english.tar.gz: train the program to differentiate english from french text

... but you can train it by pointing it at any set of directories. The first
directory's contents will take on a desired output value of 1, and the last
directory's contents will take on a desired output value of 0; intervening
directories will take intermediate desired output values.

### ioccc corpora

The ioccc-1 corpus was obtained by taking all files matching /[a-z]+\.c/ from
the all.tar.gz download from www.ioccc.org.

The ioccc-0 corpus was obtained by searching github.com for "language:c",
and using some sed/curl scripts, doing "raw" downloads of the files on the
first 10 pages of results whose filenames matched /.*\.c/.

This is a completely inappropriate use of a neural network :)

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
   which stopped after 18,000 iterations.
 * interpreting output of "> 0.5" as "probably a .png" and "< 0.5" as "probably
   a .gif"
 * produced a network with about 81% accuracy

### english corpora

## Limitations
The topology of the network is fixed: 2^16 inputs, 6 hidden neurons, and one
output.

The learning rate is hard-coded to 0.3.

You must include the trailing directory separator on training directories
(this allows the program to be portable without wasting precious bytes on
figuring out how to concatenate directory and file names...)

Serialized network files are only portable between systems with the same
floating-point representation.

Making sure not to overfit the network to the training data is a bit of a
black art. I used a wrapper script to implement early stopping with a set of
test data separated from the training data.

## Obfuscation:

## Links:
 * [Example MLP in java](https://github.com/jimmikaelkael/multi-layer-perceptron)
 * [Feed-forward neural networks](http://en.wikipedia.org/wiki/Feedforward_neural_network)
 * [Bigram](http://en.wikipedia.org/wiki/Bigram)
 * [Logistic functions](http://en.wikipedia.org/wiki/Logistic_function)
 * [Graph of the logistic function in use](https://www.google.com/search?q=1/(1%2Bexp(-x)))
 * [public spam/ham corpora](http://spamassassin.apache.org/publiccorpus/)
 * [Back propagation](http://www.learnartificialneuralnetworks.com/backpropagation.html)
 * [Example in C++](http://www.codeproject.com/KB/recipes/BP.aspx)
 * [Nice tutorial](http://www.shiffman.net/teaching/nature/nn/)
