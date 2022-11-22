
FILE_PATH="../assets/*"

substr="/"

for file in $FILE_PATH
do
    if [[ $file == *"jpg"* ]]; then
        temp=${file#*/}
        temp=${temp#*/}
        file_name=${temp%.*} 
        output_file_path="../ecelinux/data/$file_name.txt"

        python process_image.py $file $output_file_path

    fi
done
