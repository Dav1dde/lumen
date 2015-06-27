Lumen
=====

## THIS REPOSITORY IS DEAD - Lumen is a part of Kate/Kdevelop now and maintained on the official git server ##


Lumen is a KTextEditor autocompletion plugin for the D programming language,
which works e.g. in Kate or KDevelop.

### Images ###
**Imports:**

![Import Completion](https://raw.github.com/wiki/Dav1dde/lumen/_images/import01.png)


**Completions:**

![Variable Completion](https://raw.github.com/wiki/Dav1dde/lumen/_images/completion01.png)
![Function Completion](https://raw.github.com/wiki/Dav1dde/lumen/_images/completion02.png)


**Calltips:**

![Calltip](https://raw.github.com/wiki/Dav1dde/lumen/_images/calltip01.png)

## Installation ##

You need to install [DCD](https://github.com/Hackerpilot/DCD), add
`dcd-client` and `dcd-server` to your `$PATH`.

Furthermore on some systems you need the development packages of KDE.
If you're running Fedora this would be `kdelibs-devel` and for Debian like systems
`kdelibs-dev`.

Afterwards run the following commands:

    git clone git://github.com/Dav1dde/lumen.git
    mkdir lumen/build
    cd lumen/build
    cmake ../lumen
    make
    make install
    kbuildsycoca4

Now you should be able to enable the Plugin in Kate, like this:

    Settings -> Configure Kate -> Editor Component/Extensions

in KDevelop:

    Settings -> Configure Editor -> Extensions


## Include Paths ##

Either add your include paths to `~/.config/dcd/dcd.conf` or create a
`.lumenconfig` file and add the paths to it. Every line represents
another include path. For every opened file lumen tries to read a
`.lumenconfig` in every folder up to this file.

E.g. for the file `/home/foo/projects/lumen/test/foo.d/`, lumen will try to read
a `.lumenconfig` file in these folders:

    /home/foo/projects/lumen/test
    /home/foo/projects/lumen
    /home/foo/projects/
    /home/foo/
    /home/
    /

This is not perfect but works pretty well.

## DCD Server ##

Lumen tries to start a `dcd-server` on port `9166` (default) if that fails, it simply assumes
the server is already running. That means you can have your own `dcd-server` running, since
lumen will only shutdown servers which were started by the plugin on unload.

I personally recommend you to start your own `dcd-server` instance, since a fresh `dcd-server`
startup, which adds several include paths like Phobos/druntime, takes quite some time.
Until the server finished processing all include paths, it doesn't respond to completion requests.
A not responding completion server slows down the editor and doesn't show any completion tooltips.
My `.xinitrc` has this entry, to start the `dcd-server` together with X:

    dcd-server >~/.dcd.log 2>&1 &

Also I recommend to add Phobos and druntime to your `~/.config/dcd/dcd.conf`. This makes sure you
get completions instantly after starting KTextEditor, Kate oder KDevelop.
