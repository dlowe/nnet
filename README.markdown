## Usage:
    $ ./prog ioccc.brain < prog.c
    1.0

    $ ./prog <spam directory> <ham directory> > my.brain
    $ ./prog my.brain < some_spam
    1.0
    $ ./prog my.brain < some_ham
    0.0

## Synopsis:
This is an artificially intelligent judging tool designed to help the IOCCC
judges. Here's to shorter, more frequent contests!

## Description:
This is a multilayer perceptron (a feedforward artificial neural network)
which can be trained, using backpropagation, to classify input files. I've
included three trained brains:

 * ioccc.brain, trained to identify winning IOCCC entries
 * spam.brain, trained to identify email spam
 * english.brain, trained to identify english text

You can train it by pointing it at any pair of directories.

## Limitations
You must include the trailing directory separator on spam and ham directories (allows the
program to be portable without wasting precious bytes on figuring out how to concatenate
directory and file names...)

## Obfuscation:

## Links:
 * [Example MLP in java](https://github.com/jimmikaelkael/multi-layer-perceptron)
 * [Feed-forward neural networks](http://en.wikipedia.org/wiki/Feedforward_neural_network)
 * [Bigram](http://en.wikipedia.org/wiki/Bigram)
 * [Logistic functions](http://en.wikipedia.org/wiki/Logistic_function)
 * [Graph of the logistic function in use](https://www.google.com/search?q=1/(1%2Bexp(-x)))
 * [public spam/ham corpora](http://spamassassin.apache.org/publiccorpus/)
