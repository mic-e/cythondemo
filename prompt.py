import code
import readline
import rlcompleter
import os
import sys


def init_readline():
    import atexit

    history = "/tmp/oahistory"

    readline.set_history_length(20000)

    def save_history():
        readline.write_history_file(history)

    if os.path.exists(history):
        readline.read_history_file(history)

    atexit.register(save_history)

    from . import interactivenamespace
    ns = vars(interactivenamespace)
    readline.set_completer(rlcompleter.Completer(ns).complete)
    readline.parse_and_bind("tab: complete")


def init_prompt():
    sys.ps1 = '\x01\x1b[36m\x02>>>\x01\x1b[m\x02 '
    sys.ps2 = '\x01\x1b[36m\x02...\x01\x1b[m\x02 '

    if readline.get_current_history_length():
        # it seems like this was launched froma n interactive session,
        # and already has a histfile...
        pass
    else:
        init_readline()

init_prompt()


def interact():
    from . import interactivenamespace
    code.interact("", local=vars(interactivenamespace))


def handle_args():
    import argparse
    cli = argparse.ArgumentParser()
    cli.add_argument('--thatnumber', type=int, default=17)
    return cli.parse_args()
