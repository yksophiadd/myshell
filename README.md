# MyShell

This is a simple shell using C lang to implement.

Here is the features:
* Support non-blocking waiting for child program which is also known as background execution.
  * Usage of background execution is to add '&' at the end of the command
* Support pipe to redirect stdout to stdin of next program.
  * Usage of pipe is using '|' between two programs.

