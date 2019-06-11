if [ $1 = f ]
then
	method=f
else
	method=m
fi

if [ -z $2 ]
then
	for FILE in `find ./data -name *.in`
		do	
		    sudo ./master $FILE $method & sudo ./slave $FILE.out $method 127.0.0.1
		done
else
	sudo ./master ./data/$2.in $method & sudo ./slave ./data/$2.out $method 127.0.0.1
fi
