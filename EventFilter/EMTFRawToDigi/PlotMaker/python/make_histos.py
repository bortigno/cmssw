#!/usr/bin/env python

import sys
import ROOT
import ConfigParser
from argparse import ArgumentParser
import json
from distutils import file_util

def main():

    ROOT.gROOT.SetBatch(True)
    ##                     Xksiourmen
    ROOT.gStyle.SetOptStat(1000111111)

    input_file = ROOT.TFile("NTuples/EMTF_ntuple.root")
    output_file = ROOT.TFile("Histos/EMTF_histos.root", 'RECREATE')

    tree = input_file.Get('EMTF')

    config = json.load(open("config/AWB_plot_list_v0.json"))
    # ## Prints the JSON format of a dict/array/combination
    # print json.dumps(config)

    for var_name in config['TH1']['variables'].keys():
        var = config['TH1']['variables'][var_name]
        draw_plot_1D(tree, output_file, var_name, var)

    for var_name in config['TH2']['variables'].keys():
        var = config['TH2']['variables'][var_name]
        draw_plot_2D(tree, output_file, var_name, var)

    output_file.Close()
    del tree
    input_file.Close()


def draw_plot_1D(tree, output_file, var_name, var):
    
    print 'Trying to draw %s' % var_name

    canvas = ROOT.TCanvas(var_name, var['title'])
    hist = ROOT.TH1D(var_name, var['title'], var['nBins'], var['xMin'], var['xMax'])
    hist.Sumw2()

    var_string = '%s >> %s' % (var['expression'], var_name)
    cut_string = '%s' % var['cuts']

    # print 'var_string = %s' % var_string
    # print 'cut_string = %s' % cut_string

    tree.Draw(var_string, cut_string, '')

    hist.SetDirectory(output_file)
    hist.Write()

    canvas.SaveAs('Images/%s.png' % var_name)
    canvas.SaveAs('Images/%s.pdf' % var_name)

    file_util.copy_file('Images/%s.png' % var_name, '/afs/cern.ch/user/a/abrinke1/www/')
    file_util.copy_file('Images/%s.pdf' % var_name, '/afs/cern.ch/user/a/abrinke1/www/')

    del hist
    del canvas

def draw_plot_2D(tree, output_file, var_name, var):
    
    print 'Trying to draw %s' % var_name

    canvas = ROOT.TCanvas(var_name, var['title'])
    hist = ROOT.TH2D(var_name, var['title'], var['nBinsX'], var['xMin'], var['xMax'], var['nBinsY'], var['yMin'], var['yMax'])
    hist.Sumw2()

    var_string = '%s >> %s' % (var['expression'], var_name)
    cut_string = '%s' % var['cuts']

    # print 'var_string = %s' % var_string
    # print 'cut_string = %s' % cut_string

    tree.Draw(var_string, cut_string, '')

    hist.SetDirectory(output_file)
    hist.Write()

    canvas.SaveAs('Images/%s.png' % var_name)
    canvas.SaveAs('Images/%s.pdf' % var_name)

    # file_util.copy_file('Images/%s.png' % var_name, '/afs/cern.ch/user/a/abrinke1/www/')
    # file_util.copy_file('Images/%s.pdf' % var_name, '/afs/cern.ch/user/a/abrinke1/www/')

    del hist
    del canvas


if __name__ == '__main__':
    main()
    

    
