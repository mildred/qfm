Qfm: Qt File Manager
====================

This is a simple file manager that I want to build, with the views:

- Visual icon mode: the idea is to restore the visual mode that nautilus once
  had. One window per folder. Each folder will keep its position and size on
  screen, and each icon in the window will keep its position and size. This let
  you arrange neatly files how you want.

- Miller Columns: or the column mode of Finder. You can also find this on
  Marlin, or its fork Pantheon-Files.

I used Qt because it seems a more powerful toolkit, and indeed I have been able
to produce those two views fairly easily. I'm using GNOME on a daily basis, but
won't use Gtk (probably because I never used it, and don't know how to use it).
If I used Gtk, it would certainly not be with C, but rather a language like
Vala.

Anyway, after having looked at the code of Nautilus (that still implements the
icon mode where icons stays where you put them, mostly for the Desktop), and
after having looked at the code of Marlin (that also uses Gtk), I decided that
Gtk was not the right choice for my file manager.

So, I use Qt, but I intend to have it integrate as much as possible in a Gtk
envionment. This means I'll not be using KDE widgets, and may try to hook up
with things like gvfs. But nothing is set in stone yet.

As for the implementation of the two views of my file manager:

- The icon view is implemented qith a QListView that I subclassed in IconView
  (and that could be used separately). It already has everything, except that it
  does not save the position of icons. I'm hooking up to the
  `indexesMoved(QModelIndexList)` signal to save the icon position in the model,
  and reimplemented a layout engine in `updateGeometries()` to position the
  icons using the positions stored in the model. I'm using extended attribute
  `user.filemanager.position` to store the position (the format is the X
  coordinate, a space, and the Y coordinate).

- The Miller columns where the easiest using `QColumnView`.

I'm using a subclass of `QFileSystemModel` for the model, but it does not
provide me with network or trash support. I'll probably move to something else
like gvfs in time.
