# setup.py
from distutils.core import setup, Extension

module1 = Extension('bm',
                    sources = ['bm.cpp'])

setup (name = 'bm',
       version = '0.0.1',
       description = 'Boyer-Moore implementation',
       ext_modules = [module1])