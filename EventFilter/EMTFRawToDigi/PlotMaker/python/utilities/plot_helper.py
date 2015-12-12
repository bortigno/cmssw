## Imported from https://github.com/cms-ttH/ttHMultileptonAnalysis/blob/master/DrawPlots/python/utilities/plot_helper.py
## wget https://raw.githubusercontent.com/cms-ttH/ttHMultileptonAnalysis/master/DrawPlots/python/utilities/plot_helper.py
## Truncated for basic use - AWB 11.12.15

#!/usr/bin/env python
# import os, sys
import ROOT
# import string
# from distutils import file_util
# import shutil
# import glob
# import time
# import itertools
# import numpy


class DrawStringMaker:
    def __init__(self):
        self.draw_string = ''
        self.requirements = []
        self.factors = []

    def update_draw_string(self):
        requirements_string = ' && '.join(self.requirements)
        self.draw_string = '(' + requirements_string + ')'

        if len(self.factors) > 0:
            self.draw_string += ' * ' + ' * '.join(self.factors)

    def remove_selection_requirements(self, cut_string_list):
        self.requirements = list(set(self.requirements) - set(cut_string_list))
        if cut_string_list == ['all']:
            self.requirements = ['isCleanEvent==1']

        self.update_draw_string()

    def append_selection_requirement(self, cut_string):
        if cut_string != '':
            self.requirements.append(cut_string)

        self.update_draw_string()

    def append_selection_requirements(self, *cut_string_lists):
        cut_string_list = list(itertools.chain(*cut_string_lists))
        for cut_string in cut_string_list:
            self.append_selection_requirement(cut_string)

    def multiply_by_factor(self, weight):
        self.factors.append(str(weight))

        self.update_draw_string()

    def multiply_by_factors(self, *weights):
        weights = list(itertools.chain(*weights))
        for weight in weights:
            self.multiply_by_factor(weight)


# ### ------- For possible use later - AWB 11.12.15 ------- ###

# def get_www_base_directory():
#     try:
#         www_base_directory = os.environ['HOME']+'/www'
#         return www_base_directory
#     except:
#         print 'Problem accessing your CRC www directory.  Try emailing crcsupport@nd.edu and asking them to create one for you.'
#         sys.exit()

# def copy_to_www_area(local_plot_directory, www_plot_directory, plot_name):
#     file_util.copy_file('%s/%s.png' % (local_plot_directory, plot_name), www_plot_directory)
#     file_util.copy_file('%s/%s.pdf' % (local_plot_directory, plot_name), www_plot_directory)

# def make_sure_directories_exist(directories):
#     for directory in directories:
#         if not os.path.exists(directory):
#             os.makedirs(directory)

# def make_fresh_directory(directory):
#     if os.path.exists(directory):
#         try:
#             shutil.rmtree(directory)
#         except:
#             pass
#     os.makedirs(directory)

# def update_indexes(directory):
#     www_base_directory = get_www_base_directory()
#     directory = os.path.join(www_base_directory, directory)
#     for root, dirs, files in os.walk(directory):
#         current_dirs = dirs + ['..']
#         current_dirs.sort()
#         dir_names = ['<b>%s/</b>' % i for i in current_dirs]
#         non_image_items = [file for file in files if 'pdf' not in file and 'png' not in file]
#         non_image_items.sort()
#         non_image_item_paths = [os.path.join(root, i) for i in non_image_items]
#         non_image_item_names = dir_names + non_image_items
#         non_image_item_locations = current_dirs + non_image_items
#         non_image_mod_times = ['' for i in current_dirs] + [time.ctime(os.path.getmtime(i)) for i in non_image_item_paths]
#         png_images = [f for f in files if 'png' in f]
#         pdf_images = [f.replace('.png', '.pdf') for f in png_images]
# #         png_images.sort()
# #         pdf_images.sort()
#         snippet = '<tr><td><a href={location}>{name}</a></td><td>{mod_time}</td></tr>'
#         files_snippet = '\n'.join([snippet.format(location=location, name=name, mod_time=mod_time) for (location, name, mod_time) in zip(non_image_item_locations, non_image_item_names, non_image_mod_times)])
#         snippet = '<div class="pic photo-link smoothbox" id="{png}"><a href="{pdf}" rel="gallery"><img src="{png}" class="pic"/></a></div>'
#         image_snippet = '\n'.join([snippet.format(pdf=pdf, png=png) for (pdf, png) in zip(pdf_images, png_images)])
#         with open(os.path.join(os.environ['CMSSW_BASE'], 'src/ttHMultileptonAnalysis/DrawPlots/python/utilities/index.html'), 'r') as f:
#             index = f.read()
#         with open(os.path.join(root, 'index.html'), 'w') as f:
#             segment = index[index.find("<body>"):] #Have to do this because all of the javascript looks like python formatting statements
#             modified_segment = segment.format(files=files_snippet, images=image_snippet)
#             index = index.replace(segment, modified_segment)
#             f.write(index)

