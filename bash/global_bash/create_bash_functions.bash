home_dir="$HOME"

bash_functions_file=.bash_functions
bash_functions_file_copy=.bash_functions_copy
bash_functions_file_copy_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)


if [ ! -f ${bash_functions_file_copy_dir}/${bash_functions_file_copy} ]; then
    echo "'${bash_functions_file_copy_dir}/${bash_functions_file_copy}' doesn't exists."
    exit 1
fi

cp ${bash_functions_file_copy_dir}/${bash_functions_file_copy} ${home_dir}/${bash_functions_file}
