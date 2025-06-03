if [ "$#" -eq 0 ]; then
    echo "Expected arguments."
    exit 0;
fi

copy_dir=../copys/

mkdir -p $copy_dir

if [ "$1" == "clean" ]; then
    read -p "Are you ready, that you want to remove all dir for copys? [y/n]: " user_input
    if [ "$user_input" == "y" ] || [ -z "$user_input" ]; then
        rm -rf $copy_dir/*
    fi
    exit 0;
fi


for arg in "$@"; do
    if [ -f "$arg" ]; then
        cp $arg $copy_dir
    elif [ -d "$arg" ]; then
        cp -r $arg $arg $copy_dir
    else
        echo "$arg - doesn't exist"
    fi
done
