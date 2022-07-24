
# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))
import alabaster

html_theme_path = [alabaster.get_path()]
html_theme = 'alabaster'
html_sidebars = {
    '**': [
        'about.html',
        'navigation.html',
        'relations.html',
        'searchbox.html',
        'donate.html',
    ]
}

# -- Project information -----------------------------------------------------

project = 'M2TWEOP'
copyright = '2022, youneuoy'
author = 'youneuoy'

# The full version, including alpha/beta/rc tags
release = '2.1'



# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    # ...
    'alabaster',
    'm2r2',
    'sphinxcontrib.pdfembed'  
]
# ...

# source_suffix = '.rst'
source_suffix = ['.rst', '.md']

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#



html_theme_options = {
    'description': 'Designed to expand the capabilities of the game Medieval 2: Total War',
    'github_user': 'youneuoy',
    'github_repo': 'M2TWEOP-library',
    'github_button': True,
    'fixed_sidebar': True,
    'extra_nav_links': {
        'M2TWEOP LUA plugin': '_static/LuaLib/index.html',
    },
    'donate_url':'https://ko-fi.com/D1D4DZTHG',

}

html_favicon = '_static/EOPIcon.png'


# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']


#<p>
#<a class="badge" href="https://www.patreon.com/m2tweop">
#<img src="https://img.shields.io/endpoint.svg?url=https%3A%2F%2Fshieldsio-patreon.vercel.app%2Fapi%3Fusername%3DM2TWEOP%26type%3Dpatrons&style=flat" alt="Donate">
#</a>
#</p>

