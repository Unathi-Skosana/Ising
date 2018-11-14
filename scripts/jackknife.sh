for filename in $1/*.dat; do
    [ -e "$filename" ] || continue
		$HOME/CP344/src/ising --jk $filename $2
done
