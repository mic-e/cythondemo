import re


class PXDGenerator:
    def __init__(self, infilename, outfilename):
        # input header file
        self.infilename = infilename

        # output pxd file
        self.outfilename = outfilename

        # contains all namespaces and other '{' tokens
        self.stack = []

        # current line number
        self.lineno = 1

    def parse(self):
        # we're using the pygments lexer (mainly because that was the first
        # google hit for 'python c++ lexer', and it's fairly awesome to use)
        import pygments
        from pygments.lexers.c_cpp import CppLexer

        # 0: expect 'namespace'
        # 1: expect namespace name
        # 2: expect {
        state = 0

        lexer = CppLexer()
        Token = pygments.token.Token

        with open(self.infilename) as f:
            code = f.read()

        for token, val in lexer.get_tokens(code):
            # ignore whitespaces
            if token == Token.Text and not val.strip():
                continue

            if state == 0:
                # allow anything
                if token == Token.Keyword and val == 'namespace':
                    # advance to next state on 'namespace'
                    state = 1
                elif (token, val) == (Token.Punctuation, '{'):
                    self.stack.append('{')
                elif (token, val) == (Token.Punctuation, '}'):
                    if not self.stack:
                        raise Exception("unmatched '}'")
                    self.stack.pop()
                elif token == Token.Comment.Single and 'pxd:' in val:
                    m = re.match('^// pxd: (.*)$', val)
                    if not m:
                        raise Exception("invalid single-line pxd annotation")

                    yield [m.group(1)]
                elif token == Token.Comment.Multiline and 'pxd:' in val:
                    m = re.match('^/\* pxd:\n((.*\n)+) \*/$', val)
                    if not m:
                        raise Exception("invalid multi-line pxd annotation")
                    lines = m.group(1).rstrip('\n').split('\n')
                    annotations = []
                    first_nonempty, last_nonempty = None, None
                    for idx, line in enumerate(lines):
                        m = re.match('^ \*( (.*))?$', line)
                        if not m:
                            raise Exception(
                                "invalid multi-line pxd annotation: " +
                                "error in line {}".format(idx + self.lineno))

                        annotation = m.group(2)
                        if annotation:
                            if not first_nonempty:
                                first_nonempty = idx
                            last_nonempty = idx
                        annotations.append(annotation)

                    if not first_nonempty:
                        # none of the annotations was valid
                        raise Exception("empty multi-line pxd annotation")

                    yield annotations[first_nonempty:last_nonempty + 1]
                else:
                    # we don't care about other tokens
                    pass

                self.lineno += val.count('\n')
            elif state == 1:
                # expect Token.Name
                if token != Token.Name:
                    raise Exception("expected identifier after 'namespace'")
                state = 2
                self.stack.append(val)
            elif state == 2:
                # expect {
                if (token, val) != (Token.Punctuation, '{'):
                    raise Exception("expected '{' after 'namespace " +
                                    self.stack[-1] + "'")
                state = 0

        if self.stack:
            raise Exception("expected '}', but found EOF")

    def process(self):
        yield "# this PXD definition file was auto-generated from {}".format(
            self.infilename)

        try:
            # namespace of the previous pxd annotation
            previous_namespace = None

            for annotations in self.parse():
                yield ""

                if "{" in self.stack:
                    raise Exception("PXD annotation is brace-enclosed")
                elif not self.stack:
                    namespace = None
                else:
                    namespace = "::".join(self.stack)

                if namespace != previous_namespace:
                    yield ""

                if namespace:
                    prefix = "    "

                    if namespace != previous_namespace:
                        yield 'cdef extern from "{}" namespace "{}":'.format(
                            os.path.relpath(self.infilename,
                                            os.path.dirname(self.outfilename)),
                            namespace)
                else:
                    prefix = ""

                for annotation in annotations:
                    yield prefix + annotation

                previous_namespace = namespace

        except Exception as e:
            raise Exception(self.infilename + ":" + str(self.lineno) + " " +
                            e.args[0]) from e


if __name__ == '__main__':
    import argparse
    import os

    cli = argparse.ArgumentParser()
    cli.add_argument('input', help="input header file")
    cli.add_argument('--output', '-o', help="output filename", default=None)
    args = cli.parse_args()

    infilename, outfilename = args.input, args.output

    if outfilename is None:
        outfilename = os.path.splitext(infilename)[0] + '.pxd'

    if outfilename == '-':
        import sys
        outfile = sys.stdout
    else:
        outfile = open(outfilename, 'w')

    for line in PXDGenerator(infilename, outfilename).process():
        outfile.write(line)
        outfile.write('\n')
