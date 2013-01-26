#!/bin/bash

if [ "$(whoami)" != "root" ]; then
	echo "This script must be run as root (we need to be able to chown).";
	exit 1
fi

while true; do
	read -p "What is the groupname of the teacher? " teacherName
	# Test if group exists, throw away the output (just look at the exit status)
	egrep -i "^$teacherName:" /etc/group > /dev/null
	# The egrep found a match!
	if [ $? -eq 0 ]; then
		echo "Using user $teacherName as teacher group.";
		break;
	else
		echo "Group could not be found...";
	fi
done

echo "Please enter the top-directory used to create individual folders in for the assignment";

while true; do
	read topFolder;

	if [ -d $topFolder ]; then
		if [ "$(ls -A $topFolder)" ]; then
			echo "Folder is not empty. Please enter an empty folder";
		else
			break;
		fi
	else
		echo "Folder does not exist. I'll cut you some slack and create it myself...";
		mkdir -p "$topFolder";
		break;
	fi
	chown -R root:$teacherName "$topFolder";
done

#We have chosen a root folder, now let's add some users!
echo "Using top level folder $topFolder";

while true; do
	# As for a username, and put a break after that.
	read -p "Add new student? (Y/N): " add
	echo "";

	if [ "$add" == "y" ]  || [ "$add" == "Y" ]; then
		read -p "Student user name: " name
		echo "";		
		
		#Check if user exists
		cut -d: -f1 /etc/passwd | grep "$name" > /dev/null
		USER_EXISTS=$?

		if [ -d "$topFolder/$name" ]; then
			echo "Folder already exists, could not add user.";
		elif [ $USER_EXISTS -eq 0 ];then
			echo "Creating directory"
			mkdir "$topFolder/$name";
			chown $name:$teacherName -R "$topFolder/$name";
			echo "You added $name";

		else
			echo "This user does not exist, thus is not added.";
			echo "Please only add users existing on this system.";
		fi
	else
                chmod -R 770 "$topFolder";
		echo "All users now have their own folder, you can read everything.";
		echo "To set the deadline, run './setDeadline.sh DATE";
		echo "Program terminated";
		break;
	fi
done
