"""
Auto-generates PXD files from annotated C++ headers.
"""

import re
import os

from pygments.token import Token
from pygments.lexers.c_cpp import CppLexer


class ParserError(Exception):
    """
    Represents a fatal parsing error in PXDGenerator.
    """
    def __init__(self, filename, lineno, message):
        super().__init__("{}:{} {}".format(filename, lineno, message))


class PXDGenerator:
    """
    Represents, and performs, a single conversion of a C++ header file to a
    PXD file.

    @param infilename:
        input (C++ header) file name. is opened and read.
    @param outfilename:
        output (pxd) file name. is opened and written.
    """

    def __init__(self, infilename, outfilename):
        self.infilename = infilename
        self.outfilename = outfilename

        # current parsing state (not valid until self.parse() is called)
        self.stack, self.lineno, self.annotations = None, None, None

    def parser_error(self, message, lineno=None):
        """
        Returns a ParserError object for this generator, at the current line.
        """
        if lineno is None:
            lineno = self.lineno

        return ParserError(self.infilename, lineno, message)

    def tokenize(self):
        """
        Tokenizes the input file.

        Yields (tokentype, val) pairs, where val is a string.

        The concatenation of all val strings is equal to the input file's
        content.
        """
        # contains all namespaces and other '{' tokens
        self.stack = []
        # current line number
        self.lineno = 1

        # we're using the pygments lexer (mainly because that was the first
        # google hit for 'python c++ lexer', and it's fairly awesome to use)

        lexer = CppLexer()

        with open(self.infilename) as infile:
            code = infile.read()

        for token, val in lexer.get_tokens(code):
            # ignore whitespaces
            yield token, val
            self.lineno += val.count('\n')

    def handle_singleline_comment(self, val):
        """
        Breaks down a '//'-style single-line comment, and passes the result
        to handle_comment()

        @param val:
            the comment text, as string, including the '//'
        """
        try:
            val = re.match('^// (.*)$', val).group(1)
        except AttributeError as ex:
            raise self.parser_error("invalid single-line comment") from ex

        self.handle_comment(val)

    def handle_multiline_comment(self, val):
        """
        Breaks down a '/* */'-style multi-line comment, and passes the result
        to handle_comment()

        @param val:
            the comment text, as string, including the '/*' and '*/'
        """
        try:
            val = re.match('^/\\*(.*)\\*/$', val, re.DOTALL).group(1)
        except AttributeError as ex:
            raise self.parser_error("invalid multi-line comment") from ex

        # for a comment '/* foo\n * bar\n */', val is now 'foo\n * bar\n '
        # however, we'd prefer ' * foo\n * bar'
        val = ' * ' + val.rstrip()
        # actually, we'd prefer [' * foo', ' * bar'].
        lines = val.split('\n')

        comment_lines = []
        for idx, line in enumerate(lines):
            try:
                line = re.match('^ \\*( (.*))?$', line).group(2) or ""
            except AttributeError as ex:
                raise self.parser_error("invalid multi-line comment line",
                                        idx + self.lineno) from ex

            # if comment is still empty, don't append anything
            if comment_lines or line.strip() != "":
                comment_lines.append(line)

        self.handle_comment('\n'.join(comment_lines).rstrip())

    def handle_comment(self, val):
        """
        Handles any comment, with its format characters removed,
        extracting the pxd annotation
        """
        try:
            annotation = re.match('^.*pxd:\\s(.*)$', val, re.DOTALL).group(1)
        except AttributeError as ex:
            raise self.parser_error(
                "comment contains no valid pxd annotation:\n" + val) from ex

        # remove empty lines at end
        annotation = annotation.rstrip()

        annotation_lines = annotation.split('\n')
        for idx, line in enumerate(annotation_lines):
            if line.strip() != "":
                # we've found the first non-empty annotation line
                self.add_annotation(annotation_lines[idx:])
                return

        raise self.parser_error("pxd annotation is empty:\n" + val)

    def add_annotation(self, annotation_lines):
        """
        Adds a (current namespace, pxd annotation) tuple to self.annotations.
        """
        if "{" in self.stack:
            raise self.parser_error("PXD annotation is brace-enclosed")
        elif not self.stack:
            namespace = None
        else:
            namespace = "::".join(self.stack)

        self.annotations.append((namespace, annotation_lines))

    def handle_token(self, token, val):
        """
        Handles one token while the parser is in its regular state.

        Returns the new state integer.
        """
        # accept any token here
        if token == Token.Keyword and val == 'namespace':
            # advance to next state on 'namespace'
            return 1

        elif (token, val) == (Token.Punctuation, '{'):
            self.stack.append('{')

        elif (token, val) == (Token.Punctuation, '}'):
            try:
                self.stack.pop()
            except IndexError as ex:
                raise self.parser_error("unmatched '}'") from ex

        elif token == Token.Comment.Single and 'pxd:' in val:
            self.handle_singleline_comment(val)

        elif token == Token.Comment.Multiline and 'pxd:' in val:
            self.handle_multiline_comment(val)

        else:
            # we don't care about all those other tokens
            pass

        return 0

    def parse(self):
        """
        Parses the input file.

        Internally calls self.tokenize().

        Adds all found PXD annotations to self.annotations,
        together with info about the namespace in which they were encountered.
        """

        self.annotations = []
        state = 0

        for token, val in self.tokenize():
            # ignore whitespaces
            if token == Token.Text and not val.strip():
                continue

            if state == 0:
                state = self.handle_token(token, val)

            elif state == 1:
                # we're inside a namespace definition; expect Token.Name
                if token != Token.Name:
                    raise self.parser_error(
                        "expected identifier after 'namespace'")
                state = 2
                self.stack.append(val)

            elif state == 2:
                # expect {
                if (token, val) != (Token.Punctuation, '{'):
                    raise self.parser_error("expected '{' after 'namespace " +
                                            self.stack[-1] + "'")
                state = 0

        if self.stack:
            raise self.parser_error("expected '}', but found EOF")

    def get_pxd_lines(self):
        """
        calls self.parse() and processes the pxd annotations to pxd code lines.
        """

        yield "# this PXD definition file was auto-generated from {}".format(
            self.infilename)

        self.parse()

        # namespace of the previous pxd annotation
        previous_namespace = None

        for namespace, annotation_lines in self.annotations:
            yield ""

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

            for annotation in annotation_lines:
                yield prefix + annotation

            previous_namespace = namespace

    def generate(self):
        """
        reads the input file and writes the output file.

        on parsing failure, raises ParserError
        """
        try:
            with open(self.outfilename, 'w') as outfile:
                for line in self.get_pxd_lines():
                    outfile.write(line)
                    outfile.write('\n')

        except ParserError:
            os.remove(self.outfilename)
            raise


def main():
    """ main function """
    import argparse

    cli = argparse.ArgumentParser()
    cli.add_argument('input', help="input header file")
    cli.add_argument('--output', '-o', help="output filename", default=None)
    args = cli.parse_args()

    infilename, outfilename = args.input, args.output

    if outfilename is None:
        outfilename = os.path.splitext(infilename)[0] + '.pxd'

    PXDGenerator(infilename, outfilename).generate()

if __name__ == '__main__':
    main()
