#TODO the code for processing scripts should not live in the Editor folder, it should be part of the Engine

import sys
sys.path.insert(0, '../../Editor/Scripts')
import ProcessScripts

if __name__ == "__main__":
    ProcessScripts.ProcessScripts(sys.argv[1], True)