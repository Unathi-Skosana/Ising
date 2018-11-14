DATA="../data/"
SCRIPTS="../scripts/"

make clean >  /dev/null 2>&1
make > /dev/null 2>&1
./ising $1 $2 $3 $4 $5 $6
