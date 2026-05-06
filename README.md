# Thread Tool

## About Thread Tool

This project is to make me know the C library `pthread.h` well, to touch
on thread execution, and try `unistd.h` and the `sys` libraries a little
bit.

This is to prepare me for some projects I will do, which I will not say
for now.

## Goals for me

- [x] Learn pthreads
- [ ] Don't be embarrassed in the process
- [x] Touch up on some few UNIX libraries
- [x] Be more upfront in Async
- [ ] Impress neighbors
- [ ] Clean the dishes

## Usage

You may use the `-h` or `--help` flags for help, but I will put a brief
explanation on what the mode does, and what the flags do.

Run modes define how your executables will be run by the threads. `Async`
will not wait for any thread and pick a new executable in the given list
to run. `Queue` will make the threads wait until every thread finishes
executing. `Serial` does the same as setting the `--threads` flag to 1,
though this is placed here for convenience.

The available flags for now are `--debug` for debugging. `--force` to
bypass the high thread count warning, though I don't recommend forcing it.
Lastly `--threads` for the thread count.

## For future implementations

I have a loose use for `fork()` in the thread functions, I may replace it
with `posix_spawn()`. The use of `fork()` is justified by instant use of
`execlp()`, though I intend to make a `--interactable` flag for failed
processes.

I would want a log for this, as the current `--debug` is very ugly. I
think it would be a flag like `--log [FILE]`.

## Review

I may or may not have done a big embarassing mistake, or did more smaller
and repeated mistakes in the coding process, plus debugging. This has
been a good journey to know some things to patch up when creating stuff
like this.

I am really happy this worked as well as it did after debugging it. Even
I thought something else would break after fixing the most major bug in it.
