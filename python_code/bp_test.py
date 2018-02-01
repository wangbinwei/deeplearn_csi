# coding:UTF-8
'''
Date:20160831
@author: zhaozhiyong
'''
import numpy as np
from bp_train import get_predict
import csv
def load_data(file_name):
    '''导入数据
    input:  file_name(string):文件的存储位置
    output: feature_data(mat):特征
    '''
    #f = open(file_name)  # 打开文件
    with open(file_name) as f:
     feature_data = []
     readers = csv.reader(f)
     for line in readers:
        feature_tmp = []
        #lines = line.strip().split(",")
        for i in xrange(len(line)):
            feature_tmp.append(float(line[i]))
        feature_data.append(feature_tmp)

    return np.mat(feature_data)

def load_model(file_w0, file_w1, file_b0, file_b1):
    
    def get_model(file_name):
        f = open(file_name)
        model = []
        for line in f.readlines():
            lines = line.strip().split("\t")
            model_tmp = []
            for x in lines:
                model_tmp.append(float(x.strip()))
            model.append(model_tmp)
        f.close()
        return np.mat(model)
    
    # 1、导入输入层到隐含层之间的权重
    w0 = get_model(file_w0)
    
    # 2、导入隐含层到输出层之间的权重
    w1 = get_model(file_w1)
    
    # 3、导入输入层到隐含层之间的权重
    b0 = get_model(file_b0)
    
    # 4、导入隐含层到输出层之间的权重
    b1 = get_model(file_b1)

    return w0, w1, b0, b1

def save_predict(file_name, pre):
    '''保存最终的预测结果
    input:  pre(mat):最终的预测结果
    output:
    '''
    f = open(file_name, "w")
    m = np.shape(pre)[0]
    result = []
    sum={}
    for i in xrange(m):
        result.append(str(pre[i, 0]))
        if pre[i,0] in sum:
            sum[pre[i,0]]=sum[pre[i,0]]+1
        else:
            sum[pre[i,0]]=0
    for j in sum:
        print j,1.0*sum[j]/len(pre)

    f.write("\n".join(result))
    f.close()

if __name__ == "__main__":
    # 1、导入测试数据
    print "--------- 1.load data ------------"
    dataTest = load_data("test.csv")
    # 2、导入BP神经网络模型
    print "--------- 2.load model ------------"
    w0, w1, b0, b1 = load_model("weight_w0", "weight_w1", "weight_b0", "weight_b1")
    # 3、得到最终的预测值
    print "--------- 3.get prediction ------------"
    result = get_predict(dataTest, w0, w1, b0, b1)
    # 4、保存最终的预测结果
    print "--------- 4.save result ------------"
    pre = np.argmax(result, axis=1)
    save_predict("result", pre)