# def setup_www_directory(directory, depth=1, *extra_files_to_post):
#     make_fresh_directory(directory)
#     head = directory
#     for level in range(depth):
#         if not os.path.exists(os.path.join(head, '/index.html')):
#             file_util.copy_file(os.path.join(os.environ['CMSSW_BASE'], 'src/ttHMultileptonAnalysis/DrawPlots/python/utilities/index.html'), head)
#         head, tail = os.path.split(head)

#     for file in extra_files_to_post:
#         file_util.copy_file(file, directory)

# def setup_www_directories(directories, depth=1, *extra_files_to_post):
#     for directory in directories:
#         setup_www_directory(directory, depth, *extra_files_to_post)

# def setup_web_posting(directories, depth=1, *extra_files_to_post):
#     www_base_directory = get_www_base_directory()

#     www_plot_directories = [os.path.join(www_base_directory, directory) for directory in directories]
#     setup_www_directories(www_plot_directories, depth, *extra_files_to_post)


# class Plot:
#     def __init__(self, sample, output_file, tree, plot_name, parameters, draw_string):
#         self.plot_name = plot_name
#         title = '%s;%s;%s' % (sample, parameters['axis labels'][0], parameters['axis labels'][1])
#         bins = parameters['binning']
#         if len(bins) == 2 or len(bins) == 4: #Manual binning for 1D or 2D
#             bins[1] = numpy.array(bins[1],numpy.dtype(float))
#         if len(bins) == 4: #Manual binning for 2D
#             bins[3] = numpy.array(bins[3],numpy.dtype(float))

#         if parameters.get('plot type', 'TH1D') == 'TH1D':
#             self.plot = ROOT.TH1D(plot_name, title, *bins)
#             self.plot.Sumw2()
#             if parameters.get('can rebin'):
#                 self.plot.SetBit(ROOT.TH1.kCanRebin)
#         elif parameters.get('plot type', 'TH1D') == 'TH2D':
#             self.plot = ROOT.TH2D(plot_name, title, *bins)
#             self.plot.Sumw2()
#             if parameters.get('can rebin'):
#                 self.plot.SetBit(ROOT.TH2.kCanRebin)
#         else:
#             print 'ERROR [plot_helper.py]: Method Plot::__init__ currently only supports TH1D and TH2D histograms.  Please add support for other types if you wish to use them.'
#             sys.exit(2)
#         try:
#             tree.Project(self.plot_name, parameters['expression'], draw_string)
#         except AttributeError:
#             raise Exception("Tree is missing!  Exiting...")
#         self.plot.SetDirectory(output_file)

#     def save_image(self, *image_types): #I am choosing for now not to add the option in make_plots to save pngs (though it can be called here), since pdfs look nicer
#         if not os.path.exists('plot_pdfs'):
#             os.makedirs('plot_pdfs')
#         self.set_style()
#         ROOT.gROOT.SetBatch(ROOT.kTRUE)
#         canvas = ROOT.TCanvas('canvas', 'canvas', 700, 800)
#         self.plot.Draw()
#         for type in image_types:
#             canvas.Print('plot_pdfs/%s.%s' % (self.plot_name, type))

#     def post_to_web(self, config, lepton_category):
#         www_base_directory = get_www_base_directory()

#         self.save_image('png', 'pdf')

#         www_plot_directory = '%s/plots/%s/%s/%s' % (www_base_directory, config['label'], config['output directory'], lepton_category)
#         copy_to_www_area('plot_pdfs', www_plot_directory, self.plot_name)

#     def set_style(self): #later we can add arguments for different style sets if needed
#         #ROOT.gStyle.SetOptStat(0)
#         ROOT.gStyle.SetPadBorderMode(0)
#         ROOT.gStyle.SetFrameBorderMode(0)
#         ROOT.gStyle.SetLineWidth(2)
#         ROOT.gStyle.SetLabelOffset(0.01)
#         ROOT.gStyle.SetLabelSize(0.035)
#         ROOT.gStyle.SetHistLineWidth(2)
#         #self.plot.GetYaxis().SetNoExponent(ROOT.kTRUE)



