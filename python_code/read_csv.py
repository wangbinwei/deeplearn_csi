# coding:UTF-8
import csv
import numpy as np
def load_data(file_name):
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
        for line in reader:
            feature_tmp = []
            # lines = line.strip().split(",")
            label_tmp.append(int(line[0]))  # 首个数字为标签
            for i in xrange(len(line) - 1):
                feature_tmp.append(float(line[i + 1]))
            feature_data.append(feature_tmp)

            # print len(label_tmp)
            # for row in feature_data:
            # print row
    # 2、获取标签
    m = len(label_tmp)  # 长度2010
    n_class = len(set(label_tmp))  # 得到类别的个数，2
    kind =  set(label_tmp) #类别种类
    kind = list(kind)

    label_data = np.mat(np.zeros((m, n_class)))
    for i in xrange(m):
        if label_tmp[i] == kind[0]:
            label_data[i, 0] = 1
        else:
            label_data[i, 1] = 1
    #print feature_data
    return np.mat(feature_data), label_data, n_class

def Normaliziaton(feature):
    maxx = np.max(feature)
    minn = np.min(feature)
    feature = (feature -minn)/(maxx - minn)
    return feature



feature,label_data,n_class = load_data('Ddbf1_Ddbf2_amp1.csv')
#print label_data, n_class
#print type(feature)
#print feature[1]
#temp = Normaliziaton(feature)
#print temp
#tt  = np.mat(temp)
#print tt
#print temp

