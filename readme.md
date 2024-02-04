Yet another morse code keyer program.

This is written in Qt in a Linux environment.  The backend is using the C++ std library for threading and strings and such.  The front end is using the Qt GUI library and interface to the C++ std backend.

It is an Iambic Mode B only keyer.  There is a built in decode function that follows the timing of the audio being played.  The decoded text is displayed on a subclassed QGraphicsView object so that the spacing between characters is represented visually.

A fun project for exploring morse code, Qt, and C++ std threading.
