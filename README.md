Lumen
=====

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


Afterwards run the following commands:

    git clone git://github.com/Dav1dde/lumen.git
    mkdir lumen/build
    cd lumen/build
    cmake ../plugin
    make

    mkdir -p ~/.kde4/lib/kde4/
    cp lib/lumen.so ~/.kde4/lib/kde4/
    mkdir -p ~/.kde4/share/kde4/services
    cp ../plugin/lumen.desktop ~/.kde4/share/kde4/services/
    kbuildsycoca4

Now you should be able to enable the Plugin in Kate, like this:

    Settings -> Configure Kate -> Editor Component/Extensions

in KDevelop:

    Settings -> Configure Editor -> Extensions


## Include Paths ##

Either add your include paths to `~/.config/dcd` or create a
`.kdev_include_paths` file and add the paths to it. Every line represents
another include path. For every opened file lumen tries to read a
`.kdev_include_paths` in every folder up to this file.

E.g. for the file `/home/foo/projects/lumen/test/foo.d/`, lumen will try to read
a `.kdev_include_paths` file in these folders:

    /home/foo/projects/lumen/test
    /home/foo/projects/lumen
    /home/foo/projects/
    /home/foo/
    /home/
    /

This is not perfect but works pretty well.

By default lumen automatically adds `/usr/include/d` and `/usr/include/d/druntime/import`
as import path.


## DCD Server ##

Lumen tries to start a `dcd-server` on port `9977` if that fails, it simply assumes
the server is already running. That means you can have your own `dcd-server` running, since
lumen will only shutdown servers which were started by the plugin on unload.

I personally recommend you to start your own `dcd-server` instance, since a fresh `dcd-server`
startup which adds several include paths like phobos/druntime (lumen does that by default)
take quite some time, and until the server didn't process all the include paths it doesn't
respond to completion requests, which slows down the editor while trying to complete and also
shows no completion tooltips at all. My `.xinitrc` has this entry:

    dcd-server -p9977 >~/.dcd.log 2>&1 &

Also my `~/.config/dcd` contains paths to Phobos and druntime, this basically provides me instant
completions directly after starting KDevelop.