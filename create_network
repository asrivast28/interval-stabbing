#!/usr/bin/env python

import argparse
import os
from random import randint
import sys

import micronap.sdk as ap

class Labels(object):
    def __init__(self, paramRefs):
        self.__paramRefs = paramRefs
        self.__substList = []

    @staticmethod
    def createRangeSymbol(byteRange):
        symbol = '' 
        if isinstance(byteRange, tuple):
            xN, yN = byteRange
        else:
            xN = byteRange
            yN = byteRange
        if xN == yN:
            symbol = '\\x' + hex(xN)[2:]
        elif xN < yN:
            symbol = '\\x%s-\\x%s'%(hex(xN)[2:], hex(yN)[2:])
        return symbol

    def __setitem__(self, index, byteRange):
        if isinstance(byteRange, list):
            symbol = '[%s]'%''.join(self.createRangeSymbol(r) for r in byteRange)
        else:
            symbol = '[%s]'%self.createRangeSymbol(byteRange)
        if symbol == '[]':
            symbol = ''
        ref = self.__paramRefs[index]
        self.__substList.append((ref, symbol))

    def getSubstitutionList(self):
        return self.__substList

def labelUnsigned(b, labels, x, y):
    labels[2] = (x[0]+1, y[0]-1)
    equalPrefix = True
    for i in xrange(1, b-1):
        labels[4*(i-1)+1] = x[i-1]
        labels[4*(i-1)+4] = y[i-1]
        if x[i-1] != y[i-1]:
            equalPrefix = False
        if equalPrefix:
            labels[4*i+2] = (x[i]+1, y[i]-1)
            labels[4*i+3] = (x[i]+1, y[i]-1)
        else:
            labels[4*i+2] = (x[i]+1, 255)
            labels[4*i+3] = (0, y[i]-1)
    labels[4*(b-2)+1] = x[b-2]
    labels[4*(b-2)+4] = y[b-2]
    if equalPrefix:
        labels[4*(b-1)+2] = (x[b-1], y[b-1])
        labels[4*(b-1)+3] = (x[b-1], y[b-1])
    else:
        labels[4*(b-1)+2] = (x[b-1], 255)
        labels[4*(b-1)+3] = (0, y[b-1])

def labelSigned(b, labels, x, y):
    labels[2] = [(x[0]+1, 255), (0, y[0]-1)]
    for i in xrange(1, b-1):
        labels[4*(i-1)+1] = x[i-1]
        labels[4*(i-1)+4] = y[i-1]
        labels[4*i+2] = (x[i]+1, 255)
        labels[4*i+3] = (0, y[i]-1)
    labels[4*(b-2)+1] = x[b-2]
    labels[4*(b-2)+4] = y[b-2]
    labels[4*(b-1)+2] = (x[b-1], 255)
    labels[4*(b-1)+3] = (0, y[b-1])

def assignLabels(numBytes, signed, labels, x, y):
    import random

    # convert to binary representation
    x = bin(x & int('0b'+'1'*8*numBytes, 2))[2:].zfill(8*numBytes)
    y = bin(y & int('0b'+'1'*8*numBytes, 2))[2:].zfill(8*numBytes)
    # split the representation into bytes
    x = [int(x[b:b+8], 2) for b in xrange(0, 8*numBytes, 8)]
    y = [int(y[b:b+8], 2) for b in xrange(0, 8*numBytes, 8)]

    if not signed or (x[0] <= 127 and y[0] <= 127) or (x[0] > 127 and y[0] > 127):
        labelUnsigned(numBytes, labels, x, y)
    else:
        labelSigned(numBytes, labels, x, y)


