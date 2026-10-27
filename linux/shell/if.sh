#!/bin/bash
score=85
if [ $score -ge 90 ];then
echo "及格"
elif [ $score -ge 60 ];then
echo "优秀"
else
echo "不及格"
fi
