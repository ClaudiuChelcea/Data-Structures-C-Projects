x=0
for i in $(seq 1 1 3)
do
	echo "add_server $x"
	x=$((++x))
done
