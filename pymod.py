import code

namespace = {}

def init_prompt():
    import rlcompleter
    import readline
    import atexit
    import os
    import sys

    sys.ps1 = '\x01\x1b[36m\x02>>>\x01\x1b[m\x02 '
    sys.ps2 = '\x01\x1b[36m\x02...\x01\x1b[m\x02 '

    history = "/tmp/oahistory"

    def save_history():
        readline.write_history_file(history)

    if os.path.exists(history):
        readline.read_history_file(history)

    atexit.register(save_history)

    readline.set_completer(rlcompleter.Completer(namespace).complete)
    readline.parse_and_bind("tab: complete")

    exec("import main", namespace)

init_prompt()

def interact():
    code.interact("", local=namespace)

def handle_args():
    import argparse
    cli = argparse.ArgumentParser()
    cli.add_argument('--thatnumber', type=int, default=17)
    return cli.parse_args()
