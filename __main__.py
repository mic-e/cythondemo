def main():
    from . import interface
    interface.main(handle_args())


def handle_args():
    import argparse
    cli = argparse.ArgumentParser()
    cli.add_argument('--thatnumber', type=int, default=17)
    return cli.parse_args()


if __name__ == "__main__":
    main()
