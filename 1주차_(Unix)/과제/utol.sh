echo "working directory"
read pos
cd $pos

if [ $? -eq 0 ]
then
  for i in *
  do
    echo $i
    if [ -f $i ]; then
      tmp=`echo $i | tr "[A-Z] [a-z]" "[a-z] [A-Z]"`
      mv $i $tmp
    fi
    echo $tmp
  done
else
  echo "error : cannot access directory"
fi