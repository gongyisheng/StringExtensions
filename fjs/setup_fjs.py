# setup.py
from distutils.core import setup, Extension

module1 = Extension('fjs',
                    sources = ['fjs.c'])

setup (name = 'fjs',
       version = '0.0.1',
       description = 'fjs implementation',
       ext_modules = [module1])