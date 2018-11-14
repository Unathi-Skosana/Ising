for filename in $1/*.dat; do
    [ -e "$filename" ] || continue
		/usr/bin/python3 $HOME/CP344/scripts/histograms.py $filename
done
