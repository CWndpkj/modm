#!/usr/bin/env python
#
# $Id$

import yaml
import os

class PropertyException(Exception):
	pass

def listify(object):
	""" Helper function to put a object in a list if the object
	isn't a list allready. """
	if isinstance(object, list):
		return object
	else:
		return [object]


class GlobalProperties:
	class AvrProperties:
		""" AVR specific properties """
		def __init__(self, properties):
			self.avrdude = properties.get('avrdude', {})
			self.device = properties['device']
			self.clock = properties['clock']
	
	def __init__(self, properties, target=None):
		self.properties = properties
		
		if not target:
			target = properties.get('target', 'pc')
		self.target = target
		
		if self.target == 'atmega':
			self.avr = self.AvrProperties(properties.get('atmega', {}))
		elif self.target == 'atxmega':
			self.avr = self.AvrProperties(properties.get('atxmega', {}))
	
	def getLocalProperties(self, path, target, tag):
		try:
			local = self.properties[tag]
			relpath = path[len(target):]
			for step in relpath.split(os.sep):
				local = local[step]
		except (KeyError, TypeError):
			local = None
		
		return local if (local) else {}


class FileProperties:
	def __init__(self, filename, abspath, defines=None):
		self.name = filename
		# filename without extension
		self.basename = os.path.splitext(os.path.basename(filename))[0]
		self.abspath = abspath
		if defines:
			self.defines = defines
		else:
			self.defines = {}
		self.defines['BASENAME'] = self.basename
	
	def getDefines(self, globalDefines=None):
		""" Returns a dictionary with the defines for this file """
		if globalDefines:
			defines = self.defines.copy()
			defines.update(globalDefines)
			return defines
		else:
			return self.defines.copy()
	
	def __cmp__(self, other):
		return cmp(self.name, other)


class DirectoryProperties:
	def __init__(self, path, treeProperties, localProperties, globalProperties):
		self.localProperties = localProperties
		self.treeProperties = treeProperties
		self.globalProperties = globalProperties
		
		if treeProperties:
			self._enabled = False
			for target in listify(treeProperties['target']):
				if target == 'all' or target == globalProperties.target:
					self._enabled = True
			
			if 'enable' in localProperties:
				self._enabled = localProperties['enable']
		else:
			self._enabled = True
	
	def createFileProperties(self, filename, abspath):
		defines  = self.localProperties.get('defines', {})
		return FileProperties(filename, abspath, defines)
	
	def shouldBeBuild(self):
		return self._enabled


class PropertyParser:
	""" Parser to read property-files and create a list of files to build. """
	sourcetypes = ['.cpp', '.c', '.S']
	headertypes = ['.h', '.hpp']
	
	def __init__(self, configFile, target=None, verbose=False):
		self.verbose = verbose
		try:
			properties = yaml.load(open(configFile))
		except yaml.YAMLError, e:
			raise PropertyException("Error in configuration file:", e)
		except IOError, e:
			raise PropertyException("Could not open file '%s'!" % configFile)
		
		self.globalProperties = GlobalProperties(properties, target)
	
	def parseDirectory(self, target, tag):
		""" Reads recursively the property-files from the directories and
		returns a list of files to build.
		"""
		sources = []
		header = []
		for path, directories, files in os.walk(target):
			# exclude the SVN-directories
			if '.svn' in directories:
				directories.remove('.svn')
			
			localProperties = self.globalProperties.getLocalProperties(path, target, tag)
			directory = self._parseDirectoryProperties(path, localProperties)
			if directory.shouldBeBuild():
				for file in files:
					extension = os.path.splitext(file)[1]
					if extension in self.sourcetypes:
						filename = os.path.join(path[len(target):], file)
						abspath = os.path.abspath(os.path.join(path, file))
						sources.append(directory.createFileProperties(filename, abspath))
					elif extension  in self.headertypes:
						filename = os.path.join(path[len(target):], file)
						abspath = os.path.abspath(os.path.join(path, file))
						header.append(directory.createFileProperties(filename, abspath))
			else:
				# if the this directory should be excluded, remove all the
				# subdirectories from the list to exclude them as well
				tempDirectories = directories[:]
				for d in tempDirectories:
					directories.remove(d)
		
		return (sources, header)
	
	def _parseDirectoryProperties(self, path, localProperties):
		try:
			filename = os.path.join(path, 'properties.yaml')
			treeProperties = yaml.load(open(filename))
		except yaml.YAMLError, e:
			print "Error in configuration file:", e
			raise PropertyException(e)
		except IOError:
			treeProperties = None
		else:
			if self.verbose:
				print "parse '%s/properties.yaml'" % path
		
		return DirectoryProperties(path, treeProperties, localProperties, self.globalProperties)
	
	def getGlobalProperties(self):
		return self.globalProperties