def parseArgs():
    parser = argparse.ArgumentParser(description = 'Creates an ANML network with replications of the given macro.')
    parser.add_argument('macro', metavar = 'MACRO', help = 'ANML file for the macro to be used in the network.')
    parser.add_argument('--bytes', '-b', metavar = 'B', type = int, default = 4, help = 'Number of bytes.')
    parser.add_argument('--count', '-c', metavar = 'N', type = int, default = 0, help = 'Number of random macro instances to be included in the network.')
    parser.add_argument('--maximum', '-m', metavar = 'M', type = int, default = sys.maxint, help = 'Maximum number of macros in one FSM.')
    parser.add_argument('--file', '-f', metavar = 'FILE', help = 'Name of the file from which numbers are to be read.')
    parser.add_argument('--unsigned', '-u', action = 'store_true', help = 'Use unsigned numbers for labeling.') 

    args = parser.parse_args()

    if args.file and args.count:
        print 'WARNING: "file" and "count" argument provided together. "count" will be ignored.'

    return args

def createAnmlNetwork(args, name):
    anml = ap.Anml()
    comparator = anml.LoadAnmlMacro(args.macro)
    comparator.SetMacroDefToBeCompiled()
    anml.CompileMacros()

    network = anml.CreateAutomataNetwork(anmlId = name)

    paramRefs = {}
    ctr = 1
    error = False 
    while True:
        try:
            p = comparator.GetMacroParamFromName('%p' + str(ctr))
        except ap.ApError:
            if error:
                break
            error = True
        else:
            paramRefs[ctr] = p
            error = False
        ctr = ctr + 1


    if args.unsigned:
        #lower = 0
        lower = 1
        upper = 2**(8*args.bytes) - 1 
    else:
        lower = -1 * (2**(8*args.bytes-1))
        upper = 2**(8*args.bytes-1) - 1

    allRanges = []
    if args.file:
        with open(args.file, 'r') as f:
            allRanges = [tuple(int(n) for n in line.split()) for line in f.xreadlines()]
    else:
        for count in xrange(args.count):
            # pick two random integers such that  x <= y
            x = randint(lower, upper)
            y = randint(x, upper)
            allRanges.append((x, y))

    for m in xrange(min(args.maximum, len(allRanges))):
        network.AddMacroRef(comparator, anmlId = 'macro_%d'%m)

    n = -1
    m = args.maximum
    substitutionLists = {}
    for x, y in allRanges:
        if m == args.maximum:
            m = 0
            n = n + 1
            substitutionLists[n] = {}
        assert x >= 0 or not args.unsigned, "Found negative value for unsigned x."
        assert y >= 0 or not args.unsigned, "Found negative value for unsigned y."
        print 'Range: [%d,%d]'%(x, y)
        macroId = 'macro_%d'%m
        l = Labels(paramRefs)
        assignLabels(args.bytes, not args.unsigned, l, x, y)
        substitutionLists[n][macroId] = l.getSubstitutionList()
        m = m + 1

    network.ExportAnml('%s.anml'%name)
    return anml, substitutionLists

def compileAnmlNetwork(name, anml, allSubstitutions):
    automaton, emap = anml.CompileAnml()
    info = automaton.GetInfo()
    print 'ste_grp_used =', info.ste_grp_used
    print 'ste_count =', info.ste_count
    print 'match_res =', info.match_res
    print 'blocks_used = ', info.blocks_used
    print 'blocks_rect = ', info.blocks_rect

    changes = []
    for n, substitutionLists in allSubstitutions.iteritems():
        for instance, substitutions in substitutionLists.iteritems():
            element = emap.GetElementRefFromElementId('%s.%s'%(name, instance))
            for param, symbol in substitutions:
                change = ap.ap_symbol_change(element, symbol, param)
                changes.append(change)

        automaton.SetSymbol(emap, changes)
        automaton.Save('%s_%d.fsm'%(name, n))

args = parseArgs()
name = os.path.splitext(os.path.basename(args.macro))[0] + '_network'
anml, substitutionLists = createAnmlNetwork(args, name)

compileAnmlNetwork(name, anml, substitutionLists)
