#!/bin/bash

# Let's handle any paramters first.
if [ ! $# -eq 0 ]; then
	echo "This program sets up hand-in structures for teachers. It supposes all teachers and students have accounts on this system!";
	echo "Just run the program without arguments... We'll guide you through!";
	echo "(I will blab out that you'll be needing root permssions...)";
	exit 0;
fi


if [ "$(whoami)" != "root" ]; then
	echo "This script must be run as root (we need to be able to chown).";
	exit 1
fi

while true; do
	read -p "What is the username of the teacher? " teacherName

	# Test if user exists, throw away the output (just look at the exit status)
	cut -d: -f1 /etc/passwd | grep "$name" > /dev/null
	if [ $? -eq 0 ]; then
		echo "Using user $teacherName as teacher group.";
		break;
	else
		echo "Group could not be found...";
	fi
done

echo "Please enter the top-directory used to create individual folders in for the assignment";

while true; do
	read -p "/home/" topFolder;
	topFolder="/home/$topFolder";

	if [ -d "$topFolder" ]; then
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
done

sudo chown -R $teacherName "$topFolder";

#We have chosen a root folder, now let's add some users!
echo "Using top level folder $topFolder";
echo "";
echo "We'll now start adding students. Each student is expected to be in a group, and we process them based on the names of these groups.";

while true; do
	# As for a username, and put a break after that.
	read -p "Add new student? (Y/N): " add

	if [ "$add" == "y" ]  || [ "$add" == "Y" ]; then
		read -p "Student group name: " name
		echo "";		
		
		#Check if group exists
		egrep -i "^$teacherName:" /etc/group > /dev/null
		USER_EXISTS=$?

		if [ -d "$topFolder/$name" ]; then
			echo "Folder already exists, could not add user.";
		elif [ $USER_EXISTS -eq 0 ];then
			echo "Creating directory"
			mkdir "$topFolder/$name";
			chown -R "$teacherName:$name" "$topFolder/$name";
			chmod -R 770 "$topFolder/$name";
			echo "You added $name";

		else
			echo "This user does not exist, thus is not added.";
			echo "Please only add users existing on this system.";
		fi
	else
		break;
	fi
	# Add a break between the students
	echo "";
done

echo "All users now have their own folder, you can read everything.";

echo "";

echo "DEADLINE";

read -p "Please enter the deadline: " deadline;
echo "./deadline.sh $topFolder" | at "$deadline"

echo "Deadline is set!";
echo "Program terminated";
