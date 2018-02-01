# coding:UTF-8
import os
import string_txt
file_name = "csv_Fdbs"
__file__ = "D:\moisture_measurement\matlab\csv\%s"
__file__ = __file__ % file_name

#读取文件名
os.chdir(__file__)
file = os.listdir()
file_path=[]



#拼接文件名
for i in file:
   postfit = "%s.csv"
   file_csv = postfit % i
   file_path.append('/'+file_name+ '/'+i+ '/'+ file_csv)


for i in file_path:
    #print(i)
    string_txt.wirte_table(i)

    #print(i)
    #f=open('D:\python_code\path.txt','a')
    #f.write('%s\n'%i)

