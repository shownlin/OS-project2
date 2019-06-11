#!/bin/bash0
size1="1"
size2="5"
size3="20"
size4="300"
size5="4000"
kb="kb"
for i in {1..7};
do
    echo $size1
    fallocate -l $size1$kb data/$size1.in
    echo $size2
    fallocate -l $size2$kb data/$size2.in
    echo $size3
    fallocate -l $size3$kb data/$size3.in
    echo $size4
    fallocate -l $size4$kb data/$size4.in
    echo $size5
    fallocate -l $size5$kb data/$size5.in
    size1+="0"
    size2+="0"
    size3+="0"
    size4+="0"
    size5+="0"
done
