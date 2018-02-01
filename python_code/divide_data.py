# coding:UTF-8
import csv
import numpy as np
def divide_data_csv(file_name):
    '''导入数据
        input:  file_name(string):文件的存储位置
        output: feature_data(mat):特征
                label_data(mat):标签
                n_class(int):类别的个数
        '''
    # 1、获取特征
    with open(file_name) as f:
        feature_data = []
        label_tmp = []
        reader = csv.reader(f)
        total_data = []
        for line in reader:
            #lines = line.strip().split(",")
            total_data.append(line)
            feature_tmp = []
            label_tmp.append(int(line[0]))  # 首个数字为标签
            for i in xrange(len(line) - 1):
                feature_tmp.append(float(line[i + 1]))
            feature_data.append(feature_tmp)
    #print feature_data
            # print len(label_tmp)
            # for row in feature_data:
            # print row
    total = []
    for line in total_data:
        total_tmp = []
        for i in xrange(len(line)):
            total_tmp.append(float(line[i]))
        total.append(total_tmp)
    #print total
    # 2、获取标签
    m = len(label_tmp)  # 长度2010
    n_class = len(set(label_tmp))  # 得到类别的个数，2
    kind = set(label_tmp)  # 类别种类
    kind = list(kind)
    count_0 = 0;count_1 = 0
    for i in label_tmp:
        if i == kind[0]:
            count_0 += 1
        else:
            count_1 += 1

    #创建训练和测试的csv
    train_data = []
    test_data = []
    for i in range(int(count_0*0.8)):
        train_data.append(total_data[i])
    for j in range(int(count_1*0.8)):
        train_data.append(total_data[j+count_0])
    #print type(train_data)
    with open("train.csv", 'w') as f:
        for i in total:
            f.write(str(i).strip('[]'))
            f.write('\n')

    for i in range(int(count_0*0.2)):
        test_data.append(feature_data[int(count_0*0.8+i)])
    #for j in range(int(count_1*0.2)):
        #test_data.append(feature_data[int(count_0+ count_1*0.8+j)])

    with open("test.csv",'w') as f:
        for i in test_data:
            f.write(str(i).strip('[]'))
            f.write('\n')



divide_data_csv("Ddbf1_Ddbf5_amp1.csv")