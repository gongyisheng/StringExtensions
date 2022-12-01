# setup.py
from distutils.core import setup, Extension

module1 = Extension('stringUtils',
                    sources = ['utils.cpp'])

setup (name = 'stringUtils',
       version = '0.0.1',
       description = 'String Utilities',
       ext_modules = [module1])