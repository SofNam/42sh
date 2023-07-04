if ls Makefile; then
    echo oh wow

    if ls Makefile; then
        echo holy cow!
    else
        echo ...
    fi

    if ls not_found; then
        echo no way!
    elif ls Makefile; then
        echo Merry Christmas to you all !
    else
        echo Sabai Sabai
    fi
fi

echo The End.
