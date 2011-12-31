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

## Obfuscation:

## Links:
 * [Example MLP in java](https://github.com/jimmikaelkael/multi-layer-perceptron)
 * [Feed-forward neural networks](http://en.wikipedia.org/wiki/Feedforward_neural_network)
 * [Bigram](http://en.wikipedia.org/wiki/Bigram)
 * [Logistic functions](http://en.wikipedia.org/wiki/Logistic_function)
 * [Graph of the logistic function in use](https://www.google.com/#sclient=psy-ab&hl=en&source=hp&q=1%2F(1%2Bexp(-x))&pbx=1&oq=1%2F(1%2Bexp(-x))&aq=f&aqi=g-m1&aql=1&gs_sm=e&gs_upl=16764l19884l2l20131l4l4l0l0l0l1l425l932l2-2.0.1l3l0&bav=on.2,or.r_gc.r_pw.r_cp.,cf.osb&fp=918a461f06fdbea1&biw=1296&bih=770)
 * [public spam/ham corpora](http://spamassassin.apache.org/publiccorpus/)
