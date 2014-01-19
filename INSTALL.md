Dependencies 
========
You need poker-eval which can be found here: http://gna.org/projects/pokersource/

Installation 
========

When installed, this is the process:

    git clone https://github.com/j-c-h-e-n-g/pokenum-php.git
    cd pokenum-php
    phpize
    ./configure --enable-pokenum
    make
    make install

Add extension=pokenum.so to your php.ini or use dl('pokenum.so'); in your script.

If you instead want to compile this into your php binary do this:

    $ cd your-php-source
    $ git clone git@github.com:steevel/pokenum-php.git ext/pokenum
    $ ./buildconf
    $ ./configure --enable-pokenum [your other configure parameters]
    $ make
    $ su -c "make install"
