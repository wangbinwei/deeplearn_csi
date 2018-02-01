def wirte_table(csv_path):
  #读取csv_table
  table_file_path = "C:\\Users\ASUS\Desktop\csv_table.txt"
  #print(table_file_path)
  file = open(table_file_path , 'r')
  lines = file.readlines()
  length = len(lines)
  #文件最后一行进行修改
  t = lines[length-1].split()
  #print(t[1])
  file_name = csv_path.split('/') #file_name[2] 是要的文件名

  t[1] = csv_path+"';"
  lines[length-1] = t[0] +' '+ "'"+'/user/beeline'+t[1]+'\n'

  #表名的修改
  firstline = lines[1].split()
  firstline[3] = file_name[2]+'('
  lines[1] = firstline[0]+' '+firstline[1]+' '+firstline[2]+' '+ firstline[3]+'\n'

  #创建表
  table_path = "D:\moisture_measurement\matlab\sql_table\\total_table.txt"

  sql_file = open(table_path,'a')
  for i in lines:
      sql_file.write(i)

  sql_file.close()
  file.close()

wirte_table('/csv_Ddbf/Ddbf1_Ddbf2_amp1/Ddbf1_Ddbf2_amp1.csv')
