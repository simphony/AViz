import click

import subprocess32 as subprocess

@click.command()
@click.argument('aviz')
@click.option('--timeout', default=30, help='timeout (seconds)')
def run_aviz(aviz, timeout):
    """ run aviz with a file
    """
    try:
        subprocess.check_call(aviz, timeout=timeout)
    except subprocess.TimeoutExpired:
        # if timeout occurs then everything
        # is okay (i.e. aviz did not crash)
        pass

if __name__ == '__main__':
    run_aviz()
