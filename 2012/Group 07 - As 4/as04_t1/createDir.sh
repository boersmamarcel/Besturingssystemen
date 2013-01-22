#!/bin/bash
# This script will test if you have given a leap year or not.

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
		echo "Folder does not exist. Please enter an existing folder";
	fi
done

#We have chosen a root folder, now let's add some users!
echo "User top level folder $topFolder";

while true; do
	echo "Add new student? (Y/N): "

	read add;

	if [ "$add" == "y" ]  || [ "$add" == "Y" ]; then
		echo "Student user name: ";
		
		read name;

		#Check if user exists

		if [ -d "$topFolder/$name" ]; then
			echo "Folder already exists, could not add user.";
		else
			echo "Creating directory"
			mkdir "$topFolder/$name";
#			chown $name -R "$topFolder/$name";

			echo "You added $name";
		fi
	else
		echo "All users now have their own folder, you can read everything.";
		echo "To set the deadline, run './setDeadline.sh DATE";
		echo "Program terminated";
		break;
	fi
done
