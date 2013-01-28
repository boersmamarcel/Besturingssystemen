if [ $# -eq 1 ]; then
	echo "Usage; ";
	echo "./deadline.sh [top folder] [teacher username]";
elif [ $# -eq 2 ]; then
	echo "The deadline has passed..." >> "$1/STATUS.txt";
	sudo find "$1" -exec setfacl -b  {} ';'
	setfacl -Rm "u:$2:rw" "$1";
fi
