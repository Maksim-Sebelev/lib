if [ "$#" -eq 0 ]; then
    echo "Expected arguments."
    exit 0;
fi

copy_dir=../.copys
mkdir -p $copy_dir

if [ "$1" == "clean" ]; then
    read -p "Are you sure, that you want to REMOVE all dir for copys? [y/n]: " user_input
    if [ "$user_input" == "y" ] || [ -z "$user_input" ]; then
        rm -rf $copy_dir/{*,.[!.]*,..?*}
    fi
    exit 0;
fi

for arg in "$@"; do
    if [ -f "$arg" ]; then
        cp $arg $copy_dir/
    elif [ -d "$arg" ]; then
        if [ "$arg" == "." ]; then
            now_dir=$(basename "$(pwd)")
            mkdir -p $copy_dir/$now_dir
            cp -r $arg $copy_dir/$now_dir/
        else
            cp -r $arg $copy_dir/
        fi
    else
        echo "$arg - doesn't exists"
    fi
done
