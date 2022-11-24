#!/usr/bin/env python3

import sys, subprocess
sys.path.insert(1, 'build')
from vsbuild import vs_compile
from vsbuild import list_vs_installations
from vsbuild import get_vs_installations

if len(sys.argv) == 1:

    vs_compile()

elif len(sys.argv) == 2:

    if sys.argv[1].lower() == 'list':

        list_vs_installations()

    else:

        vs_compile(sys.argv[1])



