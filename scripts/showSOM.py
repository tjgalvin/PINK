#!/usr/bin/python

import getopt
import numpy
import matplotlib.pylab as plt
import struct
import sys

def print_usage():
    print ''
    print 'Usage:'
    print ''
    print '  showSOM.py [Options] <inputfile>'
    print ''
    print 'Options:'
    print ''
    print '  --channel, -c <int>      Number of channel to visualize (default = 0).'
    print '  --help, -h               Print this lines.'
    print '  --hex                    Hexagonal layout.'
    print ''

if __name__ == "__main__":

    try:
        opts, args = getopt.getopt(sys.argv[1:],"hc:",["channel=", "help", "hex"])
    except getopt.GetoptError:
        print_usage()
        sys.exit(1)

    imageNumber = 0
    channelNumber = 0
    hex = 0

    for opt, arg in opts:
        if opt in ("-c", "--channel"):
            channelNumber = int(arg)
        elif opt in ("-h", "--help"):
            print_usage()
            sys.exit()
            channelNumber = int(arg)
        elif opt in ("--hex"):
            hex = 1

    if len(args) != 1:
        print_usage()
        print 'ERROR: Input file is missing.'
        sys.exit(1)

    inputfile = args[0]

    print 'Input file is ', inputfile
    print 'Channel number is ', channelNumber

    inFile = open(inputfile, 'rb')
    numberOfChannels, SOM_width, SOM_height, SOM_depth, neuron_width, neuron_height = struct.unpack('i' * 6, inFile.read(4*6))

    print 'Number of channels = ', numberOfChannels
    print 'SOM_width = ', SOM_width
    print 'SOM_height = ', SOM_height
    print 'SOM_depth = ', SOM_depth
    print 'neuron_width = ', neuron_width
    print 'neuron_height = ', neuron_height

    if channelNumber >= numberOfChannels:
        print 'Channel number too large.'
        sys.exit(1)

    SOM_size = SOM_width * SOM_height * SOM_depth
    if hex:
        radius = (SOM_width - 1)/2;
        SOM_size -= radius * (radius + 1);

    print 'SOM_size = ', SOM_size

    dataSize = numberOfChannels * SOM_size * neuron_width * neuron_height
    array = numpy.array(struct.unpack('f' * dataSize, inFile.read(dataSize * 4)))

    image_width = SOM_width * neuron_width
    image_height = SOM_depth * SOM_height * neuron_height
    data = numpy.ndarray([SOM_width, SOM_height, SOM_depth, numberOfChannels, neuron_width, neuron_height], 'float', array)
    data = numpy.swapaxes(data, 0, 5) # neuron_height, SOM_height, SOM_depth, numberOfChannels, neuron_width, SOM_width
    data = numpy.swapaxes(data, 0, 2) # SOM_depth, SOM_height, neuron_height, numberOfChannels, neuron_width, SOM_width
    data = numpy.swapaxes(data, 4, 5) # SOM_depth, SOM_height, neuron_height, numberOfChannels, SOM_width, neuron_width
    data = numpy.reshape(data, (image_height, numberOfChannels, image_width))

    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    ax.set_aspect('equal')
    plt.imshow(data[:,channelNumber,:], interpolation='nearest', cmap=plt.cm.jet)
    plt.colorbar()
    plt.show()

    print 'All done.'
    sys.exit()
