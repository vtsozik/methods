rm -f *.enc *.dec
#file=test.txt
file=rsamain.Linux
./rsamain.Linux -encrypt private.key.txt $file $file.enc
./rsamain.Linux -decrypt public.key.txt $file.enc $file.dec
ls -l $file*
cksum $file.enc $file.dec $file
