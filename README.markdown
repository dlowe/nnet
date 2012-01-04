## Usage:
    $ guzip ioccc.tar.gz
    $ ./prog - ./ioccc-1/ ./ioccc-0/ < /dev/null > ioccc.judge
    $ ./prog prog.c < ioccc.judge
    prog.c 1.0
    $ ./prog squeaky_clean.c < ioccc.judge
    squeaky_clean.c 0.0

## Synopsis:
This is an artificially intelligent judging tool to help the IOCCC judges.
Here's to shorter, more frequent contests!

## Description:
This is a multilayer perceptron (a feedforward artificial neural network)
which can be trained, using on-line backpropagation, to classify input files. I've
included three training corpora:

 * english.tar.gz: train the program to identify english text
 * png.tar.gz: train the program to identify .png image files
 * ioccc.tar.gz: train the program to identify winning IOCCC entries

You can train it by pointing it at any pair of directories.

## Limitations
You must include the trailing directory separator on spam and ham directories (allows the
program to be portable without wasting precious bytes on figuring out how to concatenate
directory and file names...)

Serialized "brain" files are only portable between systems with the same floating-point
representation. Most compilers use the IEEE 754 single precision floating point format, and
the included "brain" files use the same.

Making sure not to overfit the network to the training data is a bit of a black art. One
could use a wrapper script to implement early stopping with a set of test data separated
from the training data.

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
