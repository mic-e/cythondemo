def main():
    import argparse
    cli = argparse.ArgumentParser()
    cli.add_argument('--thatnumber', type=int, default=17)
    args = cli.parse_args()

    from .if_main import main
    main(args)


if __name__ == "__main__":
    main()
